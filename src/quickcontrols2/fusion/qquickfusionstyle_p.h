/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls 2 module of the Qt Toolkit.
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

#ifndef QQUICKFUSIONSTYLE_P_H
#define QQUICKFUSIONSTYLE_P_H

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

#include <QtCore/qobject.h>
#include <QtGui/qcolor.h>
#include <QtQml/qqml.h>
#include <QtQuick/private/qquickpalette_p.h>

QT_BEGIN_NAMESPACE

class QQuickFusionStyle : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor lightShade READ lightShade CONSTANT)
    Q_PROPERTY(QColor darkShade READ darkShade CONSTANT)
    Q_PROPERTY(QColor topShadow READ topShadow CONSTANT)
    Q_PROPERTY(QColor innerContrastLine READ innerContrastLine CONSTANT)
    QML_NAMED_ELEMENT(Fusion)
    QML_SINGLETON
    QML_ADDED_IN_VERSION(2, 3)

public:
    explicit QQuickFusionStyle(QObject *parent = nullptr);

    static QColor lightShade();
    static QColor darkShade();
    static QColor topShadow();
    static QColor innerContrastLine();

    Q_INVOKABLE static QColor highlight(QQuickPalette *palette);
    Q_INVOKABLE static QColor highlightedText(QQuickPalette *palette);
    Q_INVOKABLE static QColor outline(QQuickPalette *palette);
    Q_INVOKABLE static QColor highlightedOutline(QQuickPalette *palette);
    Q_INVOKABLE static QColor tabFrameColor(QQuickPalette *palette);
    Q_INVOKABLE static QColor buttonColor(QQuickPalette *palette, bool highlighted = false, bool down = false, bool hovered = false);
    Q_INVOKABLE static QColor buttonOutline(QQuickPalette *palette, bool highlighted = false, bool enabled = true);
    Q_INVOKABLE static QColor gradientStart(const QColor &baseColor);
    Q_INVOKABLE static QColor gradientStop(const QColor &baseColor);
    Q_INVOKABLE static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50);
    Q_INVOKABLE static QColor grooveColor(QQuickPalette *palette);
};

QT_END_NAMESPACE

#endif // QQUICKFUSIONSTYLE_P_H
