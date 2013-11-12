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
* Description:  ?Description
*
*/


#ifndef C_CFCLIENTIMPL_H
#define C_CFCLIENTIMPL_H

#include <e32base.h>

#include "cfclient.h"
#include "CFClientSession.h"

class CCFContextService;
class CCFActionService;
class CCFScriptService;

/**
 * Client side API of Context Manager.
 */
NONSHARABLE_CLASS( CCFClientImpl ): public CCFClient
    {
public:

    // Symbian two phased constructor
    static CCFClientImpl* NewL(
        MCFListener& aListener );
    static CCFClientImpl* NewLC(
        MCFListener& aListener );

    // Destructor
    ~CCFClientImpl();

private: // From base classes

    // From CCFClient
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aSecurityPolicy );

    // From CCFClient
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aReadPolicy,
        const TSecurityPolicy& aWritePolicy );

    // From CCFClient
    TInt PublishContext( CCFContextObject& aContext );

    // From CCFClient
    TInt PublishContext( CCFContextObject& aContext,
        CCFContextDataObject& aData );

    // From CCFClient
    TInt SubscribeContext(CCFContextSubscription& aSubscription);

    // From CCFClient
    TInt UnsubscribeContext( CCFContextSubscription& aSubscription);

    // From CCFClient
    TInt RequestContext( CCFContextQuery& aContextQuery,
        RContextObjectArray& aRequestResults);

    // From CCFClient
    TInt RequestContextSet( RContextQueryArray& aContextQuerySet,
        RContextObjectArray& aRequestResults);

    // From CCFClient
    TInt DefineAction( const TDesC& aActionIdentifier,
        const TSecurityPolicy& aSecurityPolicy );

    // From CCFClient
    TInt SubscribeAction( CCFActionSubscription& aSubscription );

    // From CCFClient
    TInt UnsubscribeAction( CCFActionSubscription& aSubscription );

    // From CCFClient
    TInt RegisterScript( const TDesC& aScript, TInt& aScriptId );

    // From CCFClient
    TInt DeregisterScript( TInt aScriptId );

    // From CCFClient
    TInt RegisterScript( const TDesC& aScriptFileName,
        const TDesC8& aScript,
        TInt& aScriptId );

    // From CCFClient
    TInt SaveContextSourceSetting( const TDesC& aSettingFilename,
        const TUid& aContextSourceUid );

    // From CCFClient
    TInt DeleteContextSourceSetting( const TDesC& aSettingFilename,
        const TUid& aContextSourceUid );

    // From CCFClient
    TInt DeleteContextSourceSettings( const TUid& aContextSourceUid );

    // From CCFClient
    TInt SaveScript( const TDesC& aScriptFileName, TInt& aScriptId );

    // From CCFClient
    TInt SaveScript( const TDesC& aScriptFileName,
        const TDesC8& aScript, TInt& aScriptId );

    // From CCFClient
    TInt DeleteScript( const TDesC& aScriptFileName );

    // From CCFClient
    TInt DeleteScripts();
    
    // From CCFClient
    TInt UpgradeRomScript( const TDesC& aScriptFileName );
    
    // From CCFClient
    TInt UpgradeRomScript( const TDesC& aScriptFileName,
        const TDesC8& aScript );
    
    // From CCFClient
    TInt RestoreRomScript( const TDesC& aScriptFileName );

private:

    CCFClientImpl();
    void ConstructL( MCFListener& aListener );

private:

    /** Session to cfserver */
    RCFClientSession iCFClientSession;
    
    /** Context service */
    CCFContextService* iContextService;
    
    /** Action service */
    CCFActionService* iActionService;
    
    /** Script service */
    CCFScriptService* iScriptService;
    };

#endif
