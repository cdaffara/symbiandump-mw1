/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorSourcePlugIn class declaration.
*
*/



#ifndef C_SENSORSOURCEPLUGIN_H
#define C_SENSORSOURCEPLUGIN_H

#include <cfcontextsourceplugin.h>
#include <cfcontextsubscriptionlistener.h>
#include <cfcontextobject.h>
#include <cfcontextsourceinterface.h>

#include "sensorchannelbase.h"

class CCFContextSubscription;
class CSensorCommand;

/**
* Sesnsor source plugin.
* Sensor source plugin is responsible for listening sensor events
* from sensor server and map sensor events to contexts.
*
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CSensorSourcePlugIn ):
	public CCFContextSourcePlugIn,
	public MCFContextSubscriptionListener,
	public MCFContextSource
    {
    public:

        // Two phased constructors
        static CSensorSourcePlugIn* NewL(
        	TContextSourceInitParams* aParams );
        static CSensorSourcePlugIn* NewLC(
        	TContextSourceInitParams* aParams );

        // Destructor
        ~CSensorSourcePlugIn();

    private: // From base classes

        // @see CCFContextSourcePlugIn
    	void HandleSettingL( CCFContextSourceSettingArray* aSettingList );

        // @see CCFContextSourcePlugIn
    	void DefineContextsL();

        // @see CCFContextSourcePlugIn
    	void InitializeL();

    // @see CCFContextSourcePlugIn
    TAny* Extension( const TUid& aExtensionUid ) const;

    	// @see MCFContextSubscriptionListener
    	void ContextIndicationL( CCFContextIndication* aIndication );

    	// @see MCFContextSubscriptionListener
    	TInt Client( RThread& aThread ) const;

    	// @see MCFContextSubscriptionListener
    	void HandleContextSubscriptionError( TInt aError,
            const TDesC& aSource,
            const TDesC& aType );

    // @see MCFContextSource
    void Subscribers( const TDesC& aContextSource,
        const TDesC& aContextType );

    // @see MCFContextSource
    void NoSubscribers( const TDesC& aContextSource,
        const TDesC& aContextType );

    // @see MCFContextSource
    void HandleCommand( const CCFContextSourceCommand& aCommand );

    private:

        // Subscribes for specified contexts
        void SubscribeL();

        // Creates a channel
        CSensorChannelBase* CreateChannelL(
            CSensorChannelBase::TSensorChannelId aChannel,
            TUint aChannelTypeId,
            const TDesC8& aLocation,
            const TDesC8& aVendor );

    	// Search correct channel and forward source command
    	void HandleCommandL( const CCFContextSourceCommand& aCommand );

    private:

        CSensorSourcePlugIn( TContextSourceInitParams* aParams );

    private: // Data

        // Own: Sensor channels
        RPointerArray<CSensorChannelBase> iChannels;

        // Own: Subscriptions
        RPointerArray<CCFContextSubscription> iSubscriptions;

        // Own: Sensor source settings
        CCFContextSourceSettingArray* iSettings;

    	// Active double tap count - NOTE: Source commands should be used instead!
    	TInt iActiveDoubleTap;

    	// Active orientation count - NOTE: Source commands should be used instead!
    	TInt iActiveOrientation;

    	// Active proximity count - NOTE: Source commands should be used instead!
    	TInt iActiveProximity;

    	// Feature ID
    	HBufC* iFeatureId;

    };

#endif
