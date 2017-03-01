/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qv4qmlcontext_p.h"
#include <private/qv8engine_p.h>

#include <private/qqmlengine_p.h>
#include <private/qqmlcontext_p.h>

#include <private/qv4engine_p.h>
#include <private/qv4value_p.h>
#include <private/qv4objectproto_p.h>
#include <private/qv4mm_p.h>
#include <private/qv4function_p.h>
#include <private/qv4compileddata_p.h>
#include <private/qqmltypewrapper_p.h>
#include <private/qqmllistwrapper_p.h>
#include <private/qqmljavascriptexpression_p.h>
#include <private/qjsvalue_p.h>
#include <private/qv4qobjectwrapper_p.h>

QT_BEGIN_NAMESPACE

using namespace QV4;

DEFINE_OBJECT_VTABLE(QmlContextWrapper);
DEFINE_MANAGED_VTABLE(QmlContext);

void Heap::QmlContextWrapper::init(QQmlContextData *context, QObject *scopeObject, bool ownsContext)
{
    Object::init();
    readOnly = true;
    this->ownsContext = ownsContext;
    isNullWrapper = false;
    this->context = new QQmlGuardedContextData(context);
    this->scopeObject.init(scopeObject);
}

void Heap::QmlContextWrapper::destroy()
{
    if (*context && ownsContext)
        (*context)->destroy();
    delete context;
    scopeObject.destroy();
    Object::destroy();
}

ReturnedValue QmlContextWrapper::get(const Managed *m, String *name, bool *hasProperty)
{
    Q_ASSERT(m->as<QmlContextWrapper>());
    const QmlContextWrapper *resource = static_cast<const QmlContextWrapper *>(m);
    QV4::ExecutionEngine *v4 = resource->engine();
    QV4::Scope scope(v4);

    // In V8 the JS global object would come _before_ the QML global object,
    // so simulate that here.
    bool hasProp;
    QV4::ScopedValue result(scope, v4->globalObject->get(name, &hasProp));
    if (hasProp) {
        if (hasProperty)
            *hasProperty = hasProp;
        return result->asReturnedValue();
    }

    if (resource->d()->isNullWrapper)
        return Object::get(m, name, hasProperty);

    if (v4->callingQmlContext() != *resource->d()->context)
        return Object::get(m, name, hasProperty);

    result = Object::get(m, name, &hasProp);
    if (hasProp) {
        if (hasProperty)
            *hasProperty = hasProp;
        return result->asReturnedValue();
    }

    // Its possible we could delay the calculation of the "actual" context (in the case
    // of sub contexts) until it is definately needed.
    QQmlContextData *context = resource->getContext();
    QQmlContextData *expressionContext = context;

    if (!context) {
        if (hasProperty)
            *hasProperty = true;
        return result->asReturnedValue();
    }

    // Search type (attached property/enum/imported scripts) names
    // while (context) {
    //     Search context properties
    //     Search scope object
    //     Search context object
    //     context = context->parent
    // }

    QObject *scopeObject = resource->getScopeObject();

    if (context->imports && name->startsWithUpper()) {
        // Search for attached properties, enums and imported scripts
        QQmlTypeNameCache::Result r = context->imports->query(name);

        if (r.isValid()) {
            if (hasProperty)
                *hasProperty = true;
            if (r.scriptIndex != -1) {
                QV4::ScopedObject scripts(scope, context->importedScripts.valueRef());
                return scripts->getIndexed(r.scriptIndex);
            } else if (r.type) {
                return QmlTypeWrapper::create(v4, scopeObject, r.type);
            } else if (r.importNamespace) {
                return QmlTypeWrapper::create(v4, scopeObject, context->imports, r.importNamespace);
            }
            Q_ASSERT(!"Unreachable");
        }

        // Fall through
    }

    QQmlEnginePrivate *ep = QQmlEnginePrivate::get(v4->qmlEngine());

    while (context) {
        // Search context properties
        const QV4::IdentifierHash<int> &properties = context->propertyNames();
        if (properties.count()) {
            int propertyIdx = properties.value(name);

            if (propertyIdx != -1) {

                if (propertyIdx < context->idValueCount) {

                    if (ep->propertyCapture)
                        ep->propertyCapture->captureProperty(&context->idValues[propertyIdx].bindings);
                    if (hasProperty)
                        *hasProperty = true;
                    return QV4::QObjectWrapper::wrap(v4, context->idValues[propertyIdx]);
                } else {

                    QQmlContextPrivate *cp = context->asQQmlContextPrivate();

                    if (ep->propertyCapture)
                        ep->propertyCapture->captureProperty(context->asQQmlContext(), -1, propertyIdx + cp->notifyIndex);

                    const QVariant &value = cp->propertyValues.at(propertyIdx);
                    if (hasProperty)
                        *hasProperty = true;
                    if (value.userType() == qMetaTypeId<QList<QObject*> >()) {
                        QQmlListProperty<QObject> prop(context->asQQmlContext(), (void*) qintptr(propertyIdx),
                                                               QQmlContextPrivate::context_count,
                                                               QQmlContextPrivate::context_at);
                        return QmlListWrapper::create(v4, prop, qMetaTypeId<QQmlListProperty<QObject> >());
                    } else {
                        return scope.engine->fromVariant(cp->propertyValues.at(propertyIdx));
                    }
                }
            }
        }

        // Search scope object
        if (scopeObject) {
            bool hasProp = false;
            QV4::ScopedValue result(scope, QV4::QObjectWrapper::getQmlProperty(v4, context, scopeObject,
                                                                               name, QV4::QObjectWrapper::CheckRevision, &hasProp));
            if (hasProp) {
                if (hasProperty)
                    *hasProperty = true;
                return result->asReturnedValue();
            }
        }
        scopeObject = 0;


        // Search context object
        if (context->contextObject) {
            bool hasProp = false;
            result = QV4::QObjectWrapper::getQmlProperty(v4, context, context->contextObject, name, QV4::QObjectWrapper::CheckRevision, &hasProp);
            if (hasProp) {
                if (hasProperty)
                    *hasProperty = true;
                return result->asReturnedValue();
            }
        }

        context = context->parent;
    }

    expressionContext->unresolvedNames = true;

    return Encode::undefined();
}

bool QmlContextWrapper::put(Managed *m, String *name, const Value &value)
{
    Q_ASSERT(m->as<QmlContextWrapper>());
    QmlContextWrapper *resource = static_cast<QmlContextWrapper *>(m);
    ExecutionEngine *v4 = resource->engine();
    QV4::Scope scope(v4);
    if (scope.hasException())
        return false;
    QV4::Scoped<QmlContextWrapper> wrapper(scope, resource);

    uint member = wrapper->internalClass()->find(name);
    if (member < UINT_MAX)
        return wrapper->putValue(member, value);

    if (wrapper->d()->isNullWrapper) {
        if (wrapper && wrapper->d()->readOnly) {
            QString error = QLatin1String("Invalid write to global property \"") + name->toQString() +
                            QLatin1Char('"');
            ScopedString e(scope, v4->newString(error));
            v4->throwError(e);
            return false;
        }

        return Object::put(m, name, value);
    }

    // Its possible we could delay the calculation of the "actual" context (in the case
    // of sub contexts) until it is definately needed.
    QQmlContextData *context = wrapper->getContext();
    QQmlContextData *expressionContext = context;

    if (!context)
        return false;

    // See QV8ContextWrapper::Getter for resolution order

    QObject *scopeObject = wrapper->getScopeObject();

    while (context) {
        const QV4::IdentifierHash<int> &properties = context->propertyNames();
        // Search context properties
        if (properties.count() && properties.value(name) != -1)
            return false;

        // Search scope object
        if (scopeObject &&
            QV4::QObjectWrapper::setQmlProperty(v4, context, scopeObject, name, QV4::QObjectWrapper::CheckRevision, value))
            return true;
        scopeObject = 0;

        // Search context object
        if (context->contextObject &&
            QV4::QObjectWrapper::setQmlProperty(v4, context, context->contextObject, name, QV4::QObjectWrapper::CheckRevision, value))
            return true;

        context = context->parent;
    }

    expressionContext->unresolvedNames = true;

    if (wrapper->d()->readOnly) {
        QString error = QLatin1String("Invalid write to global property \"") + name->toQString() +
                        QLatin1Char('"');
        v4->throwError(error);
        return false;
    }

    return Object::put(m, name, value);
}

void Heap::QmlContext::init(QV4::ExecutionContext *outerContext, QV4::QmlContextWrapper *qml)
{
    Heap::ExecutionContext::init(outerContext->engine(), Heap::ExecutionContext::Type_QmlContext);
    outer = outerContext->d();
    strictMode = false;
    callData = outer->callData;
    lookups = outer->lookups;
    constantTable = outer->constantTable;
    compilationUnit = outer->compilationUnit;

    this->qml = qml->d();
}

Heap::QmlContext *QmlContext::createWorkerContext(ExecutionContext *parent, const QUrl &source, Value *sendFunction)
{
    Scope scope(parent);

    QQmlContextData *context = new QQmlContextData;
    context->baseUrl = source;
    context->baseUrlString = source.toString();
    context->isInternal = true;
    context->isJSContext = true;

    Scoped<QmlContextWrapper> qml(scope, scope.engine->memoryManager->allocObject<QmlContextWrapper>(context, (QObject*)0, true));
    qml->d()->isNullWrapper = true;

    qml->setReadOnly(false);
    QV4::ScopedObject api(scope, scope.engine->newObject());
    api->put(QV4::ScopedString(scope, scope.engine->newString(QStringLiteral("sendMessage"))), *sendFunction);
    qml->QV4::Object::put(QV4::ScopedString(scope, scope.engine->newString(QStringLiteral("WorkerScript"))), api);
    qml->setReadOnly(true);

    Heap::QmlContext *c = parent->d()->engine->memoryManager->alloc<QmlContext>(parent, qml);
    return c;
}

Heap::QmlContext *QmlContext::create(ExecutionContext *parent, QQmlContextData *context, QObject *scopeObject)
{
    Scope scope(parent);

    Scoped<QmlContextWrapper> qml(scope, scope.engine->memoryManager->allocObject<QmlContextWrapper>(context, scopeObject));
    Heap::QmlContext *c = parent->d()->engine->memoryManager->alloc<QmlContext>(parent, qml);
    return c;
}

QT_END_NAMESPACE
