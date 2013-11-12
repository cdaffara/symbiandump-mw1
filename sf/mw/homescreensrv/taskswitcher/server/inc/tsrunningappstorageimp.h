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
#ifndef TSREUNNINGAPPSTORAGEIMP_H
#define TSREUNNINGAPPSTORAGEIMP_H

#include "tsrunningappstorage.h"
#include "tsrunningappimp.h"

class CTsRunningAppStorage: public CBase,
                            public MTsRunningApplicationStorage
{
public:
    static CTsRunningAppStorage* NewLC();
    ~CTsRunningAppStorage();
    void HandleWindowGroupChanged( 
           MTsResourceManager &aResources, 
           const TArray<RWsSession::TWindowGroupChainInfo> & aWindowGroups );
    void HandleWindowGroupChanged( 
           MTsResourceManager &aResources, 
           const TArray<RWsSession::TWindowGroupChainInfo>& aWindowGroups,
           const TArray<RWsSession::TWindowGroupChainInfo>& aFilteredWindowGroups);
public:
    const MTsRunningApplication& operator[] (TInt aOffset) const;
    TInt Count() const;
    TInt ParentIndex( const MTsRunningApplication& aRunningApp ) const;
    TArray<TInt> BlockedWindowGroups() const;
    
private:
    TInt ParentIndex( TInt aOffset ) const;
    TInt Find(TInt aWindowGroupId, TInt aOffset =0) const;

private:
    CTsRunningAppStorage();

private:
    RPointerArray<CTsRunningApp> iRunningApps;
    RArray<TInt> iBlockedWindowGroups;

};
#endif//TSREUNNINGAPPSTORAGEIMP_H
