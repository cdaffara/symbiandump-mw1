// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include "hwrmuiplugin.h"

CHwrmUiPlugin* CHwrmUiPlugin::NewL()
	{
	CHwrmUiPlugin* self = new (ELeave)CHwrmUiPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self); 	
	return self;
	}

CHwrmUiPlugin::CHwrmUiPlugin()
	{	
	}
	
void CHwrmUiPlugin::ConstructL()
	{
	CCoeEnv* env = CCoeEnv::Static();
    if ( env )
        {//register for notification with CONE
        env->AddForegroundObserverL(*this);
        }
	}

CHwrmUiPlugin::~CHwrmUiPlugin()
	{			
    CCoeEnv* env = CCoeEnv::Static();
    if ( env )
        {
        env->RemoveForegroundObserver(*this);
        }     
	}	

void CHwrmUiPlugin::AddForegroundObserverL(MHwrmForegroundObserver* aObserver)
	{
	iHwrmForegroundObserver = aObserver;
	}

TBool CHwrmUiPlugin::InForeground() const
	{
	CCoeEnv* env = CCoeEnv::Static();	
	if ( env )
		{
		return (env->WsSession().GetFocusWindowGroup() == env->RootWin().Identifier() );
		}
	return EFalse;
	}

void CHwrmUiPlugin::HandleGainingForeground()
	{
	if(iHwrmForegroundObserver)
		{
		iHwrmForegroundObserver->HandleGainingForeground();
		}
	}

void CHwrmUiPlugin::HandleLosingForeground()
	{	
	if(iHwrmForegroundObserver)
		{
		iHwrmForegroundObserver->HandleLosingForeground();
		}		
	}
