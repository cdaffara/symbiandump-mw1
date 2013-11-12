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
#ifndef TSWINDOWGROUPSOBSERVER_H
#define TSWINDOWGROUPSOBSERVER_H

class MTsResourceManager;
class MTsRunningApplicationStorage;
/**
 * Interface declare mathods to notify about window server events
 */
class MTsWindowGroupsObserver
    {
public:
    /**
     * Method notify about window group changes.
     * @param aResources - resource manager
     * @param aStorage - list of running applications
     */
    virtual void HandleWindowGroupChanged(
                              MTsResourceManager& aResources, 
                              const MTsRunningApplicationStorage& aStorage) =0;
    };

#endif //TSWINDOWGROUPSOBSERVER_H
