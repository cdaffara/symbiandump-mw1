/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApSSSettingsObserver implementation.
*
*/


// INCLUDE FILES
#include "SysApSsSettingsObserver.h"
#include "SysApAppUi.h"
#include <avkon.hrh>
#include <rsssettings.h>

//CONSTANTS

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApSsSettingsObserver* CSysApSsSettingsObserver::NewL( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApSsSettingsObserver* CSysApSsSettingsObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApSsSettingsObserver::CSysApSsSettingsObserver::NewL" ) ) );
    CSysApSsSettingsObserver* self = new ( ELeave ) CSysApSsSettingsObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApSsSettingsObserver::CSysApSsSettingsObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------

CSysApSsSettingsObserver::CSysApSsSettingsObserver( CSysApAppUi& aSysApAppUi ) :
    iSysApAppUi( aSysApAppUi )
    {
    }

// ----------------------------------------------------------------------------
// CSysApSsSettingsObserver::~CSysApSsSettingsObserver()
// ----------------------------------------------------------------------------

CSysApSsSettingsObserver::~CSysApSsSettingsObserver()
    {
    TRACES( RDebug::Print( _L("CSysApSsSettingsObserver::~CSysApSsSettingsObserver") ) );
    iSysApSsSettings.CancelAll( *this );
    iSysApSsSettings.Close();
    }

// ----------------------------------------------------------------------------
// CSysApSsSettingsObserver::PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue )
// ----------------------------------------------------------------------------

void CSysApSsSettingsObserver::PhoneSettingChanged( TSSSettingsSetting aSetting, TInt aNewValue )
    {
    TRACES( RDebug::Print( _L("CSysApSsSettingsObserver::PhoneSettingChanged( aSetting:%d, aNewValue:%d )"), aSetting, aNewValue ) );
    if( aSetting == ESSSettingsAls )
        {
        TRAPD( err, SetSecondLineIndicatorL( aNewValue ) );
        if ( err ) err = 0; // hide compiler warning about not using variable
        }
    }

// ----------------------------------------------------------------------------
// CSysApSsSettingsObserver::ConstructL()
// ----------------------------------------------------------------------------

void CSysApSsSettingsObserver::ConstructL(  )
    {
    User::LeaveIfError(iSysApSsSettings.Open(iSysApAppUi.GetTelServer()));
    iSysApSsSettings.Register( ESSSettingsAls, *this );
    TSSSettingsSetting ssSetting( ESSSettingsAls );
    TInt ssSettingAlsValue( 0 );
    TInt err( iSysApSsSettings.Get( ssSetting, ssSettingAlsValue ) );
    if( err == KErrNone )
        {
        SetSecondLineIndicatorL( ssSettingAlsValue );
        }
    }

// ----------------------------------------------------------------------------
// CSysApSsSettingsObserver::SetSecondLineIndicatorL( TInt aSsSettingsAlsLineValue )
// ----------------------------------------------------------------------------

void CSysApSsSettingsObserver::SetSecondLineIndicatorL( const TInt aSsSettingsAlsLineValue ) const
    {
    TRACES( RDebug::Print( _L("CSysApSsSettingsObserver::SetSecondLineIndicatorL( aSsSettingsAlsLineValue:%d, "), aSsSettingsAlsLineValue ) );
    if( aSsSettingsAlsLineValue == ESSSettingsAlsAlternate )
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorSelectedLine, EAknIndicatorStateOn );        
        }
    else 
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorSelectedLine, EAknIndicatorStateOff );        
        } 
    }

// End of File
