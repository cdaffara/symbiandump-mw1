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


#ifndef __UT_CFACTIVATORENGINE_H__
#define __UT_CFACTIVATORENGINE_H__

//  EXTERNAL INCLUDES
#include <ecom/ImplementationInformation.h>
#include <CEUnitTestSuiteClass.h>

//  INTERNAL INCLUDES
#include "cfactionsubscriptionlistener.h"
#include "cfactionowner.h"
#include "cfactioncacheelement.h"

//  FORWARD DECLARATIONS
class CCFActivatorTimedSchedulerWait;
class CCFActionPlugInImpl;
class CCFActionPlugIn;
class CCFActivatorEngineActionPluginManager;
class CCFActivator;
class CCFScriptEvent;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
#define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CFActivatorEngine ): public CEUnitTestSuiteClass,
public MCFActionSubscriptionListener,
public MCFActionOwner
    {
public: // Constructors and destructors

    /**
     * Two phase construction
     */
    static UT_CFActivatorEngine* NewL();
    static UT_CFActivatorEngine* NewLC();
    /**
     * Destructor
     */
    ~UT_CFActivatorEngine();

private: // Constructors and destructors

    UT_CFActivatorEngine();
    void ConstructL();

public: // From observer interface

    // @see MCFActionSubscriptionListener
    void ActionIndicationL( CCFActionIndication* aIndication );
    
    // @see MCFActionOwner
    TCFActionOwner ActionOwner() const;

private: // New methods - only public methods tested

    static TInt CallBack( TAny* aSelf );
    void Wait( TTimeIntervalMicroSeconds32& aDelay );

    void SetupL();
    void SetupEmptyL();
    void Teardown();
    void TeardownDelayed();

    void UT_CCFActionCacheElement_NewLL();
    void UT_CCFActionCacheElement_NewLCL();
    void UT_CCFActionCacheElement_CompareByIdentifierL();
    void UT_CCFActionCacheElement_AddSubscriptionLL();
    void UT_CCFActionCacheElement_SecurityPolicyL();
    void UT_CCFActionCacheElement_RemoveSubscriptionL();

    void UT_CCFActionPlugInImpl_NewLL();
    void UT_CCFActionPlugInImpl_NewLCL();

    void UT_CCFActivator_NewLL();

    void UT_CCFActivatorEngineActionPluginManager_NewLL();
    void UT_CCFActivatorEngineActionPluginManager_NewLCL();
    
    void PluginThreadCreationFailureL();
    
    void UT_CCFActivatorEngineActionPluginManager_TriggerLL();
    void UT_CCFActivatorEngineActionPluginManager_InitializePhaseLL();

    void UT_CCFActivatorTimedSchedulerWait_NewLL();
    void UT_CCFActivatorTimedSchedulerWait_NewLCL();
    void UT_CCFActivatorTimedSchedulerWait_StartL();
    void UT_CCFActivatorTimedSchedulerWait_StopL();

private: // Data

    EUNIT_DECLARE_TEST_TABLE;

    CCFActivatorEngineActionPluginManager* iActionPluginManager;
    CCFActivatorTimedSchedulerWait* iTimedWait;

    CActiveSchedulerWait* iWait;
    CPeriodic* iPeriodic;
    TRequestStatus* iStatus;
    CCFScriptEvent* iEvent;
    RActionCacheArray iActionCache;
    TUid iOwnerUid;
    };

#endif      //  __UT_CFACTIVATORENGINE_H__

// End of file
