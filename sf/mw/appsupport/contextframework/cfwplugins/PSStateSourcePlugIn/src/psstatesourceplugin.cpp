/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPSStateSourcePlugIn class implementation.
*
*/


#define __INCLUDE_CAPABILITY_NAMES__

#include <cfcontextinterface.h>
#include <cfcontextsourcesettingarray.h>
#include <e32capability.h>

#include "psstatesourceappstatedomainpskeys.h"
#include "psstatesourceplugin.h"
#include "psstatemapper.h"
#include "psstatemapperint.h"
#include "psstatemappertext.h"

const TUint KLimit = 0xFFFFFFFF;
_LIT (KAttrMapper, "mapper" );
_LIT (KAttrCategory, "category" );
_LIT (KAttrKey, "key" );
_LIT (KAttrCapability, "capability" );
_LIT (KAttrSource, "source" );
_LIT (KAttrType, "type" );
_LIT (KAttrPassThrough, "passthrough" );


_LIT (KValueInt, "int" );
_LIT (KValueText, "text" );

_LIT (KValueTrue, "true" );

_LIT (KTagIntMapping, "IntMapping");
_LIT (KTagTextMapping, "TextMapping");

_LIT (KAttributePSValue, "psvalue");
_LIT (KAttributeContextValue, "contextvalue");


//namespace PSSOURCE
//    {
//    // Panic category
//    _LIT( KPanicCat, "PSStateSrc" );
//    
//    // Panic codes
//    enum TPanicReason
//        {
//        EInvalidPSKey
//        };
//        
//    // Local panic function
//    LOCAL_C void Panic( TInt aCode )
//        {
//        User::Panic( KPanicCat, aCode );
//        }
//    }
//
//using namespace PSSOURCE;

CPSStateSourcePlugIn* CPSStateSourcePlugIn::NewL( 
    TContextSourceInitParams* aParams )
    {
    CPSStateSourcePlugIn* self = CPSStateSourcePlugIn::NewLC( aParams );
    CleanupStack::Pop();
    
    return self;
    }
  
CPSStateSourcePlugIn* CPSStateSourcePlugIn::NewLC( 
TContextSourceInitParams* aParams )
    {
    CPSStateSourcePlugIn* self =
        new( ELeave ) CPSStateSourcePlugIn( aParams );
    CleanupStack::PushL( self );
    
    return self;
    }
  
// Destructor
CPSStateSourcePlugIn::~CPSStateSourcePlugIn()
    {
    delete iSettings;
    iMappers.ResetAndDestroy();
    }

CPSStateSourcePlugIn::CPSStateSourcePlugIn(
    TContextSourceInitParams* aParams ):
    CCFContextSourcePlugIn( aParams )
    {
    }

// METHODS



//-----------------------------------------------------------------------------
// CPSStateSourcePlugIn::HandleSettingL
//-----------------------------------------------------------------------------
//
void CPSStateSourcePlugIn::HandleSettingL(
    CCFContextSourceSettingArray* aSettingList )
	{
    
    iSettings = aSettingList;
    
    for (TInt i = 0; i < iSettings->Count(); i++ )
    	{
		CPSStateMapper* mapper = NULL;
    	
    	const RKeyValueArray& attributes = iSettings->Item( i ).Attributes();
    	// Check application and view names (also default values)
        for( TInt j = 0; j < attributes.Count(); j++ )
            {
            TPtrC key( attributes[ j ]->Key() );
            TPtrC value( attributes[ j ]->Value() );

            if( key.CompareF( KAttrMapper ) == KErrNone )
                {
                if (value.CompareF( KValueInt) == KErrNone )
                	{
                	CPSStateMapperInt* intmapper 
                		= CPSStateMapperInt::NewL( iCF );
                	mapper = (CPSStateMapper*) intmapper;
                	}
                else if (value.CompareF( KValueText) == KErrNone )
                	{
                	CPSStateMapperText* textmapper 
                		= CPSStateMapperText::NewL( iCF );
                	mapper = (CPSStateMapper*) textmapper;
                	}
                } 
            } 

		// Leave from here if PS state mapper is NULL.This may happen if none of the attributes' key value matches with the state mapper 
		if ( mapper == NULL)
			User::LeaveIfError(KErrArgument);
        
        for( TInt j = 0; j < attributes.Count(); j++ )
            {
            TPtrC key( attributes[ j ]->Key() );
            TPtrC value( attributes[ j ]->Value() );

            if( key.CompareF( KAttrCategory ) == KErrNone )
                {
               	TUint32 category;
                TInt err = ConvertHexToTUint( value, category );
				mapper->SetPSCategory(TUid::Uid(category));                
                }
            else if( key.CompareF( KAttrKey ) == KErrNone )
                {
                TUint32 key;
                TInt err = ConvertHexToTUint( value, key );
				mapper->SetPSKey(key);                
                }
            else if( key.CompareF( KAttrCapability ) == KErrNone )
                {
                TSecurityPolicy policy;
				ConvertDescToSecurityPolicy( value, policy ); 
				mapper->SetSecurityPolicy( policy );                
                }
            else if( key.CompareF( KAttrSource ) == KErrNone )
                {
				mapper->SetSourceL(value);
                }
            else if( key.CompareF( KAttrType ) == KErrNone )
                {
				mapper->SetTypeL(value);
                }
            else if( key.CompareF( KAttrPassThrough ) == KErrNone )
                {
                if (value.CompareF( KValueTrue ) == KErrNone )
                	{
                	mapper->SetPassThrough(ETrue);
                	} else 
                	{
                	mapper->SetPassThrough(EFalse);
                	}	
                }
            }
        
        const RSettingParameterArray& parameters 
        	= iSettings->Item( i ).Parameters();
        
        switch (mapper->MapperType())
        	{
        	case CPSStateMapper::EPSIntMapper:
        		{
        		ParseIntMapperParamsL(*mapper, parameters);
        		break;
        		}
            case CPSStateMapper::EPSTextMapper:
            	{
            	ParseTextMapperParamsL(*mapper, parameters);
            	break;
            	}
        	}

        iMappers.AppendL( mapper );    
    	}
	}


void CPSStateSourcePlugIn::ParseIntMapperParamsL(CPSStateMapper& aMapper,
	const RSettingParameterArray& aParameters)
	{
	CPSStateMapperInt* intmapper = (CPSStateMapperInt*) &aMapper;
    
    for( TInt j = 0; j < aParameters.Count(); ++j )
        {
        if ( aParameters[ j ]->Name() != KTagIntMapping )
            {
            continue;
            }

        TPtrC psValueDesc( KNullDesC );
        TPtrC contextValue( KNullDesC );
        const RKeyValueArray& paramAttr = aParameters[ j ]->Attributes();
        for( TInt k = 0; k < paramAttr.Count(); ++k )
            {
            TPtrC key( paramAttr[ k ]->Key() );
            TPtrC value( paramAttr[ k ]->Value() );
            
            if( key.CompareF( KAttributePSValue ) == KErrNone )
                {
                psValueDesc.Set( value );
                }
            
            else if( key.CompareF( KAttributeContextValue ) == KErrNone )
                {
                contextValue.Set( value );
                }
            }

		TInt psValue;
		
		TLex lex( psValueDesc );
		lex.Val(psValue);

		intmapper->AddMappingL(psValue, contextValue);
        }
	}


void CPSStateSourcePlugIn::ParseTextMapperParamsL(CPSStateMapper& aMapper,
	const RSettingParameterArray& aParameters)
	{
	CPSStateMapperText* textmapper = (CPSStateMapperText*) &aMapper;
    
    for( TInt j = 0; j < aParameters.Count(); ++j )
        {
        if ( aParameters[ j ]->Name() != KTagTextMapping )
            {
            continue; 
            }

        TPtrC psValueDesc( KNullDesC );
        TPtrC contextValue( KNullDesC );
        const RKeyValueArray& paramAttr = aParameters[ j ]->Attributes();
        for( TInt k = 0; k < paramAttr.Count(); ++k )
            {
            TPtrC key( paramAttr[ k ]->Key() );
            TPtrC value( paramAttr[ k ]->Value() );
            
            if( key.CompareF( KAttributePSValue ) == KErrNone )
                {
                psValueDesc.Set( value );
                }
            
            else if( key.CompareF( KAttributeContextValue ) == KErrNone )
                {
                contextValue.Set( value );
                }
            }

		textmapper->AddMappingL(psValueDesc, contextValue);
        }
	}

//-----------------------------------------------------------------------------
// CPSStateSourcePlugIn::DefineContextsL
//-----------------------------------------------------------------------------
//
void CPSStateSourcePlugIn::DefineContextsL()
    {
    for (TInt i = 0; i < iMappers.Count(); i++)
    	{
    	iMappers[i]->Define();	
    	}
    }
    
//-----------------------------------------------------------------------------
// CPSStateSourcePlugIn::InitializeL
//-----------------------------------------------------------------------------
//
void CPSStateSourcePlugIn::InitializeL()
    {
    for (TInt i = 0; i < iMappers.Count(); i++)
    	{
    	iMappers[i]->InitializeL();	
    	}
    }

    
//-----------------------------------------------------------------------------
// CPSStateSourcePlugIn::ConvertHexToTUint
//-----------------------------------------------------------------------------
//
TInt CPSStateSourcePlugIn::ConvertHexToTUint( const TDesC& aString, 
    TUint32& aValue )
    {
    TInt err = KErrNone;

    TLex lex( aString );

    // Get rid of '0x'
    lex.Inc( 2 );
    if( lex.BoundedVal( aValue, EHex, KLimit ) != KErrNone )
        {
        err = KErrNotFound;
        }

    return err;
    }


//-----------------------------------------------------------------------------
// CPSStateSourcePlugIn::ConvertDescToSecurityPolicy
//-----------------------------------------------------------------------------
//
void CPSStateSourcePlugIn::ConvertDescToSecurityPolicy( const TDesC& aString, 
    TSecurityPolicy& aSecurityPolicy )
	{
	TCapability capabilities[7] = 
		{
		ECapability_None,
		ECapability_None,
		ECapability_None,
		ECapability_None,
		ECapability_None,
		ECapability_None,
		ECapability_None
		};
		
	TLex lexer( aString );
	TInt tokenLength = 0;	
	TInt i = 0;
	do  {
		lexer.SkipSpace();
		lexer.Mark();
		lexer.SkipCharacters();
	
		tokenLength = lexer.TokenLength();
		if (tokenLength != 0)
			{
			TPtrC token = lexer.MarkedToken();
			if ( i < 7 )
				{
				capabilities[i++] = ConvertToCapabilityNumber( token );
				}
			else
				{
				// TSecurityPolicy can hold maximum of 7 capabilities
				// if more are given, we make a security policy
				// that always fails.
				capabilities[0] = ECapability_Denied;
				}
			}
		}
	while( tokenLength != 0 );
			
	TSecurityPolicy policy  (
		capabilities[0],
		capabilities[1],
		capabilities[2],
		capabilities[3],
		capabilities[4],
		capabilities[5],
		capabilities[6] );
		
	aSecurityPolicy = policy;
	}

//-----------------------------------------------------------------------------
// CPSStateSourcePlugIn::ConvertDescToSecurityPolicy
//-----------------------------------------------------------------------------
//
TCapability CPSStateSourcePlugIn::ConvertToCapabilityNumber( 
const TDesC& aString )
	{
	for( TInt i = 0; i < ECapability_Limit; i++ )
		{
		const TPtrC8 capName8 
			= TPtrC8( (const unsigned char*) CapabilityNames[i] );
		TBuf<20> capName;
		capName.Copy( capName8 );
		
		if ( aString.CompareF( capName ) == KErrNone )
			{
			return (TCapability) i;
			}
		}
	
	// In the case the capability is not found from the list
	// a capability that always fail is returned.

	return ECapability_Denied;
	}

