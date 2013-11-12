/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFEngine class declaration.
*
*/


#ifndef C_CFENGINE_H
#define C_CFENGINE_H


#include <e32base.h>
#include <f32file.h>

#include "cfcontextobject.h"
#include "CFServSession.h"
#include "cfextendedcontextinterface.h"
#include "CFActionInterface.h"
#include "CFScriptInterface.h"
#include "cfactionhandler.h"
#include "cfcommon.h"
#include "CFSecurityChecker.h"
#include "cfcontextsourcecommandinterface.h"

class CCFContextManager;
class CCFStarter;
class CCFEComObserver;
class CCFServSession;
class CCFContextSourceManager;
class CCFScriptHandler;
class CCFActivator;
class MCFActionSubscriptionListener;
class CCFAsyncCmdQueue;
class CCFScriptEvent;


/**
* Context Framework Engine class.
*/
NONSHARABLE_CLASS( CCFEngine ): public CBase,
    public MCFExtendedContextInterface,
    public MCFActionInterface,
    public MCFScriptInterface,
    public MCFActionHandler,
    public MCFContextSourceCommand
    {
    public: // New methods
    
        static CCFEngine* NewL( MCFSecurityChecker& aSecurityChecker );
        static CCFEngine* NewLC( MCFSecurityChecker& aSecurityChecker );

        ~CCFEngine();
        
    private: // From base classes
    
        // @see MCFContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy );
            
        // @see MCFContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy,
            MCFContextSource& aOwner );
            
        // @see MCFContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy,
            MCFContextSource& aOwner );

        // @see MCFContextInterface
        TInt PublishContext( CCFContextObject& aContext,
            RThread& aClientThread );
        
        // @see MCFContextInterface
        TInt PublishContext( CCFContextObject& aContext,
            CCFContextDataObject& aData,
            RThread& aClientThread );
        
        // @see MCFContextInterface
        TInt SubscribeContext( CCFContextSubscription* aSubscription,
            MCFContextSubscriptionListener* aListener,
            RThread& aClientThread );
        
        // @see MCFContextInterface
        void UnsubscribeContext( CCFContextSubscription& aSubscription,
            MCFContextSubscriptionListener& aListener );
                
        // @see MCFContextInterface
        void UnsubscribeContexts(
            MCFContextSubscriptionListener& aObserver );
        
        // @see MCFContextInterface
        TInt RequestContext(
            RContextObjectArray& aResultBuffer,
            CCFContextQuery& aContextQuery,
            RThread& aClientThread );

		// @see MCFContextInterface            
        TInt GetSecurityPolicy( const TDesC& aContextSource,
            const TDesC& aContextType,
			TSecurityPolicy& aPolicy );

        // @see MCFExtendedContextInterface
		TInt GetReadSecurityPolicy( const TDesC& aContextSource,
				const TDesC& aContextType,
				TSecurityPolicy& aPolicy );

		// @see MCFExtendedContextInterface
		TInt GetWriteSecurityPolicy( const TDesC& aContextSource,
				const TDesC& aContextType,
				TSecurityPolicy& aPolicy );
		
		// @see MCFExtendedContextInterface
        TInt DefineContext( const TDesC& aContextSource,
                const TDesC& aContextType,
                const TSecurityPolicy& aReadPolicy,
                const TSecurityPolicy& aWritePolicy,
                MCFContextSource* aPublisher, const TUid& aPublisherUid );
            
        // @see MCFExtendedContextInterface
        void DeregisterPublisher( MCFContextSource& aPublisher );

        // @see MCFExtendedContextInterface
        TInt InstallContextSourceSetting( RFile& aSettingFile,
                const TUid& aContextSourceUid,
                RThread& aClientThread );
        
        // @see MCFExtendedContextInterface
        TInt UninstallContextSourceSetting( const TDesC& aSettingFilename,
                const TUid& aContextSourceUid,
                RThread& aClientThread );
        
        // @see MCFExtendedContextInterface
        TInt UninstallContextSourceSettings( const TUid& aContextSourceUid,
                RThread& aClientThread );
        
        // @see MCFExtendedContextInterface
        TInt PublishContextFromAction( CCFContextObject& aContext,
            TBool aAutoDefine );
        
        // @see MCFActionInterface
        TInt DefineAction( const TDesC& aActionIdentifier,
            const TSecurityPolicy& aSecurityPolicy,
            const TUid& aOwnerUid,
            MCFActionOwner* aOwner );
        
        // @see MCFActionInterface
        TInt SubscribeAction(
            CCFActionSubscription* aActionSubscription,
            MCFActionSubscriptionListener* aListener,
            RThread& aClientThread );            

        // @see MCFActionInterface
        void UnsubscribeAction( CCFActionSubscription& aActionSubscription,
            MCFActionSubscriptionListener& aListener );
            
        // @see MCFActionInterface
        void UnsubscribeActions( MCFActionSubscriptionListener& aListener );
        
        // @see MCFActionInterface
        void DeregisterActions( MCFActionOwner* aOwner );
        
        // @see MCFScriptInterface 
        TInt RegisterScript( const TDesC& aName,
            const TDesC8& aScript,
            TInt& aScriptId,
            const RThread& aClientThread,
            MCFScriptOwner& aOwner );
        
        // @see MCFScriptInterface 
        TInt DeregisterScript( TInt aScriptId,
            const RThread& aClientThread );

        // @see MCFScriptInterface 
        TInt DeregisterScript( const TDesC& aScriptName,
            const RThread& aClientThread );
        
        // @see MCFScriptInterface
        TInt DeregisterScripts( const RThread& aClientThread );

        // @see MCFScriptInterface
        void DeregisterScriptOwner( MCFScriptOwner& aOwner );

        // @see MCFScriptInterface 
        TInt SaveScript( const TDesC8& aScript,
            TInt aScriptId,
            const RThread& aClient );

        // @see MCFScriptInterface 
        TInt DeleteScript( const TDesC& aScriptName,
            RThread& aClientThread );
        
        // @see MCFScriptInterface 
        TInt DeleteScripts( RThread& aClientThread );
        
        // @see MCFScriptInterface 
        TBool AlreadyExists( const TDesC& aScriptName,
            const RThread& aClient,
            TInt& aScriptId ) const;
            
        // @see MCFScriptInterface 
        void CleanupPersistentDataByName( const TDesC& aScriptName,
            const RThread& aClientThread );
        
        // @see MCFScriptInterface 
        void CleanupPersistentDataByUid( const RThread& aClient );
        
        // @see MCFScriptInterface 
        TInt UpdateScript( TInt aScriptID,
            const RThread& aOwnerThread,
            const TDesC8& aUpdatedScript,
            MCFScriptOwner& aOwner );
        
        // @see MCFScriptInterface 
        TInt RestoreRomScript( TInt aScriptId, const RThread& aClient );

        // @see MCFScriptInterface 
        TInt IsUpgradeAllowed( const TDesC& aName,
            const TDesC8& aScript,
            const RThread& aOwnerThread );

        // @see MCFActionHandler 
        void FireActionL( CCFScriptEvent* aEvent );

        // From MCFActionHandler
        void FireActionL( const CCFContextSourceCommand& aCommand );

        // @see MCFActionHandler 
        TInt GetActionSecurityPolicy( const TDesC& aActionIdentifier,
            TSecurityPolicy& aPolicy );

        // @see MCFContextSourceCommand
        void HandleCommandL( const CCFContextSourceCommand& aCommand );

    private: // New functions

        /**
         * Appends publish context into asynchronous command queue if the
         * context publication is valid.
         * 
         * Leaves with:
         * - KErrNotFound, context not defined
         * - KErrAccessDenied, insufficient capabilities to write the context
         * - any other system-wide error code
         * 
         * @since S60 5.0
         * @param aContext Context object to be published.
         * @param aClientThread Handle to the current client.
         * @param aData Data object associated with the context.
         * @return None.
         */
        void EnqueuePublishContextL( CCFContextObject& aContext,
            RThread& aClientThread,
            CCFContextDataObject* aData = NULL );

        /**
         * Adds unsubscribe context cleanup into front of asynchronous command
         * queue.
         * 
         * Leaves with:
         * - KErrNoMemory, no memory to create the command
         * - any other system-wide error code
         * 
         * @since S60 5.0
         * @return None.
         */
        void EnqueueUnsubscribeContextCleanupL();

        /**
         * Appends fire action into asynchronous command queue.
         * 
         * Leaves with:
         * - KErrNoMemory, no memory to create the command
         * - any other system-wide error code
         * 
         * @since S60 5.0
         * @param aActionHandler Action handler for firing the action.
         * @param aEvent The action (script event). Ownership is transferred.
         * @return None.
         */
        void EnqueueFireActionL( MCFActionHandler& aActionHandler,
                CCFScriptEvent* aEvent );

        /**
         * Appends source command action into asynchronous command queue.
         * 
         * Leaves with:
         * - KErrNoMemory, no memory to create the command
         * - any other system-wide error code
         * 
         * @since S60 5.0
         * @param aSourceCommandHandler Source command handler.
         * @param aCommand The context source command action.
         * @return None.
         */
        void EnqueueSourceCommandActionL(
                MCFContextSourceCommand& aSourceCommandHandler,
                const CCFContextSourceCommand& aCommand );

    private:
    
        void ConstructL();
        CCFEngine( MCFSecurityChecker& aSecurityChecker );
        
    private: // Data
    
        // Own: CF starter
        CCFStarter* iStarter;
        
        // Own: ECom observer
        CCFEComObserver* iEComObserver;
    
        // Own: Data manager
        CCFContextManager* iContextManager;
        
        // Own: Context source manager
        CCFContextSourceManager* iContextSourceManager;
        
        // Own: Script Engine
        CCFScriptHandler* iScriptEngine;
        
        // Own: Activator engine
        CCFActivator* iActivatorEngine;
        
        // Own: File server session.
        // Keep this as the only session for whole CF!
        RFs iFs;
        
        // CFSecurityChecker
        MCFSecurityChecker* iSecurityChecker;
        
        // Maximum data object size
        TInt iMaxDataObjectSize;

        // Own: Queue for asynchronous commands.
        CCFAsyncCmdQueue* iAsyncCmdQueue;
    };

#endif  // C_CFENGINE_H
