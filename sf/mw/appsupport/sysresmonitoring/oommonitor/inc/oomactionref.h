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


#ifndef OOMACTIONREF_H_
#define OOMACTIONREF_H_

#include <e32base.h>
#include "oomactionconfig.h"

class COomRunPlugin;

/**
 *  Encapsulates a reference to an action. 
 *  
 *  Objects of this T-class are instantiated at memory freeing time in preference to the  
 *  COomAction derived objects, so that we do not instantiate anything on the heap at a time
 *  when the device is, by-definition, low on memory.
 *
 *  @lib oommonitor.lib
 *  @since S60 v5.0
 */
class TActionRef
    {
public:
    
    enum TActionType
        {
        EAppPlugin,
        ESystemPlugin,
        EAppClose
        };

    //constructor for Plugin actions
    TActionRef(TActionType aType, TInt aPriority, TOomSyncMode aSyncMode, TInt aRamEstimate, COomRunPlugin& aRunPlugin, TUint aWgIndexOfTargetApp);

    //constructor for AppClose actions
    TActionRef(TActionType aType, TInt aPriority, TOomSyncMode aSyncMode, TInt aRamEstimate, TInt aWgId, TUint aWgIndex);
    
    TActionType Type() const;
    TUint Priority() const;
    TOomSyncMode SyncMode() const;
    TInt RamEstimate() const;
    TInt WgId() const;
    TInt WgIndex() const;
    COomRunPlugin& RunPlugin();

private: //data
    
    TActionType iType;
    TUint iPriority;
    TOomSyncMode iSyncMode; //needed as we don't have reference to the config 
    TInt iRamEstimate; //needed as we don't have reference to the config 
    TInt iWgId; //For AppClose
    TInt iWgIndex;
    COomRunPlugin* iRunPlugin; //For Plugins. Not owned
    };

#endif /*OOMACTIONREF_H_*/
