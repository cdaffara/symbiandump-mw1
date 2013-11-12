/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPSStateMapperInt class implementation.
*
*/


#include <cfcontextinterface.h>
#include <cfcontextobject.h>
#include <e32debug.h>

#include "psstatemapperint.h"

/*
namespace
    {
    #ifdef _DEBUG
    
    // Panic category
    _LIT( KPanicCat, "PSStateMapInt" );
    
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

CPSStateMapperInt* CPSStateMapperInt::NewL(
    MCFContextInterface& aCF )
    {
    CPSStateMapperInt* self = CPSStateMapperInt::NewLC( aCF );
    CleanupStack::Pop();
    
    return self;
    }
  
CPSStateMapperInt* CPSStateMapperInt::NewLC(
    MCFContextInterface& aCF )
    {
    CPSStateMapperInt* self = new( ELeave ) CPSStateMapperInt( aCF );
    CleanupStack::PushL( self );
    self->ConstructL( );
    
    return self;
    }
  
// Destructor
CPSStateMapperInt::~CPSStateMapperInt()
    {
   	if(iStartCB)
		{
		iStartCB->Cancel();
		delete iStartCB; iStartCB = 0;
		}
		
    iIntNameMappings.ResetAndDestroy();
    }

CPSStateMapperInt::CPSStateMapperInt(
    MCFContextInterface& aCF ) : CPSStateMapper( aCF )
    {
    }
    
// METHODS


TInt CPSStateMapperInt::MapperType() const
	{
	return EPSIntMapper;
	}

void CPSStateMapperInt::AddMappingL(TInt aPSValue, const TDesC& aContextValue)
	{
	
	CIntNamePair* mapping = CIntNamePair::NewLC(aPSValue, aContextValue );
	iIntNameMappings.AppendL(mapping);
	CleanupStack::Pop(); // mapping
	}


void CPSStateMapperInt::Define()
	{
	TInt err = RProperty::Define( iPSCategory,
        iPSKey,
        RProperty::EInt );
    
#ifdef _DEBUG
    RDebug::Printf("CPSStateMapperInt::Define: iPSCategory=%08X, iPSKey=%08X, err = %d", iPSCategory.iUid, iPSKey, err);
#endif

    iCF.DefineContext( *iSource,
        *iType,
        iSecurityPolicy );
	}

void CPSStateMapperInt::InitializeL()
	{
	User::LeaveIfError(
        iProperty.Attach( iPSCategory, iPSKey ) );
    
    iStartCB = new (ELeave) CAsyncCallBack(TCallBack(CPSStateMapperInt::StartCallback, this), EPriorityStandard);
	iStartCB->CallBack();
	
	}

//-----------------------------------------------------------------------------
// CPSStateMapperInt::ProperyChangedL
//-----------------------------------------------------------------------------
//
void CPSStateMapperInt::ProperyChangedL()
    {
    TBool mappingFound = EFalse;
    
    TInt psvalue = 0;
    if (iProperty.Get ( psvalue ) == KErrNone)
    	{
#ifdef _DEBUG
    	RDebug::Printf("CPSStateMapperInt::ProperyChangedL: iPSCategory=%08X, iPSKey=%08X, val = %d", iPSCategory.iUid, iPSKey, psvalue);
#endif
    	TInt count = iIntNameMappings.Count();
	    for (TInt i = 0; i < count; i++)
	    	{
	    	if (iIntNameMappings[i]->Int() == psvalue)
	    		{
	    		mappingFound = ETrue;
	    			
	    		// Configure context object
	            iContext->SetSourceL( *iSource );
	            iContext->SetTypeL( *iType );
	            iContext->SetValueL( iIntNameMappings[i]->Name() );	
	    		
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
	
			TBuf<10> contextValue;
			contextValue.Zero();
			contextValue.AppendNum(psvalue);
	        iContext->SetValueL( contextValue );	
			
			// Publish
	        RThread thread;
	        iCF.PublishContext( *iContext, thread );
	        thread.Close();
	    	}
        }
    else
    	{

    	}
        
    }

TInt CPSStateMapperInt::StartCallback(TAny* aPtr)
	{
	static_cast<CPSStateMapperInt*>(aPtr)->DoStart();
	
	return 0;
	}
	
void CPSStateMapperInt::DoStart()
	{
	Subscribe();
    TRAP_IGNORE(ProperyChangedL());
	}
