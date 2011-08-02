/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtDeclarative module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-ONLY$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QJSVALUEITERATOR_P_H
#define QJSVALUEITERATOR_P_H

#include <qscripttools_p.h>
#include <qjsvalue_p.h>

#include <qv8_p.h>

QT_BEGIN_NAMESPACE

class QV8Engine;

class QJSValueIteratorPrivate : public QScriptLinkedNode {
public:
    inline QJSValueIteratorPrivate(const QJSValuePrivate* value);
    inline ~QJSValueIteratorPrivate();

    inline bool hasNext() const;
    inline bool next();

    inline QString name() const;

    inline QScriptPassPointer<QJSValuePrivate> value() const;

    inline bool isValid() const;
    inline QV8Engine* engine() const;

    inline void invalidate();
private:
    Q_DISABLE_COPY(QJSValueIteratorPrivate)

    QScriptSharedDataPointer<QJSValuePrivate> m_object;
    v8::Persistent<v8::Array> m_names;
    uint32_t m_index;
    uint32_t m_count;
};


QT_END_NAMESPACE

#endif // QJSVALUEITERATOR_P_H
