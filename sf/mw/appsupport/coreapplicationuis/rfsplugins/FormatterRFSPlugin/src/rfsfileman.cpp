/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of CRfsFileMan class
*
*/


#include <f32file.h>

#include "dirstackentry.h"
#include "rfsfileman.h"
#include "trace.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CRfsFileMan::NewL
// ---------------------------------------------------------------------------
//
CRfsFileMan* CRfsFileMan::NewL( RFs& aFs )
    {
    CRfsFileMan* self = new ( ELeave ) CRfsFileMan( aFs );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CRfsFileMan::~CRfsFileMan
// ---------------------------------------------------------------------------
//
CRfsFileMan::~CRfsFileMan()
    {
    FUNC_LOG;

    iDirStack.ResetAndDestroy();
    iDirStack.Close();
    }


// ---------------------------------------------------------------------------
// CRfsFileMan::DeleteL
// ---------------------------------------------------------------------------
//
void CRfsFileMan::DeleteL( const TDesC& aFullPath, const TEntry& aRootEntry )
    {
    FUNC_LOG;

    if ( aRootEntry.IsDir() )
        {
        INFO_1( "Checking contents of '%S'", &aFullPath );

        HBufC* buffer = HBufC::NewLC( KMaxPath );
        TPtr ptr = buffer->Des();
        CDirStackEntry::PushDirEntryL( iDirStack, aFullPath, iFs ); // Setup root dir
        while ( iDirStack.Count() > 0 )
            {
            CDirStackEntry& stackEntry = *( iDirStack[ iDirStack.Count() - 1 ] );
            const TEntry* dirEntry = stackEntry.GetNextEntry( ptr );
            if ( dirEntry )
                {
                if ( dirEntry->IsDir() )
                    {
                    CDirStackEntry::PushDirEntryL( iDirStack, ptr, iFs );
                    }
                else
                    {
                    Delete( ptr, *dirEntry );
                    }
                }
            else
                {
                // Dir has been processed. Now it is empty and can be deleted.
                stackEntry.FullPath( ptr );
                TEntry entry;
                if( iFs.Entry( ptr, entry ) == KErrNone )
                    {
                    Delete( ptr, entry );
                    }

                CDirStackEntry::PopAndDestroyDirEntry( iDirStack );
                }
            }

        CleanupStack::PopAndDestroy( buffer );
        }
    else
        {
        Delete( aFullPath, aRootEntry );
        }
    }


// ---------------------------------------------------------------------------
// CRfsFileMan::CRfsFileMan
// ---------------------------------------------------------------------------
//
CRfsFileMan::CRfsFileMan( RFs& aFs ) : iFs( aFs )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsFileMan::ConstructL
// ---------------------------------------------------------------------------
//
void CRfsFileMan::ConstructL()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CRfsFileMan::Delete
// ---------------------------------------------------------------------------
//
TInt CRfsFileMan::Delete( const TDesC& aFullPath, const TEntry& aEntry )
    {
    FUNC_LOG;
    INFO_1( "Delete '%S'", &aFullPath );

    TBool isDir( aEntry.IsDir() );
    TInt err( isDir ? iFs.RmDir( aFullPath ) : iFs.Delete( aFullPath ) );
    if ( err == KErrAccessDenied )
        {
        ResetAttributes( aFullPath, aEntry );
        err = isDir ? iFs.RmDir( aFullPath ) : iFs.Delete( aFullPath );
        }
    ERROR_1( err, "Failed to delete '%S'", &aFullPath );
    return err;
    }


// ---------------------------------------------------------------------------
// CRfsFileMan::ResetAttributes
// ---------------------------------------------------------------------------
//
void CRfsFileMan::ResetAttributes( const TDesC& aFullPath, const TEntry& aEntry )
    {
    FUNC_LOG;

    if ( aEntry.IsReadOnly() )
        {
        TInt err = iFs.SetEntry(
            aFullPath, aEntry.iModified, 0, KEntryAttReadOnly | KEntryAttHidden );
        ERROR( err, "Failed to reset attributes" );
        }
    }
