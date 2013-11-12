/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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


//  CLASS HEADER
#include "MT_CFSisUpgrade.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>
#include <EUnitDecorators.h>
#include <bautils.h>
#include <ecom/ecom.h>
#include <cfclient.h>
#include <cfcontextobject.h>
#include <cfcontextquery.h>
#include <e32property.h>
#include <cfcontextsubscription.h>
#include <cfcontextindication.h>
#include <cfactionindication.h>
#include <cfactionsubscription.h>

//  INTERNAL INCLUDES
#include "cftestdelay.h"
#include "cleanupresetanddestroy.h"
#include "contextsource_v1.hrh"
#include "action_v1.hrh"
#include "action_v2.hrh"
#include "actiondef_v1.h"
#include "actiondef_v2.h"
#include "operation_v1.hrh"

// CONSTANTS
const TInt KSecond = 1000000;

// CONSTRUCTION
MT_CFSisUpgrade* MT_CFSisUpgrade::NewL()
    {
    MT_CFSisUpgrade* self = MT_CFSisUpgrade::NewLC();
    CleanupStack::Pop();

    return self;
    }

MT_CFSisUpgrade* MT_CFSisUpgrade::NewLC()
    {
    MT_CFSisUpgrade* self = new( ELeave ) MT_CFSisUpgrade();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
MT_CFSisUpgrade::~MT_CFSisUpgrade()
    {
    }

// Default constructor
MT_CFSisUpgrade::MT_CFSisUpgrade()
    {
    }

// Second phase construct
void MT_CFSisUpgrade::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void MT_CFSisUpgrade::ContextIndicationL(
    const CCFContextIndication& aChangedContext )
    {
    const CCFContextObject& co = aChangedContext.Context();
    iCo->SetSourceL( co.Source() );
    iCo->SetTypeL( co.Type() );
    iCo->SetValueL( co.Value() );
    iContextIndication = ETrue;
    }


void MT_CFSisUpgrade::ActionIndicationL(
    const CCFActionIndication& aActionToExecute )
    {
    iActionId = aActionToExecute.Identifier();
    iActionIndication = ETrue;
    }


void MT_CFSisUpgrade::HandleContextFrameworkError( TCFError /*aError*/,
    const TDesC& /*aSource*/,
    const TDesC& /*aType */)
    {

    }


TAny* MT_CFSisUpgrade::Extension( const TUid& aExtensionUid ) const
    {
    TAny* extension = NULL;
    if( aExtensionUid == KCFScriptOwnerInterfaceUid )
        {
        MCFScriptOwnerInterface* interface = (MCFScriptOwnerInterface*)this;
        extension = interface;
        }
    return extension;
    }


void MT_CFSisUpgrade::HandleScriptsRemovedL( const RArray<TInt>& aScriptIds )
    {
    iScriptsRemovedNotified = ETrue;
    for( TInt i = 0; i < aScriptIds.Count(); i++ )
        {
        iScriptIds.AppendL( aScriptIds[i] );
        }
    }


void MT_CFSisUpgrade::SetupL(  )
    {
    iDelay = CCFTestDelay::NewL();

    User::LeaveIfError( iFs.Connect() );

    User::LeaveIfError( iSwInst.Connect() );

    iCFClient = CCFClient::NewL( *this );

    iCo = CCFContextObject::NewL();

    iContextIndication = EFalse;

    iActionIndication = EFalse;

    iActionId.Zero();
    }


void MT_CFSisUpgrade::Teardown(  )
    {
    iDelay->Start( KSecond * 1 );

    iScriptIds.Close();

    delete iCo;
    iCo = NULL;

    delete iCFClient;
    iCFClient = NULL;

    iSwInst.Close();

    iDelay->Start( KSecond * 1 ); // SwInstApp server closing takes some time.

    iFs.Close();

    delete iDelay;
    iDelay = NULL;
    }


void MT_CFSisUpgrade::SetInstallOptions( SwiUI::TInstallOptionsPckg& aOptions )
    {
    aOptions().iUpgrade = SwiUI::EPolicyAllowed;
    aOptions().iOCSP = SwiUI::EPolicyAllowed;
    aOptions().iIgnoreOCSPWarnings = SwiUI::EPolicyAllowed;
    aOptions().iUntrusted = SwiUI::EPolicyAllowed;
    aOptions().iPackageInfo = SwiUI::EPolicyAllowed;
    aOptions().iCapabilities = SwiUI::EPolicyAllowed;
    aOptions().iKillApp = SwiUI::EPolicyAllowed;
    aOptions().iOverwrite = SwiUI::EPolicyAllowed;
    aOptions().iDrive = 'c';
    aOptions().iUsePhoneLang = ETrue;
    aOptions().iUpgradeData = SwiUI::EPolicyAllowed;
    }


void MT_CFSisUpgrade::ResetContextObjectL()
    {
    iContextIndication = EFalse;
    iCo->SetSourceL( KNullDesC );
    iCo->SetTypeL( KNullDesC );
    iCo->SetValueL( KNullDesC );
    }


void MT_CFSisUpgrade::ResetActionL()
    {
    iActionIndication = EFalse;
    iActionId.Zero();
    }


void MT_CFSisUpgrade::ResetScriptsRemovedL()
    {
    iScriptsRemovedNotified = EFalse;
    iScriptIds.Reset();
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade1L(  )
    {
    _LIT( KSisPath, "z:\\data\\cftestdata\\testsis\\contextsource_v1\\contextsource.sisx" );
    
    CCFContextObject* co = CCFContextObject::NewLC();
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath );
    EUNIT_ASSERT_DESC( exists, "Context upgrade V1 not found" );

    // x. Install context source v1 sis file
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    TInt err = iSwInst.SilentInstall( KSisPath, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );

    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );
    
    // Check that context source v1 has been installed
    CCFContextQuery* coQuery = CCFContextQuery::NewLC();
    RContextObjectArray array;
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    CCFContextObject* requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );
    
    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueYes] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // x. Uninstall context source v1 sis file
    // -------------------------------------------------------------------------
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KContextSource_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    // For some reason uninstall fails when done for the first time
    // Binaries are cleared though...
    EUNIT_ASSERT_DESC( err == KErrNone || err == SwiUI::KSWInstErrAccessDenied,
        "Silent uninstall failed" );
    
    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );

    // Check that context source v1 has been installed
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );
    
    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueNo] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );

    // x. Clean up
    // -------------------------------------------------------------------------    
    CleanupStack::PopAndDestroy( co );
    }
    

void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade2L(  )
    {
    _LIT( KSisPath, "z:\\data\\cftestdata\\testsis\\contextsource_v1\\contextsource.sisx" );
    
    CCFContextSubscription* subscript = CCFContextSubscription::NewLC();
    subscript->SetContextSourceL( KContextSource_V1_Source );
    subscript->SetContextTypeL( KContextSource_V1_Type );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath );
    EUNIT_ASSERT_DESC( exists, "Context upgrade V1 not found" );

    // x. Install context source v1 sis file and subscribe context
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    TInt err = iSwInst.SilentInstall( KSisPath, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );

    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );
    
    // Check that context source v1 has been installed
    CCFContextQuery* coQuery = CCFContextQuery::NewLC();
    RContextObjectArray array;
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    CCFContextObject* requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueYes] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // Subscribe the context
    err = iCFClient->SubscribeContext( *subscript );
    EUNIT_ASSERT_DESC( err == KErrNone, "Context subscription failed" );
    
    // x. Uninstall context source v1 sis file and unsubscribe context
    // -------------------------------------------------------------------------
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KContextSource_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );

    // Check that context source v1 has been installed
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueNo] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // Unsubscribe context
    err = iCFClient->UnsubscribeContext( *subscript );
    EUNIT_ASSERT_DESC( err == KErrNone, "Context unsubscription failed" );

    // x. Subscribe for a context and install context source v1 sis
    //    Check that context indication is received
    // -------------------------------------------------------------------------
    // Subscribe the context
    ResetContextObjectL();
    err = iCFClient->SubscribeContext( *subscript );
    EUNIT_ASSERT_DESC( err == KErrNone, "Context subscription failed" );
    
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );

    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );
    
    // Check that context source v1 has been installed
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueYes] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // Check that context has been indicated
    EUNIT_ASSERT_DESC( iContextIndication, "Context indication not received " );
    EUNIT_ASSERT_DESC( iCo->Source() == KContextSource_V1_Source, "Invalid context source" );
    EUNIT_ASSERT_DESC( iCo->Type() == KContextSource_V1_Type, "Invalid context type" );
    EUNIT_ASSERT_DESC( iCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueYes] ), "Invalid context type" );

    // x. Uninstall context source v1 sis file and unsubscribe context
    // -------------------------------------------------------------------------
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KContextSource_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );

    // Check that context source v1 has been installed
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueNo] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // Unsubscribe context
    err = iCFClient->UnsubscribeContext( *subscript );
    EUNIT_ASSERT_DESC( err == KErrNone, "Context unsubscription failed" );

    // x. Clean up
    // -------------------------------------------------------------------------    
    CleanupStack::PopAndDestroy( subscript );
    }
    

void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade3L(  )
    {
    _LIT( KSisPath_V1,
        "z:\\data\\cftestdata\\testsis\\contextsource_v1\\contextsource.sisx" );
    _LIT( KSisPath_V2,
        "z:\\data\\cftestdata\\testsis\\contextsource_v2\\contextsource.sisx" );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Context upgrade V1 not found" );
    exists = BaflUtils::FileExists( iFs, KSisPath_V2 );
    EUNIT_ASSERT_DESC( exists, "Context upgrade V2 not found" );
    
    // x. Install context source v1 sis and verify
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    TInt err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );
    
    // Check that context source v1 has been installed
    CCFContextQuery* coQuery = CCFContextQuery::NewLC();
    RContextObjectArray array;
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    CCFContextObject* requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );
    
    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueYes] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // x. Install context source v2 sis upgrade and verify
    // -------------------------------------------------------------------------
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V2, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );
    
    // Check that context source v1 has been removed
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V1_Source );
    coQuery->SetTypeL( KContextSource_V1_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V1_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V1_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V1_Type_Values[EContextSource_V1_ValueNo] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // Check that context source v2 has been upgraded
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V2_Source );
    coQuery->SetTypeL( KContextSource_V2_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V2_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V2_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V2_Type_Values[EContextSource_V2_ValueYes] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // x. Uninstall context source v2 sis upgrade and verify
    // -------------------------------------------------------------------------
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KContextSource_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    // Wait for few seconds to make sure that installation has completed
    iDelay->Start( KSecond * 2 );
    
    // Check that context source v2 has been upgraded
    coQuery = CCFContextQuery::NewLC();
    CleanupResetAndDestroyPushL( array );
    
    coQuery->SetSourceL( KContextSource_V2_Source );
    coQuery->SetTypeL( KContextSource_V2_Type );
    err = iCFClient->RequestContext( *coQuery, array );
    EUNIT_PRINT( _L("RequestContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Request context failed" );
    EUNIT_ASSERT_DESC( array.Count() > 0, "Invalid number of contexts" );

    requestCo = array[0];
    EUNIT_PRINT( _L("RequestContext: [%S], [%S], [%S]"),
        &requestCo->Source(), &requestCo->Type(), &requestCo->Value() );

    EUNIT_ASSERT_DESC( requestCo->Source() == KContextSource_V2_Source,
        "Invalid source" );
    EUNIT_ASSERT_DESC( requestCo->Type() == KContextSource_V2_Type,
        "Invalid type" );
    EUNIT_ASSERT_DESC( requestCo->Value() == TPtrC(
        KContextSource_V2_Type_Values[EContextSource_V2_ValueNo] ),
        "Invalid value" );

    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( coQuery );
    
    // x. Clean up
    // -------------------------------------------------------------------------
    // Nothing to do
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade4L(  )
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\action_v1\\action.sisx" );
    
    CCFContextObject* co = CCFContextObject::NewLC();
    co->SetSourceL( _L("action.source") );
    co->SetTypeL( _L("action.source.v1") );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Action upgrade V1 not found" );
    
    _LIT_SECURITY_POLICY_PASS( passSec );
    TInt err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone || err == KErrAlreadyExists,
        "Define context failed" );
    
    // x. Install action v1 sis
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Test that action v1 has been installed
    // -------------------------------------------------------------------------    
    // Check that action has been initialized
    TInt value = 0;
    err = RProperty::Get( KAction_V1PSUid, KAction_V1PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V1NotTriggered, "Action v1 not triggered" );

    // Register script
    TInt scriptId = 0;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-actionV1.xml"), scriptId );
    EUNIT_PRINT( _L("RegisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );

    // Publish context
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Wait that action thread has been executed
    iDelay->Start( KSecond * 2 );
    
    // Check that action has been executed
    value = 0;
    err = RProperty::Get( KAction_V1PSUid, KAction_V1PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V1Triggered, "Action v1 not triggered" );  
    
    // x. Uninstall action v1 sis
    // -------------------------------------------------------------------------
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KAction_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    // Wait that action plug-in is fully uninstalled
    iDelay->Start( KSecond * 2 );
    
    // Publish context to double check that action is not triggered and no
    // panics occurs
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Wait that action thread has been executed
    iDelay->Start( KSecond * 2 );

    // Check that action plug-in is really uninstalled
    value = 0;
    err = RProperty::Get( KAction_V1PSUid, KAction_V1PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V1NotTriggered, "Action v1 still triggered" );    

    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( co );
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade5L(  )
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\action_v1\\action.sisx" );
    _LIT( KSisPath_V2, "z:\\data\\cftestdata\\testsis\\action_v2\\action.sisx" );
    
    CCFContextObject* co = CCFContextObject::NewLC();
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Action upgrade V1 not found" );
    exists = BaflUtils::FileExists( iFs, KSisPath_V2 );
    EUNIT_ASSERT_DESC( exists, "Action upgrade V2 not found" );

    co->SetSourceL( _L("action.source") );
    co->SetTypeL( _L("action.source.v1") );
    _LIT_SECURITY_POLICY_PASS( passSec );
    TInt err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    
    co->SetSourceL( _L("action.source") );
    co->SetTypeL( _L("action.source.v2") );
    err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    
    // x. Install action v1 sis
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Test that action v1 has been installed
    // -------------------------------------------------------------------------    
    // Check that action has been initialized
    TInt value = 0;
    err = RProperty::Get( KAction_V1PSUid, KAction_V1PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V1NotTriggered, "Action v1 not triggered" );

    // Register script for action v1
    TInt scriptId = 0;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-actionV1.xml"), scriptId );
    EUNIT_PRINT( _L("RegisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );

    // Publish context
    co->SetSourceL( _L("action.source") );
    co->SetTypeL( _L("action.source.v1") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Wait that action thread has been executed
    iDelay->Start( KSecond * 2 );
    
    // Check that action has been executed
    value = 0;
    err = RProperty::Get( KAction_V1PSUid, KAction_V1PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V1Triggered, "Action v1 not triggered" );

    // x. Install action v2 sis
    // -------------------------------------------------------------------------
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V2, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    iDelay->Start( KSecond * 2 );
    
    // x. Check that action v1 is uninstalled
    // -------------------------------------------------------------------------
    value = 0;
    err = RProperty::Get( KAction_V1PSUid, KAction_V1PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V1NotTriggered, "Action v1 still triggered" );
    
    // x. Check that action v2 is installed
    // -------------------------------------------------------------------------
    value = 0;
    err = RProperty::Get( KAction_V2PSUid, KAction_V2PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V2NotTriggered, "Action v2 not triggered" );

    // Register script for action v2
    scriptId = 0;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-actionV2.xml"), scriptId );
    EUNIT_PRINT( _L("RegisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );

    // Publish context
    co->SetSourceL( _L("action.source") );
    co->SetTypeL( _L("action.source.v2") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Wait that action thread has been executed
    iDelay->Start( KSecond * 2 );
    
    // Check that action has been executed
    value = 0;
    err = RProperty::Get( KAction_V2PSUid, KAction_V2PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V2Triggered, "Action v2 not triggered" );

    // x. Uninstall action v2 sis
    // -------------------------------------------------------------------------
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KAction_V2DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    // Wait that action plug-in is fully uninstalled
    iDelay->Start( KSecond * 2 );
    
    // Publish context to double check that action is not triggered and no
    // panics occurs
    co->SetSourceL( _L("action.source") );
    co->SetTypeL( _L("action.source.v2") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    // Wait that action thread has been executed
    iDelay->Start( KSecond * 2 );

    // Check that action plug-in is really uninstalled
    value = 0;
    err = RProperty::Get( KAction_V2PSUid, KAction_V2PSCategory, value );
    EUNIT_ASSERT_DESC( err == KErrNone, "P&S key get failed" );
    EUNIT_ASSERT_DESC( value == EAction_V2NotTriggered, "Action v2 still triggered" );
    
    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( co );
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade6L(  )
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFActionSubscription* action = CCFActionSubscription::NewLC();
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    
    // Define needed context
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    TInt err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    action->SetActionIdentifierL( _L("operationAction_v1") );
    err = iCFClient->DefineAction( action->ActionIdentifier(), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );
    err = iCFClient->SubscribeAction( *action );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );
    
    // x. Install operation v1 and check that installation is succesfull
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );
    
    // Check that the operation plug-in is installed by registering script
    // which uses the new condition operation
    TInt scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId1 > 0, "Register script failed" );
    
    // Publish context and wait for action indication
    ResetActionL();
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_PRINT( _L("PublishContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    
    iDelay->Start( KSecond * 2 );
    
    EUNIT_PRINT( _L("ActionIndication: [%d], [%S]"), iActionIndication, &iActionId );
    
    EUNIT_ASSERT_DESC( iActionIndication, "Action indication not received" );
    EUNIT_ASSERT_DESC( iActionId == _L("operationAction_v1"), "Action indication incorrect" );
        
    // x. Uninstall operation v1 and check that uninstallation is succesfull
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );
    
    // Check that script removal notification is received
    EUNIT_ASSERT_DESC( iScriptsRemovedNotified, "Script removal not notified" );
    TBool found = EFalse;
    for( TInt i = 0; i < iScriptIds.Count(); i++ )
        {
        if( iScriptIds[i] == scriptId1 )
            {
            found = ETrue;
            break;
            }
        }
    EUNIT_ASSERT_DESC( found, "Script id not found" );
    
    // Deregister script - should fail
    err = iCFClient->DeregisterScript( scriptId1 );
    EUNIT_PRINT( _L("DeregisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err != KErrNone, "Deregister script failed" );
    
    // Try to register script again - should fail
    scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err = %d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err != KErrNone, "Register script failed" );

    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( action );
    CleanupStack::PopAndDestroy( co );
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade7L(  )
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-2.xml not found" );
    
    // Define needed context
    TInt err = iCFClient->DefineContext(
        _L("operation.source"), _L("operation.type.v1"), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    err = iCFClient->DefineAction( _L("operationAction_v1"), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );

    // x. Install operation v1 and register two scripts
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // register script1
    TInt scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId1 > 0, "Register script failed" );

    // register script2
    TInt scriptId2 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml"), scriptId2 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId2 > 0, "Register script failed" );

    // x. Uninstall operation v1
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Check that a notification regarding the two registered scripts are
    //    received.
    // -------------------------------------------------------------------------
    EUNIT_ASSERT_DESC( iScriptsRemovedNotified, "Script removal not notified" );
    TBool foundScriptId1 = EFalse;
    TBool foundScriptId2 = EFalse;
    for( TInt i = 0; i < iScriptIds.Count(); i++ )
        {
        if( iScriptIds[i] == scriptId1 )
            {
            foundScriptId1 = ETrue;
            }
        else if( iScriptIds[i] == scriptId2 )
            {
            foundScriptId2 = ETrue;
            }
        }
    EUNIT_ASSERT_DESC( foundScriptId1, "Script id 1 not found" );
    EUNIT_ASSERT_DESC( foundScriptId2, "Script id 2 not found" );
    
    // x. Clean up
    // -------------------------------------------------------------------------
    // Nothing to do
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade8L()
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    
    // Define needed context
    TInt err = iCFClient->DefineContext(
        _L("operation.source"), _L("operation.type.v1"), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    err = iCFClient->DefineAction( _L("operationAction_v1"), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );

    // x. Install operation v1 and one script
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // register script1
    TInt scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId1 > 0, "Register script failed" );
    
    // x. Delete client and uninstall operation v1
    // -------------------------------------------------------------------------
    delete iCFClient;
    iCFClient = NULL;

    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Create client again and try to deregister and register script
    // -------------------------------------------------------------------------
    iCFClient = CCFClient::NewL( *this );

    // Deregister script - should fail
    err = iCFClient->DeregisterScript( scriptId1 );
    EUNIT_PRINT( _L("DeregisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err != KErrNone, "Deregister script failed" );
    
    // Try to register script again - should fail
    scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err = %d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err != KErrNone, "Register script failed" );

    // x. Clean up
    // -------------------------------------------------------------------------
    // Nothing to do
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade9L()
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    // Create another session to cfserver
    CCFClient* cfClient2 = CCFClient::NewLC( *this );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-2.xml not found" );
    
    // Define needed context
    TInt err = iCFClient->DefineContext(
        _L("operation.source"), _L("operation.type.v1"), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    err = iCFClient->DefineAction( _L("operationAction_v1"), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );    

    // x. Install operation v1
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Register script test-operationV1-1.xml from first client
    // -------------------------------------------------------------------------
    TInt scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId1 > 0, "Register script failed" );

    // x. Register script test-operationV1-2.xml from first client
    // -------------------------------------------------------------------------
    TInt scriptId2 = KErrNotFound;
    err = cfClient2->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml"), scriptId2 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId2 > 0, "Register script failed" );

    // x. Uninstall operation v1
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );
    
    // x. Check that both script removals are notified
    // -------------------------------------------------------------------------
    EUNIT_ASSERT_DESC( iScriptsRemovedNotified, "Script removal not notified" );
    TBool foundScriptId1 = EFalse;
    TBool foundScriptId2 = EFalse;
    for( TInt i = 0; i < iScriptIds.Count(); i++ )
        {
        if( iScriptIds[i] == scriptId1 )
            {
            foundScriptId1 = ETrue;
            }
        else if( iScriptIds[i] == scriptId2 )
            {
            foundScriptId2 = ETrue;
            }
        }
    EUNIT_ASSERT_DESC( foundScriptId1, "Script id 1 not found" );
    EUNIT_ASSERT_DESC( foundScriptId2, "Script id 2 not found" );

    // x. Try to deregister and register scripts again
    // -------------------------------------------------------------------------
    // Deregister script 1 - should fail
    err = iCFClient->DeregisterScript( scriptId1 );
    EUNIT_PRINT( _L("DeregisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err != KErrNone, "Deregister script failed" );
    
    // Try to register script 1 again - should fail
    scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err = %d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err != KErrNone, "Register script failed" );

    // Deregister script 2 - should fail
    err = cfClient2->DeregisterScript( scriptId2 );
    EUNIT_PRINT( _L("DeregisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err != KErrNone, "Deregister script failed" );
    
    // Try to register script 2 again - should fail
    scriptId2 = KErrNotFound;
    err = cfClient2->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml"), scriptId2 );
    EUNIT_PRINT( _L("ResgisterScript: err = %d, scriptId = %d"), err, scriptId2 );
    EUNIT_ASSERT_DESC( err != KErrNone, "Register script failed" );

    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( cfClient2 );
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade10L()
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    // Create another session to cfserver
    CCFClient* cfClient2 = CCFClient::NewLC( *this );
    
    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-2.xml not found" );
    
    // Define needed context
    TInt err = iCFClient->DefineContext(
        _L("operation.source"), _L("operation.type.v1"), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    err = iCFClient->DefineAction( _L("operationAction_v1"), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );    

    // x. Install operation v1
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Register script test-operationV1-1.xml from first client
    // -------------------------------------------------------------------------
    TInt scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId1 > 0, "Register script failed" );

    // x. Register script test-operationV1-2.xml from second client
    // -------------------------------------------------------------------------
    TInt scriptId2 = KErrNotFound;
    err = cfClient2->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-2.xml"), scriptId2 );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId2 );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId2 > 0, "Register script failed" );
    
    // Delete client 2
    CleanupStack::PopAndDestroy( cfClient2 );

    // x. Uninstall operation v1
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Check that script removal for script 1 is notified
    // -------------------------------------------------------------------------
    EUNIT_ASSERT_DESC( iScriptsRemovedNotified, "Script removal not notified" );
    TBool foundScriptId1 = EFalse;
    for( TInt i = 0; i < iScriptIds.Count(); i++ )
        {
        if( iScriptIds[i] == scriptId1 )
            {
            foundScriptId1 = ETrue;
            }
        }
    EUNIT_ASSERT_DESC( foundScriptId1, "Script id 1 not found" );

    // x. Try to deregister and register scripts again
    // -------------------------------------------------------------------------
    // Deregister script 1 - should fail
    err = iCFClient->DeregisterScript( scriptId1 );
    EUNIT_PRINT( _L("DeregisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err != KErrNone, "Deregister script failed" );
    
    // Try to register script 1 again - should fail
    scriptId1 = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId1 );
    EUNIT_PRINT( _L("ResgisterScript: err = %d, scriptId = %d"), err, scriptId1 );
    EUNIT_ASSERT_DESC( err != KErrNone, "Register script failed" );

    // x. Clean up
    // -------------------------------------------------------------------------
    // Nothing to do.
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade11L()
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT( KSisPath_V2, "z:\\data\\cftestdata\\testsis\\operation_v2\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFActionSubscription* action = CCFActionSubscription::NewLC();

    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists( iFs, KSisPath_V2 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V2 not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV2.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV2.xml not found" );
    
    // Define needed context
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    TInt err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v2") );
    err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    action->SetActionIdentifierL( _L("operationAction_v1") );
    err = iCFClient->DefineAction( action->ActionIdentifier(), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );    
    err = iCFClient->SubscribeAction( *action );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );
    
    action->SetActionIdentifierL( _L("operationAction_v2") );
    err = iCFClient->DefineAction( action->ActionIdentifier(), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );    
    err = iCFClient->SubscribeAction( *action );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );
    
    // x. Install operation v1
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Register test-operationV1-1.xml and check results
    // -------------------------------------------------------------------------
    TInt scriptId = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"), scriptId );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Register script failed" );
    
    // Publish context and check that action indication is received
    ResetActionL();
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_PRINT( _L("PublishContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    
    iDelay->Start( KSecond * 2 );
    
    EUNIT_PRINT( _L("ActionIndication: [%d], [%S]"), iActionIndication, &iActionId );
    
    EUNIT_ASSERT_DESC( iActionIndication, "Action indication not received" );
    EUNIT_ASSERT_DESC( iActionId == _L("operationAction_v1"), "Action indication incorrect" );

    // x. Upgrade operation v1 with operation v2
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V2, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // x. Check that script removal for script 1 is notified
    // -------------------------------------------------------------------------
    EUNIT_ASSERT_DESC( iScriptsRemovedNotified, "Script removal not notified" );
    TBool foundScriptId = EFalse;
    for( TInt i = 0; i < iScriptIds.Count(); i++ )
        {
        if( iScriptIds[i] == scriptId )
            {
            foundScriptId = ETrue;
            }
        }
    EUNIT_ASSERT_DESC( foundScriptId, "Script id not found" );
    
    // x. Register test-operationV2.xml and check results
    // -------------------------------------------------------------------------
    scriptId = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV2.xml"), scriptId );
    EUNIT_PRINT( _L("ResgisterScript: err =%d, scriptId = %d"), err, scriptId );
    EUNIT_ASSERT_DESC( err == KErrNone, "Register script failed" );
    EUNIT_ASSERT_DESC( scriptId > 0, "Register script failed" );

    // Publish context and check that action indication is received
    ResetActionL();
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v2") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_PRINT( _L("PublishContext: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );
    
    iDelay->Start( KSecond * 2 );
    
    EUNIT_PRINT( _L("ActionIndication: [%d], [%S]"), iActionIndication, &iActionId );
    
    EUNIT_ASSERT_DESC( iActionIndication, "Action indication not received" );
    EUNIT_ASSERT_DESC( iActionId == _L("operationAction_v2"), "Action indication incorrect" );

    // x. Uninstall operation v2
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );
    
    // x. Check that script removal for script 1 is notified
    // -------------------------------------------------------------------------
    EUNIT_ASSERT_DESC( iScriptsRemovedNotified, "Script removal not notified" );
    foundScriptId = EFalse;
    for( TInt i = 0; i < iScriptIds.Count(); i++ )
        {
        if( iScriptIds[i] == scriptId )
            {
            foundScriptId = ETrue;
            }
        }
    EUNIT_ASSERT_DESC( foundScriptId, "Script id not found" );

    // x. Try to deregister and register test-operationV2.xml
    // -------------------------------------------------------------------------
    // Deregister script 1 - should fail
    err = iCFClient->DeregisterScript( scriptId );
    EUNIT_PRINT( _L("DeregisterScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err != KErrNone, "Deregister script failed" );
    
    // Try to register script 1 again - should fail
    scriptId = KErrNotFound;
    err = iCFClient->RegisterScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV2.xml"), scriptId );
    EUNIT_PRINT( _L("ResgisterScript: err = %d, scriptId = %d"), err, scriptId );
    EUNIT_ASSERT_DESC( err != KErrNone, "Register script failed" );

    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( action );
    CleanupStack::PopAndDestroy( co );
    }


void MT_CFSisUpgrade::MT_CFSisUpgrade_InstallCfSisUpgrade12L()
    {
    _LIT( KSisPath_V1, "z:\\data\\cftestdata\\testsis\\operation_v1\\operation.sisx" );
    _LIT( KSisPath_V3, "z:\\data\\cftestdata\\testsis\\operation_v3\\operation.sisx" );
    _LIT_SECURITY_POLICY_PASS( passSec );
    
    CCFContextObject* co = CCFContextObject::NewLC();
    CCFActionSubscription* action = CCFActionSubscription::NewLC();

    // x. Preconditions
    // -------------------------------------------------------------------------
    TBool exists = BaflUtils::FileExists( iFs, KSisPath_V1 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V1 not found" );
    exists = BaflUtils::FileExists( iFs, KSisPath_V3 );
    EUNIT_ASSERT_DESC( exists, "Operation upgrade V3 not found" );

    exists = BaflUtils::FileExists(
        iFs, _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_ASSERT_DESC( exists, "test-operationV1-1.xml not found" );
    
    // Define needed context
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    TInt err = iCFClient->DefineContext( co->Source(), co->Type(), passSec, passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define context failed" );
    
    // Define needed action and subscribe it
    action->SetActionIdentifierL( _L("operationAction_v1") );
    err = iCFClient->DefineAction( action->ActionIdentifier(), passSec );
    EUNIT_ASSERT_DESC( err == KErrNone, "Define action failed" );    
    err = iCFClient->SubscribeAction( *action );
    EUNIT_ASSERT_DESC( err == KErrNone, "Subscribe action failed" );    

    // x. Install operation v1
    // -------------------------------------------------------------------------
    SwiUI::TInstallOptionsPckg instOptionsPckg;
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V1, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );
    
    // x. Save script test-operationV1-1.xml
    // -------------------------------------------------------------------------
    TInt scriptId = KErrNotFound;
    err = iCFClient->SaveScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml"),
        scriptId );
    EUNIT_PRINT( _L("SaveScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Save script failed" );
    
    // check that script is really saved and activated
    ResetActionL();
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_PRINT( _L("PublishContext: err = %d" ), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    iDelay->Start( KSecond * 2 );
    
    // check that action indication is received
    EUNIT_PRINT( _L("ActionIndication: [%d], [%S]"), iActionIndication, &iActionId );
    
    EUNIT_ASSERT_DESC( iActionIndication, "Action indication not received" );
    EUNIT_ASSERT_DESC( iActionId == _L("operationAction_v1"), "Action indication incorrect" );

    // x. Install operation v3
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SetInstallOptions( instOptionsPckg );
    err = iSwInst.SilentInstall( KSisPath_V3, instOptionsPckg );
    EUNIT_PRINT( _L("SilentInstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // check that script removal is not informed
    EUNIT_ASSERT_DESC( !iScriptsRemovedNotified, "Script removal notification error" );
    
    // check that script has been automatically re-loaded
    ResetActionL();
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_PRINT( _L("PublishContext: err = %d" ), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    iDelay->Start( KSecond * 2 );
    
    // check that action indication is received
    EUNIT_PRINT( _L("ActionIndication: [%d], [%S]"), iActionIndication, &iActionId );
    
    EUNIT_ASSERT_DESC( iActionIndication, "Action indication not received" );
    EUNIT_ASSERT_DESC( iActionId == _L("operationAction_v1"), "Action indication incorrect" );

    // x. Uninstall operation v3
    // -------------------------------------------------------------------------
    ResetScriptsRemovedL();
    SwiUI::TUninstallOptionsPckg uninstOptionsPckg;
    uninstOptionsPckg().iKillApp = SwiUI::EPolicyAllowed;
    uninstOptionsPckg().iBreakDependency = SwiUI::EPolicyAllowed;
    err = iSwInst.SilentUninstall( TUid::Uid( KOperation_V1DllUid ),
        uninstOptionsPckg, SwiUI::KSisxMimeType );
    EUNIT_PRINT( _L("SilentUninstall: err = %d"), err );
    
    iDelay->Start( KSecond * 2 );

    // check that script removal is not informed
    EUNIT_ASSERT_DESC( !iScriptsRemovedNotified, "Script removal notification error" );
    
    // delete script
    err = iCFClient->DeleteScript(
        _L("z:\\data\\cftestdata\\testscripts\\test-operationV1-1.xml") );
    EUNIT_PRINT( _L("DeleteScript: err = %d"), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Delete script failed" );

    // check that script has been correctly deleted
    ResetActionL();
    co->SetSourceL( _L("operation.source") );
    co->SetTypeL( _L("operation.type.v1") );
    co->SetValueL( _L("true") );
    err = iCFClient->PublishContext( *co );
    EUNIT_PRINT( _L("PublishContext: err = %d" ), err );
    EUNIT_ASSERT_DESC( err == KErrNone, "Publish context failed" );

    iDelay->Start( KSecond * 2 );
    
    EUNIT_ASSERT_DESC( !iActionIndication, "Action indication still received" );

    // x. Clean up
    // -------------------------------------------------------------------------
    CleanupStack::PopAndDestroy( action );
    CleanupStack::PopAndDestroy( co );
    }


//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    MT_CFSisUpgrade,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "Context Source Sis - 1",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade1L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade1L, Teardown)
    
EUNIT_TEST(
    "Context Source Sis - 2",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade2L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade2L, Teardown)
    
EUNIT_TEST(
    "Context Source Sis - 3",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade3L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade3L, Teardown)
    
EUNIT_TEST(
    "Action Sis - 1",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade4L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade4L, Teardown)
    
EUNIT_TEST(
    "Action Sis - 2",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade5L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade5L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 1",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade6L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade6L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 2",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade7L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade7L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 3",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade8L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade8L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 4",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade9L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade9L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 5",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade10L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade10L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 6",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade11L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade11L, Teardown)
    
EUNIT_TEST(
    "Operation Sis - 7",
    "",
    "MT_CFSisUpgrade_InstallCfSisUpgrade12L",
    "FUNCTIONALITY",
    SetupL, MT_CFSisUpgrade_InstallCfSisUpgrade12L, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
