/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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

*
*/






#ifndef RFSHELIXPLUGIN_H

#define RFSHELIXPLUGIN_H





#include <e32base.h>

#include <f32file.h>

#include <rfsPlugin.h>







class CRfsHelixPlugin: public CRFSPlugin

    {

public:



    

    static CRfsHelixPlugin* NewL(TAny* aInitParams);

    virtual ~CRfsHelixPlugin();

    void RestoreFactorySettingsL( const TRfsReason aType );

    void GetScriptL( const TRfsReason aType, TDes& aPath );

    void ExecuteCustomCommandL( const TRfsReason aType, TDesC& aCommand );

private:



    CRfsHelixPlugin();



	CRfsHelixPlugin(TAny* aInitParams);





    void ConstructL();



    };







#endif      // RFSHELIXPLUGIN_H



