    /*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

// INCLUDE FILES

#include <ecom/implementationproxy.h>
#include <sacls.h>

#include "cawidgetscannerplugin.h"
#include "cawidgetstoragehandler.h"

// Function used to return an instance of the proxy table.
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY(0x20028707, CCaWidgetScannerPlugin::NewL)
    };

#ifdef COVERAGE_MEASUREMENT
#pragma CTC SKIP
#endif //COVERAGE_MEASUREMENT (proxy for instantiation)
// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
            TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof(TImplementationProxy);
    return ImplementationTable;
    }
#ifdef COVERAGE_MEASUREMENT
#pragma CTC ENDSKIP
#endif //COVERAGE_MEASUREMENT

// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
CCaWidgetScannerPlugin::CCaWidgetScannerPlugin()
    {
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerPlugin::ConstructL( TPluginParams* aPluginParams )
    {
    User::LeaveIfError( iFs.Connect() );
    iMmcWatcher = CCaMmcWatcher::NewL( iFs, this );
    iInstallNotifier = CCaInstallNotifier::NewL( *this, 
            CCaInstallNotifier::ESisInstallNotification );
    iStorageHandler = CCaWidgetStorageHandler::NewL(
            aPluginParams->storageProxy,
            *aPluginParams->softwareRegistry, iFs );
    SynchronizeL();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
CCaWidgetScannerPlugin * CCaWidgetScannerPlugin::NewL(
        TPluginParams* aPluginParams )
    {
    CCaWidgetScannerPlugin * self = NewLC( aPluginParams );
    CleanupStack::Pop( self );
    return self;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
CCaWidgetScannerPlugin * CCaWidgetScannerPlugin::NewLC(
        TPluginParams* aPluginParams )
    {
    CCaWidgetScannerPlugin * self = new ( ELeave ) CCaWidgetScannerPlugin();
    CleanupStack::PushL( self );
    self->ConstructL( aPluginParams );
    return self;
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
CCaWidgetScannerPlugin::~CCaWidgetScannerPlugin()
    {
    delete iStorageHandler;
    delete iInstallNotifier;
    delete iMmcWatcher;
    iFs.Close();
    }

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerPlugin::SynchronizeL()
    {
    iStorageHandler->SynchronizeL();
    }


// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerPlugin::HandleInstallNotifyL()
    {
    SynchronizeL();
    }
// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void CCaWidgetScannerPlugin::MmcChangeL()
    {
    SynchronizeL();
    }

// end of file
