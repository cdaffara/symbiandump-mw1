/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of the CProEngToneHandler.
*
*/



// INCLUDE FILES
#include    "CProEngToneHandler.h"
#include    "CProEngMediaVariation.h"
#include    <bautils.h> // BaflUtils
#include    <barsread.h>  // TResourceReader
#include    <barsc.h>  // RResourceFile
#include    <e32const.h> // KKilo
#include 	<centralrepository.h>
#include    <ProfileEngineDomainCRKeys.h> // KProEngRingingToneMaxSize
#include    <ProfileEng.hrh>

namespace
	{
	// CONSTANTS
	_LIT( KProEngROMDriveLetter, "Z:" );
	// The filename of the resource file
	//_LIT( KProEngResourceFileName, "Z:ProEngWrapper.RSC" );
	}

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CProEngToneHandler::CProEngToneHandler
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CProEngToneHandler::CProEngToneHandler()
    {
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::ConstructL()
    {
    DrmConstructL();
    User::LeaveIfError( iFs.Connect() );
    iMediaVariation = CProEngMediaVariation::NewL();
    GetMaxToneFileSizeL( iToneFileSizeLimitKB );
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngToneHandler* CProEngToneHandler::NewLC()
    {
    CProEngToneHandler* self = new( ELeave ) CProEngToneHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CProEngToneHandler* CProEngToneHandler::NewL()
    {
    CProEngToneHandler* self = CProEngToneHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// Destructor
CProEngToneHandler::~CProEngToneHandler()
    {
    delete iMediaVariation;
    iFs.Close();
    ReleaseDrm();
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::CheckToneFileL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::CheckToneFileL( const TDesC& aFileName, TInt aToneType )
    {
    // Check the file which is tried to set as ringing or alert tone.
    TInt returnValue( DoCheckToneFileL( aFileName, aToneType ) );
    if( returnValue != KErrNone )
        {
        return returnValue;
        }

    if( IsProtected( aFileName ) && CanSetAutomated( aFileName ) )
        {
        // If the file in question is unactivated protected content,
        // ask user if he/she wants to activate it.
        if( AskAutomated( aFileName ) == KErrCancel )
            {
            // User doesn't want to activate unactivated protected content.
            return KErrCancel;
            }
        }

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::HandleAutomatedContentL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::HandleAutomatedContent( TProfileSettingId aSettingId,
                                                 const TDesC& aFileName,
                                                 const TDesC& aOldFileName)

    {
    // Register file as automated content
    TInt err( SetAutomated( aSettingId, aFileName ) );
    if( !err )
        {
        // Remove the old tone from the automated content list
        RemoveAutomated( aOldFileName );
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::DoCheckToneFileL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::DoCheckToneFileL( const TDesC& aFileName, TInt aToneType )
    {
    if( aFileName.Length() == 0 )
        {
        // The filename is empty, ok.
        // It will be changed to "No_sound.wav" when writing to Profiles Engine.
        return KErrNone;
        }

    if( !BaflUtils::FileExists( iFs, aFileName ) )
        {
        // The file does not exist. No good (start the dance).
        return KErrNotFound;
        }

    // The file exists. Check its data (MIME) type.
    TBuf< KMaxDataTypeLength > dataType;
    iMediaVariation->GetDataTypeL( aFileName, dataType );

    if( !iMediaVariation->IsSupported( dataType ) )
        {
        return KErrNotSupported;
        }


    // File size check for ringing tones 1&2
    if (aToneType == EProfileSettingIdRingingTone  ||
        aToneType == EProfileSettingIdRingingTone2)
        {
        if (KErrNone != CheckFileSizeLimit( aFileName ))
            {
            return KErrTooBig;
            }
        }

    // The file is a sound file. Can it be found on ROM
    if( aFileName.Left( KProEngROMDriveLetter().Length() ).
        CompareF( KProEngROMDriveLetter ) == 0 )
        {
        // Tone files on ROM are always valid
        return KErrNone;
        }

    TInt result( KErrNone );
    if( IsProtected( aFileName ) )
        {
        if( !iMediaVariation->IsAllowedProtected( dataType ) )
            {
            return KErrArgument;
            }

        result = CheckProtectedFileL( aFileName );

        }
    else if( !iMediaVariation->IsAllowedUnProtected( dataType ) )
        {
        result = KErrPermissionDenied;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::ShowErrorNoteL
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::ShowErrorNoteL( TInt /*aResourceId*/ )
    {
    }


// -----------------------------------------------------------------------------
// CProEngToneHandler::CheckFileSizeLimitL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::CheckFileSizeLimit( const TDesC& aFileName ) const
    {
    TInt error = KErrNone;

    if ( iToneFileSizeLimitKB )
        {
        if ( CheckToneFileSize( aFileName, iToneFileSizeLimitKB ) != KErrNone )
            {
            error = KErrTooBig;
            }
        }

    return error;
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::GetMaxToneFileSizeL
// -----------------------------------------------------------------------------
//
void CProEngToneHandler::GetMaxToneFileSizeL( TInt& aMaxSizeKB ) const
    {
	CRepository* cenrep = CRepository::NewL( KCRUidProfileEngine );
	TInt error = cenrep->Get( KProEngRingingToneMaxSize, aMaxSizeKB );
    delete cenrep;

	if ( error != KErrNone )
    	{
        aMaxSizeKB = 0;
    	}
	if ( aMaxSizeKB < 0 )
	    {
	     aMaxSizeKB = 0;
	    }
    }

// -----------------------------------------------------------------------------
// CProEngToneHandler::CheckToneFileSizeL
// -----------------------------------------------------------------------------
//
TInt CProEngToneHandler::CheckToneFileSize( const TDesC& aFile,
                                            TInt aSizeLimitKB ) const
    {
    // Get file size
    TInt size = 0;
    TInt error = KErrNone;

    TEntry entry;
    error = iFs.Entry( aFile, entry );
    if ( !error )
        {
        size = entry.iSize;
        }

    // Check
    aSizeLimitKB *= KKilo;
    if ( aSizeLimitKB  &&  size > aSizeLimitKB )
        {
        error = KErrTooBig;
        }

    return error;
    }

//  End of File
