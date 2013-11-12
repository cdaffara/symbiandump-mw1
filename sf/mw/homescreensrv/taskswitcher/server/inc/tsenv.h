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
* Description:
*
*/
#ifndef TSENV_H
#define TSENV_H

#include <QObject>
#include <QPixmap>

class TsEnv: public QObject
{
    Q_OBJECT
public slots:
    void createThumbnail(const QPixmap& source, 
                         int angle =0, 
                         const void* userData =0);

signals:
    void thumbnailCreated(QPixmap thumbnail, const void *userData);

};

#endif //TSENV_H
