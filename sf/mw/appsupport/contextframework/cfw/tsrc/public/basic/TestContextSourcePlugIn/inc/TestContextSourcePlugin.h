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
* Description:  ?Description
*
*/


#ifndef C_TESTCONTEXTSOURCEPLUGIN_H
#define C_TESTCONTEXTSOURCEPLUGIN_H

#include <CFContextSourcePlugIn.h>
#include <cfcontextsourceinterface.h>
#include <cfcontextsourceupgrade.h>
#include <cfcontextsubscriptionlistener.h>
#include <cfcontextsourcecommand.h>

#include "cfpropertylistenercallback.h"

class CCFPropertyListener;
class CCFContextObjectImpl;
class CCFContextSubscription;

// P&S definitions
const TUid KContextSourceTestCategory = { 0x0100FFFF };
const TUint32 KPublishContextKey = 0x00000001;
const TUint32 KDisableChecks = 0x00000002;

/**
* 
*/
NONSHARABLE_CLASS( CTestContextSourcePlugIn ):
    public CCFContextSourcePlugIn,
    public MCFPropertyListenerCallBack,
    public MCFContextSource,
    public MCFContextSourceUpgrade,
    public MCFContextSubscriptionListener
    {
    public:
    
        static CTestContextSourcePlugIn* NewL( 
        	TContextSourceInitParams* aParams);
        static CTestContextSourcePlugIn* NewLC( 
        	TContextSourceInitParams* aParams);
            
        ~CTestContextSourcePlugIn();
    
    private: // from CCFContextSourcePlugIn

        // @see CCFContextSourcePlugIn
        TAny* Extension( const TUid& aExtensionUid ) const;

        // @see CCFContextSourcePlugIn
    	void HandleSettingL( CCFContextSourceSettingArray* aSettingList );
            
        // @see CCFContextSourcePlugIn
    	void DefineContextsL();
    	
        // @see CCFContextSourcePlugIn
    	void InitializeL();
    	
    private: // From MCFPropertyListenerCallBack
    
        // @see MCFPropertyListenerCallBack
        void HandlePropertyChangedL();
    	
    private: // from MCFContextSource

        // from MCFContextSource
        void Subscribers( const TDesC& aContextSource,
             const TDesC& aContextType );

        // from MCFContextSource
        void NoSubscribers( const TDesC& aContextSource,
             const TDesC& aContextType );

        // from MCFContextSource
        void HandleCommand( const CCFContextSourceCommand& aCommand );

    private: // From MCFContextSourceUpgrade

        // @see MCFContextSourceUpgrade
        TInt CheckValidity( RThread& aClientThread,
                const CCFContextSourceSettingArray& aSettingList );

        // @see MCFContextSourceUpgrade
        void UpdateSettingsL( CCFContextSourceSettingArray* aSettingList );

    private: // From MCFContextSubscriptionListener
    
        // @see MCFContextSubscriptionListener
        void ContextIndicationL( CCFContextIndication* aIndication );
        
        // @see MCFContextSubscriptionListener
        virtual TInt Client( RThread& aThread ) const;
        
        // @see MCFContextSubscriptionListener
        virtual void HandleContextSubscriptionError( TInt aError,
            const TDesC& aSource,
            const TDesC& aType );

    private:

        CCFContextObjectImpl* ContextWithValueForSubscriberNotifyL(
                const TDesC& aSubscriberContextSource,
                const TDesC& aSubscriberContextType,
                const TDesC& aOkValueForNewContext );

        void GetSourceCommandResponseValue( TPtrC& aResponseValue,
                const CCFContextSourceCommand& aCommand,
                const TDesC& aCmdType );

    protected:

        CTestContextSourcePlugIn( TContextSourceInitParams* aParams );
        void ConstructL();
        
    private:
    
        TBool iHandleSettings;
        TBool iDefineContexts;
        TBool iInitialize;
        CCFPropertyListener* iPSListenerA;
        TInt iObservingContextCount;
        CCFContextSubscription* iSubscriberSub1;
        CCFContextSubscription* iSubscriberSub2;
        TBool iSubscriberSub1Owned;
        TBool iSubscriberSub2Owned;
        RArray< TCFSourceCommandSenderId > iActiveRequests;
    };

#endif
