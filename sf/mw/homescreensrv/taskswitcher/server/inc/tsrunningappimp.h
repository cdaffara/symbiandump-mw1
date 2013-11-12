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
#ifndef TSRUNNINGAPP_H
#define TSRUNNINGAPP_H

#include <e32base.h>
#include <w32std.h>

#include "tsrunningapp.h"

class CApaWindowGroupName;
class MTsResourceManager;

class CTsRunningApp: public CBase,
                     public MTsRunningApplication
{
public:
    static CTsRunningApp* NewLC( MTsResourceManager& aResources,
                                 const RWsSession::TWindowGroupChainInfo& aInfo );
    ~CTsRunningApp();

private:
    CTsRunningApp( MTsResourceManager& aResources,
                   const RWsSession::TWindowGroupChainInfo& aInfo );
    const CApaWindowGroupName& WindowGroupNameL() const;

public://from MTsRunningApplication
    TUid UidL()const;
    const TDesC& CaptionL() const;
    TBool IsHiddenL() const;
    TBool IsSystemL() const;
    TInt WindowGroupId() const;
    TInt ParentWindowGroupId() const;
    TBool IsEmbeded() const;
    

private:
    MTsResourceManager& iResources;
    const RWsSession::TWindowGroupChainInfo iWindowGroupInfo;
    mutable CApaWindowGroupName* iWindowGroupName;
    mutable TPtrC iCaption;
};

#endif//TSRUNNINGAPP_H
