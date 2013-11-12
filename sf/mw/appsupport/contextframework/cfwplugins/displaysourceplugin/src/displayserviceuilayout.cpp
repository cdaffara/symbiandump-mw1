/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   CDisplayServiceUILayout class implementation.
*
*/


// SYSTEM INCLUDES
#include <cfcontextobject.h>
#include <cfcontextinterface.h>
#include <apgwgnam.h>
#include <coedef.h>

// USER INCLUDES
#include "displayserviceuilayout.h"
#include "displaysourcecontextdef.h"
#include "displaysourcepluginpanic.h"
#include "trace.h"

// ======================== LOCAL FUNCTIONS ========================

#ifdef _DEBUG

/** Rotation constants */
_LIT( KGraphicsOrientationNormal, "Normal" );
_LIT( KGraphicsOrientationRotated90, "Rotated 90" );
_LIT( KGraphicsOrientationRotated180, "Rotated 180" );
_LIT( KGraphicsOrientationRotated270, "Rotated 270" );

LOCAL_C const TDesC& RotationAsDesC( TInt aRotation )
    {
    switch( aRotation )
        {
        case CFbsBitGc::EGraphicsOrientationNormal:
            {
            return KGraphicsOrientationNormal;
            }
        case CFbsBitGc::EGraphicsOrientationRotated90:
            {
            return KGraphicsOrientationRotated90;
            }
        case CFbsBitGc::EGraphicsOrientationRotated180:
            {
            return KGraphicsOrientationRotated180;
            }
        case CFbsBitGc::EGraphicsOrientationRotated270:
            {
            return KGraphicsOrientationRotated270;
            }
        default:
            {
            return KNullDesC;
            }
        }
    }

#endif

// ======================== MEMBER FUNCTIONS ========================

// ======================== CDisplayServiceUILayout ========================

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceUILayout::CDisplayServiceUILayout( MCFContextInterface& aCF ):
    CDisplayServiceBase( CDisplayServiceBase::EDisplayUILayout, aCF ),
    iCurrentLayout( EUILayoutValueUnknown )
    {
    FUNC_LOG;
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CDisplayServiceUILayout::ConstructL()
    {
    FUNC_LOG;
    
    // Call base class
    CDisplayServiceBase::ConstructL();
    
    // Configure context source and type
    iContext->SetSourceL( KDisplaySource );
    iContext->SetTypeL( KDisplayUILayoutType );
    
    // Define context:
    // Device: Display.UI.Layout
    TInt err = iCF.DefineContext(
        KDisplaySource, KDisplayUILayoutType, KDisplayUILayoutSec );
    User::LeaveIfError( err );
    
    // Define context:
    // Device: Display.UI.Rotation
    err = iCF.DefineContext(
        KDisplaySource, KDisplayUIRotationType, KDisplayUIRotationSec );
    User::LeaveIfError( err );
    
    // Create window server event handler
    iWsEventHandler = CWsEventHandler::NewL( *this );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceUILayout* CDisplayServiceUILayout::NewL(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;

    CDisplayServiceUILayout* self = CDisplayServiceUILayout::NewLC( aCF );
    CleanupStack::Pop( self );
    
    return self;
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// Leaves pointer in the cleanup stack.
// ---------------------------------------------------------------------------
//
CDisplayServiceUILayout* CDisplayServiceUILayout::NewLC(
    MCFContextInterface& aCF )
    {
    FUNC_LOG;

    CDisplayServiceUILayout* self =
        new ( ELeave ) CDisplayServiceUILayout( aCF );
    CleanupStack::PushL( self );
    self->ConstructL();
    
    return self;
    }

// ---------------------------------------------------------------------------
// C++ destructor.
// ---------------------------------------------------------------------------
//
CDisplayServiceUILayout::~CDisplayServiceUILayout()
    {
    FUNC_LOG;

    delete iWsEventHandler;
    }

//------------------------------------------------------------------------------
// CDisplayServiceUILayout::StartL
//------------------------------------------------------------------------------
//
void CDisplayServiceUILayout::StartL()
    {
    FUNC_LOG;

    // Get initial values
    TDisplayUILayoutValues layout;
    CFbsBitGc::TGraphicsOrientation rotation;
    iWsEventHandler->ResolveUILayout( layout, rotation );

    // Publish initial UI layout context value
    PublishContextL( layout, ETrue );
    
    // Publish initial UI rotation context value
    PublishContextL( rotation, ETrue );
    
    //  Start listening window server events
    iWsEventHandler->IssueRequest();
    }

//------------------------------------------------------------------------------
// CDisplayServiceUILayout::Stop
//------------------------------------------------------------------------------
//
void CDisplayServiceUILayout::Stop()
    {
    FUNC_LOG;
    
    iWsEventHandler->Cancel();
    }

//------------------------------------------------------------------------------
// CDisplayServiceUILayout::HandleWsEventL
//------------------------------------------------------------------------------
//
void CDisplayServiceUILayout::HandleWsEventL( const TWsEvent &aEvent )
    {
    FUNC_LOG;
    
    INFO_1( "New window server event received: Event: [%d]", aEvent.Type() );

    // New window server event received, solve the event type
    switch( aEvent.Type() )
        {
        case EEventWindowGroupsChanged:
        case EEventScreenDeviceChanged:
            {
            TDisplayUILayoutValues layout = EUILayoutValueUnknown;
            CFbsBitGc::TGraphicsOrientation rotation =
                CFbsBitGc::EGraphicsOrientationNormal;
            iWsEventHandler->ResolveUILayout( layout, rotation );
            PublishContextL( layout );
            PublishContextL( rotation );
            break;
            }
        default:
            {
            // Nothing to do here
            break;
            }
        }
    }

//------------------------------------------------------------------------------
// CDisplayServiceUILayout::HandleWsEventErrorL
//------------------------------------------------------------------------------
//
void CDisplayServiceUILayout::HandleWsEventErrorL( TInt aError )
    {
    FUNC_LOG;
    
    // Remove warnings in urel builds
    (void)aError;
    
    ERROR( aError,
        "Window server event error occured. UI layout set to 'unknown" );

    // Error occured and it is impossible to know the current UI layout.
    // Publish context:
    // Device: Display.UI.Layout: Unknown
    iContext->SetValueL( TPtrC(
        KDisplayUILayoutValues[EUILayoutValueUnknown] ) );
    iCF.PublishContext( *iContext, iThread );
    }

//------------------------------------------------------------------------------
// CDisplayServiceUILayout::PublishContextL
//------------------------------------------------------------------------------
//
void CDisplayServiceUILayout::PublishContextL( TDisplayUILayoutValues aLayout,
    TBool aForce )
    {
    FUNC_LOG;

    if( aLayout != iCurrentLayout || aForce )
        {
        iContext->SetTypeL( KDisplayUILayoutType );
        iContext->SetValueL( TPtrC( KDisplayUILayoutValues[aLayout] ) );
        iCF.PublishContext( *iContext, iThread );
        iCurrentLayout = aLayout;
        }
    }

//------------------------------------------------------------------------------
// CDisplayServiceUILayout::PublishContextL
//------------------------------------------------------------------------------
//
void CDisplayServiceUILayout::PublishContextL(
    CFbsBitGc::TGraphicsOrientation aRotation,
    TBool aForce )
    {
    FUNC_LOG;

    if( aRotation != iCurrentRotation || aForce )
        {
        iContext->SetTypeL( KDisplayUIRotationType );
        iContext->SetValueL( TPtrC( KDisplayUIRotationValues[aRotation] ) );
        iCF.PublishContext( *iContext, iThread );
        iCurrentRotation = aRotation;
        }
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
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// ---------------------------------------------------------------------------
//
void CWsEventHandler::ConstructL()
    {
    FUNC_LOG;
    
    // Connect to window server server
    User::LeaveIfError( iWsSession.Connect() );
    
    // Construct window group
    iWindowGroup = new( ELeave ) RWindowGroup( iWsSession );
    User::LeaveIfError( iWindowGroup->Construct( (TUint32)this, EFalse ) );
    User::LeaveIfError( iWindowGroup->EnableScreenChangeEvents() );
    User::LeaveIfError( iWindowGroup->EnableGroupChangeEvents() );
    iWindowGroup->SetOrdinalPosition( 0, ECoeWinPriorityNeverAtFront );
    iWindowGroup->EnableReceiptOfFocus( EFalse );
    
    // Hide the invisible window from the task manager
    iWindowGroupName = CApaWindowGroupName::NewL( iWsSession );
    iWindowGroupName->SetHidden( ETrue );
    iWindowGroupName->SetWindowGroupName( *iWindowGroup );

    // Construct screen device
    iWsScreenDevice = new( ELeave ) CWsScreenDevice( iWsSession );
    User::LeaveIfError( iWsScreenDevice->Construct() );
    }

// ---------------------------------------------------------------------------
// Symbian two phased constructor.
// ---------------------------------------------------------------------------
//
CWsEventHandler* CWsEventHandler::NewL( MWsEventObserver& aObserver )
    {
    FUNC_LOG;

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
    FUNC_LOG;

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
    FUNC_LOG;
    
    Cancel();
    
    // Cleanup screen device
    delete iWsScreenDevice;
    
    // Cleanup window group name
    delete iWindowGroupName;
       
    // Cleanup window group
    if( iWindowGroup )
        {
        iWindowGroup->DisableGroupChangeEvents();
        iWindowGroup->DisableScreenChangeEvents();
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
    FUNC_LOG;

    __ASSERT_DEBUG( !IsActive(), Panic( ERequestAlreadyIssued ) );
    
    // Request events from window server
    iWsSession.EventReady( &iStatus );
    SetActive();
    }

//------------------------------------------------------------------------------
// CWsEventHandler::ResolveUILayout
//------------------------------------------------------------------------------
//
void CWsEventHandler::ResolveUILayout( TDisplayUILayoutValues& aLayout,
    CFbsBitGc::TGraphicsOrientation& aRotation ) const
    {
    FUNC_LOG;
    
    // Resolve the current UI layout from screen dimensions
    TInt mode = iWsScreenDevice->CurrentScreenMode();
    INFO_1( "Current screen mode: [%d]", mode );
    
    TPixelsAndRotation pixelsAndRotation;
    iWsScreenDevice->GetScreenModeSizeAndRotation( mode, pixelsAndRotation );
    INFO_3( "Current screen mode size and rotation: Size: [W=%d, H=%d], Rotation: [%S]",
        pixelsAndRotation.iPixelSize.iWidth,
        pixelsAndRotation.iPixelSize.iHeight,
        &RotationAsDesC( pixelsAndRotation.iRotation ) );
    
    // Resolve current UI rotation
    aRotation = pixelsAndRotation.iRotation;
    
    // Resovle current UI layout mode
    TSize screenSize( pixelsAndRotation.iPixelSize );
    if( screenSize.iWidth < screenSize.iHeight )
        {
        // Assuming portrait layout
        INFO( "Current UI layout resolved to be in 'Portrait' mode" );
        aLayout = EUILayoutValuePortrait;
        }
    else if( screenSize.iWidth > screenSize.iHeight )
        {
        // Assuming landscape layout
        INFO( "Current UI layout resolved to be in 'Landscape' mode" );
        aLayout = EUILayoutValueLandscape;
        }
    else
        {
        // Need to check the rotation even though this might not be correct
        // if wsini.ini has not been configured properly.
        CFbsBitGc::TGraphicsOrientation orientation(
            pixelsAndRotation.iRotation );
        if( orientation == CFbsBitGc::EGraphicsOrientationNormal ||
            orientation == CFbsBitGc::EGraphicsOrientationRotated180 )
            {
            // Assuming portrait layout
            INFO( "Current UI layout resolved to be in 'Portrait' mode" );
            aLayout = EUILayoutValuePortrait;
            }
        else
            {
            // Assuming landscape layout
            INFO( "Current UI layout resolved to be in 'Landscape' mode" );
            aLayout = EUILayoutValueLandscape;
            }
        }
    }

//------------------------------------------------------------------------------
// CWsEventHandler::RunL
//------------------------------------------------------------------------------
//
void CWsEventHandler::RunL()
    {
    FUNC_LOG;
    
    TInt err = iStatus.Int();
    if( err == KErrNone )
        {
        // No errors occured, fetch event
        TWsEvent wsEvent;
        iWsSession.GetEvent( wsEvent );
        
        // Continue listening
        IssueRequest();
        
        // Forward event to observer
        iObserver.HandleWsEventL( wsEvent );
        }
    else
        {
        // Continue listening
        IssueRequest();
        
        // Forward error to observer
        iObserver.HandleWsEventErrorL( err );
        }    
    }

//------------------------------------------------------------------------------
// CWsEventHandler::DoCancel
//------------------------------------------------------------------------------
//
void CWsEventHandler::DoCancel()
    {
    FUNC_LOG;
    
    // Cancel event ready from window server
    iWsSession.EventReadyCancel();
    }

//------------------------------------------------------------------------------
// CWsEventHandler::RunError
//------------------------------------------------------------------------------
//
TInt CWsEventHandler::RunError( TInt /*aError*/ )
    {
    FUNC_LOG;

    // Issue a new request, other error handling is not performed since the
    // problem has occured in the observer code
    IssueRequest();
    
    return KErrNone;
    }

// End of file
