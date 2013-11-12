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
* Description:  Implementation of CDirStackEntry class
*
*/


#include <f32file.h>

#include "dirstackentry.h"
#include "trace.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CDirStackEntry::NewLC
// ---------------------------------------------------------------------------
//
CDirStackEntry* CDirStackEntry::NewLC( const TDesC& aDirName, RFs& aFs )
    {
    CDirStackEntry* self = new ( ELeave ) CDirStackEntry;
    CleanupStack::PushL( self );
    self->ConstructL( aDirName, aFs );
    return self;
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::~CDirStackEntry
// ---------------------------------------------------------------------------
//
CDirStackEntry::~CDirStackEntry()
    {
    FUNC_LOG;

    delete iDirName;
    delete iDirList;
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::FullPath
// ---------------------------------------------------------------------------
//
void CDirStackEntry::FullPath( TDes& aFullPath )
    {
    FUNC_LOG;

    aFullPath.Copy( *iDirName );
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::GetNextEntry
// ---------------------------------------------------------------------------
//
const TEntry* CDirStackEntry::GetNextEntry( TDes& aFullPath )
    {
    FUNC_LOG;

    const TEntry* ret = NULL;
    if ( iDirListIndex < iDirList->Count() )
        {
        aFullPath.Copy( *iDirName );
        ret = &( ( *iDirList )[ iDirListIndex++ ] );
        aFullPath.Append( ret->iName );
        if ( ret->IsDir() )
            {
            aFullPath.Append( KPathDelimiter );
            }
        }
    else
        {
        aFullPath.Zero();
        }
    return ret;
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::PushDirEntryL
// ---------------------------------------------------------------------------
//
void CDirStackEntry::PushDirEntryL(
    RPointerArray<CDirStackEntry>& aStack,
    const TDesC& aDirName,
    RFs& aFs )
    {
    FUNC_LOG;

    CDirStackEntry* stackEntry = CDirStackEntry::NewLC( aDirName, aFs );
    aStack.AppendL( stackEntry );
    CleanupStack::Pop( stackEntry );
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::PopAndDestroyDirEntry
// ---------------------------------------------------------------------------
//
void CDirStackEntry::PopAndDestroyDirEntry(
    RPointerArray<CDirStackEntry>& aStack )
    {
    FUNC_LOG;

    TInt last( aStack.Count() - 1 );
    if ( last >= 0 )
        {
        CDirStackEntry* stackEntry = aStack[last];
        aStack.Remove( last );
        delete stackEntry;
        }
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::CDirStackEntry
// ---------------------------------------------------------------------------
//
CDirStackEntry::CDirStackEntry()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CDirStackEntry::ConstructL
// ---------------------------------------------------------------------------
//
void CDirStackEntry::ConstructL( const TDesC& aDirName, RFs& aFs )
    {
    FUNC_LOG;
    INFO_1( "New dir stack entry: '%S'", &aDirName );

    iDirName = aDirName.AllocL();
    User::LeaveIfError( aFs.GetDir( aDirName,
        KEntryAttDir | KEntryAttHidden | KEntryAttSystem, ESortNone, iDirList ) );
    }
