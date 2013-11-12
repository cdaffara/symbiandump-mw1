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
* Description:  CPSStateMapper class implementation.
*
*/


#include "psstatemapper.h"
#include "psstatemapperint.h"

namespace
    {
    #ifdef _DEBUG
    
    // Panic category
    _LIT( KPanicCat, "PSStateMap" );
    
    // Panic codes
    enum TPanicReason
        {
        EInvalidConstructionParameters,
        EPropertyAlreadySubscribed
        };
        
    // Local panic function
    LOCAL_C void Panic( TInt aCode )
        {
        User::Panic( KPanicCat, aCode );
        }
    
    #endif
    }
  
// Destructor
CPSStateMapper::~CPSStateMapper()
    {
    Cancel();
    iProperty.Close();
    delete iSource;
    delete iType;
    delete iContext;
    }

CPSStateMapper::CPSStateMapper( MCFContextInterface& aCF ) : 
	CActive( EPriorityStandard ),
	iCF (aCF)
    {
    CActiveScheduler::Add( this );
    }
    
void CPSStateMapper::ConstructL( )
    {
    iContext = CCFContextObject::NewL();
    }

// METHODS


void CPSStateMapper::SetPSCategory (TUid aPSCategory)
	{
	iPSCategory = aPSCategory;	
	}

void CPSStateMapper::SetPSKey (TUint32 aKey)
	{
	iPSKey = aKey;	
	}

void CPSStateMapper::SetSecurityPolicy (const TSecurityPolicy& aSecurityPolicy)
	{
	iSecurityPolicy = aSecurityPolicy;	
	}

void CPSStateMapper::SetSourceL ( const TDesC& aSource )
	{
	delete iSource;
	iSource = NULL;
	iSource = aSource.AllocL();
	}



void CPSStateMapper::SetTypeL ( const TDesC& aType )
	{
	delete iType;
	iType = NULL;
	iType = aType.AllocL();	
	}    

void CPSStateMapper::SetPassThrough(TBool aPassThrough)
	{
	iPassThrough = aPassThrough;
	}

//-----------------------------------------------------------------------------
// CPSStateMapper::Source
//-----------------------------------------------------------------------------
//
const TDesC& CPSStateMapper::Source() const
    {
    return *iSource;
    }

//-----------------------------------------------------------------------------
// CPSStateMapper::Type
//-----------------------------------------------------------------------------
//
const TDesC& CPSStateMapper::Type() const
    {
    return *iType;
    }

//-----------------------------------------------------------------------------
// CPSStateMapper::SecurityPolicy
//-----------------------------------------------------------------------------
//
const TSecurityPolicy& CPSStateMapper::SecurityPolicy() const
    {
    return iSecurityPolicy;
    }

//-----------------------------------------------------------------------------
// CPSStateMapper::Subscribe
//-----------------------------------------------------------------------------
//
void CPSStateMapper::Subscribe()
    {
    __ASSERT_DEBUG( !IsActive(), Panic( EPropertyAlreadySubscribed ) );
    
    iProperty.Subscribe( iStatus );
    SetActive();
    }

//-----------------------------------------------------------------------------
// CPSStateMapper::RunL
//-----------------------------------------------------------------------------
//
void CPSStateMapper::RunL()
    {
    if( iStatus == KErrNone )
        {
        ProperyChangedL();
        }
        
    Subscribe();
    }

//-----------------------------------------------------------------------------
// CPSStateMapper::DoCancel
//-----------------------------------------------------------------------------
//
void CPSStateMapper::DoCancel()
    {
    iProperty.Cancel();
    }

//-----------------------------------------------------------------------------
// CPSStateMapper::RunError
//-----------------------------------------------------------------------------
//
TInt CPSStateMapper::RunError( TInt /*aError*/ )
    {
    Subscribe();
    
    return KErrNone;
    }
