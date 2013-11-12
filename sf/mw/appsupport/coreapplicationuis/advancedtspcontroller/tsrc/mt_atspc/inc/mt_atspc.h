/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_ATSPC_H__
#define __MT_ATSPC_H__

// INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <remcon/remcontargetselectorpluginobserver.h>
#include <cflistener.h>


class CRemConTargetSelectorPlugin;
class MRemConTargetSelectorPluginInterfaceV2;

// CLASS DEFINITION
/**
 * Generated EUnit test suite class.
 */
NONSHARABLE_CLASS( mt_atspc )
	: public CEUnitTestSuiteClass,
	  public MRemConTargetSelectorPluginObserver,
	  public MCFListener
    {
    public:  // Constructors and destructor

        static mt_atspc* NewL();
        static mt_atspc* NewLC();
        ~mt_atspc();

// from MRemConTargetSelectorPluginObserver
        void OutgoingCommandAddressed(TInt aError) {}
        void OutgoingCommandPermitted(TBool aIsPermitted) {}
        void IncomingCommandAddressed(TInt aError) {}
        void OutgoingResponsePermitted(TBool aIsPermitted) { iObserverResponse = aIsPermitted; }
        void IncomingNotifyAddressed(TClientInfo* aClientInfo, TInt aError) { iObserverResponse = aError; }
        void ReAddressNotifies() {}
        TInt GetConnections(TSglQue<TRemConAddress>& aConnections) { return 0; }

    private: // Constructors

        mt_atspc();
        void ConstructL();

    private: // New methods

         void SetupL();
        
         void Teardown();
        
         void T_TestAddressIncomingCommandCoreApiL();
         void T_TestAddressIncomingCommandSynchApiL();
         void T_TestAddressIncomingCommandPocApiL();
         void T_TestAddressIncomingCommandCallHandlingApiL();
         void T_TestAddressOutgoingCommandL();
         void T_TestAddressIncomingNotifyL();
         void T_TestPermitOutgoingCommandL();
         void T_TestCancelOutgoingCommandL();
         void T_TestPermitOutgoingResponseL();
         void T_TestCancelOutgoingResponseL();
        
         virtual void MrctspoDoOutgoingCommandAddressed(TInt aError) { iObserverResponse = aError; }
         virtual void MrctspoDoOutgoingCommandPermitted(TBool /*aIsPermitted*/) {}
         virtual void MrctspoDoIncomingCommandAddressed(TInt aError ) { iObserverResponse = aError; }
         virtual TInt MrctspoDoGetConnections(TSglQue<TRemConAddress>& /*aConnections*/) { return 0; }
         virtual void MrctspoDoOutgoingResponsePermitted(TBool /*aIsPermitted*/) {}
         virtual void MrctspoDoIncomingNotifyAddressed(TClientInfo* /*aClientInfo*/, TInt aError ) { iObserverResponse = aError; }
         virtual void MrctspoDoReAddressNotifies() {}

public: // from MCFListener         
    void ContextIndicationL( const CCFContextIndication& aChangedContext ) {}
    void ActionIndicationL( const CCFActionIndication& aActionToExecute ) {}
    void HandleContextFrameworkError( TCFError aError, const TDesC& aSource, const TDesC& aType ) {}
    TAny* Extension( const TUid& aExtensionUid ) const { return NULL; }

private: // Data
        CRemConTargetSelectorPlugin* iAtspc;

        TInt iObserverResponse;
        
        MRemConTargetSelectorPluginInterfaceV2* iTspIf2;
        
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __MT_ATSPC_H__
