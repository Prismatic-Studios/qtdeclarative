/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtQuick module of the Qt Toolkit.
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

#ifndef QSGDISTANCEFIELDGLYPHNODE_P_P_H
#define QSGDISTANCEFIELDGLYPHNODE_P_P_H

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

#include <QtQuick/qsgmaterial.h>
#include <QtQuick/private/qsgplaintexture_p.h>
#include "qsgdistancefieldglyphnode_p.h"
#include "qsgadaptationlayer_p.h"

QT_BEGIN_NAMESPACE

class QSGPlainTexture;

class Q_QUICK_PRIVATE_EXPORT QSGDistanceFieldTextMaterial: public QSGMaterial
{
public:
    QSGDistanceFieldTextMaterial();
    ~QSGDistanceFieldTextMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;
    int compare(const QSGMaterial *other) const override;

    virtual void setColor(const QColor &color);
    const QVector4D &color() const { return m_color; }

    void setGlyphCache(QSGDistanceFieldGlyphCache *a) { m_glyph_cache = a; }
    QSGDistanceFieldGlyphCache *glyphCache() const { return m_glyph_cache; }

    void setTexture(const QSGDistanceFieldGlyphCache::Texture * tex) { m_texture = tex; }
    const QSGDistanceFieldGlyphCache::Texture * texture() const { return m_texture; }

    void setFontScale(qreal fontScale) { m_fontScale = fontScale; }
    qreal fontScale() const { return m_fontScale; }

    QSize textureSize() const { return m_size; }

    bool updateTextureSize();
    bool updateTextureSizeAndWrapper();
    QSGTexture *wrapperTexture() const { return m_sgTexture; }

protected:
    QSize m_size;
    QVector4D m_color;
    QSGDistanceFieldGlyphCache *m_glyph_cache;
    const QSGDistanceFieldGlyphCache::Texture *m_texture;
    qreal m_fontScale;
    QSGPlainTexture *m_sgTexture;
};

class Q_QUICK_PRIVATE_EXPORT QSGDistanceFieldStyledTextMaterial : public QSGDistanceFieldTextMaterial
{
public:
    QSGDistanceFieldStyledTextMaterial();
    ~QSGDistanceFieldStyledTextMaterial();

    QSGMaterialType *type() const override = 0;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override = 0;
    int compare(const QSGMaterial *other) const override;

    void setStyleColor(const QColor &color);
    const QVector4D &styleColor() const { return m_styleColor; }

protected:
    QVector4D m_styleColor;
};

class Q_QUICK_PRIVATE_EXPORT QSGDistanceFieldOutlineTextMaterial : public QSGDistanceFieldStyledTextMaterial
{
public:
    QSGDistanceFieldOutlineTextMaterial();
    ~QSGDistanceFieldOutlineTextMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;
};

class Q_QUICK_PRIVATE_EXPORT QSGDistanceFieldShiftedStyleTextMaterial : public QSGDistanceFieldStyledTextMaterial
{
public:
    QSGDistanceFieldShiftedStyleTextMaterial();
    ~QSGDistanceFieldShiftedStyleTextMaterial();

    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;
    int compare(const QSGMaterial *other) const override;

    void setShift(const QPointF &shift) { m_shift = shift; }
    const QPointF &shift() const { return m_shift; }

protected:
    QPointF m_shift;
};

class Q_QUICK_PRIVATE_EXPORT QSGHiQSubPixelDistanceFieldTextMaterial : public QSGDistanceFieldTextMaterial
{
public:
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;
    void setColor(const QColor &color) override {
        const auto rgbColor = color.toRgb();
        m_color = QVector4D(rgbColor.redF(), rgbColor.greenF(), rgbColor.blueF(), rgbColor.alphaF());
    }
};

class Q_QUICK_PRIVATE_EXPORT QSGLoQSubPixelDistanceFieldTextMaterial : public QSGDistanceFieldTextMaterial
{
public:
    QSGMaterialType *type() const override;
    QSGMaterialShader *createShader(QSGRendererInterface::RenderMode renderMode) const override;
    void setColor(const QColor &color) override {
        const auto rgbColor = color.toRgb();
        m_color = QVector4D(rgbColor.redF(), rgbColor.greenF(), rgbColor.blueF(), rgbColor.alphaF());
    }
};

QT_END_NAMESPACE

#endif
