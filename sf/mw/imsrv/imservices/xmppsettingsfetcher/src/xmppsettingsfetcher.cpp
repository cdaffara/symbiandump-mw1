/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  fetches xmpp settings from settings store 
*
*/

#include "xmppsettingsfetcher.h"
#include "xmppservicesettingsapi.h"

#include "xmppparameters.h"
#include <xmppparams.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CXmppSettingsFetcher::ReadSettings
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CXmppSettingsFetcher::ReadSettingsL( TUint32 aSettingsId, CXmppParameters& aParams )
    {
    TBool settingsFound = EFalse;
    RArray<TUint32> settingIdArray;
    CXmppSettingsApi* api = CXmppSettingsApi::NewLC();

	CleanupClosePushL( settingIdArray );
    api->GetSettingsRecordIdArrayL( settingIdArray );

	//first store the settingsid
	aParams.SetSettingsId(aSettingsId);

    for ( TInt i=0; i<settingIdArray.Count(); i++ )
        {
        if (settingIdArray[i] == aSettingsId ) //settings id we are looking for
        	{
        	//settings with integer values
	        TInt paramValueInt(0);
	        api->GetParamL( settingIdArray[i], 
	        					XmppParams::KXmppParamServerPort(), 
	        					paramValueInt );
			aParams.SetServerPort(paramValueInt);
				                                                    
	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamHttpsProxyServerPort(),
								paramValueInt );
			aParams.SetProxyPort(paramValueInt);

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamSnapId(),
								paramValueInt );
			aParams.SetIapId(paramValueInt);

        	//settings with boolean values
			TBool paramValueBool(EFalse);
	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamOldSSLRequired(),
								paramValueBool );
			aParams.SetSsl(paramValueBool);
			
        	//settings with descriptor values
	        RBuf paramValueBuf;
	        paramValueBuf.CreateL( KMaxLength );
			paramValueBuf.CleanupClosePushL();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamXmppServerAddress(),
								paramValueBuf );
			aParams.SetServerAddress(paramValueBuf);
	        paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
								XmppParams::KXmppParamUsername(),
	                            paramValueBuf );
			aParams.SetUserName(paramValueBuf);
	        paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamPassword(),
	                            paramValueBuf );
			aParams.SetPasswd(paramValueBuf);
	        paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamResource(),
	                            paramValueBuf );
			aParams.SetResource(paramValueBuf);
	        paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamHttpsProxyServerAddress(),
	                            paramValueBuf );
			aParams.SetProxyServer(paramValueBuf);
			paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamConnMgrBus(),
	                            paramValueBuf );
			aParams.SetConnMgrBus(paramValueBuf);
			paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamConnMgrPath(),
	                            paramValueBuf );
			aParams.SetConnMgrPath(paramValueBuf);
			paramValueBuf.Zero();

	        api->GetParamL( settingIdArray[i],
	        					XmppParams::KXmppParamProtocol(),
	                            paramValueBuf );
			aParams.SetProtocol(paramValueBuf);

	        CleanupStack::PopAndDestroy( &paramValueBuf );
			settingsFound = ETrue;
	        break;
        	}
        }

    CleanupStack::PopAndDestroy( &settingIdArray );
    CleanupStack::PopAndDestroy( api );
    if(!settingsFound)
    	return KErrNotFound;
    return KErrNone;
    }

// End of File

