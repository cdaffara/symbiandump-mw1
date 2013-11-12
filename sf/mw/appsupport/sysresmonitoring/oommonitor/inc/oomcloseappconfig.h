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


#ifndef OOMCLOSEAPPCONFIG_
#define OOMCLOSEAPPCONFIG_

#include "oomactionconfig.h"

/*
 * 
 */
/**
 *  The OOM action of closing an application in order to free up memory.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS(COomCloseAppConfig) : public COomActionConfig
    {
public:
    /**
     * Two-phased constructor.
     * @param aId The Uid of the applicaton
     */
    static COomCloseAppConfig* NewL(TInt32 aId);
    
    ~COomCloseAppConfig();

    /**
     * Calculates the priority of a close app action, based on its position on the window group list
     * 
     * @param aWindowGroupList a fully constructed, collapsed window group list
     * @param aAppIndexInWindowGroup the position in the window group list
     */
    inline TUint CalculateCloseAppPriority(const COomWindowGroupList& aWindowGroupList, TUint aAppIndexInWindowGroup);
    
private:
    
    COomCloseAppConfig(TInt32 aId);
    };

#include "oomcloseappconfig.inl"

#endif /*OOMCLOSEAPPCONFIG_*/
