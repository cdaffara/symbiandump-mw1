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
* Description:  Implementation of CExcludeListEntry class
*
*/


#include <f32file.h>

#include "excludelistentry.h"
#include "trace.h"

// CONSTANTS
const TText KWild1 = '*';
const TText KWild2 = '?';
_LIT( KPreserveAll, "\\*" );

// ================= LOCAL FUNCTIONS =======================

// ---------------------------------------------------------------------------
// ContainsWildCards
// ---------------------------------------------------------------------------
//
static TBool ContainsWildCards( const TDesC& aString )
    {
    return ( aString.Locate( KWild1 ) != KErrNotFound ||
             aString.Locate( KWild2 ) != KErrNotFound );
    }

// ---------------------------------------------------------------------------
// ContainsSingleWildCardEnding
// ---------------------------------------------------------------------------
//
static TBool ContainsSingleWildCardEnding( const TDesC& aString )
    {
    return ( aString.Right( 2 ) == KPreserveAll );
    }


// ---------------------------------------------------------------------------
// FolderLevel
// ---------------------------------------------------------------------------
//
static TInt FolderLevel( const TDesC& aPath )
	{
    // Count how many backslashes are in string to figure out the deepness.
    // Ignore the last backslash
    TInt len( aPath.Length() - 1 );
    TInt ret( 0 );
    for ( TInt i( 0 ); i < len; ++i )
	    {
    	if ( aPath[i] == KPathDelimiter )
	    	{
		    ++ret;
    		}
	    }
    return ret;
    }


// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// CExcludeListEntry::TMatchEntry::TMatchEntry
// ---------------------------------------------------------------------------
//
CExcludeListEntry::TMatchEntry::TMatchEntry( const TDesC& aPath )
  : iPath( aPath ),
    iFolderLevel( FolderLevel( aPath ) )
    {
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::NewL
// ---------------------------------------------------------------------------
//
CExcludeListEntry* CExcludeListEntry::NewL( const TDesC& aPath )
    {
    CExcludeListEntry* self = NewLC( aPath );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::NewLC
// ---------------------------------------------------------------------------
//
CExcludeListEntry* CExcludeListEntry::NewLC( const TDesC& aPath )
    {
    CExcludeListEntry* self = new ( ELeave ) CExcludeListEntry;
    CleanupStack::PushL( self );
    self->ConstructL( aPath );
    return self;
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::~CExcludeListEntry
// ---------------------------------------------------------------------------
//
CExcludeListEntry::~CExcludeListEntry()
    {
    FUNC_LOG;

    delete iPath;
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::Path
// ---------------------------------------------------------------------------
//
TPtrC CExcludeListEntry::Path() const
    {
    return iPath->Des();
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::Type
// ---------------------------------------------------------------------------
//
CExcludeListEntry::TPreserveType CExcludeListEntry::Type(
    const CExcludeListEntry::TMatchEntry& aMatchEntry ) const
    {
    if ( iType == EPreserveAll && aMatchEntry.iFolderLevel < iFolderLevel - 1 )
        {
        // Preserve all files in the directory identified by this entry but
        // Not necessarily all files in its parent folders.
        INFO( "EPreserveAll -> EPreserveMatching" );
        return EPreserveMatching;
        }

    return iType;
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::Compare
// ---------------------------------------------------------------------------
//
TInt CExcludeListEntry::Compare(
    const CExcludeListEntry& aLhs,
    const CExcludeListEntry& aRhs )
    {
    return aLhs.iPath->CompareF( *( aRhs.iPath ) );
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::CompareMatch
// ---------------------------------------------------------------------------
//
TInt CExcludeListEntry::CompareMatch(
    const TMatchEntry* aMatchEntry,
    const CExcludeListEntry& aExclude )
    {
    FUNC_LOG;

    TPtrC path( aMatchEntry->iPath );
    if ( aMatchEntry->iFolderLevel == aExclude.iFolderLevel ||
         ( aExclude.iType == EPreserveAll &&
           aMatchEntry->iFolderLevel == aExclude.iFolderLevel - 1 ) )
        {
        INFO( "Full match" );
        // Simple match is enough because items are in the same folder level.
        if ( path.MatchF( *( aExclude.iPath ) ) != KErrNotFound )
    		{
	    	return 1;
		    }
     	}
    else
        {
        TPtrC exclude( *( aExclude.iPath ) );
        TPtrC partialExclude( exclude.Left( Min( exclude.Length(), path.Length() ) ) );

        INFO( "Partial match" );
        if ( path.MatchF( partialExclude ) != KErrNotFound || // Check if path is a top level part of excluded item
             path.MatchF( exclude ) != KErrNotFound ) // Check if path matches to excluded item
            {
            return 1;
            }
        }

    return 0;
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::CExcludeListEntry
// ---------------------------------------------------------------------------
//
CExcludeListEntry::CExcludeListEntry()
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CExcludeListEntry::ConstructL
// ---------------------------------------------------------------------------
//
void CExcludeListEntry::ConstructL( const TDesC& aPath )
    {
    FUNC_LOG;

    iFolderLevel = FolderLevel( aPath );
    iPath = aPath.AllocL();
    iType = EPreserve; // Default
    if ( ContainsWildCards( aPath ) )
        {
        if ( ContainsSingleWildCardEnding( aPath ) )
            {
            // Wild card match all
            iType = EPreserveAll;
            }
        else
            {
            // Wild card pattern match
            iType = EPreserveMatching;
            }
        }

    INFO_2( "Exclude path: '%S', type %d", iPath, iType );
    }
