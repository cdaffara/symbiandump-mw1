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
#ifndef TSWINDOWGROUPSOBSERVERBASE_H
#define TSWINDOWGROUPSOBSERVERBASE_H

#include <e32base.h>

#include "tswindowgroupsobserver.h"
#include "tswindowgroupsmonitor.h"

/**
 * Window server observer implementation. Class automaticly subscribe / cancel subscription
 * at construction / destruction level.
 * 
 */
class CTsWindowGroupsObserverBase: public CBase,
                                   public MTsWindowGroupsObserver
    {
public:
    ~CTsWindowGroupsObserverBase();

protected:
    CTsWindowGroupsObserverBase( MTsWindowGroupsMonitor & aMonitor);
    void BaseConstructL();

private:
    MTsWindowGroupsMonitor & iMonitor;

    };

#endif //TSWINDOWGROUPSOBSERVERBASE_H
