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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/


#ifndef OOMACTION_H_
#define OOMACTION_H_

#include <e32base.h>

class MOomActionObserver;

/*
 * The base class for all OOM actions (i.e. close application or run OOM plug-in).
 * 
 * A base class is used because both types of action have common aspects, specifically:
 *  - They are prioritised according to application idle time
 *  - They need to be prioritised against each other
 *  - For each action it is possible to either continue immediately or to wait for completion
 *  
 * @lib oommonitor.lib
 * @since S60 v5.0
 */
NONSHARABLE_CLASS(COomAction) : public CBase
    {
public:

    virtual ~COomAction();
    
    /**
     * Activate the OOM action
     *
     * @since S60 5.0
     * @param aBytesRequested ?description
     */ 
    virtual void FreeMemory(TInt aBytesRequested, TBool aIsDataPaged) = 0;
    
    /**
     * @since S60 5.0
     * @return ETrue if the action is currently freeing memory, else EFalse
     */ 
    virtual TBool IsRunning() = 0;
    
protected:
    // 
    /**
     * To be called by the derived class after the memory has been freed (or if it fails)
     * 
     * @since S60 5.0
     * @param aError KErrNone if memory has successfully been freed, otherwise any system wide error code
     */ 
    void MemoryFreed(TInt aError);
    
    COomAction(MOomActionObserver& aStateChangeObserver);
    
private: //data
    
    enum TOomActionState
        {
        EOomIdle,
        EOomFreeingMemory
        };  
    TOomActionState iState;
    
    MOomActionObserver& iStateChangeObserver;
    };

#endif /*OOMACTION_H_*/
