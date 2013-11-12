/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


// INCLUDE FILES
#include <cfcontextinterface.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>
#include <cfcontextsourcesettingparameter.h>
#include <cfkeyvaluepair.h>
#include <centralrepository.h>
#include <apgwgnam.h>
#include <applicationorientation.h>
#include <aknappui.h>
#include <coedef.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <vwsdefpartner.h>
#endif
#include "ApplicationStateSourcePlugin.h"
#include "ApplicationStateSourceContextDef.h"
#include "ContextFrameworkPrivateCRKeys.h"
#include "trace.h"

// CONSTANTS

#ifdef _DEBUG
// Panic codes
enum TPanicCode
    {
    EInvalidUid,
    EInvalidAttribute
    };
#endif

_LIT( KDefaultApplicationName,  "DefaultAppName" );
_LIT( KDefaultViewName,         "DefaultViewName" );
_LIT( KAttributeAppUid,         "AppUid" );
_LIT( KAttributeAppName,        "AppName" );
_LIT( KTagView,                 "View" );
_LIT( KAttributeUid,            "Uid" );
_LIT( KAttributeName,           "Name" );

const TUint KLimit = 0xFFFFFFFF;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::CApplicationStateSourcePlugIn
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CApplicationStateSourcePlugIn::CApplicationStateSourcePlugIn( TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams ),
    iDefStateSetManyTimes( 0 )
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::ConstructL
// Symbian second-phase constructor that may leave.
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::ConstructL()
    {
    FUNC_LOG;

    iDefaultAppName = KApplicationStateDefaultValue().AllocL();
    iDefaultViewName = KApplicationStateDefaultValue().AllocL();
    iContext = CCFContextObject::NewL();
    iWsEventHandler = CWsEventHandler::NewL( *this );
    iWsEventHandler->IssueRequest();
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CApplicationStateSourcePlugIn* CApplicationStateSourcePlugIn::NewL(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CApplicationStateSourcePlugIn* self = CApplicationStateSourcePlugIn::NewLC(
        aParams );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CApplicationStateSourcePlugIn* CApplicationStateSourcePlugIn::NewLC(
    TContextSourceInitParams* aParams )
    {
    FUNC_LOG;

    CApplicationStateSourcePlugIn* self = new( ELeave ) CApplicationStateSourcePlugIn(
        aParams );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor
CApplicationStateSourcePlugIn::~CApplicationStateSourcePlugIn()
    {
    FUNC_LOG;

    if( iCRAppOrientationListener )
        {
        iCRAppOrientationListener->StopListening();
        delete iCRAppOrientationListener;
        }
    iUidOrientationList.Close();    
    delete iCRAppOrientation;
    iApplicationSettings.ResetAndDestroy();
    delete iSettings;
    
    delete iDefaultViewName;
    delete iDefaultAppName;
    delete iApplDefaultState;
    delete iContext;
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::HandleSettingL
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* aSettingList )
    {
    FUNC_LOG;

    // Take ownership from settings
    iSettings = aSettingList;
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::DefineContextsL
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::DefineContextsL()
    {
    FUNC_LOG;

    // Define contexts here
    _LIT_SECURITY_POLICY_C2(
        appStatePolicy, ECapabilitySwEvent, ECapabilityUserEnvironment );

    TInt err = iCF.DefineContext( KApplicationStateSource,
       KApplicationStateForegroundApplicationType,
       appStatePolicy );
    User::LeaveIfError( err );
    
    err = iCF.DefineContext( KApplicationStateSource,
       KApplicationStateForegroundApplicationViewType,
       appStatePolicy );
    User::LeaveIfError( err );

    err = iCF.DefineContext( KApplicationStateSource,
       KApplicationStateBackgroundApplicationType,
       appStatePolicy );
    User::LeaveIfError( err );
    
    err = iCF.DefineContext( KApplicationStateSource,
        KFgApplicationOrientation,
        KFgApplicationOrientationSec );
    User::LeaveIfError( err );
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::InitializeL
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::InitializeL()
    {
    FUNC_LOG;

    iContext->SetSourceL( KApplicationStateSource );

    // Get default app orientation
    CRepository* cenRep = CRepository::NewL( KCRUidCFApplDefaultState );
    TInt err = cenRep->Get( KApplDefaultState, iDefaultFgAppOrientation );
    if( err != KErrNone )
        {
        ERROR( err, "Failed to get default fg app orientation" );
        iDefaultFgAppOrientation = 0;
        }
    delete cenRep;
    cenRep = NULL;

    // Create KCRUidDefaultAppOrientation repository for default app orientation
    iCRAppOrientation = CRepository::NewL( KCRUidDefaultAppOrientation ); 
    
    // Create KCRUidDefaultAppOrientation repository listener
    iCRAppOrientationListener = CCenRepNotifyHandler::NewL(
        *this, *iCRAppOrientation );
    iCRAppOrientationListener->StartListeningL();
        
    if( iSettings )
        {
        InitializeApplicationsFromXMLL();
        InitializeFgApplicationL();

        // Free memory allocated for settings since we don't need them anymore
        delete iSettings;
        iSettings = NULL;
        }

    
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::InitializeApplicationsFromXMLL
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::InitializeApplicationsFromXMLL()
    {
    FUNC_LOG;

     for( TInt i = 0; i < iSettings->Count(); ++i )
        {
        // Check attributes
        TBool defaultValueDefinitions = EFalse;
        TPtrC appUid( KNullDesC );
        TPtrC appName( KNullDesC );
        const RKeyValueArray& attributes = iSettings->Item( i ).Attributes();

        // Check application and view names (also default values)
        for( TInt j = 0; j < attributes.Count(); ++j )
            {
            TPtrC key( attributes[ j ]->Key() );
            TPtrC value( attributes[ j ]->Value() );

            if( key.CompareF( KAttributeAppUid ) == KErrNone )
                {
                appUid.Set( value );
                }
            else if( key.CompareF( KAttributeAppName ) == KErrNone )
                {
                appName.Set( value );
                }
            else if( key.CompareF( KDefaultApplicationName ) == KErrNone )
                {
                delete iDefaultAppName;
                iDefaultAppName = NULL;
                iDefaultAppName = value.AllocL();
                defaultValueDefinitions = ETrue;
                }
            else if( key.CompareF( KDefaultViewName ) == KErrNone )
                {
                delete iDefaultViewName;
                iDefaultViewName = NULL;
                iDefaultViewName = value.AllocL();
                defaultValueDefinitions = ETrue;
                }
            }

        if ( defaultValueDefinitions )
            {
            continue; // Default value definitions contain only attributes.
            }

        if ( appUid.Length() == 0 || appName.Length() == 0 )
            {
            ERROR_GEN( "Either AppUid or AppName tag (or both) not found, skipping" );
            continue; // Skip this definition.
            }

        TUint32 uid = 0;
        TInt err = ConvertToHex( appUid, uid );
        if( err != KErrNone )
            {
            ERROR_1( err, "Invalid uid [%S] for an application, skipping", &appUid );
            continue; // Skip this definition.
            }

        // below:                                  CLEANUP<< appStateSettings
        CCFApplicationStateSettings* appStateSettings
            = CCFApplicationStateSettings::NewLC( TUid::Uid( uid ), appName );

        // Check parameters
        const RSettingParameterArray& parameters = iSettings->Item( i ).Parameters();
        for( TInt j = 0; j < parameters.Count(); ++j )
            {
            if ( parameters[ j ]->Name() != KTagView )
                {
                continue; // At the moment, handle only view tags
                }

            TPtrC viewUid( KNullDesC );
            TPtrC viewName( KNullDesC );
            const RKeyValueArray& paramAttr = parameters[ j ]->Attributes();
            for( TInt k = 0; k < paramAttr.Count(); ++k )
                {
                TPtrC key( paramAttr[ k ]->Key() );
                TPtrC value( paramAttr[ k ]->Value() );

                // Check view uid
                if( key.CompareF( KAttributeUid ) == KErrNone )
                    {
                    viewUid.Set( value );
                    }
                // Check view name
                else if( key.CompareF( KAttributeName ) == KErrNone )
                    {
                    viewName.Set( value );
                    }
                }

            if ( viewUid.Length() == 0 || viewName.Length() == 0 )
                {
                ERROR_GEN( "Either View's Uid or Name tag (or both) not found, skipping" );
                continue; // Skip this definition.
                }

            uid = 0;
            TInt err = ConvertToHex( viewUid, uid );
            if( err != KErrNone )
                {
                ERROR_1( err, "Invalid view uid [%S], skipping", &viewUid );
                continue; // Skip this definition.
                }

            // below:                              CLEANUP<< viewUidName
            CCFUidNamePair* viewUidName
                = CCFUidNamePair::NewLC( TUid::Uid( uid ), viewName );
            appStateSettings->AddViewSettingsL( viewUidName );
            CleanupStack::Pop( viewUidName );   // CLEANUP>> viewUidName
            }

        iApplicationSettings.AppendL( appStateSettings );
        CleanupStack::Pop( appStateSettings );  // CLEANUP>> appStateSettings
        }
    }



//------------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::HandleNotifyGeneric
//------------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::HandleNotifyGeneric( TUint32 aId )
    {
    FUNC_LOG;

    // Get the new value from cenrep
    TInt val = 0;
    TInt err = iCRAppOrientation->Get( aId, val );
    if( err == KErrNone )
        {
        // New value found, check if the element is found from cache
        TInt index = FindByUid( TUid::Uid( aId ) );
        if( index != KErrNotFound )
            {
            // Update cache element
            TUidOrientationPair& pair = iUidOrientationList[index];
            pair.iOrientation = val;
            INFO_2( "Updated cache uid: [0x%x] to orientation: [%d]", pair.iUid.iUid, val );
            }
        else
            {
            // Add new cache element
            TUidOrientationPair pair( TUid::Uid( aId ), val );
            err = iUidOrientationList.Append( pair );
            if( err == KErrNone )
                {
                INFO_2( "Added cache uid: [0x%x], orientation: [%d]", pair.iUid.iUid, val );
                }
            else
                {
                ERROR_1( err, "Failed to add cache uid: [0x%x]", pair.iUid.iUid );
                }
            }
        }
    else
        {
        ERROR_1( err, "Failed to get new value from CR key: [0x%x]", aId );
        }
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::DoHandleViewEventL
// Maps view activations to contexts.
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::DoHandleViewEventL(
    const TVwsViewEvent& aEvent )
    {
    FUNC_LOG;

    TPtrC bgApp( *iDefaultAppName );
    TPtrC fgApp( *iDefaultAppName );
    TPtrC fgView( *iDefaultViewName );
    TBool bgFound( EFalse );
    TBool fgFound( EFalse );

    if ( aEvent.iViewTwoId.iAppUid == TUid::Null() )
        {
        // Previous foreground application was closed, get the stored name for
        // background application's name.
        bgFound = ETrue;
        bgApp.Set( iPreviousForegroundApplication );
        }

    for ( TInt i = 0; i < iApplicationSettings.Count(); ++i )
        {
        CCFApplicationStateSettings* appSettings = iApplicationSettings[ i ];
        if ( !fgFound && aEvent.iViewOneId.iAppUid == appSettings->Uid() )
            {
            fgApp.Set( appSettings->Name() );
            appSettings->GetViewName( aEvent.iViewOneId.iViewUid, fgView );
            fgFound = ETrue;
            }
        if ( !bgFound && aEvent.iViewTwoId.iAppUid == appSettings->Uid() )
            {
            bgApp.Set( appSettings->Name() );
            bgFound = ETrue;
            }
        if ( bgFound && fgFound )
            {
            break;
            }
        }

    TBool publishFgApp( EFalse );
    TBool publishFgView( EFalse );
    TBool publishBgApp( EFalse );

    if ( iPreviousForegroundApplication.Compare( fgApp ) != 0 )
        {
        publishFgApp = ETrue;
        }
    else if ( iPreviousForegroundView.Compare( fgView ) != 0 )
        {
        publishFgView = ETrue;
        }

    if ( bgApp != fgApp )
        {
        publishBgApp = ETrue;
        }

    iPreviousForegroundApplication.Set( fgApp ); // Store for next round
    iPreviousForegroundView.Set( fgView );

    // Publish foreground application and view contexts.
    RThread thread;

    if ( publishFgApp )
        {
        iContext->SetTypeL( KApplicationStateForegroundApplicationType );
        iContext->SetValueL( fgApp );
        iCF.PublishContext( *iContext, thread );
        }

    if ( publishFgApp || publishFgView )
        {
        iContext->SetTypeL( KApplicationStateForegroundApplicationViewType );
        iContext->SetValueL( fgView );
        iCF.PublishContext( *iContext, thread );
        }

    if ( publishBgApp )
        {
        iContext->SetTypeL( KApplicationStateBackgroundApplicationType );
        iContext->SetValueL( bgApp );
        iCF.PublishContext( *iContext, thread );
        }
    
    // Publish foreground application orientation context
    TPtrC fgAppOrientation( ResolveFgAppOrientation( aEvent.iViewOneId.iAppUid ) );
    if( fgAppOrientation.Length() )
        {
        // Fg app found
        if( fgAppOrientation.Compare( iPreviousOrientation ) != KErrNone )
            {
            // Update for next round
            iPreviousOrientation.Set( fgAppOrientation );
            
            // Fg app orientation changed, publish new context
            iContext->SetTypeL( KFgApplicationOrientation );
            iContext->SetValueL( fgAppOrientation );
            iCF.PublishContext( *iContext, thread );
            }
        }

    thread.Close();
    }

//------------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::ResolveFgAppOrientation
//------------------------------------------------------------------------------
//
TPtrC CApplicationStateSourcePlugIn::ResolveFgAppOrientation(
    const TUid& aUid )
    {
    FUNC_LOG;
    
    TPtrC fgAppOrientation( KNullDesC );
    TInt val = 0;
    
    // Check cache first
    TInt index = FindByUid( aUid );
    if( index != KErrNotFound )
        {
        // App found from cache
        val = iUidOrientationList[index].iOrientation;
        }
    else
        {
        // Get the SID from cenrep and update cache
        TInt err = iCRAppOrientation->Get( aUid.iUid, val );
        if( err != KErrNone )
            {
            INFO_1( "Application uid: [0x%x] not found, using default orientation",
                aUid.iUid );
            val = iDefaultFgAppOrientation;
            }
        
        // Update cache
        TUidOrientationPair pair( aUid, val );
        err = iUidOrientationList.Append( pair );
        if( err != KErrNone )
            {
            ERROR_2( err, "Failed to add uid: [0x%x], orientation: [%d] pair in cache",
                aUid.iUid, val );
            }
        }
    
    // Resolve context value
    if( val == CAknAppUiBase::EAppOrientationPortrait )
        {
        // Portrait
        fgAppOrientation.Set( TPtrC(
            KFgApplicationOrientationVal[EFgApplicationOrientationPortrait] ) );
        }
    else if( val == CAknAppUiBase::EAppOrientationLandscape )
        {
        // Landscape
        fgAppOrientation.Set( TPtrC(
            KFgApplicationOrientationVal[EFgApplicationOrientationLandscape] ) );
        }
    else
        {
        // Automatic
        fgAppOrientation.Set( TPtrC(
            KFgApplicationOrientationVal[EFgApplicationOrientationAutomatic] ) );
        }
    
    return fgAppOrientation;
    }

// -----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::InitializeFgApplicationL
// -----------------------------------------------------------------------------
//
void CApplicationStateSourcePlugIn::InitializeFgApplicationL()
    {
    FUNC_LOG;

    RWsSession wsSession;
    User::LeaveIfError( wsSession.Connect() );
    CleanupClosePushL( wsSession );
    TUid appUid( TUid::Null() );

    TInt focScr( wsSession.GetFocusScreen() );
    TInt focWndGrp( wsSession.GetFocusWindowGroup() );
    TInt wndGrpPrio( wsSession.GetWindowGroupOrdinalPriority( focWndGrp ) );
    
    INFO_1( "CApplicationStateSourcePlugIn::focScr     = [%d]", focScr );
    INFO_1( "CApplicationStateSourcePlugIn::focWndGrp  = [%d]", focWndGrp );
    INFO_1( "CApplicationStateSourcePlugIn::wndGrpPrio = [%d]", wndGrpPrio );

    CApaWindowGroupName* wndGrpName = CApaWindowGroupName::NewL( wsSession );
    CleanupStack::PushL( wndGrpName );

    wndGrpName->ConstructFromWgIdL( focWndGrp );

    TPtrC appCaption = wndGrpName->Caption();
    TPtrC appName = wndGrpName->AppUid().Name();

    // if focScr == 0 it's not CoverUi
    // if focScr != 0 it's CoverUi

    // if wndGrpPrio == 0 it's S60 application

    if ( ( focScr != 0 ) || ( ( focScr == 0 ) && ( wndGrpPrio == 0 ) ) )
        {
        INFO( "CApplicationStateSourcePlugIn::It's CoverUi or S60 application" );
        // It is CoverUi
        // or
        // It is Not CoverUi and it is S60 application

        appUid = wndGrpName->AppUid();
        INFO_1( "appUid = [0x%x]", appUid.iUid );
        }
    else
        {
        // Default values will still be published
        TUid tmpAppUid( TUid::Null() );
        tmpAppUid = wndGrpName->AppUid();

        INFO( "CApplicationStateSourcePlugIn::It's neiher CoverUi nor S60 application" );
        INFO_1( "appUid = [0x%x]", tmpAppUid.iUid );
        }

    INFO_1( "appCaption = [%S]", &appCaption );
    INFO_1( "appName    = [%S]", &appName );

    CleanupStack::PopAndDestroy( wndGrpName );
    CleanupStack::PopAndDestroy( &wsSession );

    TPtrC bgApp( *iDefaultAppName );
    TPtrC fgApp( *iDefaultAppName );
    TPtrC fgView( *iDefaultViewName );

    if ( appUid != TUid::Null() )
        {
        for ( TInt i = 0; i < iApplicationSettings.Count(); ++i )
            {
            CCFApplicationStateSettings* appSettings = iApplicationSettings[ i ];
            if ( appUid == appSettings->Uid() )
                {
                fgApp.Set( appSettings->Name() );
                break;
                }
            }
        }

    iPreviousForegroundApplication.Set( fgApp ); // Store for next round
    iPreviousForegroundView.Set( fgView );

    RThread thread;

    // Publish foreground application
    iContext->SetTypeL( KApplicationStateForegroundApplicationType );
    iContext->SetValueL( fgApp );
    iCF.PublishContext( *iContext, thread );

    // Publish  foreground view
    iContext->SetTypeL( KApplicationStateForegroundApplicationViewType );
    iContext->SetValueL( fgView );
    iCF.PublishContext( *iContext, thread );

    // Publish background application
    iContext->SetTypeL( KApplicationStateBackgroundApplicationType );
    iContext->SetValueL( bgApp );
    iCF.PublishContext( *iContext, thread );
    
    // Publish foreground application orientation
    TPtrC fgAppOrientation( ResolveFgAppOrientation( appUid ) );
    iContext->SetTypeL( KFgApplicationOrientation );
    iContext->SetValueL( fgAppOrientation );
    iCF.PublishContext( *iContext, thread );
    
    // Store for next round
    iPreviousOrientation.Set( fgAppOrientation );

    thread.Close();
    }


//-----------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::ConvertToHex
//-----------------------------------------------------------------------------
//
TInt CApplicationStateSourcePlugIn::ConvertToHex( const TDesC& aString,
    TUint32& aValue )
    {
    FUNC_LOG;

    TInt err = KErrNone;
    TLex lex( aString );

    // Get rid of '0x'
    const TInt skip = 2;
    lex.Inc( skip );
    if( lex.BoundedVal( aValue, EHex, KLimit ) != KErrNone )
        {
        err = KErrNotFound;
        }
    return err;
    }

//------------------------------------------------------------------------------
// CApplicationStateSourcePlugIn::FindByUid
//------------------------------------------------------------------------------
//
TInt CApplicationStateSourcePlugIn::FindByUid( const TUid& aUid ) const
    {
    FUNC_LOG;
    
    TInt index = KErrNotFound;
    for( TInt i = 0; i < iUidOrientationList.Count(); i++ )
        {
        if( iUidOrientationList[i].iUid == aUid )
            {
            index = i;
            break;
            }
        }
    return index;
    }
void CApplicationStateSourcePlugIn::HandleWsEventL( RWsSession& aWsSession )
	{
	TUid appUid( TUid::Null() );
    TPtrC bgApp( *iDefaultAppName );
    TPtrC fgApp( *iDefaultAppName );
    TPtrC fgView( *iDefaultViewName );

	TInt focWndGrp( aWsSession.GetFocusWindowGroup() );
    CApaWindowGroupName* wndGrpName = CApaWindowGroupName::NewL( aWsSession );
    CleanupStack::PushL( wndGrpName );
    wndGrpName->ConstructFromWgIdL( focWndGrp );
    appUid = wndGrpName->AppUid();

    for ( TInt i = 0; i < iApplicationSettings.Count(); ++i )
        {
        CCFApplicationStateSettings* appSettings = iApplicationSettings[ i ];
        if ( appUid == appSettings->Uid() )
            {
            fgApp.Set( appSettings->Name() );
            appSettings->GetViewName( appUid, fgView );
            bgApp.Set( iPreviousForegroundApplication );
            break;
            }
        }
    TBool publishFgApp( EFalse );
    TBool publishFgView( EFalse );
    TBool publishBgApp( EFalse );

    if ( iPreviousForegroundApplication.Compare( fgApp ) != 0 )
        {
        publishFgApp = ETrue;
        }
    else if ( iPreviousForegroundView.Compare( fgView ) != 0 )
        {
        publishFgView = ETrue;
        }

    if ( bgApp != fgApp )
        {
        publishBgApp = ETrue;
        }

    iPreviousForegroundApplication.Set( fgApp ); // Store for next round
    iPreviousForegroundView.Set( fgView );

   RThread thread;

    if ( publishFgApp )
        {
        iContext->SetTypeL( KApplicationStateForegroundApplicationType );
        iContext->SetValueL( fgApp );
        iCF.PublishContext( *iContext, thread );
        }

    if ( publishFgApp || publishFgView )
        {
        iContext->SetTypeL( KApplicationStateForegroundApplicationViewType );
        iContext->SetValueL( fgView );
        iCF.PublishContext( *iContext, thread );
        }

    if ( publishBgApp )
        {
        iContext->SetTypeL( KApplicationStateBackgroundApplicationType );
        iContext->SetValueL( bgApp );
        iCF.PublishContext( *iContext, thread );
        }
	
	thread.Close();
    CleanupStack::PopAndDestroy( wndGrpName );

	}

// ======================== CWsEventHandler ========================

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CWsEventHandler::CWsEventHandler( MWsEventObserver& aObserver ):
    CActive( CActive::EPriorityStandard ),
    iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CWsEventHandler::ConstructL()
    {
    // Connect to window server server
    User::LeaveIfError( iWsSession.Connect() );
    
    // Construct window group
    iWindowGroup = new( ELeave ) RWindowGroup( iWsSession );
    User::LeaveIfError( iWindowGroup->Construct( (TUint32)this, EFalse ) );
    User::LeaveIfError( iWindowGroup->EnableGroupChangeEvents() );
    iWindowGroup->SetOrdinalPosition( 0, ECoeWinPriorityNeverAtFront );
    iWindowGroup->EnableReceiptOfFocus( EFalse );
    
    // Hide the invisible window from the task manager
    iWindowGroupName = CApaWindowGroupName::NewL( iWsSession );
    iWindowGroupName->SetHidden( ETrue );
    iWindowGroupName->SetWindowGroupName( *iWindowGroup );

    }
// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CWsEventHandler* CWsEventHandler::NewL( MWsEventObserver& aObserver )
    {
    CWsEventHandler* self = CWsEventHandler::NewLC( aObserver );
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CWsEventHandler* CWsEventHandler::NewLC( MWsEventObserver& aObserver )
    {
    CWsEventHandler* self = new ( ELeave ) CWsEventHandler( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CWsEventHandler::~CWsEventHandler()
    {
    Cancel();
        
    // Cleanup window group name
    delete iWindowGroupName;
       
    // Cleanup window group
    if( iWindowGroup )
        {
        iWindowGroup->DisableGroupChangeEvents();
        iWindowGroup->Close();
        delete iWindowGroup;
        }
    
    // Cleanup window server session
    iWsSession.Close();
    }

//------------------------------------------------------------------------------
// CWsEventHandler::IssueRequest
//------------------------------------------------------------------------------
//
void CWsEventHandler::IssueRequest()
    {
    // Request events from window server
    iWsSession.EventReady( &iStatus );
    SetActive();
    }


//------------------------------------------------------------------------------
// CWsEventHandler::RunL
//------------------------------------------------------------------------------
//
void CWsEventHandler::RunL()
    {
    TInt err = iStatus.Int();
    if( err == KErrNone )
        {
        // No errors occured, fetch event
        TWsEvent wsEvent;
        iWsSession.GetEvent( wsEvent );
        
        // Continue listening
        IssueRequest();
		switch (wsEvent.Type()) 
			{
		case EEventWindowGroupsChanged :
	       // Forward event to observer
	       iObserver.HandleWsEventL( iWsSession );
			break;
		default:
			break;
			}
        }
    }

//------------------------------------------------------------------------------
// CWsEventHandler::DoCancel
//------------------------------------------------------------------------------
//
void CWsEventHandler::DoCancel()
    {
    // Cancel event ready from window server
    iWsSession.EventReadyCancel();
    }

//------------------------------------------------------------------------------
// CWsEventHandler::RunError
//------------------------------------------------------------------------------
//
TInt CWsEventHandler::RunError( TInt /*aError*/ )
    {
    // Issue a new request, other error handling is not performed since the
    // problem has occured in the observer code
    IssueRequest();
    
    return KErrNone;
    }

	
