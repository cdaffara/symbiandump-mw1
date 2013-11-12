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
#ifndef TSSCREENSHOTPLUGIN_H
#define TSSCREENSHOTPLUGIN_H

#include <w32std.h>
#include <graphics/wsgraphicdrawerinterface.h>
#include <graphics/wsgraphicdrawer.h>

#include "tsnotifier.h"

class CTsOrientationMonitor;
class CTsScreenshotNotifier;
class CTsIdList;

class CTsScreenshotPlugin: public CWsGraphicDrawer, 
                           public MWsEventHandler,
                           public MTsNotifier
                           
{
public:
    static CTsScreenshotPlugin* NewL();
    ~CTsScreenshotPlugin();

private:
    void DoDraw(MWsGc&, const TRect&, const TDesC8&) const;
    void HandleMessage(const TDesC8&);
    void HandleMessageL(const TDesC8&);
    void ConstructL(MWsGraphicDrawerEnvironment& aEnv, 
                    const TGraphicDrawerId& aId, 
                    MWsClient& aOwner, 
                    const TDesC8& aData);
    void DoHandleEvent(const TWservCrEvent& aEvent);
    void TakeScreenshot(TInt);
    void TakeScreenshotL(TInt);
    void NotifyWindowGroupToBackground(TInt aWindowGroupId);
    void NotifyWindowGroupToBackgroundL(TInt aWindowGroupId);
    TInt OrientationToAngle();
    
public:
    void SendMessageL( const TDesC8& aMessage );

private:
    CTsIdList* iAllowedList;
    RPointerArray<CTsScreenshotNotifier> iCache;
    TInt iWindowGroupId;
};

#endif //TSSCREENSHOTPLUGIN_H
