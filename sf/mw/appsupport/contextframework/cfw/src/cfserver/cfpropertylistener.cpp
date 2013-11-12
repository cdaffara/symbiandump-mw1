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
* Description:  CCFPropertyListener class implementation.
*
*/


#include "cfpropertylistener.h"
#include "cfpropertylistenercallback.h"
#include "cftrace.h"

// CONSTANTS

#ifdef _DEBUG

_LIT( KPanicCat, "CFPropertyListener" );

enum TPanicReason
    {
    EAlreadyListening
    };
    
LOCAL_C void Panic( TInt aReason )
    {
    User::Panic( KPanicCat, aReason );
    }

#endif

// MEMBER FUNCTIONS

CCFPropertyListener* CCFPropertyListener::NewL(
    const TUid& aCategory,
    const TUint32& aKey,
    MCFPropertyListenerCallBack& aCallBack )
    {
    FUNC_LOG;
    
    CCFPropertyListener* self = CCFPropertyListener::NewLC( aCategory,
        aKey,
        aCallBack );
    CleanupStack::Pop( self );
    
    return self;
    }
  
CCFPropertyListener* CCFPropertyListener::NewLC(
    const TUid& aCategory,
    const TUint32& aKey,
    MCFPropertyListenerCallBack& aCallBack )
    {
    FUNC_LOG;
    
    CCFPropertyListener* self = new( ELeave ) CCFPropertyListener( aCallBack );
    CleanupStack::PushL( self );
    self->ConstructL( aCategory, aKey );
    
    return self;
    }
  
// Destructor
CCFPropertyListener::~CCFPropertyListener()
    {
    FUNC_LOG;
    
	if(iStartCB)
		{
		iStartCB->Cancel();
		delete iStartCB; iStartCB = 0;
		}
    	
    Cancel();
    iProperty.Close();
    }

CCFPropertyListener::CCFPropertyListener( MCFPropertyListenerCallBack& aCallBack ):
    CActive( EPriorityStandard ),
    iCallBack( aCallBack )
    {
    FUNC_LOG;
    
    CActiveScheduler::Add( this );
    }

void CCFPropertyListener::ConstructL( const TUid& aCategory,
    const TUint32& aKey )
    {
    FUNC_LOG;
    
    User::LeaveIfError( iProperty.Attach( aCategory, aKey ) );
        
    INFO_2( "Attached to property UID: %x, Key: %x", aCategory, aKey );

	iStartCB = new (ELeave) CAsyncCallBack(TCallBack(CCFPropertyListener::StartCallback, this), EPriorityStandard);
	iStartCB->CallBack();
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFPropertyListener::Property
//-----------------------------------------------------------------------------
//
RProperty& CCFPropertyListener::Property()
    {
    FUNC_LOG;
    
    return iProperty;
    }

//-----------------------------------------------------------------------------
// CCFPropertyListener::RunL
//-----------------------------------------------------------------------------
//
void CCFPropertyListener::RunL()
    {
    FUNC_LOG;
    
    if( iStatus == KErrNone )
        {
        iCallBack.HandlePropertyChangedL();
        }
    Listen();
    }

//-----------------------------------------------------------------------------
// CCFPropertyListener::DoCancel
//-----------------------------------------------------------------------------
//
void CCFPropertyListener::DoCancel()
    {
    FUNC_LOG;
    
    iProperty.Cancel();
    }

//-----------------------------------------------------------------------------
// CCFPropertyListener::RunError
//-----------------------------------------------------------------------------
//
TInt CCFPropertyListener::RunError( TInt /*aError*/ )
    {
    FUNC_LOG;
    
    Listen();
    return KErrNone;
    }

//-----------------------------------------------------------------------------
// CCFPropertyListener::Listen
//-----------------------------------------------------------------------------
//
void CCFPropertyListener::Listen()
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( !IsActive(), Panic( EAlreadyListening ) );
    
    iProperty.Subscribe( iStatus );
    SetActive();
    }

//-----------------------------------------------------------------------------
// CCFPropertyListener::StartCallback
//-----------------------------------------------------------------------------
//
TInt CCFPropertyListener::StartCallback(TAny* aPtr)
	{
	static_cast<CCFPropertyListener*>(aPtr)->DoStart();
	
	return 0;
	}
	
//-----------------------------------------------------------------------------
// CCFPropertyListener::DoStart
//-----------------------------------------------------------------------------
//
void CCFPropertyListener::DoStart()
	{
	Listen();
    TRAP_IGNORE(iCallBack.HandlePropertyChangedL());
	}
