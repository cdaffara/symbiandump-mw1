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
* Description:
*
*/

#ifndef PRFWTESTPRESENCEDATAUTILS_H
#define PRFWTESTPRESENCEDATAUTILS_H


#include <e32base.h>
#include "presenceinfoimp.h"
#include "presenceinfofilterimp.h"
#include "ximpidentityimp.h"


// CLASS DESCRIPTION
/**
 * Helper class presence data comparison and generation.
 */
class XIMPTestPresenceDataUtils
    {

public:
/**
 * Enumeration defining the supported
 * test data groups.
 * A, B and C have unique values.
 * The combinations are unions of those.
 */
enum TPresenceDataGroup
    {
    Group_None,
    Group_A,
    Group_B,
    Group_C,
    Group_AB,
    Group_AC,
    Group_BC,
    Group_ABC,
    };

// TODO Add the defined names here (if needed for verification).

public: //Generator functions
    IMPORT_C static CXIMPIdentityImp* GenerateIdentityLC( const TDesC8* aPrefix, TPresenceDataGroup aGroup = Group_None );
    IMPORT_C static CPresenceInfoImp* GenerateInfoLC( const TDesC8* aPrefix, TPresenceDataGroup aGroup = Group_None );
    IMPORT_C static CPresenceInfoFilterImp* GenerateInfoFilterLC( const TDesC8* aPrefix, TPresenceDataGroup aGroup = Group_None );
    
public: //Comparator functions

    // Contains. All a1 info are in a2
    IMPORT_C TBool Contains( MPresenceInfo& a1, MPresenceInfo& a2 ) const;

    
    // Equality comparison
    IMPORT_C TBool Equals( CPresenceInfoImp& a1, CPresenceInfoImp& a2 ) const;

    // Equality assertion
    // TODO these are containment currently. Used by t_dmattrobjs. Modify...
    IMPORT_C static void AssertEquality( MPresenceInfo& a1,
                                   MPresenceInfo& a2 );

//private: // Helpers
    IMPORT_C static void InsertPrefixL( const TDesC8* aPrefix, RBuf8& buf );
    };

#endif //PRFWTESTPRESENCEDATAUTILS_H


