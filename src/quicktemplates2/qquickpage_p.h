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

#ifndef QQUICKPAGE_P_H
#define QQUICKPAGE_P_H

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

#include <QtQuickTemplates2/private/qquickpane_p.h>
#include <QtQml/qqmllist.h>

QT_BEGIN_NAMESPACE

class QQuickPagePrivate;

class Q_QUICKTEMPLATES2_PRIVATE_EXPORT QQuickPage : public QQuickPane
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QQuickItem *header READ header WRITE setHeader NOTIFY headerChanged FINAL)
    Q_PROPERTY(QQuickItem *footer READ footer WRITE setFooter NOTIFY footerChanged FINAL)
    // 2.5 (Qt 5.12)
    Q_PROPERTY(qreal implicitHeaderWidth READ implicitHeaderWidth NOTIFY implicitHeaderWidthChanged FINAL REVISION(2, 5))
    Q_PROPERTY(qreal implicitHeaderHeight READ implicitHeaderHeight NOTIFY implicitHeaderHeightChanged FINAL REVISION(2, 5))
    Q_PROPERTY(qreal implicitFooterWidth READ implicitFooterWidth NOTIFY implicitFooterWidthChanged FINAL REVISION(2, 5))
    Q_PROPERTY(qreal implicitFooterHeight READ implicitFooterHeight NOTIFY implicitFooterHeightChanged FINAL REVISION(2, 5))
    QML_NAMED_ELEMENT(Page)
    QML_ADDED_IN_VERSION(2, 0)

public:
    explicit QQuickPage(QQuickItem *parent = nullptr);
    ~QQuickPage();

    QString title() const;
    void setTitle(const QString &title);

    QQuickItem *header() const;
    void setHeader(QQuickItem *header);

    QQuickItem *footer() const;
    void setFooter(QQuickItem *footer);

    // 2.5 (Qt 5.12)
    qreal implicitHeaderWidth() const;
    qreal implicitHeaderHeight() const;

    qreal implicitFooterWidth() const;
    qreal implicitFooterHeight() const;

Q_SIGNALS:
    void titleChanged();
    void headerChanged();
    void footerChanged();
    // 2.5 (Qt 5.12)
    void implicitHeaderWidthChanged();
    void implicitHeaderHeightChanged();
    void implicitFooterWidthChanged();
    void implicitFooterHeightChanged();

protected:
    QQuickPage(QQuickPagePrivate &dd, QQuickItem *parent);

    void componentComplete() override;

    void spacingChange(qreal newSpacing, qreal oldSpacing) override;

#if QT_CONFIG(accessibility)
    QAccessible::Role accessibleRole() const override;
    void accessibilityActiveChanged(bool active) override;
#endif

private:
    Q_DISABLE_COPY(QQuickPage)
    Q_DECLARE_PRIVATE(QQuickPage)
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickPage)

#endif // QQUICKPAGE_P_H
