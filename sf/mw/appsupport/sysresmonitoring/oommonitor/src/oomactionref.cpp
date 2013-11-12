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
* Description:  Classes for executing OOM actions (e.g. closing applications and running plugins).
*
*/



#include "oomactionref.h"

TActionRef::TActionRef(TActionType aType, TInt aPriority, TOomSyncMode aSyncMode, TInt aRamEstimate, COomRunPlugin& aRunPlugin, TUint aWgIndexOfTargetApp)
    : iType(aType), iPriority(aPriority), iSyncMode(aSyncMode), iRamEstimate(aRamEstimate), iWgIndex(aWgIndexOfTargetApp), iRunPlugin(&aRunPlugin)
    {   
    }

TActionRef::TActionRef(TActionType aType, TInt aPriority, TOomSyncMode aSyncMode, TInt aRamEstimate, TInt aWgId, TUint aWgIndex)
: iType(aType), iPriority(aPriority), iSyncMode(aSyncMode), iRamEstimate(aRamEstimate), iWgId(aWgId), iWgIndex(aWgIndex), iRunPlugin(NULL)
    {   
    }

    
TActionRef::TActionType TActionRef::Type() const
    {
    return iType;
    }

TUint TActionRef::Priority() const
    {
    return iPriority;
    }

TOomSyncMode TActionRef::SyncMode() const
    {
    return iSyncMode;
    }
    
TInt TActionRef::RamEstimate() const
    {
    return iRamEstimate;
    }

TInt TActionRef::WgId() const
    {
    return iWgId;
    }

TInt TActionRef::WgIndex() const
    {
    return iWgIndex;
    }

COomRunPlugin& TActionRef::RunPlugin()
    {
    return *iRunPlugin;
    }
