/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <qtest.h>
#include <QtQuick/private/qquickitem_p.h>
#include <QtQuickTestUtils/private/qmlutils_p.h>
#include <QtQuickControls2/private/qquickstyle_p.h>
#include <QtQuickTemplates2/private/qquickapplicationwindow_p.h>
#include <QtQuickControlsTestUtils/private/controlstestutils_p.h>

using namespace QQuickControlsTestUtils;

class tst_qquickuniversalstyleconf : public QQmlDataTest
{
    Q_OBJECT

public:
    tst_qquickuniversalstyleconf();

private slots:
    void conf();
};

tst_qquickuniversalstyleconf::tst_qquickuniversalstyleconf()
    : QQmlDataTest(QT_QMLTEST_DATADIR)
{
}

void tst_qquickuniversalstyleconf::conf()
{
    QQuickControlsApplicationHelper helper(this, QLatin1String("applicationwindow.qml"));
    QVERIFY2(helper.ready, helper.failureMessage());

    QFont customFont;
    customFont.setFamilies(QStringList{QLatin1String("Courier")});
    customFont.setPixelSize(22);

    QQuickApplicationWindow *window = helper.appWindow;
    window->show();
    QVERIFY(QTest::qWaitForWindowExposed(window));
    // We specified a custom background color and font, so the window should have them.
    QCOMPARE(window->property("color").value<QColor>(), QColor("#444444"));
    QCOMPARE(window->property("font").value<QFont>(), customFont);

    // We specified a custom foreground color and font, so the label should have them.
    QQuickItem *label = window->property("label").value<QQuickItem*>();
    QVERIFY(label);
    QCOMPARE(label->property("color").value<QColor>(), QColor("#E51400"));
    QCOMPARE(label->property("font").value<QFont>(), customFont);
}

QTEST_MAIN(tst_qquickuniversalstyleconf)

#include "tst_qquickuniversalstyleconf.moc"
