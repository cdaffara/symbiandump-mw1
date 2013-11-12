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

#ifndef __CXMPPSETTINGSFETCHER_H__
#define __CXMPPSETTINGSFETCHER_H__

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CXmppParameters;

/**
 * CXmppSettingsFetcher
 *
 * Fetches settings from the xmpp settings store
 *
 */
class CXmppSettingsFetcher : public CBase
    {

public: 
    /**
     * Reads the settings corresponding to the settingsid from
     *   the xmpp settings store and stores them to the out parameter
     *
     * @param aSettingsId provided settingsid
     * @param aParams settings read from the store are stored in this parameter
     */
	IMPORT_C static TInt ReadSettingsL( TUint32 aSettingsId, CXmppParameters& aParams );

   };


#endif // __CXMPPSETTINGSFETCHER_H__
