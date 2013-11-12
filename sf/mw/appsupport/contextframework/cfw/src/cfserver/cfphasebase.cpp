/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFPhaseBase class implementation.
*
*/


#include "cfphasebase.h"
#include "cftrace.h"

#ifdef _DEBUG

// Panic category
_LIT( KPanicCat, "Ph.Base" );

// Panic codes
enum TPanicCode
    {
    ENoRequestToComplete
    };

// Local panic function
LOCAL_C void Panic( TInt aCode )
    {
    User::Panic( KPanicCat, aCode );
    }

#endif

CCFPhaseBase::~CCFPhaseBase()
    {
    FUNC_LOG;
    
    if( iStarterRequest )
        {
        Complete( KErrCancel );
        }
    }

void CCFPhaseBase::ConstructL()
    {
    FUNC_LOG;
    }

CCFPhaseBase::CCFPhaseBase( TCFPhaseId aId, MCFContextInterface& aCF ):
    iId( aId ),
    iCF( aCF )
    {
    FUNC_LOG;
    }

// METHODS

//-----------------------------------------------------------------------------
// CCFPhaseBase::PhaseId
//-----------------------------------------------------------------------------
//
TInt CCFPhaseBase::PhaseId() const
    {
    FUNC_LOG;
    
    return iId;
    }

//-----------------------------------------------------------------------------
// CCFPhaseBase::Complete
//-----------------------------------------------------------------------------
//
void CCFPhaseBase::Complete( TInt aError )
    {
    FUNC_LOG;
    
    __ASSERT_DEBUG( iStarterRequest, Panic( ENoRequestToComplete ) );
    
    User::RequestComplete( iStarterRequest, aError );
    }

//------------------------------------------------------------------------------
// CCFPhaseBase::Cancel
//------------------------------------------------------------------------------
//
void CCFPhaseBase::Cancel()
    {
    FUNC_LOG;

    __ASSERT_DEBUG( iStarterRequest, Panic( ENoRequestToComplete ) );
    if ( iStarterRequest )
		{
		User::RequestComplete( iStarterRequest, KErrCancel );
		}
    }

//-----------------------------------------------------------------------------
// CCFPhaseBase::HandleEvent
//-----------------------------------------------------------------------------
//
void CCFPhaseBase::HandleEvent( MCFStarterEventHandler::TCFStarterEvents /*aEvent*/ )
    {
	  FUNC_LOG;
    }
