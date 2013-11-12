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


#ifndef OOMPLUGINWAITER_H_
#define OOMPLUGINWAITER_H_

#include <e32base.h>

class COomRunPlugin;


/*
 *  A class used for waiting for a pre-determined period before completing the plugin operation
 *  It is intended to be used to force a delay between the call to the plugin and the memory check,
 *  thus allowing the plugin to free some memory first.
 * 
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomPluginWaiter) : public CTimer
    {
public:
    static COomPluginWaiter* NewL(TInt aMillisecondsToWait, COomRunPlugin& aCallbackClass);
    
    // Start the timer, it will call the plugin back when it expires
    void Start();
    
protected:
    void RunL();
    
    COomPluginWaiter(TInt aMillisecondsToWait, COomRunPlugin& aCallbackClass);
    
private:
    TInt iMillisecondsToWait;
    COomRunPlugin& iCallbackClass;
    };

#endif /*OOMPLUGINWAITER_H_*/
