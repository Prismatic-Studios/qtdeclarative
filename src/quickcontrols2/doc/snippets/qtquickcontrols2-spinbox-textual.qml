/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:FDL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Free Documentation License Usage
** Alternatively, this file may be used under the terms of the GNU Free
** Documentation License version 1.3 as published by the Free Software
** Foundation and appearing in the file included in the packaging of
** this file. Please review the following information to ensure
** the GNU Free Documentation License version 1.3 requirements
** will be met: https://www.gnu.org/licenses/fdl-1.3.html.
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick
import QtQuick.Controls

//! [1]
SpinBox {
    id: spinBox
    from: 0
    to: items.length - 1
    value: 1 // "Medium"

    property var items: ["Small", "Medium", "Large"]

    validator: RegularExpressionValidator {
        regularExpression: new RegExp("(Small|Medium|Large)", "i")
    }

    textFromValue: function(value) {
        return items[value];
    }

    valueFromText: function(text) {
        for (var i = 0; i < items.length; ++i) {
            if (items[i].toLowerCase().indexOf(text.toLowerCase()) === 0)
                return i
        }
        return spinBox.value
    }
}
//! [1]
