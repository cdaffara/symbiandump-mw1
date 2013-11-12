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
* Description:  ?Description
*
*/


#ifndef M_CFEXTENDEDCONTEXTINTERFACE_H
#define M_CFEXTENDEDCONTEXTINTERFACE_H

#include <cfcontextinterface.h>
#include <f32file.h>

/**
* Extended context interface
*
* @since S60 5.0
* @lib None-
*/
class MCFExtendedContextInterface : public MCFContextInterface
    {
    public: // New functions
    
		/**
		* Gets the read security policy of particular context.
		*
		* @param aContextSource Context Source.
		* @param aContextType Context Type.
		* @param aPolicy security policy which will be set by this method.
		* @return KErrNone if context definition found, KErrNotFound if not.
		*/
		virtual TInt GetReadSecurityPolicy( const TDesC& aContextSource,
         const TDesC& aContextType,
         TSecurityPolicy& aPolicy ) = 0;


		/**
		* Gets the write security policy of particular context.
		*
		* @param aContextSource Context Source.
		* @param aContextType Context Type.
		* @param aPolicy security policy which will be set by this method.
		* @return KErrNone if context definition found, KErrNotFound if not.
		*/
		virtual TInt GetWriteSecurityPolicy( const TDesC& aContextSource,
         const TDesC& aContextType,
         TSecurityPolicy& aPolicy ) = 0;

		
        /**
         * Defines a new context in Context Framework.
         * Every context needs to be defined before it can be published,
         * subscribed or requested.
         *
         * Security policy for reading and writing are different.
         * 
         * If some client subscribes for this context, the contex owner
         * will be notified through MCFContextSource interface.
         *
         * @since S60 5.0
         * @param aContextSource Source of the context.
         * @param aContextType Type of the context.
         * @param aReadWritePolicy Needed capabilities for reading and writing
         *		of the context.
         * @param aPublisher Publisher listening subscibers/nosubscribers
         * messages. Can be also NULL.
         * @param aPublisherUid UID of the publisher process
         * @return Possible return values:
         * - KErrNone, no errors, also if context exists but the same
         * publisher (same publisherUID) redefines it.
         * - KErrAlreadyExists, defined context already exists.
         */
         virtual TInt DefineContext( const TDesC& aContextSource,
             const TDesC& aContextType,
             const TSecurityPolicy& aReadPolicy,
             const TSecurityPolicy& aWritePolicy,
             MCFContextSource* aPublisher, const TUid& aPublisherUid ) = 0;
         
         /**
          * Remove all subscriptions for the particular listener.
  		  *
  		  * @since S60 5.0
          * @param aObserver Listener whose subscriptions will be removed.
          * @return None
          */
         virtual void DeregisterPublisher(
        		 MCFContextSource& aPublisher ) = 0; 

        /**
        * Installs a context source setting.
        * Setting is stored if install succeeds.
        *
        * @since S60 5.0
        * @param aSettingFile Open file handle to the setting.
        * @param aContextSourceUid Implementation UID of the context source to
        *   receive setting.
        * @param aClientThread Client thread making the request.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotSupported, context source does not support the settings.
        * - KErrBadHandle, context source not found/loaded.
        * - KErrPermissionDenied, client has insufficient capabilities to
        *   install the settings to the context source.
        * - KErrExtensionNotSupported, context source does not support
        *   installing settings.
        * - Otherwise system-wide error code.
        */
        virtual TInt InstallContextSourceSetting( RFile& aSettingFile,
                const TUid& aContextSourceUid,
                RThread& aClientThread ) = 0;

        /**
        * Uninstalls a context source setting.
        * Setting is removed if uninstall succeeds.
        *
        * @since S60 5.0
        * @param aSettingFilename Filename (without path) that was used to
        *   install the setting.
        * @param aContextSourceUid Implementation UID of the context source.
        * @param aClientThread Client thread making the request.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, specified setting file not found.
        * - KErrBadHandle, context source not found/loaded.
        * - KErrPathNotFound, specified context source does not have any
        *   settings.
        * - KErrExtensionNotSupported, context source does not support
        *   uninstalling settings.
        * - Otherwise system-wide error code.
        */
        virtual TInt UninstallContextSourceSetting(
                const TDesC& aSettingFilename,
                const TUid& aContextSourceUid,
                RThread& aClientThread ) = 0;

        /**
        * Uninstalls context source settings registered by a client.
        * Uninstalls all settings ever registered by the client for the context
        * source. Settings are removed if uninstall succeeds.
        *
        * @since S60 5.0
        * @param aContextSourceUid Implementation UID of the context source.
        * @param aClientThread Client thread making the request.
        * @return Possible return values:
        * - KErrNone, no errors.
        * - KErrNotFound, specified client has not installed any setting files.
        * - KErrBadHandle, context source not found/loaded.
        * - KErrPathNotFound, specified context source does not have any
        *   settings.
        * - KErrExtensionNotSupported, context source does not support
        *   uninstalling settings.
        * - Otherwise system-wide error code.
        */
        virtual TInt UninstallContextSourceSettings(
                const TUid& aContextSourceUid,
                RThread& aClientThread ) = 0;

        /**
        * Publishes a new value of a context into Context Framework.
        * Subscribed clients will be notified of the change.
        * Context can be auto-defined if it has not been defined before.
        *
        * @since S60 5.0
        * @param aContext Context object to be added.
        * @param aAutoDefine Automatically define context.
        * @return Possible return values:
        * - KErrNone, no errors
        * - KErrAccessDenied, insufficient capabilities for the context
        * - KErrArgument if context source, type and value not defined
        */
        virtual TInt PublishContextFromAction( CCFContextObject& aContext,
            TBool aAutoDefine ) = 0;

    protected:
    
	    // Deny destruction through this class
	    virtual ~MCFExtendedContextInterface(){};		
		
    };

#endif
