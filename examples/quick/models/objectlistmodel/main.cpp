/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QGuiApplication>
#include <QList>

#include <qqmlengine.h>
#include <qqmlcontext.h>
#include <qqml.h>
#include <QtQuick/qquickitem.h>
#include <QtQuick/qquickview.h>

#include "dataobject.h"

/*
   This example illustrates exposing a QList<QObject*> as a
   model in QML
*/

//![0]
int main(int argc, char ** argv)
{
    QGuiApplication app(argc, argv);

    const QStringList colorList = {"red",
                                   "green",
                                   "blue",
                                   "yellow"};

    const QStringList moduleList = {"Core", "GUI", "Multimedia", "Multimedia Widgets", "Network",
                                    "QML", "Quick", "Quick Controls", "Quick Dialogs",
                                    "Quick Layouts", "Quick Test", "SQL", "Widgets", "3D",
                                    "Android Extras", "Bluetooth", "Concurrent", "D-Bus",
                                    "Gamepad", "Graphical Effects", "Help", "Image Formats",
                                    "Location", "Mac Extras", "NFC", "OpenGL", "Platform Headers",
                                    "Positioning", "Print Support", "Purchasing", "Quick Extras",
                                    "Quick Timeline", "Quick Widgets", "Remote Objects", "Script",
                                    "SCXML", "Script Tools", "Sensors", "Serial Bus",
                                    "Serial Port", "Speech", "SVG", "UI Tools", "WebEngine",
                                    "WebSockets", "WebView", "Windows Extras", "XML",
                                    "XML Patterns", "Charts", "Network Authorization",
                                    "Virtual Keyboard", "Quick 3D", "Quick WebGL"};

    QList<QObject *> dataList;
    for (const QString &module : moduleList)
        dataList.append(new DataObject("Qt " + module, colorList.at(rand() % colorList.length())));

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setInitialProperties({{ "model", QVariant::fromValue(dataList) }});
//![0]

    view.setSource(QUrl("qrc:/objectlistmodel/view.qml"));
    view.show();

    return app.exec();
}
