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
* Description:  Plugin Interface
*
*/


#ifndef C_WAITACTIVE_H
#define C_WAITACTIVE_H

#include <e32base.h>

/**
 *  Helper wait active class
 */
 class CWaitActive : public CTimer
    {
    
public:
	
	
	static CWaitActive* NewL( );

	~CWaitActive();
	
	void Wait(TTimeIntervalMicroSeconds32 anInterval);
    
protected:
    
    /**
    * From CActive, RunL.
    * Handles the active object’s request completion event
    */
    void RunL();
    
     
private:
   
    CWaitActive();
   
    void ConstructL();
    
private:      

	/*
	* Own.
    */
	CActiveSchedulerWait* iActiveScheduler;	
    };
    

#endif // C_WAITACTIVE_H