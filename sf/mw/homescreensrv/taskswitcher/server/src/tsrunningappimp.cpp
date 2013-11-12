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

#include <apgwgnam.h>

#include "tsrunningappimp.h"
#include "tsresourcemanager.h"

//------------------------------------------------------------------------------
CTsRunningApp* CTsRunningApp::NewLC( 
                             MTsResourceManager& aResources,
                             const RWsSession::TWindowGroupChainInfo& aInfo )
    {
    CTsRunningApp* self = new(ELeave) CTsRunningApp(aResources, aInfo);
    CleanupStack::PushL( self );
    return self;
    }

//------------------------------------------------------------------------------
CTsRunningApp::~CTsRunningApp()
    {
    delete iWindowGroupName;
    }

//------------------------------------------------------------------------------
CTsRunningApp::CTsRunningApp( MTsResourceManager& aResources,
               const RWsSession::TWindowGroupChainInfo& aInfo )
:
iResources(aResources),
iWindowGroupInfo(aInfo)
    {
    //No implementation required
    }

//------------------------------------------------------------------------------
const CApaWindowGroupName& CTsRunningApp::WindowGroupNameL() const
    {
    if( 0 == iWindowGroupName )
        {
        iWindowGroupName = CApaWindowGroupName::NewL( iResources.WsSession(), 
                                                      WindowGroupId() );
        }
    return *iWindowGroupName;
    }

//------------------------------------------------------------------------------
TUid CTsRunningApp::UidL()const
    {
    return WindowGroupNameL().AppUid();
    }

//------------------------------------------------------------------------------
const TDesC& CTsRunningApp::CaptionL() const
    {
    iCaption.Set(WindowGroupNameL().Caption());
    return iCaption;
    
    }

//------------------------------------------------------------------------------
TInt CTsRunningApp::WindowGroupId() const
    {
    return iWindowGroupInfo.iId;
    }

//------------------------------------------------------------------------------
TInt CTsRunningApp::ParentWindowGroupId() const
    {
    return iWindowGroupInfo.iParentId;
    }

//------------------------------------------------------------------------------
TBool CTsRunningApp::IsEmbeded() const
    {
    return 0 < iWindowGroupInfo.iParentId;
    }

//------------------------------------------------------------------------------
TBool CTsRunningApp::IsHiddenL() const
    {
    return WindowGroupNameL().Hidden();
    }

//------------------------------------------------------------------------------
TBool CTsRunningApp::IsSystemL() const
    {
    return  WindowGroupNameL().IsSystem();
    }
