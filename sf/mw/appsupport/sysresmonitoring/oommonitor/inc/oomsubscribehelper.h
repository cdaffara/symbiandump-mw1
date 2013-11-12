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
* Description:  Main classes for Out of Memory Monitor.
*
*/


#ifndef OOMSUBSCRIBEHELPER_H
#define OOMSUBSCRIBEHELPER_H

#include <e32base.h>

class RProperty;

/**
 *  This class is a simple active object which monitors the publish subscribe key method of calling the 
 *  requesting that the OOM Monitor starts to free memory
 *  
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(CSubscribeHelper) : public CActive
    {
public:
    CSubscribeHelper(TCallBack aCallBack, RProperty& aProperty);
    ~CSubscribeHelper();

public: // New functions
    void Subscribe();
    void StopSubscribe();

private: // from CActive
    void RunL();
    void DoCancel();

private:
    TCallBack   iCallBack;
    RProperty&  iProperty;
    };

#endif /*OOMSUBSCRIBEHELPER_H*/
