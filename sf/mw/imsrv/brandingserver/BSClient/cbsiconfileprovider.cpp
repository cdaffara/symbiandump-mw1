/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   Icon file provider.
 *
*/



// INCLUDE FILES
#include    "cbsiconfileprovider.h"
#include	<aknappui.h>
#include    <AknIconUtils.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CBSIconFileProvider::CBSIconFileProvider
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CBSIconFileProvider::CBSIconFileProvider()
    {
    }
// -----------------------------------------------------------------------------
// CBSIconFileProvider::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CBSIconFileProvider::ConstructL(RFile& aFile)
    {
    User::LeaveIfError( iFile.Duplicate( aFile ) );
    }

// -----------------------------------------------------------------------------
// CBSIconFileProvider::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CBSIconFileProvider* CBSIconFileProvider::NewL(
        RFile& aFile )
    {
    CBSIconFileProvider* self = new( ELeave ) CBSIconFileProvider();

    CleanupStack::PushL( self );
    self->ConstructL(aFile);
    CleanupStack::Pop( self );
    return self;
    }


// Destructor
CBSIconFileProvider::~CBSIconFileProvider()
    {
    delete iFilename;
    iFile.Close();
    }

// -----------------------------------------------------------------------------
// CBSIconFileProvider::RetrieveIconFileHandleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBSIconFileProvider::RetrieveIconFileHandleL(
            RFile& aFile, const TIconFileType /*aType*/ )
    {
    aFile.Duplicate( iFile );
    }

// -----------------------------------------------------------------------------
// CBSIconFileProvider::Finished
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CBSIconFileProvider::Finished()
    {
    // commit suicide because Avkon Icon Server said so
    delete this;
    }

//  End of File
