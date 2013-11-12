/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description :
 *
 */
#ifndef TSWINDOWGROUPSMONITOR_H
#define TSWINDOWGROUPSMONITOR_H

#include "tswindowgroupsobserver.h"

/**
 * Interface declare methods to subscribe window server events
 */
class MTsWindowGroupsMonitor
{
public:
    /**
     * Method make subscription for window server events
     * @param aObserver - events observer
     */
    virtual void SubscribeL(MTsWindowGroupsObserver & aObserver) =0;
    
    /**
     * Method cancel subscription for window server events
     * @param aObserver - events observer
     */
    virtual void Cancel(MTsWindowGroupsObserver & aObserver) =0;
};

#endif //TSWINDOWGROUPSMONITOR_H
