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

#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QDir>
#include <QGroupBox>
#include <QScreen>
#include <QQmlApplicationEngine>
#include <QQmlError>
#include <QQuickView>
#include <QQuickWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget widget;
    widget.setWindowTitle(QT_VERSION_STR);

    QHBoxLayout *hLayout = new QHBoxLayout(&widget);
    QGroupBox *groupBox = new QGroupBox("QuickWidget", &widget);
    QVBoxLayout *vLayout = new QVBoxLayout(groupBox);
    QQuickWidget *quickWidget = new QQuickWidget(groupBox);
    quickWidget->setMinimumSize(360, 520);
    vLayout->addWidget(quickWidget);
    quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    quickWidget->setSource(QUrl(QLatin1String("qrc:/main.qml")));
    if (quickWidget->status() == QQuickWidget::Error) {
        qWarning() << quickWidget->errors();
        return 1;
    }
    hLayout->addWidget(groupBox);

    const QString gallerySource =
        QDir::cleanPath(QLatin1String(SRCDIR"../../../examples/quickcontrols2/gallery/gallery.qml"));
    QQmlApplicationEngine engine(QUrl::fromLocalFile(gallerySource));
    QObject *root = engine.rootObjects().value(0, nullptr);
    if (!root || !root->isWindowType()) {
        qWarning() << "Load error" << gallerySource;
        return 1;
    }
    groupBox = new QGroupBox("QQuickView/createWindowContainer", &widget);
    vLayout = new QVBoxLayout(groupBox);
    QWidget *container = QWidget::createWindowContainer(qobject_cast<QWindow *>(root), groupBox);
    container->setMinimumSize(360, 520);
    vLayout->addWidget(container);
    hLayout->addWidget(groupBox);

    const QRect availableGeometry = widget.screen()->availableGeometry();
    widget.move(availableGeometry.center() - QPoint(widget.sizeHint().width() / 2, widget.sizeHint().height() / 2));

    widget.show();

    return app.exec();
}
