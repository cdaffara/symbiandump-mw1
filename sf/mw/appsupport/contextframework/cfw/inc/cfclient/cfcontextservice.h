/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextService class declaration.
*
*/


#ifndef CFCONTEXTSERVICE_H
#define CFCONTEXTSERVICE_H

// SYSTEM INCLUDE FILES
#include <cfcontextobject.h>
#include <cfcontextquery.h>
#include <cfcontextsubscription.h>

// USER INCLUDE FILES
#include "cfservicebase.h"

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION
class CCFContextSubscriptionListener;
class CCFServerMessageListener;
class CCFContextDataObject;

/**
 * Context service class.
 *
 * @lib cfclient.lib
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextService ): public CCFServiceBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFContextService
     */
    static CCFContextService* NewL( RCFClientSession& aSession,
        MCFListener& aListener );
    static CCFContextService* NewLC( RCFClientSession& aSession,
        MCFListener& aListener );

    /**
     * C++ destructor.
     */
    virtual ~CCFContextService( );

public:

    // @see CCFClient
    void PublishContextL( CCFContextObject& aContext );
    
    // @see CCFClient
    void PublishContextL( CCFContextObject& aContext,
        CCFContextDataObject& aData );
    
    // @see CCFClient
    void SubscribeContextL( CCFContextSubscription& aSubscription );
    
    // @see CCFClient
    void UnsubscribeContextL( 
        CCFContextSubscription& aSubscription);
    
    // @see CCFClient
    void RequestContextL( CCFContextQuery& aContextQuery,
        RContextObjectArray& aRequestResults);
    
    // @see CCFClient
    void RequestContextSetL( RContextQueryArray& aContextQuerySet,
        RContextObjectArray& aRequestResults);
        
    // @see CCFClient
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aReadSecurityPolicy,
        const TSecurityPolicy& aWriteSecurityPolicy );
    
    // @see CCFClient
    TInt InstallContextSourceSetting( const TDesC& aSettingFilename,
        const TUid& aContextSourceUid );

    // @see CCFClient
    TInt UninstallContextSourceSetting( const TDesC& aSettingFilename,
        const TUid& aContextSourceUid );

    // @see CCFClient
    TInt UninstallContextSourceSettings( const TUid& aContextSourceUid );

private:

    // Checks is context is legal
    TBool IsContextLegalForAdd( const CCFContextObject& aContext );
    
    // Checks is context data is legal
    TInt IsContextDataLegalForAdd( const CCFContextDataObject& aContextData );

    // Read context objects from request stream
    void ReadStreamContextObjectL( const TDesC8& aData,
        RContextObjectArray& aArray );

    // Stream context subscription
    HBufC8* WriteStreamContextSubscriptionLC(
        CCFContextSubscription& aSubscription );
        
    // Stream context object
    HBufC8* WriteStreamContextObjectLC( CCFContextObject& aContext );
        
    // Stream context query array
    HBufC8* WriteStreamContextQueryLC( RContextQueryArray& aQueryArray );
        
    // Stream context query array
    HBufC8* WriteStreamContextQueryLC( CCFContextQuery& aQuery );

    // Stream context data object
    HBufC8* WriteStreamContextDataObjectLC(
        CCFContextDataObject& aContextData );

private:

    CCFContextService( RCFClientSession& aSession, MCFListener& aListener );
    void ConstructL( );

private: // Data

    /** Request buffer dynamic size */
    TInt iRequestBufferSize;

    /** Context data object maximum size **/
    TInt iContextDataMaxSize;

    /** Context subscription listener */
    CCFContextSubscriptionListener* iContextSubscriptionListener;

    /** Server message listener */
    CCFServerMessageListener* iServerMessageListener;
    };

#endif // CFCONTEXTSERVICE_H

