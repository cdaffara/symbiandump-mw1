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
#include "tswindowgroupsobserverbase.h"

// -----------------------------------------------------------------------------
/**
 * First phase constructor
 */
CTsWindowGroupsObserverBase::CTsWindowGroupsObserverBase(
                                              MTsWindowGroupsMonitor & aMonitor)
:
    iMonitor(aMonitor)
{}

// -----------------------------------------------------------------------------
/**
 * Second phase constructor.
 * Function automaticly subscribe window server events
 */
void CTsWindowGroupsObserverBase::BaseConstructL()
{
    iMonitor.SubscribeL(*this);
}

// -----------------------------------------------------------------------------
/**
 * Destructor
 * Function automaticly cancel subscrption to window server events
 */
CTsWindowGroupsObserverBase::~CTsWindowGroupsObserverBase()
{
    iMonitor.Cancel(*this);
}
