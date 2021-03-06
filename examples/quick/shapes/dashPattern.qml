/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
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

import QtQuick
import QtQuick.Shapes

Rectangle {
    color: "lightGray"
    width: 256
    height: 256
    Shape {
        width: 200
        height: 150
        anchors.centerIn: parent
        ShapePath {
            strokeWidth: 4
            strokeColor: "red"
            fillGradient: RadialGradient {
                centerX: 100; centerY: 100; centerRadius: 100
                SequentialAnimation on focalRadius {
                    loops: Animation.Infinite
                    NumberAnimation { from: 1; to: 20; duration: 2000 }
                    NumberAnimation { from: 20; to: 1; duration: 2000 }
                }
                SequentialAnimation on focalX {
                    loops: Animation.Infinite
                    NumberAnimation { from: 50; to: 150; duration: 3000 }
                    NumberAnimation { from: 150; to: 50; duration: 3000 }
                }
                SequentialAnimation on focalY {
                    loops: Animation.Infinite
                    NumberAnimation { from: 50; to: 150; duration: 1000 }
                    NumberAnimation { from: 150; to: 50; duration: 1000 }
                }
                GradientStop { position: 0; color: "#ffffff" }
                GradientStop { position: 0.11; color: "#f9ffa0" }
                GradientStop { position: 0.13; color: "#f9ff99" }
                GradientStop { position: 0.14; color: "#f3ff86" }
                GradientStop { position: 0.49; color: "#93b353" }
                GradientStop { position: 0.87; color: "#264619" }
                GradientStop { position: 0.96; color: "#0c1306" }
                GradientStop { position: 1; color: "#000000" }
            }
            fillColor: "blue" // ignored with the gradient set
            strokeStyle: ShapePath.DashLine
            dashPattern: [ 1, 4 ]
            startX: 20; startY: 20
            PathLine { x: 180; y: 130 }
            PathLine { x: 20; y: 130 }
            PathLine { x: 20; y: 20 }
        }
    }
}
