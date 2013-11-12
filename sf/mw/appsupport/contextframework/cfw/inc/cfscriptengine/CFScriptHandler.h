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
* Description:  CCFScriptHandler class declaration.
*
*/


#ifndef C_CFSCRIPTHANDLER_H
#define C_CFSCRIPTHANDLER_H

#include <f32file.h>
#include <badesca.h>
#include <gmxmlparser.h>

#include "CFScriptEngineInterface.h"
#include "cfstarterobserver.h"
#include "CFSecurityChecker.h"
#include "cfstateobserver.h"
#include "cfscriptinfo.h"
#include "cfstartereventhandler.h"

class MCFActionHandler;
class MCFExtendedContextInterface;
class CCFScript;
class CCFOperationPluginManager;

/**
 * ScriptHandler implementing the main public interface for ScriptEngine
 * functionality.
 *
 * @lib CFScriptEngine
 * @since Series 60 2.6
 */
NONSHARABLE_CLASS( CCFScriptHandler ):
public CBase,
public MCFScriptEngineInterface,
public MCFStarterObserver,
public MMDXMLParserObserver,
public MMDXMLParserDataProvider,
public MCFStateObserver
    {
public: // Constructors and destructor

    IMPORT_C static CCFScriptHandler* NewL(
        MCFExtendedContextInterface& aCF,
        RFs& aFs,
        MCFActionHandler& aScriptEventListener,
        MCFSecurityChecker& aSecurityChecker );
    IMPORT_C static CCFScriptHandler* NewLC(
        MCFExtendedContextInterface& aCF,
        RFs& aFs,
        MCFActionHandler& aScriptEventListener,
        MCFSecurityChecker& aSecurityChecker );

    IMPORT_C ~CCFScriptHandler();

public: // From base classes

    // @see MCFScriptEngineInterface
    TInt RemoveScriptById( TInt aScriptId, const RThread& aOwner );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptsBySession( const MCFActionHandler& aSession );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptByName( const TDesC& aScriptName,
        const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    TInt RemoveScriptByUid( const TUid& aUid );

    // @see MCFScriptEngineInterface
    TInt AddScript( const TDesC& aName,
        const TDesC8& aScript,
        const TUid& aOwner,
        const RThread& aOwnerThread,
        MCFActionHandler& aActionHandler,
        MCFScriptOwner* aScriptOwner );

    // @see MCFScriptEngineInterface
    TInt NumberOfScriptsByOwner( const TUid& aOwner );

    // @see MCFScriptEngineInterface
    TInt NumberOfScripts();

    // @see MCFScriptEngineInterface
    TInt ScriptLength( TInt aScriptId );

    // @see MCFScriptEngineInterface
    const RArray<TInt>& GetEveryScriptId();

    // @see MCFScriptEngineInterface
    const RArray<TInt>& GetEveryScriptIdByOwner(
        const TUid& aScriptOwner );

    // @see MCFScriptEngineInterface
    TInt UpdateScript( TInt aScriptID,
        const RThread& aOwnerThread,
        const TDesC8& aUpdatedScript,
        MCFScriptOwner* aScriptOwner );

    // @see MCFScriptEngineInterface
    TInt SaveScript( const TDesC8& aScript, TInt aScriptId,
        const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    TInt DeleteScriptByName( const TDesC& aScriptName,
        const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    TInt DeleteScriptByUid( const TUid& aUid );

    // @see MCFScriptEngineInterface
    TBool AlreadyExists( const TDesC& aScriptName,
        const TUid& aOwnerUid,
        TInt& aScriptId ) const;

    // @see MCFScriptEngineInterface
    void CleanupPersistentDataByName( const TDesC& aScriptName,
        const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    void CleanupPersistentDataByUid( const TUid& aOwnerUid );

    // @see MCFScriptEngineInterface
    TInt RestoreRomScript( TInt aScriptId, const TUid& aOwnerUid,
        const RThread& aClient );

    // @see MCFScriptEngineInterface
    TInt IsUpgradeAllowed( const TDesC& aName,
        const TDesC8& aScript,
        const TUid& aOwner,
        const RThread& aOwnerThread,
        MCFActionHandler& aActionHandler );
        
    // @see MCFScriptEngineInterface
    TInt RemoveScriptByProviderUid( const TUid& aProviderUid, TBool aRollback );
    
    // @see MCFScriptEngineInterface
    void RollbackScripts();
    
    // @see MCFScriptEngineInterface
    void DeregisterScriptOwner( MCFScriptOwner* aScriptOwner );

    // @see MCFStarterObserver
    void InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhaseId );

    // @see MMDXMLParserObserver
    void ParseFileCompleteL();

    // @see MMDXMLParserDataProvider
    void GetData( TPtrC8& aPtr, TRequestStatus& aStatus );

    // @see MMDXMLParserDataProvider
    void Disconnect();

    // from MCFStateObserver
    void UpdatePlugInsL();

        // @see MCFStarterObserver
        void SetEventHandler( MCFStarterEventHandler& /*aEventHandler*/ );

private:

    CCFScriptHandler( MCFExtendedContextInterface& aCF,
        RFs& aFs,
        MCFActionHandler& aScriptEventListener,
        MCFSecurityChecker& aSecurityChecker );
    void ConstructL();

private: // New functions

    /**
     * This method does actual adding to file store and calls
     * AddScriptToArrayL
     * to add script to internal array. It may leave.
     * @since Series 60 2.6
     * @param aName is script's name, filename or client thread's secure id.
     * @param aScript is a new script to be added.
     * @param aOwner is the owner's unique Uid.
     * @param aOwnerThread Thread of the owner process.
     * @param aActionHandler Action handler.
     * @param aDoSecurityCheck Defines if a security check is needed.
     * @param aScriptId If set then this same script id is given to the script.
     * @return script Id given to the script by the handler.
     * 	Returned value is > 0, otherwise error occurred.
     */
    TInt AddScriptL( const TDesC& aName,
        const TDesC8& aScript,
        const TUid& aOwner,
        const RThread& aOwnerThread,
        MCFActionHandler& aActionHandler,
        TBool aDoSecurityCheck,
        MCFScriptOwner* aScriptOwner,
        TInt aScriptId = KErrNotFound );

    // Initilizes device starting phase
    void InitDeviceStartingPhaseL();

    // Initializes device started phase
    void InitDeviceStartedPhaseL();

    // Complete file path
    void CompleteFilePath( TDes& aFileName );

    // Loads script from file
    HBufC8* LoadScriptFromFile( const TDesC& aFilePath );

    // Get scripts by Uid
    void GetScriptsByUid( const TUid& aUid,
        RPointerArray<CCFScript>& aArray ) const;

    // Create a ram file path for a script
    HBufC* ScriptFilePath( CCFScriptInfo& aInfo ) const;

    // Create a ram file path for a script
    HBufC* ScriptFilePath( const TDesC& aName, const TUid& aOwnerUid ) const;

    // Create a ram file path for a script
    void ScriptFilePath( CCFScriptInfo& aInfo, TDes& aFile ) const;

    // Saves a script
    TInt DoSave( const TDesC& aFileName, const TDesC8& aScript );
    
    // Loads a script
    TInt DoLoad( const TDesC& aFilePath );
    
    // Get script uid and name from file path
    void ParseUidAndName( const TDesC& aFilePath,
        TUid& aUid, TPtrC& aName ) const;
    
    // Create a new script object
    CCFScript* CreateScriptL( const TDesC& aName,
        const TDesC8& aScript,
        const TUid& aOwner,
        const RThread& aOwnerThread,
        MCFActionHandler& aActionHandler,
        TBool aDoSecurityCheck,
        MCFScriptOwner* aScriptOwner,
        TInt aScriptId );
        
    // Rollback a script based on existing script info
    void RollbackScriptL( const CCFScriptInfo& aInfo, const TDesC& aFilePath );
    
    // Resolves script ids by owner session
    void NotifyScriptIds( MCFScriptOwner* aScriptOwner );
    
    // Adds a script information in rollback array
    void AddRollbackInfoL( CCFScript& aScript );
    
    // Deletes a script file from system drive
    TInt DeleteScriptFile( const TDesC& aFilePath, const TUid& aOwnerUid );

private: // Data

    enum EInternalParserDataProvidingState
        {
        KInit = 0,
        KDataSent,
        KDone,
        KError
        };

    // Internal state of the parser data providing.
    EInternalParserDataProvidingState iParserDataProvidingState;

    // unique Id assigned to new script
    TInt iNextId;

    // Own: Scripts
    RPointerArray<CCFScript> iScripts;

    // Own: Script ids
    RArray<TInt> iScriptIds;

    // Loaded scripts in starting phase
    RPointerArray<CCFScript> iLoadedScripts;

    // XML DOM parser.
    CMDXMLParser* iParser;

    // Data provider's data for the parser that is valid during AddScriptL.
    TPtrC8 iParserData;

    // Wait for DOM parser to complete.
    CActiveSchedulerWait* iWaitParsing;

    // Ref: Context manager
    MCFExtendedContextInterface& iCF;

    // Ref: File server session
    RFs& iFs;

    // Ref: Script event listener
    MCFActionHandler& iScriptEventListener;

    // Ref: Security checker
    MCFSecurityChecker& iSecurityChecker;

    // Own: Operation plugin manager
    CCFOperationPluginManager* iOperationPluginManager;

    // Default Rom drive letter
    TChar iDefaultRomDrive;

    // Default system drive
    TChar iDefaultSystemDrive;
    
    // Script which is being updated
    CCFScript* iUpdatedScript;
    
    // Script info array for rollback
    RScriptInfoArray iRollbackList;
    };

#endif // C_CFSCRIPTHANDLER_H

