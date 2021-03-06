/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the tools applications of the Qt Toolkit.
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

#ifndef QQMLJSLOADERGENERATOR_P_H
#define QQMLJSLOADERGENERATOR_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#include <private/qtqmlcompilerexports_p.h>

#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtCore/private/qglobal_p.h>

QT_BEGIN_NAMESPACE

bool Q_QMLCOMPILER_PRIVATE_EXPORT qQmlJSGenerateLoader(const QStringList &compiledFiles,
                                               const QString &outputFileName,
                                               const QStringList &resourceFileMappings,
                                               QString *errorString);
QString Q_QMLCOMPILER_PRIVATE_EXPORT qQmlJSSymbolNamespaceForPath(const QString &relativePath);

QT_END_NAMESPACE

#endif // QQMLJSLOADERGENERATOR_P_H
