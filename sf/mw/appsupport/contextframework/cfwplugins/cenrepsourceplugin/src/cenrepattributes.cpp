/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCenRepAttributes class implementation.
*
*/


#include <cfdefinitions.h>
#include <cfcontextsourcesetting.h>
#include <cfcontextsourcesettingarray.h>

#include "cenrepattributes.h"
#include "cenreptrace.h"

// Capabilities
_LIT_SECURITY_POLICY_PASS( KCenRepSourceInteractionSec );

// ======== LOCAL FUNCTIONS =========

// ======== MEMBER FUNCTIONS ========

CCenRepAttributes::~CCenRepAttributes()
    {
    FUNC_LOG;
    // Delete context
    delete iContext;
    }

CCenRepAttributes::CCenRepAttributes(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings ) :
    iSettings( aSettings ),
    iCF( aCF )
    {
    FUNC_LOG;
    }

//----------------------------------------------------------------------------
// CCenRepAttributes::ConvertToHex
//----------------------------------------------------------------------------
//
TInt CCenRepAttributes::ConvertToHex( const TDesC& aString, TUint32& aValue )
    {
    FUNC_LOG;

    const TUint KLimit = 0xffffffff;
    _LIT( KHexPattern, "0x*" );
    TInt err = KErrNotFound;

    if ( err != aString.Match( KHexPattern ) )
        {
        TLex lex( aString );

        // Get rid of '0x'
        const TInt incAmount( 2 );
        lex.Inc( incAmount );
        err = lex.BoundedVal( aValue, EHex, KLimit );
        }

    return err;
    }

//----------------------------------------------------------------------------
// CCenRepAttributes::ConvertToInt
//----------------------------------------------------------------------------
//
TInt CCenRepAttributes::ConvertToInt( const TDesC& aString, TInt& aValue ) const
    {
    FUNC_LOG;

    TInt err = KErrNone;

    TLex lex( aString );

    if( lex.Val( aValue ) != KErrNone )
        {
        err = KErrNotFound;
        }

    return err;
    }

// ---------------------------------------------------------------------------
// CCenRepAttributes::InitCenRepKeyL
// ---------------------------------------------------------------------------
//
TUint32 CCenRepAttributes::InitCenRepKeyL( TInt aIndex )
    {
    FUNC_LOG;

    TUint32 cenRepKey = 0;

    iParamArray = iSettings.Item( aIndex ).Parameters();
    const RKeyValueArray& attrArray = iSettings.Item( aIndex ).Attributes();

    iParamCount = iParamArray.Count();

     // Check attributes
    TInt attrCount = attrArray.Count();

    DefineContext();

    // Find given key
    for( TInt i = 0; i < attrCount; i++ )
        {
        // Currently only interested given parameter key
        if ( attrArray[i]->Key().CompareF( KKey ) == KErrNone )
            {
            TInt err = ConvertToHex( attrArray[i]->Value(), cenRepKey );
            User::LeaveIfError( err );
            }
        }
   return cenRepKey;
   }

// ---------------------------------------------------------------------------
// CCenRepAttributes::DefineContext
// ---------------------------------------------------------------------------
//
void CCenRepAttributes::DefineContext()
    {
    FUNC_LOG;

    TPtrC value( KNullDesC );
    TPtrC key( KNullDesC );

    TPtrC conTypePtr ( KNullDesC );
    TPtrC conSourcePtr ( KNullDesC );


    for( TInt i = 0; i < iParamCount; i++ )
        {
        const RKeyValueArray& paramAttr = iParamArray[i]->Attributes();
        TInt paramAttrCount = paramAttr.Count();
        for( TInt j = 0; j < paramAttrCount; j++ )
            {
            key.Set( paramAttr[j]->Key() );
            value.Set( paramAttr[j]->Value() );

            // Check ContextSource
            if ( key.CompareF( KContextSource ) == KErrNone )
                {
                conSourcePtr.Set( value );
                }

            // Check ContextType
            else if ( key.CompareF( KContextType ) == KErrNone )
                {
                conTypePtr.Set( value);
                }
            }
        iCF.DefineContext( conSourcePtr,
                           conTypePtr,
                           KCenRepSourceInteractionSec );
        }
   }

// ---------------------------------------------------------------------------
// CCenRepAttributes::PublishContext
// ---------------------------------------------------------------------------
//
void CCenRepAttributes::PublishContext()
    {
    FUNC_LOG;

    RThread thread;
    iCF.PublishContext( *iContext, thread );
    thread.Close();
    }
