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
* Description:  Configuration representation classes for Out of Memory Monitor.
*
*/


#ifndef OOMAPPLICATIONCONFIGINL_H_
#define OOMAPPLICATIONCONFIGINL_H_

#include "oompanic.h"

inline COomCloseAppConfig* COomApplicationConfig::GetAppCloseConfig()
    {
    return iCloseAppConfig;
    }

inline void COomApplicationConfig::SetAppCloseConfig(COomCloseAppConfig* aActionConfig)
    {
    __ASSERT_ALWAYS(iCloseAppConfig == NULL, OomMonitorPanic(KSameAppClosureConfiguredTwice));
    
    iCloseAppConfig = aActionConfig;
    }


inline TUint COomApplicationConfig::Id()
    {
    return iApplicationId;
    }

#endif /*OOMAPPLICATIONCONFIGINL_H_*/
