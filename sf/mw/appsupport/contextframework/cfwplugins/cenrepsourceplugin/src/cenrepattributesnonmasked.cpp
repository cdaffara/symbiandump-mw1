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
* Description:  CCenRepAttributesNonMasked class implementation.
*
*/


#include "cenrepattributesnonmasked.h"
#include "cenreptrace.h"

// ======== LOCAL FUNCTIONS =========

// ======== MEMBER FUNCTIONS ========

CCenRepAttributesNonMasked* CCenRepAttributesNonMasked::NewL(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings  )
    {
    FUNC_LOG;

    CCenRepAttributesNonMasked* self = CCenRepAttributesNonMasked::NewLC(
        aCF,
        aSettings );
    CleanupStack::Pop( self );

    return self;
    }

CCenRepAttributesNonMasked* CCenRepAttributesNonMasked::NewLC(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings  )
    {
    FUNC_LOG;

    CCenRepAttributesNonMasked* self =
        new( ELeave ) CCenRepAttributesNonMasked( aCF, aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

CCenRepAttributesNonMasked::~CCenRepAttributesNonMasked()
    {
    FUNC_LOG;
    }

CCenRepAttributesNonMasked::CCenRepAttributesNonMasked(
    MCFContextInterface& aCF,
    CCFContextSourceSettingArray& aSettings  ) :
    CCenRepAttributes ( aCF, aSettings )
    {
    FUNC_LOG;
    }

void CCenRepAttributesNonMasked::ConstructL()
    {
    FUNC_LOG;

    iContext = CCFContextObject::NewL();
    }

//----------------------------------------------------------------------------
// CCenRepAttributesNonMasked::UpdateContextL
//----------------------------------------------------------------------------
//
void CCenRepAttributesNonMasked::UpdateContextL( const TInt aKeyValue )
    {
    FUNC_LOG;

    TInt valueValue = 0;
    TPtrC value( KNullDesC );
    TPtrC key( KNullDesC );

    TPtrC conTypePtr ( KNullDesC );
    TPtrC conSourcePtr ( KNullDesC );
    TPtrC conValuePtr ( KNullDesC );

    for( TInt i = 0; i < iParamCount; i++ )
        {
        TBool publishOk( ETrue );
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
                TInt err = ConvertToInt( value, valueValue );
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

            // Check ContextValue
            else if ( key.CompareF( KContextValue ) == KErrNone )
                {
                conValuePtr.Set( value );
                }
            }

        if ( aKeyValue == valueValue )
            {
            iContext->SetSourceL( conSourcePtr );
            iContext->SetTypeL( conTypePtr );
            iContext->SetValueL( conValuePtr );

            if ( publishOk )
                {
                PublishContext();
                }
            }
         }
    }
