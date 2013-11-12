/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
* 
* Description: 
*
*/




#include "BrowserRfsPlugin.h"

_LIT(KScriptPath, "z:\\resource\\browserrfs.txt");

CBrowserRfsPlugin::CBrowserRfsPlugin()
    {
    }

CBrowserRfsPlugin::CBrowserRfsPlugin(TAny* /*aInitParams*/): CRFSPlugin()
    {
    }


CBrowserRfsPlugin::~CBrowserRfsPlugin()
    {
    }


// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CBrowserRfsPlugin* CBrowserRfsPlugin::NewL(TAny* aInitParams)
    {
    CBrowserRfsPlugin* self = new (ELeave) CBrowserRfsPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//

void CBrowserRfsPlugin::ConstructL()
    {
//    #ifdef _DEBUG
//        RDebug::Print(_L("CBrowserRfsPlugin::ConstructL()"));
//    #endif
    }

// ---------------------------------------------------------
// RestoreFactorySettingsL
// ---------------------------------------------------------
//

//#ifdef _DEBUG
void CBrowserRfsPlugin::RestoreFactorySettingsL( const TRfsReason aType )
{
// TODO:
//     We can expand the functionalities here further 
//    #ifdef _DEBUG
//        RDebug::Print(_L("CBrowserRfsPlugin::RestoreFactorySettings"));
//    #endif
}




void CBrowserRfsPlugin::GetScriptL( const TRfsReason aType, TDes& aPath )
    {
        aPath.Copy( KScriptPath);   	
/*    if(aType == EDeepRfs)
        {
        aPath.Copy(KScriptPath);
        }
*/        
    }



void CBrowserRfsPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                           TDesC& /*aCommand*/ )
    {
//    #ifdef _DEBUG
//        RDebug::Print(_L("CBrowserRfsPlugin::RestoreFactorySettings"));
//    #endif
    }


