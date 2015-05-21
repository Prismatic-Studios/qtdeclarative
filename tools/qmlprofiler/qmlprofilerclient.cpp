/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtQml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmlprofilerclient.h"

#include <QtCore/QStack>
#include <QtCore/QStringList>

ProfilerClient::ProfilerClient(const QString &clientName,
                             QQmlDebugConnection *client)
    : QQmlDebugClient(clientName, client),
      m_enabled(false)
{
}

ProfilerClient::~ProfilerClient()
{
}

void ProfilerClient::clearData()
{
    emit cleared();
}

bool ProfilerClient::isEnabled() const
{
    return m_enabled;
}

void ProfilerClient::stateChanged(State status)
{
    if ((m_enabled && status != Enabled) ||
            (!m_enabled && status == Enabled))
        emit enabledChanged();

    m_enabled = status == Enabled;

}

class QmlProfilerClientPrivate
{
public:
    QmlProfilerClientPrivate()
        : inProgressRanges(0) , features(std::numeric_limits<quint64>::max())
    {
        ::memset(rangeCount, 0,
                 QQmlProfilerService::MaximumRangeType * sizeof(int));
    }

    qint64 inProgressRanges;
    QStack<qint64> rangeStartTimes[QQmlProfilerService::MaximumRangeType];
    QStack<QStringList> rangeDatas[QQmlProfilerService::MaximumRangeType];
    QStack<QmlEventLocation> rangeLocations[QQmlProfilerService::MaximumRangeType];
    QStack<QQmlProfilerService::BindingType> bindingTypes;
    int rangeCount[QQmlProfilerService::MaximumRangeType];

    quint64 features;
};

QmlProfilerClient::QmlProfilerClient(
        QQmlDebugConnection *client)
    : ProfilerClient(QStringLiteral("CanvasFrameRate"), client),
      d(new QmlProfilerClientPrivate)
{
}

QmlProfilerClient::~QmlProfilerClient()
{
    delete d;
}

void QmlProfilerClient::setFeatures(quint64 features)
{
    d->features = features;
}

void QmlProfilerClient::clearData()
{
    ::memset(d->rangeCount, 0,
             QQmlProfilerService::MaximumRangeType * sizeof(int));
    d->bindingTypes.clear();
    ProfilerClient::clearData();
}

void QmlProfilerClient::sendRecordingStatus(bool record)
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    stream << record << -1 << d->features;
    sendMessage(ba);
}

inline QQmlProfilerService::ProfileFeature featureFromRangeType(
        QQmlProfilerService::RangeType range)
{
    switch (range) {
        case QQmlProfilerService::Painting:
            return QQmlProfilerService::ProfilePainting;
        case QQmlProfilerService::Compiling:
            return QQmlProfilerService::ProfileCompiling;
        case QQmlProfilerService::Creating:
            return QQmlProfilerService::ProfileCreating;
        case QQmlProfilerService::Binding:
            return QQmlProfilerService::ProfileBinding;
        case QQmlProfilerService::HandlingSignal:
            return QQmlProfilerService::ProfileHandlingSignal;
        case QQmlProfilerService::Javascript:
            return QQmlProfilerService::ProfileJavaScript;
        default:
            return QQmlProfilerService::MaximumProfileFeature;
    }
}

void QmlProfilerClient::messageReceived(const QByteArray &data)
{
    QByteArray rwData = data;
    QDataStream stream(&rwData, QIODevice::ReadOnly);

    // Force all the 1 << <FLAG> expressions to be done in 64 bit, to silence some warnings
    const quint64 one = static_cast<quint64>(1);

    qint64 time;
    int messageType;

    stream >> time >> messageType;

    if (messageType >= QQmlProfilerService::MaximumMessage)
        return;

    if (messageType == QQmlProfilerService::Event) {
        int event;
        stream >> event;

        if (event == QQmlProfilerService::EndTrace) {
            emit this->traceFinished(time);
        } else if (event == QQmlProfilerService::AnimationFrame) {
            if (!(d->features & one << QQmlProfilerService::ProfileAnimations))
                return;
            int frameRate, animationCount;
            int threadId = 0;
            stream >> frameRate >> animationCount;
            if (!stream.atEnd())
                stream >> threadId;
            emit this->frame(time, frameRate, animationCount, threadId);
        } else if (event == QQmlProfilerService::StartTrace) {
            emit this->traceStarted(time);
        } else if (event == QQmlProfilerService::Key || event == QQmlProfilerService::Mouse) {
            if (!(d->features & one << QQmlProfilerService::ProfileInputEvents))
                return;
            emit this->inputEvent((QQmlProfilerService::EventType)event, time);
        }
    } else if (messageType == QQmlProfilerService::Complete) {
        emit complete();
    } else if (messageType == QQmlProfilerService::SceneGraphFrame) {
        if (!(d->features & one << QQmlProfilerService::ProfileSceneGraph))
            return;
        int sgEventType;
        int count = 0;
        qint64 params[5];

        stream >> sgEventType;
        while (!stream.atEnd()) {
            stream >> params[count++];
        }
        while (count<5)
            params[count++] = 0;
        emit sceneGraphFrame((QQmlProfilerService::SceneGraphFrameType)sgEventType, time,
                             params[0], params[1], params[2], params[3], params[4]);
    } else if (messageType == QQmlProfilerService::PixmapCacheEvent) {
        if (!(d->features & one << QQmlProfilerService::ProfilePixmapCache))
            return;
        int pixEvTy, width = 0, height = 0, refcount = 0;
        QString pixUrl;
        stream >> pixEvTy >> pixUrl;
        if (pixEvTy == (int)QQmlProfilerService::PixmapReferenceCountChanged ||
                pixEvTy == (int)QQmlProfilerService::PixmapCacheCountChanged) {
            stream >> refcount;
        } else if (pixEvTy == (int)QQmlProfilerService::PixmapSizeKnown) {
            stream >> width >> height;
            refcount = 1;
        }
        emit pixmapCache((QQmlProfilerService::PixmapEventType)pixEvTy, time,
                         QmlEventLocation(pixUrl,0,0), width, height, refcount);
    } else if (messageType == QQmlProfilerService::MemoryAllocation) {
        if (!(d->features & one << QQmlProfilerService::ProfileMemory))
            return;
        int type;
        qint64 delta;
        stream >> type >> delta;
        emit memoryAllocation((QQmlProfilerService::MemoryType)type, time, delta);
    } else {
        int range;
        stream >> range;

        if (range >= QQmlProfilerService::MaximumRangeType)
            return;

        if (!(d->features & one << featureFromRangeType(
                  static_cast<QQmlProfilerService::RangeType>(range))))
            return;

        if (messageType == QQmlProfilerService::RangeStart) {
            d->rangeStartTimes[range].push(time);
            d->inProgressRanges |= (static_cast<qint64>(1) << range);
            ++d->rangeCount[range];

            // read binding type
            if (range == (int)QQmlProfilerService::Binding) {
                int bindingType = (int)QQmlProfilerService::QmlBinding;
                if (!stream.atEnd())
                    stream >> bindingType;
                d->bindingTypes.push((QQmlProfilerService::BindingType)bindingType);
            }
        } else if (messageType == QQmlProfilerService::RangeData) {
            QString data;
            stream >> data;

            int count = d->rangeCount[range];
            if (count > 0) {
                while (d->rangeDatas[range].count() < count)
                    d->rangeDatas[range].push(QStringList());
                d->rangeDatas[range][count-1] << data;
            }

        } else if (messageType == QQmlProfilerService::RangeLocation) {
            QString fileName;
            int line;
            int column = -1;
            stream >> fileName >> line;

            if (!stream.atEnd())
                stream >> column;

            if (d->rangeCount[range] > 0) {
                d->rangeLocations[range].push(QmlEventLocation(fileName, line,
                                                            column));
            }
        } else {
            if (d->rangeCount[range] > 0) {
                --d->rangeCount[range];
                if (d->inProgressRanges & (static_cast<qint64>(1) << range))
                    d->inProgressRanges &= ~(static_cast<qint64>(1) << range);

                QStringList data = d->rangeDatas[range].count() ?
                            d->rangeDatas[range].pop() : QStringList();
                QmlEventLocation location = d->rangeLocations[range].count() ?
                            d->rangeLocations[range].pop() : QmlEventLocation();

                qint64 startTime = d->rangeStartTimes[range].pop();
                QQmlProfilerService::BindingType bindingType = QQmlProfilerService::QmlBinding;
                if (range == (int)QQmlProfilerService::Binding)
                    bindingType = d->bindingTypes.pop();
                emit this->range((QQmlProfilerService::RangeType)range,
                                 bindingType, startTime, time - startTime, data, location);
                if (d->rangeCount[range] == 0) {
                    int count = d->rangeDatas[range].count() +
                                d->rangeStartTimes[range].count() +
                                d->rangeLocations[range].count();
                    if (count != 0)
                        qWarning() << "incorrectly nested data";
                }
            }
        }
    }
}

V8ProfilerClient::V8ProfilerClient(QQmlDebugConnection *client)
    : ProfilerClient(QStringLiteral("V8Profiler"), client)
{
}

V8ProfilerClient::~V8ProfilerClient()
{
}

void V8ProfilerClient::sendRecordingStatus(bool record)
{
    QByteArray ba;
    QDataStream stream(&ba, QIODevice::WriteOnly);
    QByteArray cmd("V8PROFILER");
    QByteArray option(record ? "start" : "stop");
    QByteArray title("");

    stream << cmd << option << title;
    sendMessage(ba);
}

void V8ProfilerClient::messageReceived(const QByteArray &data)
{
    QByteArray rwData = data;
    QDataStream stream(&rwData, QIODevice::ReadOnly);

    int messageType;

    stream >> messageType;

    if (messageType == V8Complete) {
        emit complete();
    } else if (messageType == V8Entry) {
        QString filename;
        QString function;
        int lineNumber;
        double totalTime;
        double selfTime;
        int depth;

        stream  >> filename >> function >> lineNumber >> totalTime >>
                   selfTime >> depth;
        emit this->range(depth, function, filename, lineNumber, totalTime,
                         selfTime);
    }
}

