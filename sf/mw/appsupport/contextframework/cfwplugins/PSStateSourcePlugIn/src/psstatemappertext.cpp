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
* Description:  CPSStateMapperText class implementation.
*
*/


#include <cfcontextinterface.h>
#include <cfcontextobject.h>

#include "psstatemappertext.h"

/*
namespace
    {
    #ifdef _DEBUG
    
    // Panic category
    _LIT( KPanicCat, "PSStateMapText" );
    
    // Panic codes
    enum TPanicReason
        {
        EInvalidConstructionParameters
        };
        
    // Local panic function
    LOCAL_C void Panic( TInt aCode )
        {
        User::Panic( KPanicCat, aCode );
        }
    
    #endif
    }
*/

CPSStateMapperText* CPSStateMapperText::NewL(
    MCFContextInterface& aCF )
    {
    CPSStateMapperText* self = CPSStateMapperText::NewLC( aCF );
    CleanupStack::Pop();
    
    return self;
    }
  
CPSStateMapperText* CPSStateMapperText::NewLC(
    MCFContextInterface& aCF )
    {
    CPSStateMapperText* self = new( ELeave ) CPSStateMapperText( aCF );
    CleanupStack::PushL( self );
    self->ConstructL( );
    
    return self;
    }
  
// Destructor
CPSStateMapperText::~CPSStateMapperText()
    {
    if(iStartCB)
    	{
    	iStartCB->Cancel();
    	delete iStartCB; iStartCB = 0;
    	}
    iTextMappings.ResetAndDestroy();
    }

CPSStateMapperText::CPSStateMapperText(
    MCFContextInterface& aCF ) : CPSStateMapper( aCF )
    {
    }
    
// METHODS

TInt CPSStateMapperText::MapperType() const
	{
	return EPSTextMapper;
	}

void CPSStateMapperText::AddMappingL(const TDesC& aPSValue,
	const TDesC& aContextValue)
	{
	CTextPair* mapping = CTextPair::NewLC(aPSValue, aContextValue );
	iTextMappings.AppendL(mapping);
	CleanupStack::Pop(); // mapping
	}


void CPSStateMapperText::Define()
	{
	RProperty::Define( iPSCategory,
        iPSKey,
        RProperty::EText );
    
    iCF.DefineContext( *iSource,
        *iType,
        iSecurityPolicy );
	}

void CPSStateMapperText::InitializeL()
	{
	User::LeaveIfError(
        iProperty.Attach( iPSCategory, iPSKey ) );
	iStartCB = new (ELeave) CAsyncCallBack(TCallBack(CPSStateMapperText::StartCallback, this), EPriorityStandard);
	iStartCB->CallBack();
	}

//-----------------------------------------------------------------------------
// CPSStateMapperText::ProperyChangedL
//-----------------------------------------------------------------------------
//
void CPSStateMapperText::ProperyChangedL()
    {
    TBool mappingFound = EFalse;
    
    HBufC* psvalue = HBufC::NewLC( 512 ); // >> cleanup
    TPtr bufPtr = psvalue->Des();
    bufPtr.Zero();

    if ( iProperty.Get ( bufPtr ) == KErrNone )
    	{
	    for (TInt i = 0; i < iTextMappings.Count(); i++)
	    	{
	    	if (iTextMappings[i]->Text1() == *psvalue)
	    		{
	    		mappingFound = ETrue;
	    			
	    		// Configure context object
	            iContext->SetSourceL( *iSource );
	            iContext->SetTypeL( *iType );
	            iContext->SetValueL( iTextMappings[i]->Text2() );	
	    		
	    		// Publish
                RThread thread;
                iCF.PublishContext( *iContext, thread );
                thread.Close();
	    		}		
	    	}

		if ( !mappingFound && iPassThrough )
	    	{
	    	// Configure context object
            iContext->SetSourceL( *iSource );
            iContext->SetTypeL( *iType );
            iContext->SetValueL( *psvalue );	
    		
    		// Publish
            RThread thread;
            iCF.PublishContext( *iContext, thread );
            thread.Close();
	    	}
        }
    
    CleanupStack::PopAndDestroy(); // psvalue
    }

TInt CPSStateMapperText::StartCallback(TAny* aPtr)
	{
	static_cast<CPSStateMapperText*>(aPtr)->DoStart();
	
	return 0;
	}
	
void CPSStateMapperText::DoStart()
	{
	Subscribe();
    TRAP_IGNORE(ProperyChangedL());
	}

