/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __MT_CFSISUPGRADE_H__
#define __MT_CFSISUPGRADE_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <f32file.h>
#include <swinstapi.h>
#include <cflistener.h>
#include <cfscriptownerinterface.h>

//  INTERNAL INCLUDES
#include "contextsourcedef_v1.h"
#include "contextsourcedef_v2.h"

//  FORWARD DECLARATIONS
class CCFTestDelay;
class CCFClient;
class CCFContextObject;

//  CLASS DEFINITION
/**
 * Auto-generated test suite
 *
 */
NONSHARABLE_CLASS( MT_CFSisUpgrade ):
public CEUnitTestSuiteClass,
public MCFListener,
public MCFScriptOwnerInterface
    {
public: // Constructors and destructors

    /**
     * Two phase construction
     */
    static MT_CFSisUpgrade* NewL();
    static MT_CFSisUpgrade* NewLC();
    /**
     * Destructor
     */
    ~MT_CFSisUpgrade();

private:

    // From MCFClienter
    void ContextIndicationL(
        const CCFContextIndication& aChangedContext );

    // From MCFClienter
    void ActionIndicationL(
        const CCFActionIndication& aActionToExecute );
    
    // From MCFClienter
    void HandleContextFrameworkError( TCFError aError,
        const TDesC& aSource,
        const TDesC& aType );

    // From MCFClienter
    TAny* Extension( const TUid& aExtensionUid ) const;
    
    // From MCFScriptOwnerInterface
    void HandleScriptsRemovedL( const RArray<TInt>& aScriptIds );

private: // Constructors and destructors

    MT_CFSisUpgrade();
    void ConstructL();

private: // New methods

    void SetupL();
    void Teardown();
    
    void SetInstallOptions( SwiUI::TInstallOptionsPckg& aOptions );
    void ResetContextObjectL();
    void ResetActionL();
    void ResetScriptsRemovedL();

    void MT_CFSisUpgrade_InstallCfSisUpgrade1L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade2L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade3L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade4L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade5L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade6L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade7L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade8L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade9L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade10L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade11L();
    void MT_CFSisUpgrade_InstallCfSisUpgrade12L();

private: // Data

    EUNIT_DECLARE_TEST_TABLE;

    RFs iFs;

    SwiUI::RSWInstSilentLauncher iSwInst;

    CCFTestDelay* iDelay;

    CCFClient* iCFClient;
    
    CCFContextObject* iCo;
    
    TBool iContextIndication;
    
    TBool iActionIndication;
    
    TBuf<KMaxFileName> iActionId;
    
    RArray<TInt> iScriptIds;
    
    TBool iScriptsRemovedNotified;
    };

#endif      //  __MT_CFSISUPGRADE_H__

// End of file
