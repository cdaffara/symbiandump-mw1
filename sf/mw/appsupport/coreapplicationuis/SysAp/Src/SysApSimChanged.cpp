/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CSysApSimChanged
*
*/


// INCLUDE FILES
#include "SysApSimChanged.h"
#include "SysApAppUi.h"
#include <settingsinternalcrkeys.h>
//#include <LogsDomainCRKeys.h>
#include <logcli.h>
#include <centralrepository.h>
const TInt KPSetDefaultCFTimer = 30;
#include <rsssettings.h>
#include <startupdomainpskeys.h>
#include <PSVariables.h>
#include "SysAp.hrh"
//  LOCAL CONSTANTS AND MACROS  

   
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSysApSimChanged::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------

CSysApSimChanged* CSysApSimChanged::NewL( CSysApAppUi& aSysApAppUi, RFs& aFs )
    {
    TRACES( RDebug::Print( _L("CSysApSimChanged::NewL()") ) );
    CSysApSimChanged* self = new(ELeave) CSysApSimChanged(aSysApAppUi, aFs );
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApSimChanged::CSysApSimChanged( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApSimChanged::CSysApSimChanged( CSysApAppUi& aSysApAppUi, RFs& aFs ) :
    iSysApAppUi( aSysApAppUi ),
    iFs( aFs )
    {
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------

CSysApSimChanged::~CSysApSimChanged()
    {
    TRACES( RDebug::Print( _L("CSysApSimChanged::~CSysApSimChanged()") ) );
    }

// -----------------------------------------------------------------------------
// CSysApSimChanged::ClearRepositoriesL()
// -----------------------------------------------------------------------------

void CSysApSimChanged::ClearRepositoriesL()
    {
    const TUid KCRUidLogs = {0x101F874E};
    /**
    * Informs the Logs application about the amount of new missed calls.
    * Integer type
    **/
    const TUint32 KLogsNewMissedCalls = 0x00000006;
    TRACES( RDebug::Print( _L("CSysApSimChanged::ClearRepositoriesL(): opening KCRUidCallForwarding") ) );
    
    CRepository* repository = CRepository::NewL( KCRUidCallForwarding );
    
    repository->Set( KSettingsCFDefaultNumber1, KNullDesC );
    repository->Set( KSettingsCFDefaultNumber2, KNullDesC );
    repository->Set( KSettingsCFDefaultNumber3, KNullDesC );
    repository->Set( KSettingsCFDefaultNumberAls1, KNullDesC );
    repository->Set( KSettingsCFDefaultNumberAls2, KNullDesC );
    repository->Set( KSettingsCFDefaultNumberAls3, KNullDesC );
    repository->Set( KSettingsCFUsedFaxNumber, KNullDesC );
    repository->Set( KSettingsCFUsedDataNumber, KNullDesC );
    repository->Set( KSettingsCFTimerValue, KPSetDefaultCFTimer );
    repository->Set( KSettingsCFTimerValueAls, KPSetDefaultCFTimer );
    
    delete repository;
    
    TRACES( RDebug::Print( _L("CSysApSimChanged::ClearRepositoriesL(): opening KCRUidLogs") ) );
    repository = CRepository::NewL( KCRUidLogs );
    
    repository->Set( KLogsNewMissedCalls, 0 );

    delete repository;
    }
// -----------------------------------------------------------------------------
// CSysApSimChanged::ClearLogsL()
// -----------------------------------------------------------------------------

void CSysApSimChanged::ClearLogsL()
    {
    TRACES( RDebug::Print( _L("CSysApSimChanged::ClearLogsL()") ) );
    // Clear the event log.
    CShareActive* active = new (ELeave) CShareActive;
    CleanupStack::PushL( active );
    active->StartL();
    
    TDateTime dateTime;
    dateTime.Set( 2100, EJanuary, 1, 0, 0, 0, 0 );
    TTime date(dateTime);
    
/*    TRACES( RDebug::Print( _L("CSysApSimChanged::ClearLogsL(): trying CLogClient::NewL") ) );
    CLogClient* logClient = CLogClient::NewL( iFs );    
    logClient->ClearLog( date, active->iStatus );
*/    
    CActiveScheduler::Start();

//    delete logClient;
    CleanupStack::PopAndDestroy( active );
    }
    
// -----------------------------------------------------------------------------
// CSysApSimChanged::ClearSsSettingsL()
// -----------------------------------------------------------------------------

void CSysApSimChanged::ClearSsSettingsL()
    {
    TRACES( RDebug::Print( _L("CSysApSimChanged::ClearSsSettingsL()") ) );
    
    // Reset SSSettings values
/*    RSSSettings ssSettings;
    User::LeaveIfError( ssSettings.Open( iSysApAppUi.GetTelServer() ) );
    
    TRACES( RDebug::Print( _L("CSysApSimChanged::ClearSsSettingsL(): trying RSSSettings::HandleSIMChanged") ) );
    TInt err = ssSettings.HandleSIMChanged();
    ssSettings.Close();    
    
    User::LeaveIfError( err );
*/
    }

// -----------------------------------------------------------------------------
// CSysApSimChanged::HandleSimChangedL()
// -----------------------------------------------------------------------------
    
void CSysApSimChanged::HandleSimChangedL()
    {
    TBool simChanged;
    TBool simNotOwned;

    simChanged = ( iSysApAppUi.StateOfProperty( KPSUidStartup, KPSSimChanged ) == ESimChanged );
    simNotOwned = ( iSysApAppUi.StateOfProperty( KPSUidStartup, KPSSimOwned ) == ESimNotOwned );
    
    TRACES( RDebug::Print( _L("CSysApSimChanged::HandleSimChangedL(): simChanged=%d, simNotOwned=%d"), simChanged, simNotOwned ) );

    // Supplementary settings are always cleared when SIM has been changed, regardless whether SIM is known or not
    if ( simChanged || simNotOwned )
        {
        ClearSsSettingsL();
        }
    
    // Other repositories are cleared only if an unknown SIM has been inserted    
    if ( simNotOwned )
        {
        ClearRepositoriesL();
//        ClearLogsL();
        }
    }
// =============== CShareActive MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// These are needed for ClogClient.
// -----------------------------------------------------------------------------

CShareActive::CShareActive() : CActive(0)
    {
    CActiveScheduler::Add(this);
    }

CShareActive::~CShareActive()
    {
    Cancel();
    }

void CShareActive::DoCancel()
    {
    TRequestStatus* s=&iStatus;
    User::RequestComplete(s, KErrNone);
    }

void CShareActive::StartL()
    {
    SetActive();
    }

void CShareActive::RunL() 
    {
    TRACES( RDebug::Print( _L("CSysApSimChanged::CShareActive::RunL: iStatus=%d"), iStatus.Int() ) );
    CActiveScheduler::Stop();
    }

//  End of File



