/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Templates 2 module of the Qt Toolkit.
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

#ifndef QQUICKSTACKVIEW_P_H
#define QQUICKSTACKVIEW_P_H

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

#include <QtQuickTemplates2/private/qquickcontrol_p.h>

QT_BEGIN_NAMESPACE

class QQmlV4Function;
class QQuickTransition;
class QQuickStackElement;
class QQuickStackViewPrivate;
class QQuickStackViewAttached;
class QQuickStackViewAttachedPrivate;

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickStackView : public QQuickControl
{
    Q_OBJECT
    Q_PROPERTY(bool busy READ isBusy NOTIFY busyChanged FINAL)
    Q_PROPERTY(int depth READ depth NOTIFY depthChanged FINAL)
    Q_PROPERTY(QQuickItem *currentItem READ currentItem NOTIFY currentItemChanged FINAL)
    Q_PROPERTY(QJSValue initialItem READ initialItem WRITE setInitialItem FINAL)
    Q_PROPERTY(QQuickTransition *popEnter READ popEnter WRITE setPopEnter NOTIFY popEnterChanged FINAL)
    Q_PROPERTY(QQuickTransition *popExit READ popExit WRITE setPopExit NOTIFY popExitChanged FINAL)
    Q_PROPERTY(QQuickTransition *pushEnter READ pushEnter WRITE setPushEnter NOTIFY pushEnterChanged FINAL)
    Q_PROPERTY(QQuickTransition *pushExit READ pushExit WRITE setPushExit NOTIFY pushExitChanged FINAL)
    Q_PROPERTY(QQuickTransition *replaceEnter READ replaceEnter WRITE setReplaceEnter NOTIFY replaceEnterChanged FINAL)
    Q_PROPERTY(QQuickTransition *replaceExit READ replaceExit WRITE setReplaceExit NOTIFY replaceExitChanged FINAL)
    // 2.3 (Qt 5.10)
    Q_PROPERTY(bool empty READ isEmpty NOTIFY emptyChanged FINAL REVISION(2, 3))
    QML_NAMED_ELEMENT(StackView)
    QML_ATTACHED(QQuickStackViewAttached)
    QML_ADDED_IN_VERSION(2, 0)

public:
    explicit QQuickStackView(QQuickItem *parent = nullptr);
    ~QQuickStackView();

    static QQuickStackViewAttached *qmlAttachedProperties(QObject *object);

    bool isBusy() const;
    int depth() const;
    QQuickItem *currentItem() const;

    enum Status {
        Inactive = 0,
        Deactivating = 1,
        Activating = 2,
        Active = 3
    };
    Q_ENUM(Status)

    QJSValue initialItem() const;
    void setInitialItem(const QJSValue &item);

    QQuickTransition *popEnter() const;
    void setPopEnter(QQuickTransition *enter);

    QQuickTransition *popExit() const;
    void setPopExit(QQuickTransition *exit);

    QQuickTransition *pushEnter() const;
    void setPushEnter(QQuickTransition *enter);

    QQuickTransition *pushExit() const;
    void setPushExit(QQuickTransition *exit);

    QQuickTransition *replaceEnter() const;
    void setReplaceEnter(QQuickTransition *enter);

    QQuickTransition *replaceExit() const;
    void setReplaceExit(QQuickTransition *exit);

    enum LoadBehavior {
        DontLoad,
        ForceLoad
    };
    Q_ENUM(LoadBehavior)

    Q_INVOKABLE QQuickItem *get(int index, LoadBehavior behavior = DontLoad);
    Q_INVOKABLE QQuickItem *find(const QJSValue &callback, LoadBehavior behavior = DontLoad);

    enum Operation {
        Transition = -1, // ### Deprecated in Qt 6; remove in Qt 7.
        Immediate = 0,
        PushTransition = 1,
        ReplaceTransition = 2,
        PopTransition = 3,
    };
    Q_ENUM(Operation)

    Q_INVOKABLE void push(QQmlV4Function *args);
    Q_INVOKABLE void pop(QQmlV4Function *args);
    Q_INVOKABLE void replace(QQmlV4Function *args);

    // 2.3 (Qt 5.10)
    bool isEmpty() const;

public Q_SLOTS:
    void clear(Operation operation = Immediate);

Q_SIGNALS:
    void busyChanged();
    void depthChanged();
    void currentItemChanged();
    void popEnterChanged();
    void popExitChanged();
    void pushEnterChanged();
    void pushExitChanged();
    void replaceEnterChanged();
    void replaceExitChanged();
    // 2.3 (Qt 5.10)
    Q_REVISION(2, 3) void emptyChanged();

protected:
    void componentComplete() override;
    void geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry) override;
    bool childMouseEventFilter(QQuickItem *, QEvent *) override;

#if QT_CONFIG(quicktemplates2_multitouch)
    void touchEvent(QTouchEvent *event) override;
#endif

#if QT_CONFIG(accessibility)
    QAccessible::Role accessibleRole() const override;
#endif

private:
    Q_DISABLE_COPY(QQuickStackView)
    Q_DECLARE_PRIVATE(QQuickStackView)
};

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickStackViewAttached : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index NOTIFY indexChanged FINAL)
    Q_PROPERTY(QQuickStackView *view READ view NOTIFY viewChanged FINAL)
    Q_PROPERTY(QQuickStackView::Status status READ status NOTIFY statusChanged FINAL)
    // 2.2 (Qt 5.9)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible RESET resetVisible NOTIFY visibleChanged FINAL) // REVISION(2, 2)

public:
    explicit QQuickStackViewAttached(QObject *parent = nullptr);
    ~QQuickStackViewAttached();

    int index() const;
    QQuickStackView *view() const;
    QQuickStackView::Status status() const;

    // 2.2 (Qt 5.9)
    bool isVisible() const;
    void setVisible(bool visible);
    void resetVisible();

Q_SIGNALS:
    void indexChanged();
    void viewChanged();
    void statusChanged();
    // 2.1 (Qt 5.8)
    /*Q_REVISION(2, 1)*/ void activated();
    /*Q_REVISION(2, 1)*/ void activating();
    /*Q_REVISION(2, 1)*/ void deactivated();
    /*Q_REVISION(2, 1)*/ void deactivating();
    /*Q_REVISION(2, 1)*/ void removed();
    // 2.2 (Qt 5.9)
    /*Q_REVISION(2, 2)*/ void visibleChanged();

private:
    Q_DISABLE_COPY(QQuickStackViewAttached)
    Q_DECLARE_PRIVATE(QQuickStackViewAttached)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickStackView)
QML_DECLARE_TYPEINFO(QQuickStackView, QML_HAS_ATTACHED_PROPERTIES)

#endif // QQUICKSTACKVIEW_P_H
