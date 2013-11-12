/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence cache writer handler.
*
*/

#ifndef MPRESCACHEWRITEHANDLER_H
#define MPRESCACHEWRITEHANDLER_H

/**
 * Interface for presence cache writer API. Handler for asynchronous functions used in API.
 * This interface is implemented by the clients as per their needs. 
 *
 * @ingroup ximpprescacheapi
 * @since S60 v3.2
 */
class MPresCacheWriteHandler
    {
    public:
    
        /**
         * Handler for Asynchronous methods of MPresenceCacheWriter.
         *
         * @param TInt error code comes here 
         */    
        virtual void HandlePresenceWriteL(TInt aErrorCode) = 0;
    };

#endif // MPRESCACHEWRITEHANDLER_H