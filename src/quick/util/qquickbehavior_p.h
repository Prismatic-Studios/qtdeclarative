/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
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

#ifndef QQUICKBEHAVIOR_H
#define QQUICKBEHAVIOR_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <private/qtquickglobal_p.h>

#include <private/qqmlpropertyvalueinterceptor_p.h>
#include <private/qqmlengine_p.h>
#include <qqml.h>
#include <private/qqmlfinalizer_p.h>

QT_BEGIN_NAMESPACE

class QQuickAbstractAnimation;
class QQuickBehaviorPrivate;
class Q_QUICK_PRIVATE_EXPORT QQuickBehavior : public QObject, public QQmlPropertyValueInterceptor, public QQmlFinalizerHook
{
    Q_OBJECT
    Q_INTERFACES(QQmlFinalizerHook)
    Q_DECLARE_PRIVATE(QQuickBehavior)

    Q_INTERFACES(QQmlPropertyValueInterceptor)
    Q_CLASSINFO("DefaultProperty", "animation")
    Q_PROPERTY(QQuickAbstractAnimation *animation READ animation WRITE setAnimation)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QVariant targetValue READ targetValue NOTIFY targetValueChanged REVISION(2, 13))
    Q_PROPERTY(QQmlProperty targetProperty READ targetProperty NOTIFY targetPropertyChanged REVISION(2, 15))
    Q_CLASSINFO("DeferredPropertyNames", "animation")
    QML_NAMED_ELEMENT(Behavior)
    QML_ADDED_IN_VERSION(2, 0)

public:
    QQuickBehavior(QObject *parent=nullptr);
    ~QQuickBehavior();

    void setTarget(const QQmlProperty &) override;
    void write(const QVariant &value) override;
    bool bindable(QUntypedBindable *untypedBindable, QUntypedBindable target) override;

    QQuickAbstractAnimation *animation();
    void setAnimation(QQuickAbstractAnimation *);

    bool enabled() const;
    void setEnabled(bool enabled);

    QVariant targetValue() const;

    QQmlProperty targetProperty() const;

    void componentFinalized() override;

Q_SIGNALS:
    void enabledChanged();
    void targetValueChanged();
    void targetPropertyChanged();
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickBehavior)

#endif // QQUICKBEHAVIOR_H
