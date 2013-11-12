/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Import reader opens and reads the XML file to CMDXMLDocument 
*                object
*
*/



#include <f32file.h>
#include <gmxmldocument.h>

#include "cbsimportfilereader.h"
#include "importlogwriter.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CBSImportManager::NewL
// ---------------------------------------------------------------------------
//
CBSImportFileReader* CBSImportFileReader::NewL( TBool aUseScheduler )
    {
    CBSImportFileReader* self = NewLC( aUseScheduler );
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportManager::NewLC
// ---------------------------------------------------------------------------
//
CBSImportFileReader* CBSImportFileReader::NewLC( TBool aUseScheduler )
    {
    CBSImportFileReader* self = new (ELeave) CBSImportFileReader( aUseScheduler );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CBSImportManager::ConstructL
// ---------------------------------------------------------------------------
//
void CBSImportFileReader::ConstructL()
    {
	if( iUseScheduler )
	    {
    	// Install active scheduler
    	iScheduler = new (ELeave) CActiveScheduler();
    	CActiveScheduler::Install( iScheduler );
	    }
    else
        {
        iWaiter = new (ELeave) CActiveSchedulerWait();
        }
    
    // Create XML parser
    iParser = CMDXMLParser::NewL( this );
    }
    
// ---------------------------------------------------------------------------
// CBSImportManager::~CBSImportFileReader
// ---------------------------------------------------------------------------
//
CBSImportFileReader::~CBSImportFileReader()
    {
    delete iParser;
    delete iDocument;
    delete iWaiter;
    delete iScheduler;
	if( iUseScheduler )
	    {
	    CActiveScheduler::Install( NULL );
	    }
    }

// ---------------------------------------------------------------------------
// CBSImportManager::CBSImportFileReader
// ---------------------------------------------------------------------------
//
CBSImportFileReader::CBSImportFileReader( TBool aUseScheduler ) :
    iUseScheduler( aUseScheduler )
    {
    }

// ---------------------------------------------------------------------------
// CBSImportManager::ReadXMLFileL
// ---------------------------------------------------------------------------
//
CMDXMLDocument* CBSImportFileReader::ReadXMLFileL( RFs& aFs, const TDesC& aFileName )
    {
    RFile file;
    TInt err = file.Open( aFs, aFileName, EFileRead );
    if( err )
        {
        IMPORT_DP( D_IMPORT_LIT( "ERROR: Unable to open XML file (%S)!" ), &aFileName );
        User::Leave( err );
        }

    // takes ownership of file
    err = iParser->ParseFile( file );
    if( err )
        {
        IMPORT_DP( D_IMPORT_LIT( "ERROR: Unable to read XML file (%S)!" ), &aFileName );
        User::Leave( err );
        }
    
    // Start waiting, so that parser can start working
	// Code scanner warning active object called without checking whether it is active
	// or cancelling it first (id:10 )
    // The active object cannot be active at this point.
    // Code scanner warning using CActiveScheduler::Start (Id:3)
    // It is required to use here
    iUseScheduler ? CActiveScheduler::Start() : iWaiter->Start(); // CSI: 10,3 # See above
    
    CMDXMLDocument* tmp = iDocument;
    iDocument = NULL;
    return tmp;
    }

// ---------------------------------------------------------------------------
// From class MMDXMLParserObserver.
// CBSImportManager::ParseFileCompleteL
// ---------------------------------------------------------------------------
//
void CBSImportFileReader::ParseFileCompleteL()
    {
    iDocument = iParser->DetachXMLDoc();
    // Codescanner warning using CActiveScheduler::Start (Id:4)
    // It is required to use here
    iUseScheduler ? CActiveScheduler::Stop() : iWaiter->AsyncStop(); // CSI: 4 # See above
    }

// End of file
