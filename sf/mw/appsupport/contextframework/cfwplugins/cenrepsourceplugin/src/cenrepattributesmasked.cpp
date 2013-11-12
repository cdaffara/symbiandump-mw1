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
* Description:  CCenRepAttributesMasked class implementation.
*
*/


#include "cenrepattributesmasked.h"
#include "cenreptrace.h"

// ======== LOCAL FUNCTIONS =========

// ======== MEMBER FUNCTIONS ========

CCenRepAttributesMasked* CCenRepAttributesMasked::NewL(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings  )
    {
    FUNC_LOG;

    CCenRepAttributesMasked* self = CCenRepAttributesMasked::NewLC(
        aCF,
        aSettings );
    CleanupStack::Pop( self );

    return self;
    }

CCenRepAttributesMasked* CCenRepAttributesMasked::NewLC(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings  )
    {
    FUNC_LOG;

    CCenRepAttributesMasked* self =
        new( ELeave ) CCenRepAttributesMasked( aCF, aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

CCenRepAttributesMasked::~CCenRepAttributesMasked()
    {
    FUNC_LOG;
    }

CCenRepAttributesMasked::CCenRepAttributesMasked(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings  ) :
    CCenRepAttributes ( aCF, aSettings )
    {
    FUNC_LOG;
    }
    
void CCenRepAttributesMasked::ConstructL()
    {
    FUNC_LOG;
    
    iContext = CCFContextObject::NewL();
    }
    
TUint32 CCenRepAttributesMasked::DefinedMask()
    {
    FUNC_LOG;

    TUint32 definedMask = 0x00000000;

    for( TInt i = 0; i < iParamCount; i++ )
        {
        TUint32 valueValue = 0;
        TPtrC value( KNullDesC );
        TPtrC key( KNullDesC );
        TBool defOk( ETrue );
        
        const RKeyValueArray& paramAttr = iParamArray[i]->Attributes();
        TInt paramAttrCount = paramAttr.Count();
        TInt err( KErrGeneral );

        for( TInt j = 0; j < paramAttrCount; j++ )
            {
            key.Set( paramAttr[j]->Key() );
            value.Set( paramAttr[j]->Value() );
            if ( value.Length() <= 0 )
                {
                defOk = EFalse;
                }

            // Check Value
            if ( key.CompareF( KValue ) == KErrNone )
                {
                err = ConvertToHex( value, valueValue );
                // Do not break here because
                // existence of all values need to be checked
                }
            }

        if ( ( err == KErrNone ) && defOk )
            {
            definedMask = definedMask | valueValue;
            }
        }

     return definedMask;
    }

//-----------------------------------------------------------------------------
// CCenRepAttributesMasked::UpdateContextL
//-----------------------------------------------------------------------------
//
void CCenRepAttributesMasked::UpdateContextL( const TInt aKeyValue )
    {
    FUNC_LOG;

    TUint32 definedKeyMask = DefinedMask();

    for( TInt i = 0; i < iParamCount; i++ )
        {
        TBool publishOk( ETrue );

        TUint32 valueValue = 0;
        TPtrC value( KNullDesC );
        TPtrC key( KNullDesC );

        TPtrC conTypePtr ( KNullDesC );
        TPtrC conSourcePtr ( KNullDesC );
        TPtrC conValueSetPtr ( KNullDesC );
        TPtrC conValueUnSetPtr ( KNullDesC );
        
        const RKeyValueArray& paramAttr = iParamArray[i]->Attributes();
        TInt paramAttrCount = paramAttr.Count();
        for( TInt j = 0; j < paramAttrCount; j++ )
            {
            key.Set( paramAttr[j]->Key() );
            value.Set( paramAttr[j]->Value() );
            
            if ( value.Length() <= 0 )
                    {
                    publishOk = EFalse;
                    }

            // Check Value
            if ( key.CompareF( KValue ) == KErrNone )
                {
                TInt err = ConvertToHex( value, valueValue );
                }

            // Check ContextSource
            else if ( key.CompareF( KContextSource ) == KErrNone )
                {
                conSourcePtr.Set( value );
                }

            // Check ContextType
            else if ( key.CompareF( KContextType ) == KErrNone )
                {
                conTypePtr.Set( value);
                }

            // Check ContextValueSet
            else if ( key.CompareF( KContextValueSet ) == KErrNone )
                {
                conValueSetPtr.Set( value );
                }

            // Check ContextValueUnSet
            else if ( key.CompareF( KContextValueUnSet ) == KErrNone )
                {
                conValueUnSetPtr.Set( value );
                }
            }

        iContext->SetSourceL( conSourcePtr );
        iContext->SetTypeL( conTypePtr );
        
        if ( valueValue == 0 )
            {
            if ( definedKeyMask & aKeyValue )
                {
                iContext->SetValueL( conValueSetPtr );
                }
            else
                {
                iContext->SetValueL( conValueUnSetPtr );
                }
            }

        else if ( aKeyValue & valueValue )
            {
            iContext->SetValueL( conValueSetPtr );
            }

        else
            {
            iContext->SetValueL( conValueUnSetPtr );
            }

        if ( publishOk )
            {
            PublishContext();
            }
        }
    }
