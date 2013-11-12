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
* Description:  Defines for XIMP Framework Root server.
 *
*/

#ifndef XIMPROOTSERVERDEFS_H
#define XIMPROOTSERVERDEFS_H

#include <e32std.h>

/*
 * ============================================================================
 * XIMP Framework Root server constants
 * ============================================================================
 */
namespace NRootSrv
    {
    /**
     * Root server version numbers
     */
    namespace NVersion
        {
        const TInt KMajor = 1;
        const TInt KMinor = 1;
        const TInt KBuild = 1;
        }


    /**
     * Root server names
     */
    namespace NName
        {
        _LIT( KExecutable,      "ximprootserver.exe" );
        _LIT( KMainThread,      "XIMPRootSrv" );
        _LIT( KSymbianServer,   "!XIMPRootSrv" );
        _LIT( KServerPattern,   "*XIMP*" );
        }

    /**
     * Root server requests
     */
    namespace NRequest
        {
        //Number asynchronous message slots for Root server
        const TUint KMsgSlotCount = 4;

        //Root Server op codes
        enum TOpCodes
            {
            EBecomeRootSession = 1,

            ERootSsPrepareProtocolList,
            ERootSsPrepareSupportedFeatures,
            ERootSsFetchServerSideData,

            EOpCodeTop
            };
        }
    }

#endif  //XIMPROOTSERVERDEFS_H





