/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Non-closeable app list class
 *
*/


#ifndef TSDATALIST_H
#define TSDATALIST_H

#include <e32base.h>

#include <HbIcon>

#include "tswindowgroupsobserverbase.h"
#include "tsentry.h"
#include "tsdatastorage.h"
#include "tsdataobserver.h"
#include "tsenv.h"

class CApaWindowGroupName;
class CFbsBitmap;
class MTsRunningApplication;

NONSHARABLE_CLASS( CTsDataList ) : public CTsWindowGroupsObserverBase,
                                   public MTsDataStorage
    {
public:
    static CTsDataList* NewL( MTsResourceManager& aResources, 
                              MTsDataObserver& aObserver,
                              TsEnv& aEnv );

    ~CTsDataList();

private:
    CTsDataList( MTsResourceManager& aResources,
                 MTsDataObserver& aObserver,
                 TsEnv& aEnv);

    void ConstructL();

public:
    const RTsFswArray& Data() const;
    void HandleWindowGroupChanged( MTsResourceManager &aResources, 
                                   const MTsRunningApplicationStorage& aStorage );
    TBool IsHiddenUid( TUid aUid );
    TBool IsSupported(TInt aFunction) const;
    void HandleDataL(TInt aFunction,RReadStream& aDataStream);

private:
    void CollectAppsL( RTsFswArray& aAppsList,
                       const MTsRunningApplicationStorage& aStorage );
    void AddEntryL( const TTsEntryKey& aKey, 
                    const MTsRunningApplication& aRunningApp,
                    RTsFswArray& aNewList );
    HBufC* FindAppNameLC( const MTsRunningApplication& aRunningApp );
    TBool CheckIfExists( const CTsEntry& aEntry,
                         const RTsFswArray& aNewList ) const;
    void RegisterScreenshotL( RReadStream& aDataStream );
    void UnregisterScreenshotL( RReadStream& aDataStream );
    void ChangeVisibilityL( RReadStream& aDataStream );
    void UpdateTaskTimestampL( RReadStream& aDataStream );
    void FitDataToListL( RTsFswArray& aListToFit );
    TBool ConsiderOldDataL( const TTsEntryKey& aKey );
    CFbsBitmap* GetAppIconL( const TUid& aAppUid );
    TInt FindEntry( const RTsFswArray& aList, const TTsEntryKey& aKey ) const;
    TBool EstablishOrder( const RArray<TTsEntryKey>& aKeyList );
    TTsEntryKey GenerateKeyL( TInt );
    void HideEntryIfNotAllowed( CTsEntry* aEntry );
    TBool UpdateEntryData(const RTsFswArray& aList);
    void RebuildVisibleDataListL();
    CFbsBitmap* HbIcon2CFbsBitmap( const HbIcon& aIcon );

private:
    MTsResourceManager& iResources;
    MTsDataObserver &iObserver;
    RTsFswArray iData; // current fsw content, i.e. the task list
    RTsFswArray iVisibleData;
    RArray<TUid> iHiddenUids/** list of hidden uids */;
    CFbsBitmap* iDefaultIcon /** default icon*/;
    TsEnv& iEnv;
    };

#endif //TSDATALIST_H
