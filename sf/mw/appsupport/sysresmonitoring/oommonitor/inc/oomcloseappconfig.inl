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


#ifndef OOMCLOSEAPPCONFIGINL_H_
#define OOMCLOSEAPPCONFIGINL_H_

inline TUint COomCloseAppConfig::CalculateCloseAppPriority(const COomWindowGroupList& aWindowGroupList, TUint aAppIndexInWindowGroup)
    {
    // Calculating the priority for application closures is simple, we just get the base class to apply the rules
    return Priority(aWindowGroupList, aAppIndexInWindowGroup);
    }

#endif /*OOMCLOSEAPPCONFIGINL_H_*/
