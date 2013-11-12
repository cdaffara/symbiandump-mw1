/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CFSERVICES_H__
#define __MT_CFSERVICES_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <cfcontextsubscriptionlistener.h>

//  INTERNAL INCLUDES
#include "cfactionsubscriptionlistener.h"

//  FORWARD DECLARATIONS
class CCFContextIndicationImpl;
class CFServiceMTils;
class CCFActionIndicationImpl;
class CCFContextObject;
class CCFContextObjectImpl;
class CCFKeyValuePair;
class CCFActionSubscriptionImpl;
class CCFContextSubscriptionImpl;
class CCFContextQueryImpl;

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
NONSHARABLE_CLASS( MT_CFServices ):
    public CEUnitTestSuiteClass,
    public MCFActionSubscriptionListener,
    public MCFContextSubscriptionListener
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static MT_CFServices* NewL();
        static MT_CFServices* NewLC();
        /**
         * Destructor
         */
        ~MT_CFServices();

    private:    // Constructors and destructors

        MT_CFServices();
        void ConstructL();

    public:     // From observer interface

        // @see MCFActionSubscriptionListener
        void ActionIndicationL( CCFActionIndication* aIndication );
        
        // @see MCFContextSubscriptionListener
        void ContextIndicationL( CCFContextIndication* aIndication );

        // @see MCFContextSubscriptionListener
        TInt Client( RThread& aThread ) const;
        
        // @see MCFContextSubscriptionListener
        void HandleContextSubscriptionError( TInt aError,
            const TDesC& aSource,
            const TDesC& aType );
            
    private:    // New methods

        void SetupL();
        void SetupActionIndicationL();
        void Teardown();
        
        CCFActionIndicationImpl* ConfigureActionIndicationL(
            const TDesC& aActionId,
            TInt aNumOfParams );
        CCFActionSubscriptionImpl* ConfigureActionSubscriptionL(
            const TDesC& aActionId );
        CCFContextObjectImpl* ConfigureContextObjectLC(
            const TDesC& aSource,
            const TDesC& aType,
            const TDesC& aValue,
            TUint aConfidence,
            const TTime& aTime );
        CCFContextSubscriptionImpl* ConfigureContextSubscriptionLC(
            const TDesC& aSource,
            const TDesC& aType,
            TBool aSubTypeMatch,
            TBool aIndicateOnlyChanges );

        void MT_CCFActionIndication_NewLL();
        void MT_CCFActionIndication_NewLCL();

        void MT_CCFActionIndicationImpl_NewLL();
        void MT_CCFActionIndicationImpl_NewLCL();
        void MT_CCFActionIndicationImpl_SetIdentifierLL();
        void MT_CCFActionIndicationImpl_AddParameterLL();
        void MT_CCFActionIndicationImpl_ExternalizeLL();

        void MT_CCFActionSubscription_NewLL();
        void MT_CCFActionSubscription_NewLCL();
        
        void MT_CCFActionSubscriptionImpl_NewLL();
        void MT_CCFActionSubscriptionImpl_NewLCL();
        void MT_CCFActionSubscriptionImpl_SetActionIdentifierLL();
        void MT_CCFActionSubscriptionImpl_ExternalizeLL();
        void MT_CCFActionSubscriptionImpl_SetListenerL();

        void MT_CCFContextIndication_NewLL();
        void MT_CCFContextIndication_NewLCL();
        
        void MT_CCFContextIndicationImpl_NewLL();
        void MT_CCFContextIndicationImpl_NewLCL();
        void MT_CCFContextIndicationImpl_SetContextLL();
        void MT_CCFContextIndicationImpl_ExternalizeLL();

        void MT_CCFContextObject_NewLL();
        void MT_CCFContextObject_NewLCL();
        
        void MT_CCFContextObjectImpl_NewLL();
        void MT_CCFContextObjectImpl_NewLCL();
        void MT_CCFContextObjectImpl_SetTypeLL();
        void MT_CCFContextObjectImpl_SetValueLL();
        void MT_CCFContextObjectImpl_SetSourceLL();
        void MT_CCFContextObjectImpl_SetConfidenceL();
        void MT_CCFContextObjectImpl_SetTimestampToHomeTimeL();
        void MT_CCFContextObjectImpl_SetTimestampL();
        void MT_CCFContextObjectImpl_CopyLL();
        void MT_CCFContextObjectImpl_ExternalizeLL();
        void MT_CCFContextObjectImpl_CompareValueL();
        void MT_CCFContextObjectImpl_CompareByTypeL();
        void MT_CCFContextObjectImpl_CompareByTypeAndSourceL();
        void MT_CCFContextObjectImpl_CompareByTimeDescendingL();
        void MT_CCFContextObjectImpl_IsSameL();

        void MT_CCFContextQuery_NewLL();
        void MT_CCFContextQuery_NewLCL();
        
        void MT_CCFContextQueryImpl_NewLL();
        void MT_CCFContextQueryImpl_NewLCL();
        void MT_CCFContextQueryImpl_SetTypeLL();
        void MT_CCFContextQueryImpl_SetSourceLL();
        void MT_CCFContextQueryImpl_SetSubTypeMatchL();
        void MT_CCFContextQueryImpl_ExternalizeLL();
        void MT_CCFContextQueryImpl_MatchesL();
        void MT_CCFContextQueryImpl_MatchesQueryL();
        void MT_CCFContextQueryImpl_IsSameL();

        void MT_CCFContextSubscription_NewLL();
        void MT_CCFContextSubscription_NewLCL();
        
        void MT_CCFContextSubscriptionImpl_NewLL();
        void MT_CCFContextSubscriptionImpl_NewLCL();
        void MT_CCFContextSubscriptionImpl_SetContextTypeLL();
        void MT_CCFContextSubscriptionImpl_SetContextSourceLL();
        void MT_CCFContextSubscriptionImpl_SetSubTypeMatchL();
        void MT_CCFContextSubscriptionImpl_SetOnlyChangesIndicatedL();
        void MT_CCFContextSubscriptionImpl_SetDataObjectEnabledL();
        void MT_CCFContextSubscriptionImpl_ExternalizeLL();
        void MT_CCFContextSubscriptionImpl_SetSubscriptionListenerL();
        void MT_CCFContextSubscriptionImpl_EvaluateContextChangeLL();
        void MT_CCFContextSubscriptionImpl_IsSameL();
        void MT_CCFContextSubscriptionImpl_MatchesL();

        void MT_CCFKeyValuePair_NewLL();
        void MT_CCFKeyValuePair_NewLCL();
        void MT_CCFKeyValuePair_NewL_1L();
        void MT_CCFKeyValuePair_NewLC_1L();
        void MT_CCFKeyValuePair_KeyL();
        void MT_CCFKeyValuePair_ValueL();

        void MT_CFServiceMTils_UpdateBufferLL();
        void MT_CFServiceMTils_WriteIntoStreamLL();
        
        void MT_CCFContextDataProxy_NewLL();
        void MT_CCFContextDataProxy_NewLCL();
        void MT_CCFContextDataProxy_SetContextDataUidL();
        void MT_CCFContextDataProxy_SetContextDataL();
        void MT_CCFContextDataProxy_ExternalizeLL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE;
        
        CCFActionIndicationImpl* iActionIndication;
        CCFContextObject* iIndicatedContext;
    };

#endif      //  __MT_CFSERVICES_H__

// End of file
