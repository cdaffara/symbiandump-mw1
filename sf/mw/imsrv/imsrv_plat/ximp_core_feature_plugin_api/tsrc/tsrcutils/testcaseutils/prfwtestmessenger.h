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


#ifndef CPRFWTESTMESSENGER_H 
#define CPRFWTESTMESSENGER_H

// INCLUDES
#include <e32def.h>
#include <e32base.h>
#include <e32property.h>
#include "prfwtestmessaging.h"

// CLASS DESCRIPTION

class TXIMPTestMsg;
class MXIMPRestrictedObjectCollection;

/**
 * Asynchronous protocol for PRFW testing.
 * Implementation class and helper routines.
 * Done using Publish and Subscribe, although it's used as a collection of
 * "flags".
 *
 * RMsgQueue is not good, because we do not know the length of the array in
 * advance: there is often 1 in the array, but there could be 2 or 3 or more.
 * Each time someone writes, the array should be read. This becomes a bit
 * problematic, an easier and more robust way is to use pubsub.
 *
 * The descriptions of API methods rely on the protocol implementation, thus
 * the descriptions are meant to document what kind of effects the clients and
 * users of this class expect this class to cause.
 */
class CXIMPTestMessenger : public CBase
    {
    public:

        /**
         * @param aMsgQName The name for the queue
         */
        IMPORT_C static CXIMPTestMessenger* NewForClientSideL( TInt aId );
        IMPORT_C static CXIMPTestMessenger* NewForAdaptationSideL( TInt aId );
        ~CXIMPTestMessenger();

    private:

        CXIMPTestMessenger( TInt aId );
        
        void ConstructL();

    public:

        /**
         * Define and reset pubsub keys.
         */
        IMPORT_C void ResetPubSub();

        /**
         * Delete all pubsub keys.
         */
        IMPORT_C void DeletePubSubKeys();

        /**
         * Set index for plugin. Call before Bind-operation.
         * Default of KErrNotFound (-1) will set the key to
         * unused.
         */
        IMPORT_C void SetPluginIndex( TInt aIndex = KErrNotFound );
        
        /**
         * Get index for plugin.
         * @return Plugin index, or KErrNotFound if not found.
         */
        IMPORT_C static TInt GetPluginIndex();

        /**
         * Clears the error behavior.
         */
        IMPORT_C void SetNoError();

        /**
         * Setup for general error. Next operation will fail with this error
         * code.
         * @param aErrorCode The error code e.g. KErrNotFound
         */
        IMPORT_C void SetError( TInt aErrorCode );
        
        /**
         * @return The currently set error code.
         */
        IMPORT_C TInt GetError();

        /**
         * Setup a specialized error which has a boolean value.
         * Next operation will do the given specialized error, e.g. complete request
         * with invalid request id (EXIMPTestPlgBehaveInvalidReqId)
         * @param aKey The type defining the error, e.g. EXIMPTestPlgBehaveInvalidReqId
         */
        IMPORT_C void SetBoolean( TXIMPTestPropertyKeys aKey );
        
        /**
         * @param aKey The type defining the error, e.g. EXIMPTestPlgBehaveInvalidReqId
         * @return The value of the boolean-valued specialized error
         */
        IMPORT_C TBool GetBoolean( TXIMPTestPropertyKeys aKey ) const;

        /**
         * Setup for leave. Next operation will leave with given leave code.
         * @param aLeaveCode Code with which to leave
         */
        IMPORT_C void SetLeave( TInt aLeaveCode );
        
        /**
         * @return The currently set leave code
         */
        IMPORT_C TInt GetLeave();

        /**
         * Leaves if leave has been requested. Uses value obtained from pubsub.
         */
        IMPORT_C void HandleLeaveL();

        /**
         * General messaging support for direct access.
         * @param aKey The key to set
         * @param aValue The value to set to given key
         */
        IMPORT_C void SetValueFor( TXIMPTestPropertyKeys aKey, TInt aValue );

        /**
         * General messaging support for direct access.
         * @param aKey The key to set
         * @return The value in the given key
         */
        IMPORT_C TInt GetValueFor( TXIMPTestPropertyKeys aKey ) const;

        /**
         * Assert-methods.
         * @param aState ETrue, if successful case is expected.
         * 
         * E.g. in the case of AssertPluginDied, 
         * ETrue means:
         *    "I expect the plugin to have died, check that it did" 
         * and EFalse means:
         *    "I expect the plugin to NOT have died, check that it didn't".
         */
        IMPORT_C void AssertPluginDied( TBool aState );
        IMPORT_C void AssertOpenSessionCalled( TBool aState );
        IMPORT_C void AssertCloseSessionCalled( TBool aState );
        IMPORT_C void AssertShareSessionCalled( TBool aState );
        IMPORT_C void AssertUnshareSessionCalled( TBool aState );
        IMPORT_C void AssertSubscribePresentityGroupContentCalled( TBool aState );
        IMPORT_C void AssertSubscribePresentityGroupListCalled( TBool aState );
        IMPORT_C void AssertUnsubscribePresentityGroupContentCalled( TBool aState );
        IMPORT_C void AssertUnsubscribePresentityGroupListCalled( TBool aState );
        IMPORT_C void AssertAddPresentityGroupMemberCalled( TBool aState );
        IMPORT_C void AssertCreatePresentityGroupCalled( TBool aState );
        IMPORT_C void AssertRemovePresentityGroupMemberCalled( TBool aState );
        IMPORT_C void AssertUpdatePresentityGroupMemberDisplayNameCalled( TBool aState );
        IMPORT_C void AssertUpdatePresentityGroupDisplayNameCalled( TBool aState );
        
        IMPORT_C void SetReqCompleteParams( MXIMPRestrictedObjectCollection* aParams );
        IMPORT_C MXIMPRestrictedObjectCollection* GetReqCompleteParams();

    private:

        /**
         * Send the given message.
         * @param aMessage The message to send with the key and value fields set
         */
        void Send( TXIMPTestMsg& aMsg );

        /**
         * Receive message.
         * Returns the message with the value field set, using the same
         * message parameter which was given as input.
         * @param aMsg [inout] The message with the key field set
         */
        void Receive( TXIMPTestMsg& aMsg ) const;

        /**
         * Common internal assert method for all Assert* methods.
         * @param aKey The key to assert
         * @param aCompareState The expected state (ETrue for success)
         * @param aTrueDesc Description for success case failing
         * @param aFalseDesc Description for failure case failing
         */
        IMPORT_C void CommonAssert( TXIMPTestPropertyKeys aKey, 
                TBool aCompareState, 
                const TDesC8& aTrueDesc, 
                const TDesC8& aFalseDesc ) const;
    private:  

        // id number for this message queue
        TInt iId;
        TUid iMyUid;

        //OWN: Description buffer to hold the failure description
        mutable RBuf8 iDescription;
        
        // Params for next reqcomplete
        MXIMPRestrictedObjectCollection* iReqCompleteParams;
    };

#endif // CPRFWTESTMESSENGER_H


