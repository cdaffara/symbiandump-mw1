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
 * Description:  File containing application list classes
 *
 */

//INCLUDES:
#include <mmf/common/mmfcontrollerpluginresolver.h> // for CleanupResetAndDestroyPushL
#include <e32debug.h>
#include <apgwgnam.h>
#include <bitstd.h>
#include <AknIconUtils.h> // avkon
#include <apgicnfl.h> // fbsbitmap
#include <AknIconSrvClient.h> 
#include <fbs.h>
#include <apgwgnam.h>
#include <QSizeF>
#include <camenuiconutility.h>
#include <apgcli.h>

#include "tsdatalist.h"
#include "tsentrykeygenerator.h"
#include "tsscreenshotmsg.h"
#include "tsunregscreenshotmsg.h"
#include "tsvisibilitymsg.h"
#include "tsresourcemanager.h"
#include "tsrunningapp.h"
#include "tsrunningappstorage.h"

// size for the created app icons
const TInt KAppIconWidth = 128;
const TInt KAppIconHeight = 128;

//uids to be hidden
const TUid KHsApplicationUid = { 0x20022F35 };

// ================= MEMBER FUNCTIONS =======================

// --------------------------------------------------------------------------
/**
 * Two-phased constructor.
 */
CTsDataList* CTsDataList::NewL( MTsResourceManager& aResources,
                                MTsDataObserver& aObserver,
                                TsEnv& aEnv )
    {
    CTsDataList* self = new (ELeave) CTsDataList( aResources, 
                                                  aObserver,
                                                  aEnv);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// --------------------------------------------------------------------------
/**
 * Constructor.
 */
CTsDataList::CTsDataList(MTsResourceManager& aResources,
                         MTsDataObserver& aObserver,
                         TsEnv& aEnv) 
:
    CTsWindowGroupsObserverBase( aResources.WsMonitor() ),
    iResources( aResources ),
    iObserver( aObserver ),
    iEnv( aEnv )
    {
    }

// --------------------------------------------------------------------------
/*
 * Destructor
 */
CTsDataList::~CTsDataList()
    {
    iData.ResetAndDestroy();
    iVisibleData.Close();
    iHiddenUids.Close();
    RFbsSession::Disconnect();
    RAknIconSrvClient::Disconnect();
    delete iDefaultIcon;
    }

// --------------------------------------------------------------------------
/**
 * Performs 2nd phase construction.
 */
void CTsDataList::ConstructL()
    {
    BaseConstructL();
    iHiddenUids.AppendL( KHsApplicationUid );
    User::LeaveIfError(RFbsSession::Connect());
    RAknIconSrvClient::Connect();
    QT_TRYCATCH_LEAVING(
        iDefaultIcon = HbIcon2CFbsBitmap( HbIcon("qtg_large_application") );)
    }

// --------------------------------------------------------------------------
/**
 * Returns a reference to the current content.
 * Also performs sanity checks, e.g. associates application icons
 * when no screenshot has been received.
 * @return  ref to content array
 */
const RTsFswArray& CTsDataList::Data() const
    {
    return iVisibleData;
    }

// --------------------------------------------------------------------------
/**
 * Interface implementation
 * @see MTsWindowGroupsObserver HandleWindowGroupChanged
 */
void CTsDataList::HandleWindowGroupChanged(
                      MTsResourceManager &/*aResources*/, 
                      const MTsRunningApplicationStorage& aStorage)
    {
    TRAP_IGNORE(RDebug::Print(_L("[TaskSwitcher] processing started"));
                RTsFswArray newAppsList;
                CleanupResetAndDestroyPushL(newAppsList);
                CollectAppsL(newAppsList, aStorage);
                RDebug::Print(_L("[TaskSwitcher] gathered app info"));
                FitDataToListL(newAppsList);
                CleanupStack::PopAndDestroy(&newAppsList);
                RDebug::Print(_L("[TaskSwitcher] processing finished"));
                );
    }

// --------------------------------------------------------------------------
/**
 * Adds running apps to the list.
 * @param aAppsList array to add to
 */
void CTsDataList::CollectAppsL(RTsFswArray& aAppsList, 
                               const MTsRunningApplicationStorage& aStorage)
    {
    for( TInt i(0); i < aStorage.Count(); ++i )
        {
        TTsEntryKey key;
        TInt err = TsEntryKeyGeneraror::Generate( key, 
                                                  aStorage[i].WindowGroupId(), 
                                                  aStorage );
        //skip this entry if it is already on list or generate key failed
        if( err!=KErrNone || FindEntry( aAppsList, key ) >= 0 ) 
            {
            continue;
            }

        // get screen number (-1=console, 0=main screen, 1=cover ui)
        TInt appScreen = 0;
        TInt scrNumErr = 
            iResources.ApaSession().GetDefaultScreenNumber( appScreen, 
                                                            aStorage[i].UidL() );
        
        if( aStorage[i].UidL().iUid && 
            !aStorage[i].IsHiddenL() && 
            (appScreen == 0 || appScreen == -1) && 
            scrNumErr == KErrNone )
            {
            AddEntryL( key, aStorage[i], aAppsList );
            }
        }
    }

// --------------------------------------------------------------------------
/**
 * Called from CollectTasksL for each entry in the task list.
 * @param   aKey       entry key
 * @param   aRunningApp  running application entry
 * @param   aNewList   list to add to
 */
void CTsDataList::AddEntryL( const TTsEntryKey& aKey, 
                             const MTsRunningApplication& aRunningApp, 
                             RTsFswArray& aNewList )
    {
    CTsEntry* entry = CTsEntry::NewLC( aKey, iObserver, &iEnv );
    // check if present in old list and if yes then take some of the old data
    TBool found = ConsiderOldDataL( aKey );
    // if not present previously then find out app name
    // and check if screenshot is already available
    if( !found )
        {
        entry->SetAppUid(aRunningApp.UidL());
        HBufC* name = FindAppNameLC( aRunningApp );
        entry->SetAppNameL(*name);
        CleanupStack::PopAndDestroy( name );
        //transfer ownership to entry
        entry->SetAppIcon( GetAppIconL( aRunningApp.UidL() ) );
        }
    entry->SetCloseableApp( !aRunningApp.IsSystemL());
    // add to new list, ownership is transferred
    aNewList.AppendL( entry );
    CleanupStack::Pop( entry );
    }

// --------------------------------------------------------------------------
/**
 * Checks if there is an entry for same app in the content list.
 * If yes then it takes some of the data for the entry that
 * will correspond to the same app in the refreshed content list.
 * @param   aKey      new key in content list
 * @return  ETrue if app was found
 */
TBool CTsDataList::ConsiderOldDataL( const TTsEntryKey& aKey )
    {
    for(TInt entryIdx = 0, oldCount = iData.Count(); entryIdx < oldCount; ++entryIdx) 
        {
        if (iData[entryIdx]->Key() == aKey) 
            {
            return ETrue;
            }
        }
    return EFalse;
    }

// --------------------------------------------------------------------------
/**
 * Finds out the application name.
 * @param   aWindowName window group name or NULL
 * @param   aAppUId     application uid
 * @param   aWgId       window group id
 * @return  application name, ownership transferred to caller
 */
HBufC* CTsDataList::FindAppNameLC( const MTsRunningApplication& aRunningApp )
    {
    //Retrieve the app name
    TApaAppInfo info;
    iResources.ApaSession().GetAppInfo( info, aRunningApp.UidL() );
    TPtrC caption = info.iShortCaption;

    HBufC* tempName( 0 );
    if (!caption.Length() ) // if not set - use thread name instead
        {
        if( aRunningApp.CaptionL().Length() )
            {
            tempName = aRunningApp.CaptionL().AllocLC();
            }
        else
            {
            TThreadId threadId;
            if(KErrNone == iResources.WsSession().GetWindowGroupClientThreadId( aRunningApp.WindowGroupId(), threadId ) ) 
                {
                RThread thread;
                CleanupClosePushL( thread );
                if( KErrNone == thread.Open( threadId ) )
                    {
                    tempName = thread.Name().AllocL(); // codescanner::forgottoputptroncleanupstack
                    }
                // tempName put on cleanupstack after the if
                CleanupStack::PopAndDestroy( &thread );
                if(tempName)
                    {
                    CleanupStack::PushL(tempName);
                    }
                }
            }
        }
    else
        {
        tempName = caption.AllocLC();
        }
    if( 0 == tempName )
        {
        tempName = KNullDesC16().AllocLC();
        }
    return tempName;
    }

// --------------------------------------------------------------------------
/**
 * Fit existing class contained data list into give one.
 * Data is being changed with application type consideration that is based 
 * on aConsiderWidgets param. 
 * Function removes or add entries into data depend on given list.
 * @param   aListToFit          list with actual data  
 */
void CTsDataList::FitDataToListL( RTsFswArray& aListToFit )
    {
    TBool changed = EFalse;
    TInt dataCount = iData.Count();
    
    //remove items that dont't exists in newly collected list
    for (TInt i = dataCount - 1; i >= 0; --i) 
        {
        if( !CheckIfExists( *iData[i], aListToFit ) )
            {
            delete iData[i];
            iData.Remove( i );
            changed = ETrue;
            }
        }
    RArray<TTsEntryKey> allKeys;
    
    //add new items at start
    for(TInt i = aListToFit.Count() - 1; i >= 0; --i)
        {        
        User::LeaveIfError( allKeys.Insert(aListToFit[i]->Key(), 0) );
        if( !CheckIfExists( *aListToFit[i], iData ) ) 
            {
            HideEntryIfNotAllowed( aListToFit[i] );
            User::LeaveIfError( iData.Insert( aListToFit[i], 0 ) );
            TTime currentTimestamp;
            currentTimestamp.UniversalTime();
            iData[0]->SetTimestamp( currentTimestamp );
            aListToFit[i] = 0;
            changed = ETrue;
            }
        }
    //establish order
    TBool orderChanged = EstablishOrder( allKeys );
    //update entries data
    TBool dataChanged = UpdateEntryData( aListToFit );
    RebuildVisibleDataListL();
    if( changed || orderChanged || dataChanged )
        {
        iObserver.DataChanged();
        }
    allKeys.Close();
    }

// --------------------------------------------------------------------------
/**
 * Checks if there is an entry for same app in the given list.
 * @param   aEntry      entry
 * @param   aList    ref to list
 * @return  ETrue if app was found
 */

TBool CTsDataList::CheckIfExists( const CTsEntry& aEntry, 
                                  const RTsFswArray& aList ) const
    {
    return 0 <= FindEntry( aList, aEntry.Key() );
    }

// --------------------------------------------------------------------------
/**
 * Retrieves the bitmap for the icon of the given app.
 * @param   aAppUid application uid
 * @return  app CFbsBitmap
 */
CFbsBitmap* CTsDataList::GetAppIconL( const TUid& aAppUid )
    {
    
    CFbsBitmap* iconBitmap(0);
    TRAPD(errNo, 
          QT_TRYCATCH_LEAVING(
          const QSize size(KAppIconWidth, KAppIconHeight);
          HbIcon icon = CaMenuIconUtility::getApplicationIcon( aAppUid.iUid, size);
          iconBitmap = HbIcon2CFbsBitmap( icon );)
          User::LeaveIfNull(iconBitmap); )
    if( KErrNone != errNo )
        {
        iconBitmap = new(ELeave) CFbsBitmap;
        CleanupStack::PushL(iconBitmap);
        User::LeaveIfError( iconBitmap->Duplicate( iDefaultIcon->Handle() ) );
        CleanupStack::Pop(iconBitmap);
        }
    return iconBitmap; 
    }

// --------------------------------------------------------------------------
/**
 * Converts HbIcon to CFbsBitmap
 * @param   aIcon icon to be coverted
 * @return  CFbsBitmap
 */
CFbsBitmap* CTsDataList::HbIcon2CFbsBitmap( const HbIcon& aIcon )
    {
    CFbsBitmap* retValue(0);
    QIcon qicon = aIcon.qicon();
    QPixmap pixmap = qicon.pixmap(QSize(KAppIconWidth, KAppIconHeight));
    if( !pixmap.isNull() )
        {
        retValue = pixmap.toSymbianCFbsBitmap();
        }
    return retValue; 
    }
// --------------------------------------------------------------------------
TBool CTsDataList::IsSupported( TInt aFunction ) const
    {
    return ( RegisterScreenshotMessage == aFunction ||
             UnregisterScreenshotMessage == aFunction ||
             VisibilityChange == aFunction || 
             WindowGroupToBackgroundMessage == aFunction );
    }

// --------------------------------------------------------------------------
void CTsDataList::HandleDataL( TInt aFunction, RReadStream& aDataStream )
    {
    switch( aFunction )
        {
        case RegisterScreenshotMessage:
            RegisterScreenshotL( aDataStream );
            break;
        case UnregisterScreenshotMessage:
            UnregisterScreenshotL( aDataStream );
            break;
        case VisibilityChange:
            ChangeVisibilityL( aDataStream );
            break;        
        case WindowGroupToBackgroundMessage:
            UpdateTaskTimestampL( aDataStream );
            break;
        }
    }

// --------------------------------------------------------------------------
void CTsDataList::RegisterScreenshotL( RReadStream& aDataStream )
    {
    CTsScreenshotMsg* msg = CTsScreenshotMsg::NewLC( aDataStream );
    const TInt pos = FindEntry( iData, GenerateKeyL(msg->WindowGroupId() ) );
    User::LeaveIfError(pos);
    iData[pos]->SetScreenshotL( msg->Screenshot(), msg->Priority(), msg->Rotation() );
    CleanupStack::PopAndDestroy(msg);
    }
// --------------------------------------------------------------------------
void CTsDataList::UnregisterScreenshotL(RReadStream& aDataStream)
{
    CTsUnregisterScreenshotMsg* msg = 
        CTsUnregisterScreenshotMsg::NewLC(aDataStream);
    const TInt pos = FindEntry( iData, GenerateKeyL(msg->windowGroupId() ) );
    User::LeaveIfError(pos);
    iData[pos]->RemoveScreenshotL();
    CleanupStack::PopAndDestroy(msg);
}
// --------------------------------------------------------------------------
void CTsDataList::ChangeVisibilityL( RReadStream& aDataStream )
    {
    CTsVisibilitMsg* msg = CTsVisibilitMsg::NewLC(aDataStream);
    const TInt pos = FindEntry( iData, GenerateKeyL( msg->windowGroupId() ) );
    User::LeaveIfError(pos);
    
    msg->visibility() == iData[pos]->GetVisibility() ? 
    User::Leave( KErrInUse ) : 
    iData[pos]->SetVisibility( msg->visibility() );
    
    CleanupStack::PopAndDestroy(msg);
    RebuildVisibleDataListL();
    iObserver.DataChanged();
    }
// --------------------------------------------------------------------------
void CTsDataList::UpdateTaskTimestampL( RReadStream& aDataStream )
    {
    const TInt wgId = aDataStream.ReadInt32L();
    const TInt pos = FindEntry( iData, GenerateKeyL( wgId ) );
    User::LeaveIfError( pos );

    TTime currentTimestamp;
    currentTimestamp.UniversalTime();
    iData[pos]->SetTimestamp( currentTimestamp );

    iObserver.DataChanged();
    }

// --------------------------------------------------------------------------
/**
 * Checks if given uid is on hidden list
 * @param   aUid uid to be checked
 * @return  ETrue if uid is on hidden list
 */
TBool CTsDataList::IsHiddenUid( TUid aUid )
    {
    return 0 < iHiddenUids.Find( aUid );
    }

// --------------------------------------------------------------------------
/**
 * Finds entry in array
 * @param   aList list to find
 * @param   aKey finding key
 * @return   position or KErrNotFound
 */
TInt CTsDataList::FindEntry( const RTsFswArray& aList, 
                             const TTsEntryKey& aKey ) const
    {
    TInt pos(KErrNotFound);
    for(TInt entryIdx = 0; 
        entryIdx < aList.Count() && KErrNotFound == pos; 
        ++entryIdx)
        {
        if (aList[entryIdx]->Key() == aKey)
            {
            pos = entryIdx;
            }
        }
    return pos;
    }

// --------------------------------------------------------------------------
/**
 * Establish entry order accridung to aKeyList, all keys MUST be in iData
 * @param   aKeyList reference key list
 * @return   ETrue if changes occured
 */
TBool CTsDataList::EstablishOrder( const RArray<TTsEntryKey>& aKeyList )
    {
    TBool changed( EFalse );
    TInt lastChangedItem( KErrNotFound );
    __ASSERT_ALWAYS( iData.Count() == aKeyList.Count(), User::Panic(_L("EstablishOrder 1"), KErrBadHandle) );
    for (TInt i = 0; i < aKeyList.Count(); i++)
        {
        const TTsEntryKey& currentdataKey = iData[i]->Key();
        const TTsEntryKey& referenceKey = aKeyList[i];
        if( !(currentdataKey == referenceKey) )
            {
            TInt foundPos = FindEntry( iData, referenceKey );
            __ASSERT_ALWAYS(foundPos>=0,  User::Panic(_L("EstablishOrder 2"), KErrBadHandle));
            CTsEntry* entry = iData[foundPos];
            iData.Remove( foundPos );
            iData.Insert( entry, i );
            changed = ETrue;
            lastChangedItem = i;
            }
        }
    TTime currentTimestamp;
    currentTimestamp.UniversalTime();
    for (TInt i = lastChangedItem; i >= 0; --i)
        {
        iData[i]->SetTimestamp(currentTimestamp);
        }
    return changed;
    }

// --------------------------------------------------------------------------
/**
 * Updates entry data on mData according to list
 * @param   aList reference entires list
 * @return   ETrue if changes occured
 */
TBool CTsDataList::UpdateEntryData( const RTsFswArray& aList )
{
    __ASSERT_ALWAYS( iData.Count() == aList.Count(), 
                     User::Panic(_L("UpdateEntryData 1"), KErrBadHandle) );
    TBool changed( EFalse );
    for( TInt i=0; i<aList.Count(); i++ )
        {
        if(aList[i]) 
            {
            __ASSERT_ALWAYS(iData[i]->Key() == aList[i]->Key(), 
                            User::Panic(_L("UpdateEntryData 2"), KErrBadHandle));
            if( iData[i]->CloseableApp() != aList[i]->CloseableApp() )
                {
                iData[i]->SetCloseableApp(aList[i]->CloseableApp());
                changed = ETrue;
                }
            }
        }
    return changed;
}

// --------------------------------------------------------------------------
/**
 * Function generate task key using window group id
 * @param wgId - window group id of running application
 * @param entry key 
 */
TTsEntryKey CTsDataList::GenerateKeyL( TInt aWgId )
    {
    RArray<RWsSession::TWindowGroupChainInfo> allWgIds;
    CleanupClosePushL( allWgIds );
    User::LeaveIfError( iResources.WsSession().WindowGroupList( 0, &allWgIds ) );
    TTsEntryKey key;
    User::LeaveIfError(TsEntryKeyGeneraror::Generate(key, aWgId, allWgIds.Array()));
    CleanupStack::PopAndDestroy( &allWgIds );
    return key;
    }

// --------------------------------------------------------------------------
/**
 * Hides entrie if exist on mHiddenUids
 * @param   entry
 */
void CTsDataList::HideEntryIfNotAllowed( CTsEntry* aEntry )
    {
    if( iHiddenUids.Find( aEntry->AppUid() ) >= 0 )
        {
        aEntry->SetVisibility(Invisible);
        }
    }

void CTsDataList::RebuildVisibleDataListL()
    {
    iVisibleData.Reset();
    RArray<TInt> visibleItems(iData.Count() ? iData.Count() : 1);
    CleanupClosePushL(visibleItems);
    for( TInt iter = 0; iter < iData.Count(); ++iter )
        {
        if( Visible == iData[iter]->GetVisibility() )
            {
            visibleItems.AppendL(iter);
            }
        }
    for( TInt iter = 0; iter < visibleItems.Count(); ++iter ) 
        {
        iVisibleData.AppendL( iData[visibleItems[iter]] );
        }
    CleanupStack::PopAndDestroy(&visibleItems);
    }
// end of file
