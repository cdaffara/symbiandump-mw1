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
* Description:  Declaration of CMessageCentreRFSPlugin class
*
*/


#ifndef C_MSGCENTRERFSPLUGIN_H
#define C_MSGCENTRERFSPLUGIN_H

//  INCLUDES
#include <rfsPlugin.h>

// CLASS DECLARATION
class CMsgCentreRFSPlugin: public CRFSPlugin
    {

public:

    /**
    * Two-phased constructor.
    *
    * @param aInitParams initial parameters
    * @return The new object
    */
    static CMsgCentreRFSPlugin* NewL( TAny* aInitParams );

    /**
    * Destructor.
    */
    virtual ~CMsgCentreRFSPlugin();

public: // From CRFSPlugin

    /**
    * @see CRFSPlugin.
    */
    void RestoreFactorySettingsL( const TRfsReason aType );

    /**
    * @see CRFSPlugin.
    */
    void GetScriptL( const TRfsReason aType, TDes& aPath );

    /**
    * @see CRFSPlugin.
    */
    void ExecuteCustomCommandL( const TRfsReason aType, TDesC& aCommand );

private: // Data

    };

#endif // C_MSGCENTRERFSPLUGIN_H
