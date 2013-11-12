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
* Description:  Presence Service Connection globals holder
 *
*/


#include "ximpglobals.h"
#include "ximppluginfactory.h"
#include "ximpfeaturemanager.h"
#include "ximphostmanager.h"
#include "ximpoperationfactory.h"
#include "ximpglobalsobserver.h"

// ============================ HELPER CLASS ===============================


/**
 * CXIMPGlobals nested structure
 * to hold global objects.
 */
NONSHARABLE_STRUCT( CXIMPGlobals::SGlobals )
    {
    /**
     * Plugin Factory
     * Owned.
     */
    CXIMPPluginFactory* iPluginFactory;

    /**
     * Feature Manager
     * Owned.
     */
    CXIMPFeatureManager* iFeatureManager;

    /**
     * Host Manager
     * Owned.
     */
    CXIMPHostManager* iHostManager;

    /** 
     * Operation factory
     * Owned.
     */
    MXIMPOperationFactory* iOperationFactory;

    /**
     * Observer for events from globals.
     * Only internal hostmanager notifications for now.
     * Not owned.
     */
    MXIMPGlobalsObserver* iGlobalsObserver;
    };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPGlobals::InstallL()
// Singleton access method.
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPGlobals::InstallL( 
        MXIMPOperationFactory* aOperationFactory,
        MXIMPGlobalsObserver* aGlobalsObserver
        )
    {
    CXIMPGlobals* singleton = ( CXIMPGlobals* ) Dll::Tls();

    if( !singleton )
        {
        singleton = CXIMPGlobals::NewLC( aOperationFactory, aGlobalsObserver );
        User::LeaveIfError( Dll::SetTls( singleton ) );
        CleanupStack::Pop( singleton );
        }
    else
        {
        User::Panic( _L("cximpglobals"), KErrAlreadyExists );
        }
    }

// -----------------------------------------------------------------------------
// CXIMPGlobals::Instance()
// Singleton access method.
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPGlobals* CXIMPGlobals::Instance()
    {
    CXIMPGlobals* singleton = ( CXIMPGlobals* ) Dll::Tls();
    if( !singleton )
        {
        User::Panic( _L("cximpglobals"), KErrNotFound );
        }

    return singleton;
    }

// -----------------------------------------------------------------------------
// CXIMPGlobals::Uninstall()
// Singleton access method.
// -----------------------------------------------------------------------------
//
EXPORT_C void CXIMPGlobals::UninstallD()
    {
    delete ( CXIMPGlobals* ) Dll::Tls();
    }

// -----------------------------------------------------------------------------
// CXIMPGlobals::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CXIMPGlobals* CXIMPGlobals::NewLC( MXIMPOperationFactory* aOperationFactory,
        MXIMPGlobalsObserver* aGlobalsObserver
        )
    {
    CXIMPGlobals* self = new( ELeave ) CXIMPGlobals;
    CleanupStack::PushL( self );
    self->ConstructL( aOperationFactory, aGlobalsObserver );
    return self;
    }

// -----------------------------------------------------------------------------
// CXIMPGlobals::CXIMPGlobals()
// -----------------------------------------------------------------------------
//
CXIMPGlobals::CXIMPGlobals()
    {
    }

// -----------------------------------------------------------------------------
// CXIMPGlobals::~CXIMPGlobals()
// -----------------------------------------------------------------------------
//
CXIMPGlobals::~CXIMPGlobals()
    {
    //Destroy global objects
    if( iGlobals )
        {
        delete iGlobals->iPluginFactory;
        delete iGlobals->iFeatureManager;
        delete iGlobals->iHostManager;
        delete iGlobals->iOperationFactory;
        }

    //And the holder
    delete iGlobals;
    Dll::SetTls( NULL );
    
    #if _BullseyeCoverage
    cov_write();
    #endif
    }

// -----------------------------------------------------------------------------
// CXIMPGlobals::ConstructL()
// -----------------------------------------------------------------------------
//
void CXIMPGlobals::ConstructL( 
        MXIMPOperationFactory* aOperationFactory,
        MXIMPGlobalsObserver* aGlobalsObserver
        )
    {
    //Reserve the object holder and zero initialize it
    iGlobals = new ( ELeave ) SGlobals;
    Mem::FillZ( iGlobals, sizeof( SGlobals ) );

    //Construct needed global objects
    iGlobals->iPluginFactory = CXIMPPluginFactory::NewL();
    iGlobals->iFeatureManager = CXIMPFeatureManager::NewL();
    iGlobals->iHostManager = CXIMPHostManager::NewL();
    if( !aOperationFactory )
        {
        User::Leave( KErrArgument );
        }

    iGlobals->iOperationFactory = aOperationFactory;
    iGlobals->iGlobalsObserver = aGlobalsObserver;
    }


EXPORT_C CXIMPPluginFactory* CXIMPGlobals::PluginFactory()
       {
       return iGlobals->iPluginFactory;
       }


EXPORT_C CXIMPFeatureManager* CXIMPGlobals::FeatureManager()
       {
       return iGlobals->iFeatureManager;
       }

EXPORT_C CXIMPHostManager* CXIMPGlobals::HostManager()
    {
    return iGlobals->iHostManager;
    }
    
EXPORT_C MXIMPOperationFactory* CXIMPGlobals::OperationFactory()
    {
    return iGlobals->iOperationFactory;
    }

EXPORT_C void CXIMPGlobals::NoMoreHosts()
    {
    // inform the observer (PscServer) about host dying.
    iGlobals->iGlobalsObserver->AllHostsDied();
    }    

// End of file


