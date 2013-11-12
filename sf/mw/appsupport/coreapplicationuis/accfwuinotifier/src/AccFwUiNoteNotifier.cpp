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
* Description:  Implementation of information note about connected accessory
*
*/


// INCLUDE FILES
#include <featmgr.h>
#include <accfwnoteuinotifier.rsg>
#include <data_caging_path_literals.hrh> 
#include <SecondaryDisplay/SecondaryDisplayAccFwAPI.h>
#include <bautils.h>
#include <hbdevicemessageboxsymbian.h>
#include "AccFwUiNoteNotifier.h"
#include "acc_debug.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
const TUid KScreenOutputChannel = { 0x10282E0A };

_LIT( KAccFwUiNotifierResource, "accfwnoteuinotifier.rsc" );
// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::CAccFwUiNoteNotifier
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CAccFwUiNoteNotifier::CAccFwUiNoteNotifier()
	: CActive( EPriorityStandard)
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::CAccFwUiNoteNotifier()" );

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::CAccFwUiNoteNotifier() - return void" );
    }

// Destructor
CAccFwUiNoteNotifier::~CAccFwUiNoteNotifier()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::~CAccFwUiNoteNotifier()" );

    delete iNoteText;
    CActive::Cancel();
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccPolAccessoryPolicy::~CAccFwUiNoteNotifier() - return void" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::ConstructL() 
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CAccFwUiNoteNotifier::ConstructL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::ConstructL()" );

    //get pointer to CEikonEnv
	iCoeEnv = CEikonEnv::Static();    

    if ( !iCoeEnv )
        {
        User::Leave( KErrGeneral );        
        }

	//Open resource file
    TFileName fileName;
    fileName.Zero();

    TFileName drivePath;
    Dll::FileName( drivePath );

    fileName.Append( TParsePtrC( drivePath ).Drive() );
    fileName.Append( KDC_RESOURCE_FILES_DIR );
    fileName.Append( KAccFwUiNotifierResource );

    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

	//handle to resource file
    iResourceFileOffset = iCoeEnv->AddResourceFileL( fileName );

    //add to scheduler
    CActiveScheduler::Add( this );
    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::ConstructL() - return void" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::NewL() 
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CAccFwUiNoteNotifier* CAccFwUiNoteNotifier::NewL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::NewL()" );

    CAccFwUiNoteNotifier* self = new ( ELeave ) CAccFwUiNoteNotifier;
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::CAccFwUiNoteNotifier::NewL() - return" );
    return self;
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::Release()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAccFwUiNoteNotifier::Release()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Release() - enter" );

    if ( iResourceFileOffset )
        {
        iCoeEnv->DeleteResourceFile( iResourceFileOffset );
        }

    iCoeEnv = NULL; // This is not owned, pointer needs to be zeroed

    delete this;

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Release() - return" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::RegisterL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAccFwUiNoteNotifier::TNotifierInfo CAccFwUiNoteNotifier::RegisterL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RegisterL() - enter" );
    iInfo.iUid = KAccFwUiNoteNotifierUid;
    iInfo.iChannel = KScreenOutputChannel;
    iInfo.iPriority = ENotifierPriorityHigh;
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RegisterL() - return" );
    return iInfo;
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::Info()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CAccFwUiNoteNotifier::TNotifierInfo CAccFwUiNoteNotifier::Info() const
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Info() - enter" );
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Info() - return" );
    return iInfo;
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::StartL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC8 CAccFwUiNoteNotifier::StartL( const TDesC8& /*aBuffer*/ )
    {
	API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::StartL() - enter" );
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::StartL() - return" );
    return KNullDesC8();
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::StartL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAccFwUiNoteNotifier::StartL(
    const TDesC8& aBuffer, 
    TInt /*aReplySlot*/,
    const RMessagePtr2& aMessage)
    {    
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::StartL() - enter" );

    // Heap buffer for default text
    HBufC* defaultText = NULL;
    TBool showNote = EFalse;

    if ( aBuffer.Length() )
        {
		//Do nothing as accessory is rightly detected by accessory server and adaptation.
        }
    else
        {
        // Read resource string and adds heap buffer into cleanup stack.
        // This needs to be before creating 'note' for correct cleanup handling
        defaultText = iCoeEnv->AllocReadResourceLC( R_TEXT_UNSUPP_ACCESSORY_CONNECTED );
        iNoteText = HBufC::NewL( defaultText->Length() );
        TPtr ptr( iNoteText->Des() );
        ptr.Append( defaultText->Des() );
        CleanupStack::PopAndDestroy( defaultText );
        showNote = ETrue;
        }

    iMessage = aMessage;
    if( showNote )
        {
        iIsCancelled = EFalse;
        SetActive();
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone ); // RunL() function will get called
        }
    else
        {
        iMessage.Complete( KErrNone ); // Just complete the message
        }
                
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::StartL() - return" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::RunL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------

void CAccFwUiNoteNotifier::RunL()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RunL() - enter" );

	if ( iIsCancelled )
	    {
		return;	
	    }
    
   	CHbDeviceMessageBoxSymbian *messageBox = CHbDeviceMessageBoxSymbian::NewL(CHbDeviceMessageBoxSymbian::EInformation);
	           CleanupStack::PushL(messageBox);
	           messageBox->SetTextL(*iNoteText);
	           messageBox->ShowL();
	           CleanupStack::PopAndDestroy(messageBox); // messageBox
	           delete iNoteText;
	           iNoteText = NULL;

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RunL() - Complete message" );
	iMessage.Complete( KErrNone );

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RunL() - return" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::RunError()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CAccFwUiNoteNotifier::RunError(TInt aError)
    {
    API_TRACE_1( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RunError() - %d", aError );
    if( !iMessage.IsNull() )
        {
        iMessage.Complete( aError );
        }
    aError = KErrNone;
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::RunError() - return" );
    return aError;
    }
    
// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::DoCancel()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
void CAccFwUiNoteNotifier::DoCancel()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::DoCancel() - enter" );
    if( !iMessage.IsNull() )
        {
        iMessage.Complete( KErrCancel );
        }
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::DoCancel() - return" );
    }
    
// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::Cancel()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAccFwUiNoteNotifier::Cancel()
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Cancel() - enter" );

    iIsCancelled = ETrue;
    
    	   // Cancel active object and free resources
        if ( IsActive() )
            {
            CActive::Cancel();    
            }
	      delete iNoteText;
        iNoteText = NULL;

        API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Cancel() - Complete message with Cancel" );
        iMessage.Complete( KErrCancel );
       

    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::Cancel() - return" );
    }

// -----------------------------------------------------------------------------
// CAccFwUiNoteNotifier::UpdateL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC8 CAccFwUiNoteNotifier::UpdateL( const TDesC8& /*aBuffer*/ )
    {
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::UpdateL() - enter" );
    API_TRACE_( "[AccFW: ACCFWUINOTIFIER] CAccFwUiNoteNotifier::UpdateL() - return" );
    return KNullDesC8();
    }

//  End of File
