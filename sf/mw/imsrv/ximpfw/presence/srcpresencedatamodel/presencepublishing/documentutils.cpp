/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Document utils
*
*/

#include "documentutils.h"
#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "ximpobjecthelpers.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// DocumentUtils::InfoFilterUnionL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* DocumentUtils::InfoFilterUnionL(
        CPresenceInfoFilterImp& aSource1,
        CPresenceInfoFilterImp& aSource2 )
    {
    RPointerArray< CPresenceInfoFilterImp > array;
    CleanupClosePushL( array );
    array.AppendL( &aSource1 );
    array.AppendL( &aSource2 );
    CPresenceInfoFilterImp* retVal = InfoFilterUnionL( array );
    CleanupStack::PopAndDestroy(); // array
    return retVal;
    }


// ---------------------------------------------------------------------------
// DocumentUtils::DocumentUnionL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* DocumentUtils::InfoFilterUnionL(
        const RPointerArray< CPresenceInfoFilterImp >& aSources )
    {
    CPresenceInfoFilterImp* target = NULL;
    TInt countOfSources = aSources.Count();
    if( countOfSources == 0 ) // No sources to combine.
        {
        target = CPresenceInfoFilterImp::NewLC();
        CleanupStack::Pop(); // target
        return target;
        }

    // Create a new one instead of cloning the first to avoid possible duplicates.
    // Algorithm is: given filters a,b,c: 
    //   new = empty. 
    //   new = union(new,a)
    //   new = union(new,b)
    //   new = union(new,c)
    // result in new.
   
    target = CPresenceInfoFilterImp::NewL();

    for ( TInt i = 0; i < aSources.Count(); i++ )
        {
        CleanupStack::PushL( target );

        CPresenceInfoFilterImp* filter = aSources[ i ];
        CPresenceInfoFilterImp* tmp = CPresenceInfoFilterImp::UnionL( *target, *filter );

        CleanupStack::PopAndDestroy( target );

        target = tmp;
        }

    return target;
    }


EXPORT_C CPresenceInfoImp* DocumentUtils::CollectFilteredPresenceInfoLC(
        const CPresenceInfoFilterImp& aPif,
        const CPresenceInfoImp& aSource )
    {
    CPresenceInfoImp* newInfo = aSource.FilteredInfoLC( aPif );
    return newInfo;
    }

// End of file
