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

#include "qquickicon_p.h"

#include <private/qqmlcontextdata_p.h>
#include <private/qqmldata_p.h>

QT_BEGIN_NAMESPACE

class QQuickIconPrivate : public QSharedData
{
public:
    // This is based on QFont's resolve_mask.
    enum ResolveProperties {
        NameResolved = 0x0001,
        SourceResolved = 0x0002,
        WidthResolved = 0x0004,
        HeightResolved = 0x0008,
        ColorResolved = 0x0010,
        CacheResolved = 0x0020,
        AllPropertiesResolved = 0x1ffff
    };
    int resolveMask = 0;

    QString name;
    QUrl source;
    QUrl resolvedSource;
    int width = 0;
    int height = 0;
    QColor color = Qt::transparent;
    bool cache = true;
};

QQuickIcon::QQuickIcon()
    : d(new QQuickIconPrivate)
{
}

QQuickIcon::QQuickIcon(const QQuickIcon &other)
    : d(other.d)
{
}

QQuickIcon::~QQuickIcon()
{
}

QQuickIcon &QQuickIcon::operator=(const QQuickIcon &other)
{
    d = other.d;
    return *this;
}

bool QQuickIcon::operator==(const QQuickIcon &other) const
{
    return d == other.d || (d->name == other.d->name
                            && d->source == other.d->source
                            && d->resolvedSource == other.d->resolvedSource
                            && d->width == other.d->width
                            && d->height == other.d->height
                            && d->color == other.d->color
                            && d->cache == other.d->cache);
}

bool QQuickIcon::operator!=(const QQuickIcon &other) const
{
    return !(*this == other);
}

bool QQuickIcon::isEmpty() const
{
    return d->name.isEmpty() && d->source.isEmpty();
}

QString QQuickIcon::name() const
{
    return d->name;
}

void QQuickIcon::setName(const QString &name)
{
    if ((d->resolveMask & QQuickIconPrivate::NameResolved) && d->name == name)
        return;

    d.detach();
    d->name = name;
    d->resolveMask |= QQuickIconPrivate::NameResolved;
}

void QQuickIcon::resetName()
{
    d.detach();
    d->name = QString();
    d->resolveMask &= ~QQuickIconPrivate::NameResolved;
}

QUrl QQuickIcon::source() const
{
    return d->source;
}

void QQuickIcon::setSource(const QUrl &source)
{
    if ((d->resolveMask & QQuickIconPrivate::SourceResolved) && d->source == source)
        return;

    d.detach();
    d->source = source;
    d->resolvedSource.clear();
    d->resolveMask |= QQuickIconPrivate::SourceResolved;
}

void QQuickIcon::resetSource()
{
    d.detach();
    d->source = QString();
    d->resolvedSource.clear();
    d->resolveMask &= ~QQuickIconPrivate::SourceResolved;
}

QUrl QQuickIcon::resolvedSource() const
{
    return d->resolvedSource.isEmpty() ? d->source : d->resolvedSource;
}

// must be called by the property owner (e.g. Button) prior to emitting changed signal.
void QQuickIcon::ensureRelativeSourceResolved(const QObject *owner)
{
    if (d->source.isEmpty())
        return;
    if (!d->resolvedSource.isEmpty())
        return; // already resolved relative to (possibly) different owner
    const QQmlData *data = QQmlData::get(owner);
    if (!data || !data->outerContext)
        return;
    d.detach();
    d->resolvedSource = data->outerContext->resolvedUrl(d->source);
}

int QQuickIcon::width() const
{
    return d->width;
}

void QQuickIcon::setWidth(int width)
{
    if ((d->resolveMask & QQuickIconPrivate::WidthResolved) && d->width == width)
        return;

    d.detach();
    d->width = width;
    d->resolveMask |= QQuickIconPrivate::WidthResolved;
}

void QQuickIcon::resetWidth()
{
    d.detach();
    d->width = 0;
    d->resolveMask &= ~QQuickIconPrivate::WidthResolved;
}

int QQuickIcon::height() const
{
    return d->height;
}

void QQuickIcon::setHeight(int height)
{
    if ((d->resolveMask & QQuickIconPrivate::HeightResolved) && d->height == height)
        return;

    d.detach();
    d->height = height;
    d->resolveMask |= QQuickIconPrivate::HeightResolved;
}

void QQuickIcon::resetHeight()
{
    d.detach();
    d->height = 0;
    d->resolveMask &= ~QQuickIconPrivate::HeightResolved;
}

QColor QQuickIcon::color() const
{
    return d->color;
}

void QQuickIcon::setColor(const QColor &color)
{
    if ((d->resolveMask & QQuickIconPrivate::ColorResolved) && d->color == color)
        return;

    d.detach();
    d->color = color;
    d->resolveMask |= QQuickIconPrivate::ColorResolved;
}

void QQuickIcon::resetColor()
{
    d.detach();
    d->color = Qt::transparent;
    d->resolveMask &= ~QQuickIconPrivate::ColorResolved;
}

bool QQuickIcon::cache() const
{
    return d->cache;
}

void QQuickIcon::setCache(bool cache)
{
    if ((d->resolveMask & QQuickIconPrivate::CacheResolved) && d->cache == cache)
        return;

    d.detach();
    d->cache = cache;
    d->resolveMask |= QQuickIconPrivate::CacheResolved;
}

void QQuickIcon::resetCache()
{
    d.detach();
    d->cache = true;
    d->resolveMask &= ~QQuickIconPrivate::CacheResolved;
}

QQuickIcon QQuickIcon::resolve(const QQuickIcon &other) const
{
    QQuickIcon resolved = *this;
    resolved.d.detach();

    if (!(d->resolveMask & QQuickIconPrivate::NameResolved))
        resolved.d->name = other.d->name;

    if (!(d->resolveMask & QQuickIconPrivate::SourceResolved)) {
        resolved.d->source = other.d->source;
        resolved.d->resolvedSource = other.d->resolvedSource;
    }

    if (!(d->resolveMask & QQuickIconPrivate::WidthResolved))
        resolved.d->width = other.d->width;

    if (!(d->resolveMask & QQuickIconPrivate::HeightResolved))
        resolved.d->height = other.d->height;

    if (!(d->resolveMask & QQuickIconPrivate::ColorResolved))
        resolved.d->color = other.d->color;

    if (!(d->resolveMask & QQuickIconPrivate::CacheResolved))
        resolved.d->cache = other.d->cache;

    return resolved;
}

QT_END_NAMESPACE
