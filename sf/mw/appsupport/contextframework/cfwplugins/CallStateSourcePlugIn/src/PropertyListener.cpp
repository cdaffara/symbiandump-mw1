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
* Description:  CPropertyListener class implementation.
*
*/


#include "PropertyListener.h"
#include "PropertyListenerCallBack.h"

// CONSTANTS
#ifdef _DEBUG
_LIT( KPanicCat, "PropertyListener" );

enum TPanicReason
    {
    EAlreadyListening
    };
    
LOCAL_C void Panic( TInt aReason )
    {
    User::Panic( KPanicCat, aReason );
    }
#endif

CPropertyListener* CPropertyListener::NewL(
    const TUid& aCategory,
    const TUint32& aKey,
    MPropertyListenerCallBack& aCallBack )
    {
    CPropertyListener* self = CPropertyListener::NewLC( aCategory,
        aKey,
        aCallBack );
    CleanupStack::Pop( self );
    
    return self;
    }
  
CPropertyListener* CPropertyListener::NewLC(
    const TUid& aCategory,
    const TUint32& aKey,
    MPropertyListenerCallBack& aCallBack )
    {
    CPropertyListener* self = new( ELeave ) CPropertyListener( aCallBack );
    CleanupStack::PushL( self );
    self->ConstructL( aCategory, aKey );
    
    return self;
    }
  
// Destructor
CPropertyListener::~CPropertyListener()
    {
    if(iStartCB)
    	{
    	iStartCB->Cancel();
    	delete iStartCB; iStartCB = 0;
    	}
    	
    Cancel();
    iProperty.Close();
    }

CPropertyListener::CPropertyListener( MPropertyListenerCallBack& aCallBack ):
    CActive( EPriorityStandard ),
    iCallBack( aCallBack )
    {
    CActiveScheduler::Add( this );
    }

void CPropertyListener::ConstructL( const TUid& aCategory,
    const TUint32& aKey )
    {
    User::LeaveIfError( iProperty.Attach( aCategory, aKey ) );

    iStartCB = new (ELeave) CAsyncCallBack(TCallBack(CPropertyListener::StartCallback, this), EPriorityStandard);
    iStartCB->CallBack();
    }

// METHODS

//-----------------------------------------------------------------------------
// CPropertyListener::Property
//-----------------------------------------------------------------------------
//
RProperty& CPropertyListener::Property()
    {
    return iProperty;
    }

//-----------------------------------------------------------------------------
// CPropertyListener::RunL
//-----------------------------------------------------------------------------
//
void CPropertyListener::RunL()
    {
    if( iStatus == KErrNone )
        {
        Listen();
        iCallBack.HandlePropertyChangedL();
        }else
            {
            Listen();
            }
    }

//-----------------------------------------------------------------------------
// CPropertyListener::DoCancel
//-----------------------------------------------------------------------------
//
void CPropertyListener::DoCancel()
    {
    iProperty.Cancel();
    }

//-----------------------------------------------------------------------------
// CPropertyListener::RunError
//-----------------------------------------------------------------------------
//
TInt CPropertyListener::RunError( TInt /*aError*/ )
    {
    Listen();
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CPropertyListener::Listen
//-----------------------------------------------------------------------------
//
void CPropertyListener::Listen()
    {
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyListening ) );
    
    iProperty.Subscribe( iStatus );
    SetActive();
    }

//-----------------------------------------------------------------------------
// CPropertyListener::StartCallback
//-----------------------------------------------------------------------------
//
TInt CPropertyListener::StartCallback(TAny* aPtr)
	{
	static_cast<CPropertyListener*>(aPtr)->DoStart();
	
	return 0;
	}
	
//-----------------------------------------------------------------------------
// CPropertyListener::DoStart
//-----------------------------------------------------------------------------
//
void CPropertyListener::DoStart()
	{
	Listen();
    TRAP_IGNORE(iCallBack.HandlePropertyChangedL());
	}
