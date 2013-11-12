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
#include <s32mem.h>

#include "tswindowgroupsmonitor.h"
#include "tsrunningapp.h"
#include "tsrunningappstorage.h"
#include "tsscreenshotprovider.h"
#include "tstaskmonitorglobals.h"
#include "tsscreenshotmsg.h"
#include "tsidlist.h"


const TUid KPluginUID = {0x200267AE};
const TInt KSkippedApp [] = {0x20022F35 /* <-- hsapplication */,
                             0x100058F3 /* <-- sysapp*/};
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CTsScreenshotProvider* CTsScreenshotProvider::NewL( MTsDataStorage& aStorage,
                                                    MTsWindowGroupsMonitor& aMonitor)
    {
    CTsScreenshotProvider *self = new (ELeave)CTsScreenshotProvider(aStorage, aMonitor);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
CTsScreenshotProvider::CTsScreenshotProvider( MTsDataStorage& aStorage,
                                              MTsWindowGroupsMonitor& aMonitor)
:
    iStorage(aStorage),
    iMonitor(aMonitor)
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
void CTsScreenshotProvider::ConstructL()
    {
    BaseConstructL(KPluginUID, KNullDesC8);
    iCache = CTsIdList::NewL();
    iMonitor.SubscribeL(*this);
    }

// -----------------------------------------------------------------------------
CTsScreenshotProvider::~CTsScreenshotProvider()
    {
    iMonitor.Cancel(*this);
    delete iCache;
    }

// -----------------------------------------------------------------------------
void CTsScreenshotProvider::HandleWindowGroupChanged(
                                  MTsResourceManager &aResources, 
                                  const MTsRunningApplicationStorage& aStorage)
    {
    TRAP_IGNORE(HandleWindowGroupChangedL(aResources, aStorage);)
    }

// -----------------------------------------------------------------------------
/**
 * Retrieve list of applications that shouldn't be displayed in taskswitcher 
 * and send it to module responsible for screenshot taking.  
 * Interface implementation.
 * @see MTsWindowGroupsObserver::HandleWindowGroupChangedL
 */
void CTsScreenshotProvider::HandleWindowGroupChangedL(
                                   MTsResourceManager& /*aResources*/, 
                                   const MTsRunningApplicationStorage& aStorage)
    {
    const TInt count(sizeof( KSkippedApp ) / sizeof(TInt));
    CTsIdList* list = CTsIdList::NewLC();
    for( TInt allowed(0); allowed < aStorage.Count(); ++allowed )
        {
        if(!aStorage[allowed].IsHiddenL())
            {
            TBool isBlocked(EFalse);
            for( TInt blocked(0); !isBlocked && blocked < count; ++blocked )
                {
                isBlocked = (KSkippedApp[blocked] == aStorage[allowed].UidL().iUid);
                }
            if(!isBlocked)
                {
                list->AppendL(aStorage[allowed].WindowGroupId());
                }
            }
        }
    if( *iCache != *list )
        {
        RBuf8 message;
        CleanupClosePushL(message);
        message.CreateL(list->Size() + sizeof(TInt));
            
        RDesWriteStream stream;
        CleanupClosePushL(stream);
        stream.Open(message);
        stream.WriteInt32L(AllowedWindowGroups);
        stream << (*list);
        CleanupStack::PopAndDestroy(&stream);
        
        SendMessage(message);
        CleanupStack::PopAndDestroy(&message);
        }
    delete iCache;
    iCache = list;
    CleanupStack::Pop(list);
    }

// -----------------------------------------------------------------------------
void CTsScreenshotProvider::HandleMessage( const TDesC8& aMsg )
    {
    TRAP_IGNORE( HandleMessageL( aMsg ) );
    }

// -----------------------------------------------------------------------------
void CTsScreenshotProvider::HandleMessageL( const TDesC8& aMsg )
    {
    RDesReadStream msgStream( aMsg );
    CleanupClosePushL( msgStream );
    switch ( msgStream.ReadInt32L() ) 
        {
        case RegisterScreenshotMessage:
            {
            //parse msg to ACK provider ASAP and be sure that bitmap still exists
            CTsScreenshotMsg* screenshotMsg = CTsScreenshotMsg::NewLC(msgStream); 
            SendMessage(aMsg);
            
            //reset stream
            msgStream.Close();
            msgStream.Open(aMsg);
            
            //forward stream to storage
            iStorage.HandleDataL(msgStream.ReadInt32L(), msgStream);
            
            CleanupStack::PopAndDestroy(screenshotMsg);
            break;
            }
            
        case WindowGroupToBackgroundMessage:
            {
            //reset stream
            msgStream.Close();
            msgStream.Open(aMsg);
            
            //forward stream to storage
            iStorage.HandleDataL(msgStream.ReadInt32L(), msgStream);
            break;
            }
            
        default: User::Leave(KErrNotSupported);
        }
    CleanupStack::PopAndDestroy(&msgStream);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CTsScreenshotProvider::OnReplace()
    {
    //No implementation required
    }
