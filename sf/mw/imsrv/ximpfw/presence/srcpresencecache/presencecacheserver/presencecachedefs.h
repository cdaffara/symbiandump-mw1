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
* Description:  Defines for Presence Cache.
 *
*/

#ifndef PRESENCECACHEDEFS_H
#define PRESENCECACHEDEFS_H

#include <e32std.h>

/*
 * ============================================================================
 * Presence cache constants
 * ============================================================================
 */
namespace NCacheSrv
    {
    /**
     * Cache server version numbers
     */
    namespace NVersion
        {
        const TInt KMajor = 1;
        const TInt KMinor = 1;
        const TInt KBuild = 1;
        }
        
    namespace DataBase
        {
        const TInt KBuddyIdMaxLen = 100;
            
        }    


    /**
     * Cache server names
     */
    namespace NName
        {
        _LIT( KExecutable,      "presencecacheserver.exe" );
        _LIT( KMainThread,      "PrCacheSrv" );
        _LIT( KSymbianServer,   "!XIMPCacheSrv" );
        _LIT( KServerPattern,   "*XIMP*" );
        }

    /**
     * Cache server requests
     */
    namespace NRequest
        {
        //Number asynchronous message slots for Cache server
        const TUint KMsgSlotCount = 4;

        //Presence cache op synch. codes
        enum TOpSyncCodes
            {
            EPrepReadPresenceInfoSync = 1,
            EGetLastPreparedPacket,
            EGetLastAsyncPreparedPacket,
            EBuddyCountInAllServices,
            EBuddyCountInService,
            EDeleteService,
            EDeletePresence,
            //EPrepGetAllServices,
            //EGetAllServices,
            EGetServiceCount,
            EWritePresenceInfoSync,
            
            //Subscription/Notification
            ESubscribeBuddyPresenceChange, // 0:aService.iUid, 1:myIdPack
            EUnSubscribeBuddyPresenceChange,// 0:aService.iUid, 1:myIdPack
            EGetLastNotifiedtPacket, // 0:&ptrBuf
            
            }; //See the starting count in TOpAsyncCodes before adding here
               //Total number should be less than that here
            
        //Presence cache op asynch. codes
        enum TOpAsyncCodes
            {
            ENoRequestMade = 21,
            EWritePresenceInfoAsync,
            ECancelAsyncOperation,
            EPrepReadAllBuddiesPresenceInService,
            EPrepReadAllGivenBuddiesPresence,
            
            //Subscription/Notification related
            EWaitingForNotification, // 0:&sizePckg
            ECancelWaitingForNotification // 
            
            };
        }
        
    /**
     * Other constants
     */
    namespace NConstants
        {
        //Number buddy infos write in each cycle of runl
        const TUint KMaxWriteEachCycle = 5;
        const TUint KMaxReadEachCycle  = 5;
        const TUint KMaxServicesCount  = 10;
        const TUint KGranularity	   = 10;
        }
        
    }

#endif  //PRESENCECACHEDEFS_H
