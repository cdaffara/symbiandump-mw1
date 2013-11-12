/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Task list entry
*
*/

#ifndef TSTHUMBNAILPROVIDER_H
#define TSTHUMBNAILPROVIDER_H
#include <QObject>
#include <QPixmap>
#include "tsthumbnailobserver.h"

class TsThumbnailProvider: public QObject
{
    Q_OBJECT

public:
    TsThumbnailProvider(MTsThumbnailObserver& observer, QObject* obj);
    void createThumbnail(CFbsBitmap& source, 
                         int angle =0);
signals:
    void createThumbnail(QPixmap source, 
                         int angle, 
                         const void* userData);
public slots:
    void thumbnailCreated(const QPixmap& thumbnail,
                          const void *userData);
private:
    MTsThumbnailObserver& mObserver;

};

#endif //TSTHUMBNAILPROVIDER_H
