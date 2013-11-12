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
* Description:  
*
*/




#ifndef __T_OOMDUMMYAPP_PAN__
#define __T_OOMDUMMYAPP_PAN__

/** t_oomdummyapp application panic codes */
enum Tt_oomdummyappPanics
	{
	Et_oomdummyappUi = 1
	// add further panics here
	};

inline void Panic(Tt_oomdummyappPanics aReason)
	{
	_LIT(applicationName, "t_oomdummyapp");
	User::Panic(applicationName, aReason);
	}

#endif // __T_OOMDUMMYAPP_PAN__
