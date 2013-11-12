/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __UT_CFCONTEXTSOURCEMANAGER_H__
#define __UT_CFCONTEXTSOURCEMANAGER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include "cfextendedcontextinterface.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CCFContextSourceManager;
class CCFContextSourcePlugIn;

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
NONSHARABLE_CLASS( UT_CFContextSourceManager ): public CEUnitTestSuiteClass,
    public MCFExtendedContextInterface
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CFContextSourceManager* NewL();
        static UT_CFContextSourceManager* NewLC();
        /**
         * Destructor
         */
        ~UT_CFContextSourceManager();

    private:    // Constructors and destructors

        UT_CFContextSourceManager();
        void ConstructL();

    public:     // From observer interface

        // From MCFContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy );

        // From MCFContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadWritePolicy,
            MCFContextSource& aOwner );

        // From MCFContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy,
            MCFContextSource& aOwner );

        // From MCFContextInterface
        TInt PublishContext( CCFContextObject& aContext,
            RThread& aClientThread );
            
        // From MCFContextInterface
        TInt PublishContext( CCFContextObject& aContext,
            CCFContextDataObject& aData,
            RThread& aClientThread );

        // From MCFContextInterface
        TInt SubscribeContext(
            CCFContextSubscription* aSubscription,
            MCFContextSubscriptionListener* aListener,
            RThread& aClientThread );

        // From MCFContextInterface
        void UnsubscribeContext(
            CCFContextSubscription& aSubscription,
            MCFContextSubscriptionListener& aListener );

        // From MCFContextInterface
        void UnsubscribeContexts(
            MCFContextSubscriptionListener& aObserver );

        // From MCFContextInterface
        TInt RequestContext(
            RContextObjectArray& aResultBuffer,
            CCFContextQuery& aContextQuery,
            RThread& aClientThread );
            
        // From MCFContextInterface
        TInt GetSecurityPolicy( const TDesC& aContextSource,
            const TDesC& aContextType,
			TSecurityPolicy& aPolicy );
        
        // From MCFExtendedContextInterface
        TInt GetReadSecurityPolicy( const TDesC& aContextSource,
            const TDesC& aContextType,
            TSecurityPolicy& aPolicy );

        // From MCFExtendedContextInterface
        TInt GetWriteSecurityPolicy( const TDesC& aContextSource,
            const TDesC& aContextType,
            TSecurityPolicy& aPolicy );

        // From MCFExtendedContextInterface
        TInt DefineContext( const TDesC& aContextSource,
            const TDesC& aContextType,
            const TSecurityPolicy& aReadPolicy,
            const TSecurityPolicy& aWritePolicy,
            MCFContextSource* aPublisher,
            const TUid& aPublisherUid );
        
        // From MCFExtendedContextInterface
        void DeregisterPublisher( MCFContextSource& aPublisher );
        
        // From MCFExtendedContextInterface
        TInt InstallContextSourceSetting( RFile& aSettingFile,
            const TUid& aContextSourceUid,
            RThread& aClientThread );

        // From MCFExtendedContextInterface
        TInt UninstallContextSourceSetting( const TDesC& aSettingFilename,
            const TUid& aContextSourceUid,
            RThread& aClientThread );

        // From MCFExtendedContextInterface
        TInt UninstallContextSourceSettings( const TUid& aContextSourceUid,
            RThread& aClientThread );

        // From MCFExtendedContextInterface
        TInt PublishContextFromAction( CCFContextObject& aContext, TBool aAutoDefine );

    private:    // New methods

        void SetupL();
        void Teardown();
        
        void ConfigurePlugInL( CCFContextSourcePlugIn& aPlugIn );

        void UT_CCFContextSourcePlugIn_NewLL();
        void UT_CCFContextSourcePlugIn_NewLCL();

        void UT_CCFContextSourceManager_NewLL();
        void UT_CCFContextSourceManager_NewLCL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
    };

#endif
