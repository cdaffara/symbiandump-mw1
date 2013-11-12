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
 * Description: cambmiconengine.h
 *
 */

#ifndef HSMBMICONENGINE_H
#define HSMBMICONENGINE_H

#include <QIconEngineV2>
#include <QPixmap>
#include <QCache>

class CaMbmIconEngine : public QIconEngineV2
{
public:
    CaMbmIconEngine();
    CaMbmIconEngine(const CaMbmIconEngine &other);
    ~CaMbmIconEngine();
    void paint(QPainter *painter, const QRect &rect,
               QIcon::Mode mode, QIcon::State state);
    
    QSize actualSize(const QSize &size,
        QIcon::Mode mode,
        QIcon::State state);
    
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state);

    void addPixmap(const QPixmap &pixmap,
        QIcon::Mode mode,
        QIcon::State state);

    void addFile(const QString &fileName,
        const QSize &size,
        QIcon::Mode mode,
        QIcon::State state);

    QString key() const;
    QIconEngineV2 *clone() const;
    
private:
    QString pmcKey(const QSize &size);
    void getPixmapFromBitmap(const QSize &size, QPixmap& pixmap);
    void cacheBitmapL();
    
private:
    QString mMbmFilename;
    QCache<QString, QPixmap> mCache;
    CFbsBitmap *mBitmapCached;
    CFbsBitmap *mMaskBitmapCached;
};

#endif // HSMBMICONENGINE_H
