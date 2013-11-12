/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Tools to increase Presence Engine tests robustness.
 *
*/

#ifndef PENGTSTROBUSTNESSTOOLS_H__
#define PENGTSTROBUSTNESSTOOLS_H__


// INCLUDES
#include <e32std.h>
#include <eunitmacros.h>
#include "prfwtestprocessmaster.h"
#include "userafter.h"
#include "prfwtestfiletool.h"

//MACROS


// CLASS DESCRIPTION

/**
 * PE test robustness handler.
 */
class PrfwTestRobustness
    {
    public: //Robustness handling
        static inline void DoPreCleaning();

    private: //Helpers
        static inline void HandleKill( TInt aErr,
                TInt aWaitTime,
                const TDesC& aDescription );

    private: //Prohibited constructor / destructor
        PrfwTestRobustness() {}
        ~PrfwTestRobustness() {}
    };



// -----------------------------------------------------------------------------
// PrfwTestRobustness public functions
// -----------------------------------------------------------------------------
//
inline void PrfwTestRobustness::DoPreCleaning()
    {
    TInt err = PrfwTestProcessMaster::KillPscServers();
    HandleKill( err, 0, _L("PSC srv processes - Pre") );

    err = PrfwTestProcessMaster::KillRootServer();
    HandleKill( err, 0, _L("ROOT srv processes - pre") );

    TRAPD( ignore, CXIMPTestFileTool::CleanAllL() );
    }


// -----------------------------------------------------------------------------
// PrfwTestRobustness functions
// -----------------------------------------------------------------------------
//
inline void PrfwTestRobustness::HandleKill( TInt aErr,
        TInt aWaitTime,
        const TDesC& aDescription )
    {
    if( aErr > 0 )
        {
        if( aWaitTime > 0 )
            {
            TRAPD( ignore, CUserAfter::AfterL( aWaitTime ) );    
            }

        EUNIT_PRINT( _L("ProcessKill: %S, %d instance(s)"), &aDescription, aErr );
        }
    }


#endif // PRWFWTESTROBUSTNESSTOOLS_H__

//  End of File

