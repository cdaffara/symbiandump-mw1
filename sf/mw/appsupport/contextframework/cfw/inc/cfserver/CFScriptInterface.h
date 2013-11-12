/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Script interface in CF.
*
*/


#ifndef M_CFSCRIPTINTERFACE_H
#define M_CFSCRIPTINTERFACE_H

class CCFActionSubscription;
class MCFActionSubscriptionListener;
class MCFScriptOwner;
class RThread;

/**
* Interface for registering scripts.
*/
class MCFScriptInterface
    {
    public:
        
        /**
        * Registers a script.
        * Possible error codes:
        * - KErrNotFound, specified context or action in script not defined
        * - KErrAccessDenied, not enough capabilities for context or action
        *
        * @since S60 5.0
        * @param aName Script's name, filename or secure id from client thread.
        * @param aScript Script.
        * @param aScriptId Reference to script ID return parameter.
        * @param aClientThread Client thread.
        * @param aOwner Script owner session.
        * @return KErrNone if no errors.
        */
        virtual TInt RegisterScript(
            const TDesC& aName,
            const TDesC8& aScript,
            TInt& aScriptId,
            const RThread& aClientThread,
            MCFScriptOwner& aOwner ) = 0;
        
        /**
         * Deregister a script.
         *
         * @since S60 5.0
         * @param aScriptId ID of the script to be removed.
         * @param aClientThread Client thread which registered the script.
         * @return KErrNone if no errors.
         */
         virtual TInt DeregisterScript( TInt aScriptId,
             const RThread& aClientThread ) = 0;
         
         /**
          * Deregister a script.
          *
          * @since S60 5.0
          * @param aScriptName Name of the script to be removed.
          * @param aClientThread Client thread which registered the script.
          * @return KErrNone if no errors.
          */
          virtual TInt DeregisterScript( const TDesC& aScriptName,
            const RThread& aClientThread ) = 0;
          
          /**
           * Deregister a scripts from a particular Sid.
           *
           * @since S60 5.0
           * @param aClientThread Client thread which registered the script.
           * @return KErrNone if no errors.
           */
          virtual TInt DeregisterScripts( const RThread& aClientThread ) = 0;
          
          /**
           * Deregister scripts by script session. This is used when the client
           * session is closed.
           * @param aOwner Script owner session being closed.
           */
          virtual void DeregisterScriptOwner( MCFScriptOwner& aOwner ) = 0;
           
           /**
            * Save script into disk.
            * 
            * @since S60 5.0
            * @param aScript Script content to save
            * @param aScriptId Id of the script which data is stored.
            * @param aClient Client thread.
            * @return KErrNone if no errors.
            */
          virtual TInt SaveScript( const TDesC8& aScript,
              TInt aScriptId,
              const RThread& aClient ) = 0;
               
           /**
            * Delete script from disk.
            * 
            * @since S60 5.0
            * @param aScriptName Script name to delete.
            * @param aClientThread Client thread which registered the script.
            * @return KErrNone if no errors.
            */
          virtual TInt DeleteScript( const TDesC& aScriptName,
               RThread& aClientThread ) = 0;
               
           /**
            * Delete scripts from disk from a particular Sid.
            * 
            * @since S60 5.0
            * @param aScriptName Script name to delete.
            * @param aClientThread Client thread which registered the script.
            * @return KErrNone if no errors.
            */
          virtual TInt DeleteScripts( RThread& aClientThread ) = 0;
          
          /**
           * Check if the script name is already reserved.
           * 
           * @since S60 5.0
           * @param aScriptName Name of the script.
           * @param aClient Client process thread.
           * @param aScriptId Id of the script found.
           * @return ETrue if the name is reserved.
           */
          virtual TBool AlreadyExists( const TDesC& aScriptName,
              const RThread& aClient,
              TInt& aScriptId ) const = 0;
          
          /**
           * Cleanup persistent data from a script identified by name.
           * 
           * @since S60 5.0
           * @param aScriptName Name of the script.
           * @param aClient Owner of the scripts.
           * @return KErrNone if no errors.
           */
          virtual void CleanupPersistentDataByName( const TDesC& aScriptName,
              const RThread& aClientThread ) = 0;

          /**
           * Cleanup persistent data from all scripts identified by owner uid.
           * 
           * @since S60 5.0
           * @param aClient Owner of the scripts.
           * @return KErrNone if no errors.
           */
          virtual void CleanupPersistentDataByUid( const RThread& aClient ) = 0;

          /**
           * Method for updating (by replacing) a script.
           * 
           * @since Series 60 5.0
           * @param aScriptId Id of the script to be updated.
           * @param aUpdatedScript descriptor containing the new script.
           * @param aOwner Script owner session.
           * @return KErrnone if successful, system wide error code otherwise.
           */
           virtual TInt UpdateScript( TInt aScriptID,
               const RThread& aOwnerThread,
               const TDesC8& aUpdatedScript,
               MCFScriptOwner& aOwner ) = 0;
           
           /**
            * Restore upgraded rom based script with the orginal script found
            * in rom.
            * 
            * @since S60 5.0
            * @param aScriptId Script id of the existing script.
            * @return KErrNone if no errors.
            */
           virtual TInt RestoreRomScript( TInt aScriptId,
               const RThread& aClient ) = 0;

           /**
            * Parses the script and checks that client is allowed to upgrade
            * rom based script. The upgrade must have at least the same
            * upgrade capability level and the client must have the same
            * capabilities.
            * @param aScript The upgrade script.
            * @param aClient Client process thread.
            * @return KErrNone if upgrade is allowed.
            */
           virtual TInt IsUpgradeAllowed( const TDesC& aName,
               const TDesC8& aScript,
               const RThread& aOwnerThread ) = 0;
           
    protected:
    
        // Deny destruction through this class
        virtual ~MCFScriptInterface(){};
    };

#endif //M_CFSCRIPTINTERFACE_H
