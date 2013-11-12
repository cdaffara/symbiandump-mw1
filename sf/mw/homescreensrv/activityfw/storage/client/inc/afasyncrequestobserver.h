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

#ifndef AFASYNCREQUESTOBSERVER_H
#define AFASYNCREQUESTOBSERVER_H

#include <e32base.h>

class MAfAsyncRequestObserver
{
public:
    /**
     * Function inform observer about asynchronous request results
     * @param result - request result
     * @param data - response data
     */
    virtual void waitActivityRequestCompleted(int result, 
                                              const TDesC8 &data) = 0;

    /**
     * Function inform observer about asynchronous request results
     * @param result - request result
     * @param pixmap - pixmap respons
     */
    virtual void getThumbnailRequestCompleted(int result,
                                              int bitmapHandle, 
                                              void* userData) = 0;
    
    /**
     * Function inform observer about asynchronous request results
     * @param result - request result
     */
    virtual void dataChangeNotificationCompleted(int result) = 0;
    
};
#endif // AFASYNCREQUESTOBSERVER_H
