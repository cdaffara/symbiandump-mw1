/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApLocationPrivacyIndicator implementation
 *
*/


// INCLUDES

#include "SysApLocationPrivacyIndicatorImpl.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"
#include <EPos_CPosGlobalPrivacyDb.h>
#include <EPos_TPosPrivacy.h>

#include <psvariables.h>
#include <e32property.h>

#include <centralrepository.h>
#include "CoreApplicationUIsPrivateCRKeys.h"

_LIT_SECURITY_POLICY_PASS(KLocPriReadPolicy);
_LIT_SECURITY_POLICY_S0(KLocPriWritePolicy, 0x100058F3); // sysap has access


MSysApLocationPrivacyIndicator* CreateSysApLocationPrivacyIndicatorL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApLocationPrivacyIndicator::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::NewL() 
// ----------------------------------------------------------------------------
CSysApLocationPrivacyIndicator* CSysApLocationPrivacyIndicator::NewL( CSysApAppUi& aSysApAppUi )
    {
    TRACES( RDebug::Print( _L("CSysApLocationPrivacyIndicator::NewL") ) ); 
    CSysApLocationPrivacyIndicator* self = new(ELeave) CSysApLocationPrivacyIndicator( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); // self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::ConstructL() 
// ----------------------------------------------------------------------------
void CSysApLocationPrivacyIndicator::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApLocationPrivacyIndicator::ConstructL") ) ); 

    RFs fs;
    CleanupClosePushL( fs );
    User::LeaveIfError( fs.Connect() );
    CPosGlobalPrivacyDb* db;
    db = CPosGlobalPrivacyDb::NewL( fs );
    CleanupStack::PushL( db );
    db->GetGlobalPrivacyL( iEnabled , iPrivacy );
    CleanupStack::PopAndDestroy( 2 ); // fs, db
    
    CRepository* repository = CRepository::NewL( KCRUidCoreApplicationUIsConf );
    repository->Get( KCoreAppUIsLocPrivAcceptAll, iIndicatorIdAcceptAll );
    repository->Get( KCoreAppUIsLocRejectAll, iIndicatorIdRejectAll );
    repository->Get( KCoreAppUIsLocAlwaysAsk, iIndicatorIdAlwaysAsk );
    repository->Get( KCoreAppUIsLocIndividualPrivacy, iIndicatorIdIndividualPrivacy );
    delete repository;

    // ensure that no incorrect indicators will be shown
    if ( iIndicatorIdAcceptAll < 0 )
        {
        iIndicatorIdAcceptAll = KSysApNoIndicator
        }
    if ( iIndicatorIdRejectAll < 0 )
        {
        iIndicatorIdRejectAll = SysApNoIndicator;
        }
    if ( iIndicatorIdAlwaysAsk < 0 )
        {
        iIndicatorIdAlwaysAsk = KSysApNoIndicator;
        }
    if ( iIndicatorIdIndividualPrivacy < 0 )
        {
        iIndicatorIdIndividualPrivacy = KSysApNoIndicator;
        }
    }

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::CSysApLocationPrivacyIndicator() 
// ----------------------------------------------------------------------------
CSysApLocationPrivacyIndicator::CSysApLocationPrivacyIndicator( CSysApAppUi& aSysApAppUi ) :
  iSysApAppUi( aSysApAppUi )
    {
    
    }

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::~CSysApLocationPrivacyIndicator()
// ----------------------------------------------------------------------------

CSysApLocationPrivacyIndicator::~CSysApLocationPrivacyIndicator()
    {

    }

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::InitL()
// ----------------------------------------------------------------------------

void CSysApLocationPrivacyIndicator::InitL()
    {
    TRACES( RDebug::Print( _L("CSysApLocationPrivacyIndicator::InitL") ) ); 

    RProperty property;
    property.Define( KUidSystemCategory, KPSUidLocationGlobalPrivacyValue, RProperty::EInt, KLocPriReadPolicy, KLocPriWritePolicy );
    
    if ( iEnabled )
        {
        TPosPrivacy::TVerificationPolicy policy = iPrivacy.VerificationPolicy();
        if ( policy == TPosPrivacy::EAcceptAll )
            {
            property.Set( KUidSystemCategory, KPSUidLocationGlobalPrivacyValue, EPSLocPrivAcceptAll );
            }
        else if ( policy == TPosPrivacy::EAlwaysAsk )
            {
            property.Set( KUidSystemCategory, KPSUidLocationGlobalPrivacyValue, EPSLocPrivAlwaysAsk );
            }
        else if ( policy == TPosPrivacy::ERejectAll )
            {
            property.Set( KUidSystemCategory, KPSUidLocationGlobalPrivacyValue, EPSLocPrivRejectAll );
            }
        }
    else
        {
        property.Set( KUidSystemCategory, KPSUidLocationGlobalPrivacyValue, EPSLocPrivIndividualPrivacy );
        }

    TInt state( iSysApAppUi.StateOfProperty( KUidSystemCategory, KPSUidLocationGlobalPrivacyValue ) );
    iSysApAppUi.HandleLocationPrivacyIndicatorL( state );
    }

// End of File



