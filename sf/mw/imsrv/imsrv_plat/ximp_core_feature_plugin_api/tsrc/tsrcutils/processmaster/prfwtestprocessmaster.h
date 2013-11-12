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
* Description: 
 *
*/

#ifndef PRFWTESTPROCESSMASTER_H__
#define PRFWTESTPROCESSMASTER_H__


// INCLUDES
#include <e32std.h>
#include <data_caging_path_literals.hrh>
#include <f32file.h>
#include <e32base.h>

#include "ximpserverdefs.h"
#include "ximprootserverdefs.h"

// CLASS DESCRIPTION

/**
 * Test utility for managing Presence Engine processies
 */
class PrfwTestProcessMaster
    {
    public: //enumerations
        enum TPEngProcessMasterEnums
            {
            EMsgSlot_Command = 1,
            EMsgSlot_Desc16Data = 2,

            EKillProcess = 100,
            EMaxMsgDataLength = 256
            };

    public: //Public functions

        /**
         *
         *
         *
         *
         *
         */
        static inline TInt KillPscServers();
        static inline TInt KillRootServer();

    private: //Helper functions
        static inline TInt DoKillServer( const TDesC& aServer );

    };




// -----------------------------------------------------------------------------
// PrfwTestProcessMaster public functions
// -----------------------------------------------------------------------------
//
inline TInt PrfwTestProcessMaster::KillPscServers()
    {
    return PrfwTestProcessMaster::DoKillServer( NXIMPSrv::NName::KServerPattern );
    }

inline TInt PrfwTestProcessMaster::KillRootServer()
    {
    return PrfwTestProcessMaster::DoKillServer( NRootSrv::NName::KServerPattern );
    }

// -----------------------------------------------------------------------------
// PrfwTestProcessMaster private functions
// -----------------------------------------------------------------------------
//
inline TInt PrfwTestProcessMaster::DoKillServer( const TDesC& aServer )
    {
    //Compose full executable name
    TFileName processMasterExe;

    // TODO this code shouldn't leave? does it? it was TRAP+L elsewhere.
    TFileName dllPath;
    Dll::FileName( dllPath );
    TParsePtrC dllPathParser( dllPath );
    processMasterExe.Insert( 0, dllPathParser.Drive() );

    processMasterExe.Append( KDC_PROGRAMS_DIR );
    processMasterExe.Append( _L("prfwtestprocessmaster.exe") );

    //Initialize ProcessMaster process
    RProcess processMaster;
    TInt err = processMaster.Create( processMasterExe, KNullDesC );
    if( err != KErrNone )
        {
        processMaster.Close();
        return err;
        }

    err = processMaster.SetParameter( PrfwTestProcessMaster::EMsgSlot_Command,
            PrfwTestProcessMaster::EKillProcess );
    if( err != KErrNone )
        {
        processMaster.Close();
        return err;
        }


    err = processMaster.SetParameter( PrfwTestProcessMaster::EMsgSlot_Desc16Data,
            aServer );
    if( err != KErrNone )
        {
        processMaster.Close();
        return err;
        }

    //Execute ProcessMaster
    TRequestStatus rendezvousStatus;
    processMaster.Rendezvous( rendezvousStatus );
    processMaster.Resume();
    User::WaitForRequest( rendezvousStatus );
    err = rendezvousStatus.Int();


    //Get exit statuses for debuging purposes
    TExitType exitType = processMaster.ExitType();
    TExitCategoryName exitCategory = processMaster.ExitCategory();
    TInt exitReason = processMaster.ExitReason();

    processMaster.Close();

    return err;
    }


#endif // PRFWTESTPROCESSMASTER_H__

//  End of File

