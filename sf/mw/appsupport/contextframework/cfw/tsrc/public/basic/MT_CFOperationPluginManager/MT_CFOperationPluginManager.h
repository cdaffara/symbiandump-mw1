/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:
*
*/


#ifndef __MT_CFOPERATIONPLUGINMANAGER_H__
#define __MT_CFOPERATIONPLUGINMANAGER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include "cfoperationpluginmanager.h"

//  INTERNAL INCLUDES
#include "cfscriptengineinterface.h"

//  FORWARD DECLARATIONS

//  CLASS DEFINITION
/**
 * Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( MT_CFOperationPluginManager ):
public CEUnitTestSuiteClass,
public MCFScriptEngineInterface
    {
public: // Constructors and destructors

    /**
     * Two phase construction
     */
    static MT_CFOperationPluginManager* NewL();
    static MT_CFOperationPluginManager* NewLC();
    /**
     * Destructor
     */
    ~MT_CFOperationPluginManager();

private: // From MCFScriptEngineInterface

    // @see MCFScriptEngineInterface
    TInt RemoveScriptById( TInt /*aScriptId*/, 
        const RThread& /*aOwner */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt RemoveScriptsBySession( const MCFActionHandler& /*aSession */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt RemoveScriptByName( const TDesC& /*aScriptName*/,
        const TUid& /*aOwnerUid */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt RemoveScriptByUid( const TUid& /*aUid */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt AddScript( const TDesC& /*aName*/,
        const TDesC8& /*aScript*/,
        const TUid& /*aOwner*/,
        const RThread& /*aOwnerThread*/,
        MCFActionHandler& /*aSession */,
        MCFScriptOwner* /*aScriptOwner*/ )
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt NumberOfScriptsByOwner( const TUid& /*aOwner */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt NumberOfScripts()
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt ScriptLength( TInt /*aScriptId */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    const RArray< TInt >& GetEveryScriptId()
        {
        return iArray;
        };
        
    // @see MCFScriptEngineInterface
    const RArray< TInt >& GetEveryScriptIdByOwner( const TUid& /*aScriptOwner */)
        {
        return iArray;
        };
    
    // @see MCFScriptEngineInterface
    TInt UpdateScript( TInt /*aScriptID*/,
        const RThread& /*aOwnerThread*/,
        const TDesC8& /*aUpdatedScript*/,
        MCFScriptOwner* /*aScriptOwner*/ )
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt SaveScript( const TDesC8& /*aScript*/, TInt /*aScriptId*/,
        const TUid& /*aOwnerUid */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt DeleteScriptByName( const TDesC& /*aScriptName*/,
        const TUid& /*aOwnerUid */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt DeleteScriptByUid( const TUid& /*aUid */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TBool AlreadyExists( const TDesC& /*aScriptName*/,
        const TUid& /*aOwnerUid*/,
        TInt& /*aScriptId */) const
        {
        return EFalse;
        };
        
    // @see MCFScriptEngineInterface
    void CleanupPersistentDataByName( const TDesC& /*aScriptName*/,
        const TUid& /*aOwnerUid */)
        {
        
        };
    
    // @see MCFScriptEngineInterface
    void CleanupPersistentDataByUid( const TUid& /*aOwnerUid */)
        {
        
        };
    
    // @see MCFScriptEngineInterface
    TInt RestoreRomScript( TInt /*aScriptId*/,
        const TUid& /*aOwnerUid*/,
        const RThread& /*aClient */)
        {
        return KErrNone;
        };
    
    // @see MCFScriptEngineInterface
    TInt IsUpgradeAllowed( const TDesC& /*aName*/,
        const TDesC8& /*aScript*/,
        const TUid& /*aOwner*/,
        const RThread& /*aOwnerThread*/,
        MCFActionHandler& /*aActionHandler */)
        {
        return KErrNone;
        };

    // @see MCFScriptEngineInterface
    TInt RemoveScriptByProviderUid( const TUid& /*aProviderUid*/,
        TBool /*aRollback*/ )
        {
        return KErrNone;
        };

    // @see MCFScriptEngineInterface
    void RollbackScripts()
        {
        
        };

    // @see MCFScriptEngineInterface
    void DeregisterScriptOwner( MCFScriptOwner* /*aScriptOwner */)
        {

        };
    
private: // Constructors and destructors

    MT_CFOperationPluginManager();
    void ConstructL();

private: // New methods

    void SetupL();

    void SetupEmptyL();

    void Teardown();

    void T_CCFOperationPluginManager_NewLL();

    void T_CCFOperationPluginManager_NewLCL();

    void T_CCFOperationPluginManager_InitializePhaseL1L();

    void T_CCFOperationPluginManager_InitializePhaseL2L();

private: // Data

    CCFOperationPluginManager* iCCFOperationPluginManager;
    RArray<TInt> iArray;
    EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __MT_CFOPERATIONPLUGINMANAGER_H__

// End of file
