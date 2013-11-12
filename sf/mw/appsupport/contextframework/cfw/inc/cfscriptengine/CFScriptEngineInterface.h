/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the Script Engine API. API is for Script Engine
*                internal use only.
*
*/


#ifndef M_CFSCRIPTENGINEINTERFACE_H
#define M_CFSCRIPTENGINEINTERFACE_H

#include <e32base.h>

class MCFActionHandler;
class MCFScriptOwner;

/**
 *  MCFScriptEngineInterface defines the API of the Script Engine component.
 *
 *  @lib CFScriptEngine.lib
 *  @since Series 60 2.6
 */
class MCFScriptEngineInterface
    {
public:
    // New functions

    /**
     * Removes a script by its Id.
     * Only owners of the script can remove the script.
     * @since Series 60 2.6
     * @param aScriptId is id of the script to remove.
     * @param aOwner Owner thread of the script.
     * @return KErrNone if successful.
     */
    virtual TInt RemoveScriptById( TInt aScriptId, const RThread& aOwner ) = 0;

    /**
     * Removes all scripts that are added by the session aSession.
     * @since Series 60 2.6
     * @return number of scripts removed.
     */
    virtual TInt RemoveScriptsBySession(const MCFActionHandler& aSession ) = 0;

    /**
     * Remove script by script name.
     * 
     * @since S60 5.0
     * @param aScriptName Script name.
     * @param aOwner Owner thread of the script.
     * @return KErrNone if no errors.
     */
    virtual TInt RemoveScriptByName( const TDesC& aScriptName,
        const TUid& aOwnerUid ) = 0;

    /**
     * Remove scripts by the script owner Uid (SID).
     * 
     * @since S60 5.0
     * @param aUid Owner Uid.
     * @return KErrNone if no errors.
     */
    virtual TInt RemoveScriptByUid( const TUid& aUid ) = 0;

    /**
     * Adds a new script to the script engine, activating the script at the
     * same time.
     * @since Series 60 2.6
     * @param aName Script's name, filename or secure id from client thread.
     * @param aScript is a new script to be added.
     * @param aOwner is the owner's unique Uid.
     * @param aOwnerThread The thread adding the script, used for script
     * security checking.
     * @param aSession is a session that is notified when the script fires.
     * @param aScriptOwner Pointer to the script owner session.
     * @return script id given to the script by the handler.
     * 	Returned value is < 0 if aScript could not be parsed.
     */
    virtual TInt AddScript( const TDesC& aName, const TDesC8& aScript,
        const TUid& aOwner, const RThread& aOwnerThread,
        MCFActionHandler& aSession, MCFScriptOwner* aScriptOwner ) = 0;

    /**
     * Number of scripts by specified owner.
     * @since Series 60 2.6
     * @param aOwner script owner's Uid.
     * @return integer indicating number of scripts by the passed owner.
     */
    virtual TInt NumberOfScriptsByOwner( const TUid& aOwner ) = 0;

    /**
     * @since Series 60 2.6
     * @return total number of script owned.
     */
    virtual TInt NumberOfScripts( ) = 0;

    /**
     * Method for getting the length of the script descriptor.
     * @since Series 60 2.6
     * @param aScriptId script's unique Id number.
     * @return the length of the script (KErrNotFound if script not found).
     */
    virtual TInt ScriptLength( TInt aScriptId ) = 0;

    /**
     * Method for getting all script Ids to a descriptor.
     * @since Series 60 2.6
     * @return array containing all the script Ids.
     */
    virtual const RArray< TInt >& GetEveryScriptId( ) = 0;

    /**
     * Method for getting all script Ids to a descriptor according to given owner.
     * @since Series 60 2.6
     * @param aScriptOwner TUid of the owner of the scripts.
     * @return array containing all the script Ids.
     */
    virtual const RArray< TInt >& GetEveryScriptIdByOwner(
        const TUid& aScriptOwner ) = 0;

    /**
     * Method for updating (by replacing) a script.
     * @since Series 60 2.6
     * @param aScriptId Id of the script to be updated.
     * @param aUpdatedScript descriptor containing the new script.
     * @return KErrnone if successful, system wide error code otherwise.
     */
    virtual TInt UpdateScript( TInt aScriptID, const RThread& aOwnerThread,
        const TDesC8& aUpdatedScript, MCFScriptOwner* aScriptOwner ) = 0;

    /**
     * Save script in to disk.
     * 
     * @since S60 5.0
     * @param aScript Script to save.
     * @param aScriptId Id of the script where the data belongs to.
     * @param aOwnerUid Owner of the script.
     * @return KErrNone if no errors.
     */
    virtual TInt SaveScript( const TDesC8& aScript, TInt aScriptId,
        const TUid& aOwnerUid ) = 0;

    /**
     * Delete script by its name.
     * 
     * @since S60 5.0
     * @param aScriptName Name of the script.
     * @param aOwnerUid Owner of the script.
     * @return KErrNone if no errors.
     */
    virtual TInt DeleteScriptByName( const TDesC& aScriptName,
        const TUid& aOwnerUid ) = 0;

    /**
     * Delete scripts by owner uid.
     * 
     * @since S60 5.0
     * @param aUid Owner's uid.
     * @return KErrNone if no errors.
     */
    virtual TInt DeleteScriptByUid( const TUid& aUid ) = 0;

    /**
     * Check if the script name is already reserved.
     * 
     * @since S60 5.0
     * @param aScriptName Name of the script.
     * @param aOwnerUid Uid of the script owner.
     * @param aScriptId Id of the script found.
     * @return ETrue if the name is reserved.
     */
    virtual TBool AlreadyExists( const TDesC& aScriptName,
        const TUid& aOwnerUid, TInt& aScriptId ) const = 0;

    /**
     * Cleanup persistent data from a script identified by name.
     * 
     * @since S60 5.0
     * @param aScriptName Name of the script.
     * @param aOwnerUid Owner of the scripts.
     * @return KErrNone if no errors.
     */
    virtual void CleanupPersistentDataByName( const TDesC& aScriptName,
        const TUid& aOwnerUid ) = 0;

    /**
     * Cleanup persistent data from all scripts identified by owner uid.
     * 
     * @since S60 5.0
     * @param aOwnerUid Owner of the scripts.
     * @return KErrNone if no errors.
     */
    virtual void CleanupPersistentDataByUid( const TUid& aOwnerUid ) = 0;

    /**
     * Restore upgraded rom based script with the orginal script found
     * in rom.
     * 
     * @since S60 5.0
     * @param aScriptId Script id of the existing script.
     * @param aOwnerUid Owner of the scripts.
     * @param aClient Client thread for security checking.
     * @return KErrNone if no errors.
     */
    virtual TInt RestoreRomScript( TInt aScriptId, const TUid& aOwnerUid,
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
    virtual TInt IsUpgradeAllowed( const TDesC& aName, const TDesC8& aScript,
        const TUid& aOwner, const RThread& aOwnerThread,
        MCFActionHandler& aActionHandler ) = 0;

    /**
     * Removes scripts which provider has been unsintalled.
     * The script information can be stored so that the deregistered
     * scripts can be reloaded after wards.
     * @param aProviderUid Operation plug-in implementation uid.
     * @param aRollback Enable rollback.
     * @return KErrNone if no errors.
     */
    virtual TInt RemoveScriptByProviderUid( const TUid& aProviderUid,
        TBool aRollback ) = 0;

    /**
     * Restores scripts which have been stored after removing scripts by
     * provider uid.
     */
    virtual void RollbackScripts() = 0;
    
    /**
     * Deregister script owner.
     * @param aScriptOwner Pointer to the script owner.
     */
    virtual void DeregisterScriptOwner( MCFScriptOwner* aScriptOwner ) = 0;
    };

#endif // M_CFSCRIPTENGINEINTERFACE_H

