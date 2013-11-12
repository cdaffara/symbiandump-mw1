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
#include "tsenv.h"

const int thumbnailWidth(128);
const int thumbnailHeight(128);

void TsEnv::createThumbnail(const QPixmap& source, 
                            int angle, 
                            const void* userData)
{
    try {
    QPixmap thumbnailSource;
    const Qt::Axis rotationAxis(Qt::ZAxis); 
    if ( 270 <= angle ) {
        thumbnailSource = source.transformed(QTransform().rotate(270.0f, 
                                                                 rotationAxis),
                                             Qt::SmoothTransformation);
    } else if( 180 <= angle ) {
        thumbnailSource = source.transformed(QTransform().rotate(180.0f, 
                                                                 rotationAxis),
                                             Qt::SmoothTransformation);
    } else if( 90 <= angle ) {
        thumbnailSource = source.transformed(QTransform().rotate(90.0f, 
                                                                 rotationAxis),
                                             Qt::SmoothTransformation);
    } else {
        thumbnailSource = source;
    }
    const QRect expectedRect(0, 0, thumbnailWidth, thumbnailHeight);
    const QPixmap thumbanail(thumbnailSource.scaled(expectedRect.size(), 
                                                    Qt::KeepAspectRatioByExpanding,
                                                    Qt::SmoothTransformation));
    emit thumbnailCreated(thumbanail.copy(expectedRect), userData);
    } catch (...) {
    }
}
