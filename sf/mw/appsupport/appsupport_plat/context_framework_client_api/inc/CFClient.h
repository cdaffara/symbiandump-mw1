/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Context Framework Client side interface.
*
*/


#ifndef C_CFCLIENT_H
#define C_CFCLIENT_H

#include <e32base.h>
#include <cfcontextobject.h>
#include <cfcontextquery.h>

class CCFContextSubscription;
class CCFActionSubscription;
class CCFContextDataObject;
class MCFListener;
class TSecurityPolicy;

/**
* Client side API of Context Framework.
*
* Context Framework Client provides following functionality:
* - Define contexts
* - Publish contexts
* - Subscribe contexts
* - Define actions
* - Subscribe actions
* - Register new scripts
* - Deregister scripts
*
* Before contexts can be published or subscribed it first
* needs to be defined. Otherwise error code will be returned
* to client side.
*
* Before actions can be subscribed they must be defined. Every
* action must be secured with a security policy. If your action
* does not need any security, NONE capabilities can be defined.
*
* Client application can request contexts that have been published in
* Context Framework. Contexts will be delivered in an array. Array will
* contain all the context objects where the client has sufficient
* capabilities.
*
* This class is not intended for user derivation.
*
* Below is described is described examples how to utilze
* Context Framework Client.
* 
* Initialize:
* When CCFClient instance is created a connection to Context Framework Server
* is also created. Creator of the instance must implement MCFListener interface
* and pass it as a parameter.
* @code
*  CCFClient* client = CCFClient::NewLC( *this );
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* Define context:
* If the context is already been defined it cannot be redfined.
* KErrAlreadyExists is returned to the client if the context has
* already been defined. You can use a security policy that is
* suitable for your own context.
* Remember not to set bigger capabilities than your process has.
* @code
*  _LIT( KContextSource, "Source" );
*  _LIT( KContextType, "Type" );
*  _LIT_SECURITY_POLICY_PASS( KContextSec );
*  CCFClient* client = CCFClient::NewLC( *this );
*  TInt err = client->DefineContext( KContextSource, KContextType, KContextSec );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* Publish context:
* Before context can bee published it must have been defined. KErrNotFound is returned
* to the client if the published context cannot be found. KErrAccessDenied is returned
* if the client process does not have sufficient capabilities.
* @code
*  _LIT( KContextSource, "Source" );
*  _LIT( KContextType, "Type" );
*  _LIT( KContextValue, "Value" );
*  CCFContextObject* co = CCFContextObject::NewLC();
*  co->SetSourceL( KContextSource );
*  co->SetTypeL( KContextType );
*  co->SetValueL( KContextValue );
*  CCFClient* client = CCFClient::NewLC( *this );
*  TInt err = client->PublishContext( *co );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( client );
*  CleanupStack::PopAndDestroy( co );
* @endcode
*
* Subscribe context:
* Context can be subscribed without it being defined. When the client subscribes for
* a context which has not been defined the context subscription is treated as
* "unsecure". When the specified context is defined a capability check is done.
* If the client process does not have sufficient capabilities, an error is
* notified through MCFListener interface.
* When the subscribed context changes context indication is notified through
* MCFListener interface.
* @code
*  _LIT( KContextSource, "Source" );
*  _LIT( KContextType, "Type" );
*  _LIT( KContextValue, "Value" );
*  CCFClient* client = CCFClient::NewLC( *this );
*  CCFContextSubscription* subscription = CCFContextSubscription::NewLC();
*  subscription->SetSourceL( KContextSource );
*  subscription->SetTypeL( KContextType );
*  TInt err = client->SubscribeContext( *subscription );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( subscription );
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* Request context:
* Only contexts that have been defined can be requested. Since contexts
* has a certain security a security check is done. If client process does
* not have sufficient capabilities for the requested context it will not be
* returned. The request can also contain a set of queries.
* @code
*  _LIT( KContextSource, "Source" );
*  _LIT( KContextType, "Type" );
*  CCFContextQuery* query = CCFContextQuery::NewLC();
*  query->SetSourceL( KContextSource );
*  query->SetTypeL( KContextType );
*  CCFClient* client = CCFClient::NewLC( *this );
*  RContextObjectArray result;
*  TInt err = client->RequestContext( query, result );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  else if( result.Count() )
*  {
*      // handle result
*  }
*  result.ResetAndDestroy();
*  CleanupStack::PopAndDestroy( client );
*  CleanupStack::PopAndDestroy( query );
* @endcode
*
* Define action:
* Defines an action in Context Framework Server. Only one action with the
* same identifier can be defined. KErrAlreadyExists is returned if
* the action is already defined. Action needs a certain security level.
* A specific security level can be defined for every action that is defined.
* @code
*  _LIT( KAction, "ActionId" );
*  _LIT_SECURITY_POLICY_PASS( KActionSec );
*  CCFClient* client = CCFClient::NewLC( *this );
*  TInt err = client->DefineAction( KAction, KActionSec );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* Subscribe action:
* Actions must be defined before they can be subscribed. KErrNotFound is returned
* if an action is tried to be subscribed without being defined.
* Client process must have the needed security level for the action.
* If client process fail security check KErrAccessDenied is returned.
* When a certain rule is evaluated to true where the subscribed action is
* executed action indication is notified through MCFListener interface.
* @code
*  _LIT( KAction, "ActionId" );
*  CCFClient* client = CCFClient::NewLC( *this );
*  CCFActionSubscription* subscription = CCFActionSubscription::NewLC();
*  subscription->SetActionIdentifierL( KAction );
*  TInt err = client->SubscribeAction( *subscription );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( subscription );
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* Register script:
* Client can register own scripts to Context Framework. When the script is registered
* all the context objects and actions related to the script is evaluated and
* security check is made.
* Client process must have sufficient security for the context objects and
* actions used in the script.
* The new script is not permanently registered and stored. When the device is rebooted
* client needs to re-register the script.
* Notice that the script file must be in public location so that Context Framework
* server can access to it.
* @code
*  _LIT( KScriptPath, "c:\\data\\myscripts\\script.rul" );
*  CCFClient* client = CCFClient::NewLC( *this );
*  TInt ruleId = 0;
*  TInt err = client->RegisterScript( KScriptPath, ruleId );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* Deregister script:
* Before a script can be deregistered it must have been first registered.
* @code
*  _LIT( KScriptPath, "c:\\data\\myscripts\\script.rul" );
*  CCFClient* client = CCFClient::NewLC( *this );
*  TInt ruleId = 0;
*  TInt err = client->RegisterScript( KScriptPath, ruleId );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  err = client->DeregisterScript( ruleId );
*  if( err != KErrNone )
*  {
*      // handle error
*  }
*  CleanupStack::PopAndDestroy( client );
* @endcode
*
* @lib CFClient.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFClient ): public CBase
    {
    public:
    
        /**
        * Symbian two phased constructors.
        * @since S60 5.0
        * @param aListener Context Framework listener interface.
        * @return None
        */
        IMPORT_C static CCFClient* NewL( MCFListener& aListener );
        IMPORT_C static CCFClient* NewLC( MCFListener& aListener );
        
    public: // New methods
        
        /**
        * Defines a new context in Context Framework.
        * Every context needs to be defined before it can be published,
        * subscribed or requested.
        *
        * Security policy for reading and writing is the same.
        * 
        * If some client subscribes for this context, the contex owner
        * will be notified through MCFContextSource interface.
        * MCFContextSource interface will be enquired through MCFListener
        * interface extension.
        *
        * @since S60 5.0
        * @param aContextSource Source of the context.
        * @param aContextType Type of the context.
        * @param aReadWritePolicy Needed capabilities for the context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAlreadyExists, defined context already exists.
        */
        virtual TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy ) = 0;
            
        /**
        * Defines a new context in Context Framework.
        * Every context needs to be defined before it can be published,
        * subscribed or requested.
        *
        * Security policy for reading and writing are different.
        * 
        * If some client subscribes for this context, the contex owner
        * will be notified through MCFContextSource interface.
        * MCFContextSource interface will be enquired through MCFListener
        * interface extension.
        *
        * @since S60 5.0
        * @param aContextSource Source of the context.
        * @param aContextType Type of the context.
        * @param aReadPolicy Needed capabilities for reading context value.
        * @param aWritePolicy Needed capabilities for writing context value.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAlreadyExists, defined context already exists.
        */
        virtual TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy ) = 0;
            
        /**
        * Publishes a new value of a context into Context Framework.
        * Subscribed clients will be notified of the change.
        * Context must have been defined before it can be published.
        *
        * @since S60 5.0
        * @param aContext Context object to be added.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, context not defined
        * - KErrAccessDenied, insufficient capabilities for the context
        * - KErrArgument if context source, type and value not defined
        */
        virtual TInt PublishContext( CCFContextObject& aContext ) = 0;
            
        /**
        * Publishes a new value of a context into Context Framework.
        * Subscribed clients will be notified of the change.
        * Context can be associated with a specific data object.
        * If a subscriber has requested to receive also data objects
        * the data object is associated in the context indication.
        * Context must have been defined before it can be published.
        *
        * @since S60 5.0
        * @param aContext Context object to be added.
        * @param aData Data object associated the context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, context not defined
        * - KErrAccessDenied, insufficient capabilities for the context
        * - KErrArgument if context source, type and value not defined
        * - KErrNotSupported if the context data UID is NULL
        */
        virtual TInt PublishContext( CCFContextObject& aContext,
            CCFContextDataObject& aData ) = 0;
            
        /**
        * Add subscription to Context Framework to get indications
        * about context changes of certain type.
        * If client does not have suffcient capabilities error will
        * be informed through MCFListener interface. This can happen
        * if client subscribes in a context that has not yet been defined.
        * When the context is later on defined a check is made against clients
        * process capabilities. If client lacks capabilities error will
        * be notifed.
        *
        * @since S60 5.0
		* @param aSubscription Information about which kind of context
        *  changes are sent to object implementing
        *  <code>MCFListener</code>. 
        * @return Possible return values:
        * - KErrNone, no errors
        * - Otherwise system wide error code
        */
        virtual TInt SubscribeContext(
        	CCFContextSubscription& aSubscription ) = 0;
        
        /**
        * Remove subscription from context manager.
        *  If subscription is not found on the server this method leaves
        *  with <code>KErrNotFound</code>.
        *
        * @since S60 5.0
        * @param aSubscription Subscription to be removed.
        *  This must be the same object that is used in 
        *  <code>SubscribeL()</code> method.
        * @return Possible return values:
        * - KErrNone, no errors
        * - Otherwise system wide error code
        */
        virtual TInt UnsubscribeContext(
            CCFContextSubscription& aSubscription ) = 0;
        
        /**
        * Requests the latest context of the specified type, source, or type &
        * source from the Context Framework.
        * All the contexts where client has enough capabilities will
        * be returned in aRequestResult array.
		*
        * @since S60 5.0
        * @param aContextQuery 
        *  Requirements for context type and source. 
        *  If the given type is a partial ontology path, all contexts beginning
        *  with that will be returned. Accordingly,
        *  if the given source is a partial source name, all contexts from a
        *  source having that beginning will be returned.
        * @param aRequestResults Storage for the requested contexts.
        * @return Possible return values:
        * - KErrNone, no errors
        * - Otherwise system wide error code
        */
        virtual TInt RequestContext( CCFContextQuery& aContextQuery,
            RContextObjectArray& aRequestResults ) = 0;
        
        /**
        * Requests the latest context of the specified types, sources, or types
        * & sources from the Context Framework.
        * All the contexts where client has enough capabilities will
        * be returned in aRequestResult array.
        *
        * @note Using partial ontology paths in types or partial source names
        *       may result in repeated occurrence of the same contexts
        *       (redundancy), as is also the case with repeating exactly the
        *       same definition in the query set.
        *
        * @since S60 5.0
        * @param aContextQuerySet 
        * An array of query definitions containing type and source
        * requirement pairs.
        *  If the given type is a partial ontology path, all contexts beginning
        *  with that will be stored into the provided storage array. 
        *  Accordingly,if the given source is a partial source name,
        *  all contexts from a source having that beginning will also be
        *  stored. The given query set does not have to be analogous. 
        *  One element may define only the type, another only the source,
        *  and finally, some other might define both type and source.
        * @param aRequestResults Storage for the requested contexts.
        *  The call just returns and leaves this set untouchable, if the
        *  given query set is empty.
        * @return Possible return values:
        * - KErrNone, no errors
        * - Otherwise system wide error code
        */
        virtual TInt RequestContextSet( RContextQueryArray& aContextQuerySet,
            RContextObjectArray& aRequestResults ) = 0;
        
        /**
        * Defines an action in Context Framework.
        * When action is defined in Context Framework it can be subscribed.
        * When the action is needed to be executed action indication is
        * received through MCFListener interface.
        * Notice that actions can be defined also from action plug-ins.
        * If some action plug-in has already defined the action in hand
        * action definition will fail.
        * 
        * @since S60 5.0
        * @param aActionIdentifier Action identifier.
        * @param aSecurityPolicy Security policy for the action.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAlreadyExists, defined action already exists.
        * - Otherwise system wide error code
        */
        virtual TInt DefineAction( const TDesC& aActionIdentifier,
            const TSecurityPolicy& aSecurityPolicy ) = 0;
        
        /**
        * Subscribes for action.
        * When certain action is needed to be performed action indication
        * is informed through MCFListener API. 
        * Every action is needed to be defined before they can be subscribed.
        *
        * @since S60 5.0
        * @param aSubscription Action subscription
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, action not defined.
        * - KErrAccessDenied, not enough capabilities for the action.
        * - Otherwise system wide error code
        */
        virtual TInt SubscribeAction(
            CCFActionSubscription& aSubscription ) = 0;

        /**
        * Unsubscribes for action.
        *
        * @since S60 5.0
        * @param aSubscription Action subscription
        * @return Possible return values:
        * - KErrNone, no errors
        * - Otherwise system wide error code
        */
        virtual TInt UnsubscribeAction(
            CCFActionSubscription& aSubscription ) = 0;

        /**
        * Registers a new script in Context Framework.
        * If the client process wants to provide the actions defined in the
        * script it is necessary first to subscribe the actions needed and
        * then register the script. If a script with the same name is found,
        * it will be automatically replaced and the script id remains the same.
        *
        * Notice that the script is valid until the device is rebooted. 
        * After reboot, the script is needed to be registered again.
        *
        * @since S60 5.0
        * @param aScriptFileName File path to the script.
        * @param aScriptId Script ID return value.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, specified script file not found.
        * - KErrNotFound, context has not been defined.
        * - KErrNotFound, action has not been defined.
        * - KErrPermissionDenied, insufficient capabilities.
        * - Otherwise system wide error code
        */
        virtual TInt RegisterScript( const TDesC& aScriptFileName,
        	TInt& aScriptId ) = 0;
        	
        /**
        * Deregisters a script in Context Framework.
        * 
        * @since S60 5.0
        * @param aScript File path to the script.
        * @param aScriptId Script obtained from RegisterScript -method.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, script not found for ID provided.
        * - KErrPermissionDenied, insufficient capabilities.
        * - Otherwise system wide error code
        */
        virtual TInt DeregisterScript( TInt aScriptId ) = 0;
        
		/**
        * Registers a new script in Context Framework.
        * If the client process wants to provide the actions defined in the
        * script it is necessary first to subscribe the actions needed and
        * then register the script. If a script with the same name is found,
        * it will be automatically replaced and the script id remains the same.
        *
        * Notice that the script is valid until the device is rebooted. 
        * After reboot, the script is needed to be registered again.
        *
        * @since S60 5.0
        * @param aScriptFileName Name of the script.
        * @param aScript Descriptor containing the script.
        * @param aScriptId Script ID return value.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrNotFound, context has not been defined.
        * - KErrNotFound, action has not been defined.
        * - KErrPermissionDenied, insufficient capabilities.
        * - KErrArgument, Script file name is illegal.
        * - Otherwise system wide error code
        */
        virtual TInt RegisterScript( const TDesC& aScriptFileName,
            const TDesC8& aScript,
        	TInt& aScriptId ) = 0;
        
        /**
        * Saves a context source setting in Context Framework.
        * Setting is stored if install succeeds.
        *
        * @since S60 5.0
        * @param aSettingFilename Filename with full path to the setting.
        * @param aContextSourceUid Implementation UID of the context source to
        *   receive setting.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, specified setting file not found.
        * - KErrNotSupported, context source does not support the settings.
        * - KErrBadHandle, context source not found/loaded.
        * - KErrPermissionDenied, client has insufficient capabilities to
        *   install the settings to the context source.
        * - KErrExtensionNotSupported, context source does not support
        *   installing settings.
        * - Otherwise system wide error code.
        */
        virtual TInt SaveContextSourceSetting(
            const TDesC& aSettingFilename,
            const TUid& aContextSourceUid ) = 0;
        
        /**
        * Deletes a context source setting in Context Framework.
        * Setting is removed if uninstall succeeds.
        *
        * @since S60 5.0
        * @param aSettingFilename Filename (without path) that was used to
        *   install the setting.
        * @param aContextSourceUid Implementation UID of the context source.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, specified setting file not found.
        * - KErrBadHandle, context source not found/loaded.
        * - KErrPathNotFound, specified context source does not have any
        *   settings.
        * - KErrExtensionNotSupported, context source does not support
        *   uninstalling settings.
        * - Otherwise system wide error code.
        */
        virtual TInt DeleteContextSourceSetting(
            const TDesC& aSettingFilename,
            const TUid& aContextSourceUid ) = 0;
        
        /**
        * Uninstalls context source settings registered by a client in Context
        * Framework. Uninstalls all settings ever registered by the client for
        * the context source. Settings are removed if uninstall succeeds.
        *
        * @since S60 5.0
        * @param aContextSourceUid Implementation UID of the context source.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, specified client has not installed any setting files.
        * - KErrBadHandle, context source not found/loaded.
        * - KErrPathNotFound, specified context source does not have any
        *   settings.
        * - KErrExtensionNotSupported, context source does not support
        *   uninstalling settings.
        * - Otherwise system wide error code.
        */
        virtual TInt DeleteContextSourceSettings(
            const TUid& aContextSourceUid ) = 0;
            
        /**
        * Saves a new script in Context Framework private folder.
        * The script will be registered and updated when necessary.
        * The script will be automatically loaded on next device boot up.
        * If a script with the same name already exists, it will
        * be automatically replaced. 
        * 
        * @since S60 5.0
        * @param aScriptFileName The full path to a script file.
        * @param aScriptId Script ID return value.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, the specified file cannot be found.
        * - KErrNotFound, context has not been defined.
        * - KErrNotFound, action has not been defined.
        * - KErrAlreadyExists, the specified file already in use.
        * - KErrAccessDenied, insufficient capabilities.
        * - Otherwise system wide error code.
        */
        virtual TInt SaveScript( const TDesC& aScriptFileName,
            TInt& aScriptId ) = 0;
        
        /**
        * Saves a new script in Context Framework private folder.
        * The script will be registered and updated when necessary.
        * The script will be automatically loaded on next device boot up.
        * If a script with the same name already exists, it will
        * be automatically replaced. 
        *
        * @since S60 5.0
        * @param aFilename The name of the saved file.
        * @param aScript Descriptor containing the script.
        * @param aScriptId Script ID return value.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, context has not been defined.
        * - KErrNotFound, action has not been defined.
        * - KErrAlreadyExists, the specified filename already in use.
        * - KErrAccessDenied, insufficient capabilities.
        * - Otherwise system wide error code.
        */
        virtual TInt SaveScript( const TDesC& aScriptFileName,
            const TDesC8& aScript,
            TInt& aScriptId ) = 0;
        
        /**
        * Delete a script from Context Framework private folder.
        * The script will be automatically deregistered.
        * 
        * @since S60 5.0
        * @param aFilename Name of the script which is uninstalled.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, the specified script cannot be found.
        * - KErrAccessDenied, delete not allowed by this client.
        * - Otherwise system wide error code.
        */
        virtual TInt DeleteScript( const TDesC& aScriptFileName ) = 0;

        /**
        * Delete all scripts from Context Framework private folder added
        * by the particular process.
        * All the scripts are automatically deregistered.
        * 
        * @since S60 5.0
        * @param None.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, scripts not found for the client.
        * - Otherwise system wide error code.
        */
        virtual TInt DeleteScripts() = 0;
        
        /**
        * Upgrade existing script in ROM.
        * The new script is saved in C-drive and registered automatically.
        * The script located in ROM is inactive untill the upgrade
        * will be deleted. If a script with the same name is found, it will be
        * automatically replaced.
        *
        * The client must have the needed capabilities to be able to upgrade
        * rom script. The script which acts as an upgrade must also have at
        * least the same upgrade security level than the original script
        * located in rom.
        *
        * @since S60 5.0
        * @param aScriptFileName Script which to replace (full path).
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrAccessDenied, insufficient capabilities.
        * - Otherwise system wide error code.
        */
        virtual TInt UpgradeRomScript( const TDesC& aScriptFileName ) = 0;

        /**
        * Upgrade existing script in ROM.
        * The currently registered ROM based script will be deregistered and
        * the new script will be stored in RAM.
        * The script located in ROM is inactive untill the upgrade
        * will be deleted.
        * If a script with the same name is found, it will be
        * automatically replaced.
        *
        * The client must have the needed capabilities to be able to upgrade
        * rom script. The script which acts as an upgrade must also have at
        * least the same upgrade security level than the original script
        * located in rom.
        *
        * @since S60 5.0
        * @param aScriptFileName Script which to replace (only filename).
        * @param aScript Descriptor containing the script.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrAccessDenied, insufficient capabilities.
        * - Otherwise system wide error code.
        */
        virtual TInt UpgradeRomScript( const TDesC& aScriptFileName,
            const TDesC8& aScript ) = 0;
            
        /**
        * Restores the script located in ROM.
        * The script upgrade is deregistered and deleted from file system.
        * The original script located in ROM is registered in to use again.
        *
        * @since S60 5.0
        * @param aScriptFileName Script which to replace (full path).
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrAccessDenied, insufficient capabilities.
        * - Otherwise system wide error code.
        */
        virtual TInt RestoreRomScript( const TDesC& aScriptFileName ) = 0;
    };

#endif
