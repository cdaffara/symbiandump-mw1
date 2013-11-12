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

#include "prfwtestpresencedatautils.h"
#include "prfwtestcaseutilspanics.h"
#include <eunitmacros.h>

#include "ximprbufhelpers.h"

// ============================ MEMBER FUNCTIONS ===============================


// Generation
// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::GenerateIdentityLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CXIMPIdentityImp*
    XIMPTestPresenceDataUtils::GenerateIdentityLC(
                            const TDesC8* aPrefix,
                            TPresenceDataGroup aGroup )
    {
    // According to Group:
    //   Generate uri
    _LIT16( KUri, "http://google.com" );

    //   Generate protocolAddr
    _LIT16( KProtocolAddress, "protocollll" );
    
    CXIMPIdentityImp* pres = CXIMPIdentityImp::NewLC( KUri );
    return pres;    
    }



// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::GenerateInfoLC()
// -----------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoImp*
    XIMPTestPresenceDataUtils::GenerateInfoLC(
                            const TDesC8* aPrefix,
                            TPresenceDataGroup aGroup )
    {
    CPresenceInfoImp* doc = CPresenceInfoImp::NewLC();
    return doc;
    }
    
// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::GenerateInfoFilterLC()
// -----------------------------------------------------------------------------
//        
EXPORT_C CPresenceInfoFilterImp* 
    XIMPTestPresenceDataUtils::GenerateInfoFilterLC( const TDesC8* aPrefix, 
                                                     TPresenceDataGroup aGroup )
    {
    CPresenceInfoFilterImp* pif = CPresenceInfoFilterImp::NewLC();
    return pif;
    }
    



// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::InsertPrefixL
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPTestPresenceDataUtils::InsertPrefixL( const TDesC8* aPrefix, RBuf8& buf )
    {
    if ( aPrefix )
        {
        XIMPRBuf8Helper::GrowIfNeededL( buf, aPrefix->Length() + buf.Length() );
        buf.Insert( 0, *aPrefix );
        }
    }

// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::Contains()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool XIMPTestPresenceDataUtils::Contains( MPresenceInfo& a1,
                                                    MPresenceInfo& a2 ) const
    {
    return ETrue;
    }



// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::Equals()
// -----------------------------------------------------------------------------
//
EXPORT_C TBool XIMPTestPresenceDataUtils::Equals( CPresenceInfoImp& a1,
                                                  CPresenceInfoImp& a2 ) const
    {
    return ETrue;
    }



// -----------------------------------------------------------------------------
// XIMPTestPresenceDataUtils::AssertEquality()
// -----------------------------------------------------------------------------
//
EXPORT_C void XIMPTestPresenceDataUtils::AssertEquality(
    MPresenceInfo& a1,
    MPresenceInfo& a2 )
    {
    }




// end of file

