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
* Description:  CSysApCenRepHacSettingObserver implementation
*
*/


// INCLUDE FILES
#include <HWRMDomainCRKeys.h>
#include "SysApCenRepHacSettingObserver.h"
#include "SysApAppUi.h"


// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver* CSysApCenRepHacSettingObserver::NewL()
// ----------------------------------------------------------------------------

CSysApCenRepHacSettingObserver* CSysApCenRepHacSettingObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::NewL" ) ) );
    CSysApCenRepHacSettingObserver* self = new ( ELeave ) CSysApCenRepHacSettingObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver::CSysApCenRepHacSettingObserver( )
// ----------------------------------------------------------------------------

CSysApCenRepHacSettingObserver::CSysApCenRepHacSettingObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::CSysApCenRepHacSettingObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver::~CSysApCenRepHacSettingObserver()
// ----------------------------------------------------------------------------

CSysApCenRepHacSettingObserver::~CSysApCenRepHacSettingObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepHacSettingObserver") ) );
    if ( iHacSettingHandler )
        {
        iHacSettingHandler->StopListening();
        }
    delete iHacSettingHandler;
    delete iSession;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApCenRepHacSettingObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::ConstructL: trying CRepository::NewL( KCRUidHWSettings )") ) );
    
    TRAPD( err, iSession = CRepository::NewL( KCRUidHWSettings ) );
    
    if ( err == KErrNone)
        {
        iHacSettingHandler = CCenRepNotifyHandler::NewL( *this, 
                                                     *iSession, 
                                                     CCenRepNotifyHandler::EIntKey, 
                                                     KHWSettingsHearingAidMode );
        iHacSettingHandler->StartListeningL();    
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver: CRepository::NewL failed with error %d"), err ) );
        }
    
    }

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver::HandleNotifyInt()
// ----------------------------------------------------------------------------

void CSysApCenRepHacSettingObserver::HandleNotifyInt( TUint32 /* aId */, TInt /* aNewValue */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::HandleNotifyInt()" ) ) );
    
    TRAPD( err, iSysApAppUi.SetHacIndicatorL() );
    
    if ( err )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::HandleNotifyInt: err=%d" ), err ) );
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver::HandleNotifyError()
// ----------------------------------------------------------------------------

void CSysApCenRepHacSettingObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::HandleNotifyError()" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepHacSettingObserver::HacMode()  
// ----------------------------------------------------------------------------

TInt CSysApCenRepHacSettingObserver::HacMode() const
    {
    TInt hacMode(0);
    
    if ( iSession )
        {
        if ( iSession->Get( KHWSettingsHearingAidMode, hacMode ) != KErrNone )
            {
            TRACES( RDebug::Print( _L("CSysApCenRepHacSettingObserver::HacMode ERROR: hacMode: %d"), hacMode ) );
            }    
        }
        
    return hacMode;
    }
    

// End of File

