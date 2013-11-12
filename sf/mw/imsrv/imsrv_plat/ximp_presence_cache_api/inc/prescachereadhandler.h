/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence cache reader handler.
*
*/

#ifndef MPRESCACHEREADHANDLER_H
#define MPRESCACHEREADHANDLER_H

/**
 * Interface for presence cache reader API handler for asynchronous functions used in API.
 * This interface is implemented by the clients as per their needs. 
 *
 * @ingroup ximpprescacheapi
 * @since S60 v3.2
 */
 
class MPresenceBuddyInfoList;
class MPresenceBuddyInfo;

class MPresCacheReadHandler
    {
    public:
        /**
         * Handler for AllBuddiesPresenceInService method of MPresenceCacheReader.
         *
         * @param aErrorCode 
         * @param aPresenceBuddyInfoList presence infos for all buddies requested. 
         *         Ownership to callee. Can be null if not found.
         */    
        virtual void HandlePresenceReadL(TInt aErrorCode,
                            MPresenceBuddyInfoList* aPresenceBuddyInfoList) = 0;
                            
        /**
         * Handler for Buddy presence change notifications.
         *
         * @param aErrorCode 
         * @param aPresenceBuddyInfo presence info for buddy. 
         *         Ownership to callee. Can be NULL if aErrorCode exist.
         */    
        virtual void HandlePresenceNotificationL(TInt aErrorCode,
                            MPresenceBuddyInfo* aPresenceBuddyInfo) = 0;
    };

#endif // MPRESCACHEREADHANDLER_H



