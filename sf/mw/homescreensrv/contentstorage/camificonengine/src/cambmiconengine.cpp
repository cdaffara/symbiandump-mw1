/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 *
 * Description: cambmiconengine.cpp
 *
 */

#include <QPainter>
#include <QPixmap>
#include <QFileInfo>
#include <QDebug>

#include <AknIconUtils.h> // avkon
#include <apgicnfl.h> // fbsbitmap

#include "cambmiconengine.h"

const int cacheSize = 4;

inline QString CaMbmIconEngine::pmcKey(const QSize &size)
{ 
    return QString::number((size.width()<<11)|size.height(), 16);
}

CaMbmIconEngine::CaMbmIconEngine()
    : QIconEngineV2(),
      mMbmFilename(),
      mCache(cacheSize),
      mBitmapCached(0),
      mMaskBitmapCached(0)
{
}

CaMbmIconEngine::CaMbmIconEngine(const CaMbmIconEngine &other) :
    QIconEngineV2(other),
    mMbmFilename(other.mMbmFilename),
    mCache(cacheSize),
    mBitmapCached(0),
    mMaskBitmapCached(0)
{
}

CaMbmIconEngine::~CaMbmIconEngine()
{
    mCache.clear();
    delete mBitmapCached;
    delete mMaskBitmapCached;
}

void CaMbmIconEngine::cacheBitmapL()
{
    CFbsBitmap *bitmap(0);
    CFbsBitmap *maskBitmap(0);

    TInt bitmapIndex = 0;
    TInt maskIndex = 1;
    TPtrC16 filename(
        reinterpret_cast<const TUint16*>(mMbmFilename.utf16()));
    AknIconUtils::CreateIconLC( bitmap, maskBitmap, filename,
        bitmapIndex, maskIndex );
    mBitmapCached = bitmap;
    mMaskBitmapCached = maskBitmap;
    // bitmap and icon, AknsUtils::CreateIconLC doesn't specify the order
    CleanupStack::Pop(2);
}
void CaMbmIconEngine::getPixmapFromBitmap(const QSize &size, QPixmap& pixmap)
{
    if (!mBitmapCached || !mMaskBitmapCached) {
        TRAP_IGNORE(cacheBitmapL());
    }
    if (mBitmapCached && mMaskBitmapCached) {
        CFbsBitmap *bitmap = mBitmapCached;
        CFbsBitmap *maskBitmap = mMaskBitmapCached;
   
        pixmap = pixmap.fromSymbianCFbsBitmap(bitmap);
        QPixmap mask;
        mask = mask.fromSymbianCFbsBitmap(maskBitmap);
        pixmap.setAlphaChannel(mask);
        pixmap = pixmap.scaled(size, Qt::IgnoreAspectRatio);
    }
}

QSize CaMbmIconEngine::actualSize(const QSize &size,
    QIcon::Mode mode,
    QIcon::State state)
{
    Q_UNUSED(mode);
    Q_UNUSED(state);
    QString key(pmcKey(size));
    qDebug() << "pmckey: " << key;
    QSize realSize;
            
    if (mCache.contains(key)) {
        realSize = size;
    } else {
        QPixmap pixmap;
        TPtrC16 filename(
            reinterpret_cast<const TUint16*>(mMbmFilename.utf16()));
        getPixmapFromBitmap(size, pixmap);
        if (!pixmap.isNull()) {
            QPixmap *cachedPixmap = new QPixmap(pixmap);
            if (mCache.insert(key, cachedPixmap)) {
                realSize = size;
                // mCache has taken ownership of cachedPixmap. 
                qDebug() << "pmckey INSERT";
            }
        }
    }
    return realSize;
}

QPixmap CaMbmIconEngine::pixmap(const QSize &size,
    QIcon::Mode mode,
    QIcon::State state)
{
    Q_UNUSED(mode);
    Q_UNUSED(state);
    QPixmap pixmap;
    actualSize(size, mode, state);
    QString key = pmcKey(size);
    if (mCache.contains(key)) {
        pixmap = *mCache[key];
    }
    return pixmap;
}

void CaMbmIconEngine::addPixmap(const QPixmap &pixmap,
    QIcon::Mode mode,
    QIcon::State state)
{
    Q_UNUSED(pixmap);
    Q_UNUSED(mode);
    Q_UNUSED(state);
}

void CaMbmIconEngine::addFile(const QString &fileName,
    const QSize &size,
    QIcon::Mode mode,
    QIcon::State state)
{
    Q_UNUSED(size);
    Q_UNUSED(mode);
    Q_UNUSED(state);
    if (mMbmFilename.isEmpty() && !fileName.isEmpty()) {
        
        QString abs = fileName;
        if (fileName.at(0) != QLatin1Char(':')) {
            abs = QFileInfo(fileName).absoluteFilePath();
        }
        if (abs.endsWith(QLatin1String(".mbm"), Qt::CaseInsensitive))
        {
            mMbmFilename = abs;
            mMbmFilename.replace('/', '\\');
            TRAP_IGNORE(cacheBitmapL());
        } 
    }
}

void CaMbmIconEngine::paint(QPainter *painter, const QRect &rect,
                           QIcon::Mode mode, QIcon::State state)
{
    painter->drawPixmap(rect, pixmap(rect.size(), mode, state));
}

QString CaMbmIconEngine::key() const
{
    return QLatin1String("mbm");
}

QIconEngineV2 *CaMbmIconEngine::clone() const
{
    return new CaMbmIconEngine(*this);
}



