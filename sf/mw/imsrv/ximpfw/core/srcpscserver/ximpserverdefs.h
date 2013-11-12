/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Defines for XIMP Framework PSC server.
 *
*/

#ifndef XIMPPSCSERVERDEFS_H
#define XIMPPSCSERVERDEFS_H

#include <e32std.h>

/*
 * ============================================================================
 * Presence Service Connection (PSC) server constants
 * ============================================================================
 */
namespace NXIMPSrv
    {
    /**
     * PSC server version numbers
     */
    namespace NVersion
        {
        const TInt KMajor = 1;
        const TInt KMinor = 1;
        const TInt KBuild = 1;
        }


    /**
     * PSC server names
     */
    namespace NName
        {
        _LIT( KExecutable,      "ximpserver.exe" );
        _LIT( KMainThread,      "XIMPPscSrv" );
        _LIT( KSymbianServer,   "!XIMPPscSrv" );
        _LIT( KServerPattern,   "*XIMP*" );
        }


    /**
     * PSC server start-up parameters
     */
    namespace NStartupParam
        {
         //RProcess parameter slot for the protocol UID
        const TInt KProtocolUidIndex = 1;

         //Maximum length for the UID string
        const TInt KProtocolUidMaxLen = 8;
        }


    /**
     * PSC server requests
     */
    namespace NRequest
        {
        //Number asynchronous message slots for PSC server
        const TUint KMsgSlotCount = 4;

        //PSC Server op codes
        enum TOpCodes
            {
            EBecomeCtxSession = 1,

            ECtxSsQueueOperation,
            
            ECtxSsInstallEventFilter,
            ECtxSsSetEventListenScout,
            ECtxSsCancelEventListenScout,
            ECtxSsFetchTopEventDataSize,
            ECtxSsFetchTopEventData,
            ECtxSsFetchTopEventReqId,
            ECtxSsDropTopEvent,
            
            ECtxSsPrepareContextFeatures,
            ECtxSsGetPreparedData,

            ECtxSsQueueBindOperation,   // 12

            EOpCodeTop
            };
        }
    }


#endif  //XIMPPSCSERVERDEFS_H





