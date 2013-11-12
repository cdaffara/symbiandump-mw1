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
* Description:  Reference counted singleton for XIMP client side.
*
*/

#include    "ximpclientifsingleton.h"
#include    "ximpsrvrootclient.h"



// ============================ HELPER CLASS ===============================


/**
 * CXIMPClientIfSingleton nested structure
 * to hold global objects.
 */
NONSHARABLE_STRUCT( CXIMPClientIfSingleton::SGlobals )
    {
    /**
     * Root connection to XIMP server.
     * The CXIMPClientIfSingleton manages and ows the connection.
     */
    RXIMPSrvRootClient iRootClient;
    };





// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPClientIfSingleton::InstanceL()
// Singleton access method.
// -----------------------------------------------------------------------------
//
CXIMPClientIfSingleton* CXIMPClientIfSingleton::InstanceL()
    {
    CXIMPClientIfSingleton* singleton;

    singleton = ( CXIMPClientIfSingleton* ) Dll::Tls();
    if( !singleton )
        {
        singleton = CXIMPClientIfSingleton::NewLC();
        User::LeaveIfError( Dll::SetTls( singleton ) );
        CleanupStack::Pop();
        }
    else
        {
        User::LeaveIfError(singleton->Open()); // Open return KErrNone.
        }

    return singleton;
    }


// -----------------------------------------------------------------------------
// CXIMPClientIfSingleton::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CXIMPClientIfSingleton* CXIMPClientIfSingleton::NewLC()
    {
    CXIMPClientIfSingleton* self = new( ELeave ) CXIMPClientIfSingleton;
    CleanupClosePushL( *self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// CXIMPClientIfSingleton::CXIMPClientIfSingleton()
// -----------------------------------------------------------------------------
//
CXIMPClientIfSingleton::CXIMPClientIfSingleton()
    {
    }


// Destructor
CXIMPClientIfSingleton::~CXIMPClientIfSingleton()
    {
    //Destroy global objects
    if( iGlobals )
        {
        iGlobals->iRootClient.Close();
        }

    //And the holder
    delete iGlobals;
    Dll::SetTls( NULL );
    #if _BullseyeCoverage
    cov_write();
    #endif    
    }



// -----------------------------------------------------------------------------
// CXIMPClientIfSingleton::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPClientIfSingleton::ConstructL()
    {
    //Reserve the object holder and zero initialize it
    iGlobals = new ( ELeave ) SGlobals;
    Mem::FillZ( iGlobals, sizeof( SGlobals ) );

    //Construct needed global objects
    User::LeaveIfError( iGlobals->iRootClient.Connect() );
    }



// -----------------------------------------------------------------------------
// CXIMPClientIfSingleton::RootClient()
// -----------------------------------------------------------------------------
//
RXIMPSrvRootClient& CXIMPClientIfSingleton::RootClient()
    {
    return iGlobals->iRootClient;
    }



