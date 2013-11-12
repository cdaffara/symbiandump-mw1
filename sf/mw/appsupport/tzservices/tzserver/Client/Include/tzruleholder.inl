// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

inline void CTzRuleHolder::SetTzActualisedRules(CVTzActualisedRules* aRules)
	{
	delete iTzActualisedRules;
	iTzActualisedRules = aRules;
	}
	

inline void CTzRuleHolder::SetTzRules(CTzRules* aRules)
	{
	delete iTzRules;
	iTzRules = aRules;
	}

inline CVTzActualisedRules* CTzRuleHolder::TzActualisedRules()
	{
	return iTzActualisedRules;
	}

inline CTzRules* CTzRuleHolder::TzRules()
	{
	return iTzRules;
	}

inline void CTzRuleHolder::SetCurrentTzId(TUint16 aId)
	{
	iCurrentTzId = aId;
	}
	
inline TUint16 CTzRuleHolder::CurrentTzId()
	{
	return iCurrentTzId;
	}
inline RTz& CTzRuleHolder::Server()
    {
    return iTzServer;
    }   
