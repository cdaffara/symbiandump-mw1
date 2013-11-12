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


#ifndef __MT_CFCONTEXTSOURCEMANAGER_H__
#define __MT_CFCONTEXTSOURCEMANAGER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <f32file.h>

#include "cfextendedcontextinterface.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CCFContextSourceManager;
class CCFContextSourcePlugIn;
class TContextSourceInitParams;
class CCFContextSourceSettingArray;
class CCFContextSourceSettingsManager;

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
NONSHARABLE_CLASS( MT_CFContextSourceManager ):
public CEUnitTestSuiteClass,
public MCFExtendedContextInterface
    {
public: // Constructors and destructors

    /**
     * Two phase construction
     */
    static MT_CFContextSourceManager* NewL();
    static MT_CFContextSourceManager* NewLC();
    /**
     * Destructor
     */
    ~MT_CFContextSourceManager();

private: // Constructors and destructors

    MT_CFContextSourceManager();
    void ConstructL();

public: // From observer interface

    // From MCFContextInterface
    TInt DefineContext( const TDesC& aContextSource,
        const TDesC& aContextType,
        const TSecurityPolicy& aSecurityPolicy );

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
    TInt GetSecurityPolicy(
        const TDesC& /*aContextSource*/,
        const TDesC& /*aContextType*/,
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

private: // New methods

    void SetupL();
    void SetupEmptyL();
    void SetupPluginsL();
    void Teardown();

        static TInt CallBack( TAny* aSelf );
        void Wait( TTimeIntervalMicroSeconds32& aDelay );                        
        
    CCFContextSourcePlugIn* CreateAndInitializePlugInL(
        const TUid& aImplementationUid,
        TContextSourceInitParams& aParams );
    
    TBool ConfigureSettingFolder( TDes& aFile, const TUid& aUid );
    
    void ParsePlugInSettingsL( const TDesC& aFolder,
        CCFContextSourceSettingArray& aSettings );

    void MT_CCFContextSourceManager_NewLL();
    void MT_CCFContextSourceManager_NewLCL();
    void MT_CCFContextSourceManager_InitializePhaseL1L();
    void MT_CCFContextSourceManager_InitializePhaseL2L();
    void MT_CCFContextSourceManager_InstallUninstallSettingL();
    void MT_CCFContextSourceManager_TestContextSourcePluginsMemoryL();

private: // Data

    EUNIT_DECLARE_TEST_TABLE;

    CCFContextSourceManager* iContextSourceManager;
    CCFContextSourceSettingsManager* iSettingsManager;
    RFs iFs;
        CActiveSchedulerWait* iWait;
        CPeriodic* iPeriodic;        
    };

#endif      //  __MT_CFCONTEXTSOURCEMANAGER_H__

