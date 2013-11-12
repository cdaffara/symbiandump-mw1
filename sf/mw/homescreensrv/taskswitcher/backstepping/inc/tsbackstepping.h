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
* Description:
*
*/

#ifndef __CBACKSTEPPING_H
#define __CBACKSTEPPING_H

#include <w32std.h>
#include "tswindowgroupsobserverbase.h"

class CTsBacksteppingFilter;

NONSHARABLE_CLASS(CTsBackstepping) : public CTsWindowGroupsObserverBase
{
public:
    static CTsBackstepping* NewL( MTsWindowGroupsMonitor& );
    static CTsBackstepping* NewLC( MTsWindowGroupsMonitor& );
    virtual ~CTsBackstepping();

private:
    CTsBackstepping( MTsWindowGroupsMonitor& );
    void ConstructL();
    void HandleWindowGroupChanged( MTsResourceManager &rsc, 
                                   const MTsRunningApplicationStorage& aStorage );
    void HandleWindowGroupChangedL( MTsResourceManager &rsc, 
                                    const MTsRunningApplicationStorage& aStorage );
    TInt HomescreenOffsetL( const MTsRunningApplicationStorage& aStorage )const;
    void SwitchToIdleStateL( MTsResourceManager& aResources, TBool aIsFirst );

private:
    CTsBacksteppingFilter *iFilter;
    TBool iHsWasFirst;

};

#endif // __CBACKSTEPPING_H
