/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/



#include "ScriptEventNotifierSession.h"
#include <CFScriptEvent.h>

CScriptEventNotifierSession* CScriptEventNotifierSession::NewL()
    {
    CScriptEventNotifierSession* self =
        CScriptEventNotifierSession::NewLC();
    CleanupStack::Pop();

    return self;
    }

CScriptEventNotifierSession* CScriptEventNotifierSession::NewLC()
   {
    CScriptEventNotifierSession* self =
        new( ELeave ) CScriptEventNotifierSession;
    CleanupStack::PushL( self );
    self->ConstructL();

    return self;
    }

CScriptEventNotifierSession::CScriptEventNotifierSession()
    {
    }

void CScriptEventNotifierSession::ConstructL()
    {
    }

// Destructor
CScriptEventNotifierSession::~CScriptEventNotifierSession()
    {
    }

void CScriptEventNotifierSession::FireActionL( CCFScriptEvent* aEvent )
	{
	delete aEvent;		
	}

void CScriptEventNotifierSession::FireActionL(
    const CCFContextSourceCommand& /*aCommand*/ )
    {
    // Nothing must be done.
    }

TInt CScriptEventNotifierSession::GetActionSecurityPolicy(
	const TDesC& /*aActionIdentifier*/, TSecurityPolicy& aPolicy )
	{
	static _LIT_SECURITY_POLICY_PASS( alwaysPass );
	aPolicy = alwaysPass;
    return KErrNone;
	}
