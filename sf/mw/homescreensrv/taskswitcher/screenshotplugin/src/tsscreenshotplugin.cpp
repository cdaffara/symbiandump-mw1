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
* Description:
*
*/
#include <graphics/wsscreendevice.h>
#include <s32mem.h>
#include <e32property.h>
#include <wspublishandsubscribedata.h>
#include <tstaskmonitorglobals.h>
#include <stddef.h>
#include <qnamespace.h>

#include "tsscreenshotplugin.h"
#include "tsscreenshotnotifier.h"
#include "tsscreenshotmsg.h"
#include "tsidlist.h"

const TInt KInvalidGroupId(~0);
const TUid KHbPsHardwareCoarseOrientationCategoryUid = {0x20022E82};
const TUint KHbPsHardwareCoarseWsOrientationKey = 0x4F726965;

// -----------------------------------------------------------------------------
/**
 * Two phase constructor 
 */
CTsScreenshotPlugin* CTsScreenshotPlugin::NewL()
    {
    //no second step construction is required here
    //window server will initialize plugin later
    return new(ELeave)CTsScreenshotPlugin();
    }

// -----------------------------------------------------------------------------
/**
 * From CWsGraphicDrawer
 * @see CWsGraphicDrawer::ConstructL(MWsGraphicDrawerEnvironment&, const TGraphicDrawerId& , MWsClient&, const TDesC8&)
 */
void CTsScreenshotPlugin::ConstructL( MWsGraphicDrawerEnvironment& aEnv, 
                                      const TGraphicDrawerId& iId, 
                                      MWsClient& aOwner, 
                                      const TDesC8& /*data*/ )
    {
    BaseConstructL( aEnv, iId, aOwner );
    aEnv.RegisterEventHandler(this, 
                             this, 
                             TWservCrEvent::EWindowGroupChanged |
                             TWservCrEvent::EDeviceOrientationChanged);
    iWindowGroupId = KInvalidGroupId;
    iAllowedList = CTsIdList::NewL(); 
    }


// -----------------------------------------------------------------------------
/**
 * Destructor
 */
CTsScreenshotPlugin::~CTsScreenshotPlugin()
    {
    delete iAllowedList;
    Env().UnregisterEventHandler(this);
    iCache.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
/**
 * From CWsGraphicDrawer.
 * @see CWsGraphicDrawer::DoDraw(MWsGc&, const TRect&, const TDesC8&) const
 */ 
void CTsScreenshotPlugin::DoDraw(MWsGc&, const TRect&, const TDesC8&) const
    {
    //plugin is not a real drawer
    //no implementation required
    }

// -----------------------------------------------------------------------------
/**
 * From CWsGraphicDrawer
 * @see CWsGraphicDrawer::HandleMessage(const TDesC8&)
 */
void CTsScreenshotPlugin::HandleMessage( const TDesC8& aMsg )
    {
    TRAP_IGNORE( HandleMessageL( aMsg ) );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CTsScreenshotPlugin::HandleMessageL( const TDesC8& aMsg )
    {
    RDesReadStream msgStream(aMsg);
    CleanupClosePushL(msgStream);
    TInt function = msgStream.ReadInt32L();
    if(RegisterScreenshotMessage == function)
        {
        CTsScreenshotMsg *screenshotMsg = CTsScreenshotMsg::NewLC(msgStream);
        for( TInt iter(0); iter < iCache.Count(); ++iter )
            {
            if(iCache[iter]->Handle() == screenshotMsg->Screenshot().Handle())
                {
                //bitmap is not needed no more
                delete iCache[iter];
                iCache.Remove(iter);
                break;
                }
            }
        CleanupStack::PopAndDestroy( screenshotMsg );
        }
    else if( AllowedWindowGroups == function )
        {
        CTsIdList* list = CTsIdList::NewLC( msgStream );
        delete iAllowedList;
        iAllowedList = list;
        CleanupStack::Pop( list );
        }
    CleanupStack::PopAndDestroy( &msgStream );
    }

// -----------------------------------------------------------------------------
/**
 * From MWsEventHandler.
 * @see MWsEventHandler::DoHandleEvent(const TWservCrEvent&)
 */
void CTsScreenshotPlugin::DoHandleEvent(const TWservCrEvent& aEvent)
    {
    switch (aEvent.Type()) {
    case TWservCrEvent::EWindowGroupChanged:
        if( KInvalidGroupId != iWindowGroupId )
            {
            NotifyWindowGroupToBackground( iWindowGroupId );
            if(iAllowedList->IsPresent(iWindowGroupId))
                {
                TakeScreenshot( iWindowGroupId );
                }
            }
        iWindowGroupId = aEvent.WindowGroupIdentifier();
        break;
    case TWservCrEvent::EDeviceOrientationChanged:
        if( iAllowedList->IsPresent( iWindowGroupId ) )
            {
            TakeScreenshot( iWindowGroupId );
            }
        break;
        }
    }

// -----------------------------------------------------------------------------
// 
/**
 * Function take screenshot od current display
 * @param screenshot identyfier
 */
void CTsScreenshotPlugin::TakeScreenshot( TInt aId )
    {
    TRAP_IGNORE( TakeScreenshotL( aId ); )
    }

// -----------------------------------------------------------------------------
/**
 * Function take screenshot od current display
 * @param screenshot identyfier
 */
void CTsScreenshotPlugin::TakeScreenshotL( TInt aId )
    {
    if( 0 >= Env().ScreenCount() )
        {
        User::Leave(KErrCorrupt);
        }
    const TInt screenId(0); //use local variable in case of changing screen selection policy
    const MWsScreenConfig* const screenConfig = 
        Env().Screen(screenId)->ObjectInterface<MWsScreenConfig>();
    const MWsScreenDevice* const screenDevice = 
        static_cast<MWsScreenDevice*>(Env().Screen(screenId)->ResolveObjectInterface(MWsScreenDevice::EWsObjectInterfaceId));
    
    User::LeaveIfNull(screenConfig);
    User::LeaveIfNull(screenDevice);
    
    //prepare destination bitmap
    CFbsBitmap *bitmap = new (ELeave) CFbsBitmap();
    CleanupStack::PushL(bitmap);
    
    User::LeaveIfError(bitmap->Create(screenConfig->SizeInPixels(), 
                                screenConfig->DisplayMode()));
    
    screenDevice->CopyScreenToBitmapL(bitmap, 
                                      screenConfig->SizeInPixels());
    
    //prepare and send message
    CTsScreenshotNotifier* notifier = 
        CTsScreenshotNotifier::NewL(*this, aId, bitmap, OrientationToAngle());
    CleanupStack::Pop(bitmap);
    CleanupStack::PushL(notifier);
    iCache.AppendL(notifier);
    CleanupStack::Pop(notifier);
    }

// -----------------------------------------------------------------------------
//
TInt CTsScreenshotPlugin::OrientationToAngle()
    {
    TInt retVal(0), orientation(0), sensor(0);
    RProperty::Get( KRenderOrientationCategory, 
                    KRenderOrientationKey, 
                    orientation );
    
    switch( orientation )
        {
        case EDisplayOrientation90CW: retVal = 270;break;
        case EDisplayOrientation180: retVal = 180;break;
        case EDisplayOrientation270CW: retVal = 90;break;
        case EDisplayOrientationAuto:
            {
            RProperty::Get( KHbPsHardwareCoarseOrientationCategoryUid, 
                            KHbPsHardwareCoarseWsOrientationKey, sensor );
            retVal = ( sensor == Qt::Vertical ) ? 270 : 0;
            break;
            }
            
        }
    return retVal;
    }

// -----------------------------------------------------------------------------
void CTsScreenshotPlugin::SendMessageL( const TDesC8& aMessage )
    {
    User::LeaveIfError( SendMessage( aMessage ) );
    }

// -----------------------------------------------------------------------------
void CTsScreenshotPlugin::NotifyWindowGroupToBackground( TInt windowGroupId )
    {
    TRAP_IGNORE( NotifyWindowGroupToBackgroundL(windowGroupId); )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CTsScreenshotPlugin::NotifyWindowGroupToBackgroundL( TInt windowGroupId )
    {
    RBuf8 message;
    CleanupClosePushL(message);    
    message.CreateL(2 * sizeof(TInt));
        {
        RDesWriteStream stream(message);
        CleanupClosePushL(stream);
        stream.WriteInt32L(WindowGroupToBackgroundMessage);
        stream.WriteInt32L(windowGroupId);
        CleanupStack::PopAndDestroy(&stream);
        }
    SendMessageL(message);
    CleanupStack::PopAndDestroy(&message);
    }
