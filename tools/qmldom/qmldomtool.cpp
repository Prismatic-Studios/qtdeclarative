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

#include <QtCore/qdebug.h>
#include <QtCore/qfile.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qcoreapplication.h>
#include <QtCore/qdir.h>
#include <QtCore/QTextStream>
#include <QtCore/QThread>

#include <QtQmlDom/private/qqmldomtop_p.h>
#include <QtQmlDom/private/qqmldomfilewriter_p.h>
#include <QtQmlDom/private/qqmldomoutwriter_p.h>
#include <QtQmlDom/private/qqmldomelements_p.h>
#include <QtQmlDom/private/qqmldomfieldfilter_p.h>

#include <cstdio>

#if QT_CONFIG(commandlineparser)
#    include <QtCore/qcommandlineparser.h>
#endif

#ifndef QT_BOOTSTRAPPED
#    include <QtCore/qlibraryinfo.h>
#endif
using namespace QQmlJS::Dom;

namespace tt {
Q_NAMESPACE

enum class Dependencies { None, Required };
Q_ENUM_NS(Dependencies);

};
using namespace tt;

int main(int argc, char *argv[])
{
    FieldFilter filter = FieldFilter::defaultFilter();
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("qmldom");
    QCoreApplication::setApplicationVersion("1.0");
#if QT_CONFIG(commandlineparser)
    QCommandLineParser parser;
    parser.setApplicationDescription(QLatin1String("QML dom tool"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dumpOption(QStringList() << "d"
                                                << "dump",
                                  QLatin1String("Dumps the code model"));
    parser.addOption(dumpOption);
    QCommandLineOption reformatOption(QStringList() << "r"
                                                    << "reformat",
                                      QLatin1String("reformats the files explicitly passed in"));
    parser.addOption(reformatOption);

    QCommandLineOption filterOption(
            QStringList() << "f"
                          << "filter-fields",
            QLatin1String("commas separated list of fields to filter out. Prepending a field with "
                          "'-' skips the field, with '+' it adds it. The field might be prepended "
                          "by '<type>:' to apply only to elements of that type"
                          "The default filters are ")
                    + filter.describeFieldsFilter(),
            QLatin1String("fields"));
    parser.addOption(filterOption);

    QCommandLineOption qmltypesDirsOption(
            QStringList() << "I"
                          << "qmldirs",
            QLatin1String("Look for qmltypes files in specified directory"),
            QLatin1String("directory"));
    parser.addOption(qmltypesDirsOption);

    QCommandLineOption qmltypesFilesOption(QStringList() << "i"
                                                         << "qmltypes",
                                           QLatin1String("Include the specified qmltypes files"),
                                           QLatin1String("qmltypes"));
    parser.addOption(qmltypesFilesOption);

    QCommandLineOption pathToDumpOption(
            QStringList() << "path-to-dump",
            QLatin1String("adds a path to dump (by default the root path is dumped)"),
            QLatin1String("pathToDump"));
    parser.addOption(pathToDumpOption);

    QCommandLineOption dependenciesOption(
            QStringList() << "D"
                          << "dependencies",
            QLatin1String("Dependencies to load: none, required, reachable"),
            QLatin1String("dependenciesToLoad"), QLatin1String("required"));
    parser.addOption(dependenciesOption);

    QCommandLineOption reformatDirOption(
            QStringList() << "reformat-dir",
            QLatin1String(
                    "Target directory for the reformatted files, "
                    "if not given the files are reformatted in place (but backup files are kept)"),
            QLatin1String("reformatDir"));
    parser.addOption(reformatDirOption);

    QCommandLineOption nBackupsOption(
            QStringList() << "backups",
            QLatin1String("Number of backup files to generate (default is 2, the oldest, "
                          "and the last version are kept), "),
            QLatin1String("nBackups"));
    parser.addOption(nBackupsOption);

    parser.addPositionalArgument(QLatin1String("files"),
                                 QLatin1String("list of qml or js files to verify"));

    parser.process(a);

    const auto positionalArguments = parser.positionalArguments();
    if (positionalArguments.isEmpty()) {
        parser.showHelp(-1);
    }

    if (parser.isSet(filterOption)) {
        qDebug() << "filters: " << parser.values(filterOption);
        for (QString fFields : parser.values(filterOption)) {
            if (!filter.addFilter(fFields)) {
                return 1;
            }
        }
    }

    Dependencies dep = Dependencies::None;
    for (QString depName : parser.values(dependenciesOption)) {
        QMetaEnum metaEnum = QMetaEnum::fromType<Dependencies>();
        bool found = false;
        for (int i = 0; i < metaEnum.keyCount(); ++i) {
            if (QLatin1String(metaEnum.key(i)).compare(depName, Qt::CaseInsensitive) == 0) {
                found = true;
                dep = Dependencies(metaEnum.value(i));
            }
        }
        if (!found) {
            QStringList values;
            for (int i = 0; i < metaEnum.keyCount(); ++i)
                values.append(QString::fromUtf8(metaEnum.key(i)).toLower());
            qDebug().noquote() << "Invalid dependencies argument, expected one of "
                               << values.join(QLatin1Char(','));
            return 1;
        }
    }

    int nBackups = 2;
    if (parser.isSet(nBackupsOption)) {
        bool intOk;
        nBackups = parser.value(nBackupsOption).toInt(&intOk);
        if (!intOk) {
            qDebug() << "expected an integer giving the number of backups after --backups, not "
                     << parser.value(nBackupsOption);
        }
    }

    QList<Path> pathsToDump;
    for (QString pStr : parser.values(pathToDumpOption)) {
        pathsToDump.append(Path::fromString(pStr));
    }
    if (pathsToDump.isEmpty())
        pathsToDump.append(Path());

    // use host qml import path as a sane default if nothing else has been provided
    QStringList qmltypeDirs = parser.isSet(qmltypesDirsOption)
            ? parser.values(qmltypesDirsOption)
#    ifndef QT_BOOTSTRAPPED
            : QStringList { QLibraryInfo::path(QLibraryInfo::Qml2ImportsPath) };
#    else
            : QStringList {};
#    endif

    if (!parser.isSet(qmltypesFilesOption))
        qmltypeDirs << ".";

    QStringList qmltypeFiles =
            parser.isSet(qmltypesFilesOption) ? parser.values(qmltypesFilesOption) : QStringList {};
#else
    QStringList qmltypeDirs {};
    QStringList qmltypeFiles {};
#endif

    {
        QDebug dbg = qDebug();
        dbg << "dirs:\n";
        foreach (QString d, qmltypeDirs)
            dbg << "    '" << d << "'\n";
        dbg << "files:\n";
        foreach (QString f, positionalArguments)
            dbg << "    '" << f << "'\n";
        dbg << "fieldFilter: " << filter.describeFieldsFilter();
        dbg << "\n";
    }
    QQmlJS::Dom::DomEnvironment::Options options =
            QQmlJS::Dom::DomEnvironment::Option::SingleThreaded;
    if (dep == Dependencies::None)
        options = options | QQmlJS::Dom::DomEnvironment::Option::NoDependencies;
    std::shared_ptr<QQmlJS::Dom::DomEnvironment> envPtr(
            new QQmlJS::Dom::DomEnvironment(qmltypeDirs, options));
    QQmlJS::Dom::DomItem env(envPtr);
    qDebug() << "will load\n";
    if (dep != Dependencies::None)
        env.loadBuiltins();
    foreach (QString s, positionalArguments) {
        env.loadFile(s, QString(), nullptr, LoadOption::DefaultLoad);
    }
    envPtr->loadPendingDependencies(env);
    if (parser.isSet(reformatOption)) {
        for (auto s : positionalArguments) {
            DomItem qmlFile = env.path(Paths::qmldirFilePath(s));
            if (qmlFile) {
                qDebug() << "reformatting" << s;
                FileWriter fw;
                QString target = s;
                QString rDir = parser.value(reformatDirOption);
                if (!rDir.isEmpty()) {
                    QFileInfo f(s);
                    QDir d(rDir);
                    target = d.filePath(f.fileName());
                }
                switch (fw.write(
                        target,
                        [&qmlFile, target](QTextStream &ts) {
                            LineWriter lw([&ts](QStringView s) { ts << s; }, target);
                            OutWriter ow(lw);
                            qmlFile.writeOut(ow);
                            ow.eof();
                            return true;
                        },
                        nBackups)) {
                case FileWriter::Status::ShouldWrite:
                case FileWriter::Status::SkippedDueToFailure:
                    qWarning() << "failure reformatting " << s;
                    break;
                case FileWriter::Status::DidWrite:
                    qDebug() << "success";
                    break;
                case FileWriter::Status::SkippedEqual:
                    qDebug() << "no change";
                }
            }
        }
    }
    if (parser.isSet(dumpOption) || !parser.isSet(reformatOption)) {
        qDebug() << "will dump\n";
        QTextStream ts(stdout);
        auto sink = [&ts](QStringView v) {
            ts << v; /* ts.flush(); */
        };
        if (pathsToDump.length() > 1)
            sink(u"{\n");
        bool first = true;
        for (Path p : pathsToDump) {
            if (pathsToDump.length() > 1) {
                if (first)
                    first = false;
                else
                    sink(u",\n");
                sinkEscaped(sink, p.toString());
                sink(u":\n");
            }
            env.path(p).dump(sink, 0, filter);
        }
        if (pathsToDump.length() > 1)
            sink(u"}\n");
        Qt::endl(ts).flush();
    }
    for (int i = 0; i < 100; ++i)
        QThread::yieldCurrentThread(); // let buggy integrations catch up with the output
    // return a.exec();
    return 0;
}

#include "qmldomtool.moc"
