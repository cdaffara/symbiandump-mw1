// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __ASSRVSYSTEMSTATEOBSERVER_H__
#define __ASSRVSYSTEMSTATEOBSERVER_H__

/** The mixin observer class for system state changes. 
*/
class MASSrvSystemStateObserver
	{
public:
	
	/** System states that the alarm server is interested in
	*/
	enum TState
		{
		/** The system is up and running in a normal state */
		ENormal,
		/** The system is shuting down */
		EShutdown
		};

public:
	virtual void MHandleSystemStateChange(TState aState) = 0;
	};

#endif
