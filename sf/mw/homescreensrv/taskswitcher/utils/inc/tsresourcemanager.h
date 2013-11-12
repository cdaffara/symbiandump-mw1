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
#ifndef TSRESOURCEMANAGER_H
#define TSRESOURCEMANAGER_H

class RApaLsSession;
class RWsSession;
class MTsWindowGroupsMonitor;

/**
 * Interface declare methods to access initialized OS resources
 */
class MTsResourceManager
    {
public:
    /**
     * Access to initialized window server session
     */
    virtual RWsSession& WsSession() =0;
    
    /**
     * Access to initilaized APA session
     */
    virtual RApaLsSession& ApaSession() =0;
    
    /**
     * Access to window groups monitor
     */
    virtual MTsWindowGroupsMonitor& WsMonitor() =0;
    };
#endif // TSRESOURCEMANAGER_H
