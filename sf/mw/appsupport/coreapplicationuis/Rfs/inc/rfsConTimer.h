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


#ifndef __RFSCONTIMER_H
#define __RFSCONTIMER_H

#include <e32base.h>
#include <e32def.h> 
#include <e32std.h> 

#include "rfsConnectionObserver.h"

class CRfsConnectionObserver;

class CRfsConTimer : public CActive
    {
    private:
    
    CRfsConTimer(CRfsConnectionObserver * aRfsConnectionObserver); // Construction
	
    
    
public:
    
    static CRfsConTimer* CRfsConTimer::NewL(CRfsConnectionObserver * aRfsConnectionObserver);
    ~CRfsConTimer(); // Destruction
    void IssueTimerRequest(); // issue request
    
protected:   
    void DoCancel(); // Cancel request
    void RunL(); // service completed request.
    
public:
    
    RTimer iTimer; // Asynchronous service provider.
    CRfsConnectionObserver* iRfsConnectionObserver;
    
private:    // friend classes

       friend class CRfsConnectionObserver;
    };

#endif   //__RFSCONTIMER_H
