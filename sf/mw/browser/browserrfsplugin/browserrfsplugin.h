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
*
*/






#ifndef BROWSERRFSPLUGIN_H

#define BROWSERRFSPLUGIN_H





#include <e32base.h>

// #include <f32file.h>

#include <rfsPlugin.h>







class CBrowserRfsPlugin: public CRFSPlugin

    {

public:



    

    static CBrowserRfsPlugin* NewL(TAny* aInitParams);

    virtual ~CBrowserRfsPlugin();

    void RestoreFactorySettingsL( const TRfsReason aType );

    void GetScriptL( const TRfsReason aType, TDes& aPath );

    void ExecuteCustomCommandL( const TRfsReason aType, TDesC& aCommand );

private:



    CBrowserRfsPlugin();


    CBrowserRfsPlugin(TAny* aInitParams);


    void ConstructL();



    };







#endif      // BrowserRfsPLUGIN_H



