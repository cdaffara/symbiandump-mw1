/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:        ?Description
*
*/









#ifndef PROENGWRAPAPI_H
#define PROENGWRAPAPI_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>


#include <e32std.h>
#include <e32debug.h>
#include <e32base.h>
#include <f32file.h>
#include <e32property.h>
#include <MProEngEngine.h>
#include <MProEngProfile.h>
#include <MProEngProfileName.h>
#include <MProEngProfileNameArray.h>
#include <MProEngTones.h>
#include <ProEngFactory.h>
#include <MProEngToneSettings.h>
#include <MProEngAlertToneSeeker.h>
#include <MProEngAlertToneSeekerObserver.h>
#include <MProEngNotifyHandler.h>
#include <MProEngActiveProfileObserver.h>
#include <MProEngProfileActivationObserver.h>
#include <MProEngProfileObserver.h>
#include <MProEngProfileNameArrayObserver.h>
#include "CSchedulerUtility.h"



// MACROS
#define TEST_CLASS_VERSION_MAJOR 31
#define TEST_CLASS_VERSION_MINOR 8
#define TEST_CLASS_VERSION_BUILD 48

// Logging path
_LIT( KProEngWrapAPILogPath, "\\logs\\testframework\\ProEngWrapAPI\\" ); 
// Log file
_LIT( KProEngWrapAPILogFile, "ProEngWrapAPI.txt" ); 
_LIT( KProEngWrapAPILogFileWithTitle, "ProEngWrapAPI_[%S].txt" );

// MODULE DATA STRUCTURES

class CProEngVTNotifyHandlerWrapper : public CBase
    {
    public:
        CProEngVTNotifyHandlerWrapper() {}
        ~CProEngVTNotifyHandlerWrapper() { delete iNotifyHandler; }
        void SetObject( MProEngNotifyHandler* aNotifyHandler )
            {
            iNotifyHandler = aNotifyHandler;
            }
    private:
        MProEngNotifyHandler* iNotifyHandler;
    };

class CProEngVTAlertToneSeekerWrapper : public CBase
    {
    public:
        CProEngVTAlertToneSeekerWrapper() {}
        ~CProEngVTAlertToneSeekerWrapper() { delete iSeeker; }
        void SetObject( MProEngAlertToneSeeker* aSeeker )
            {
            iSeeker = aSeeker;
            }
    private:
        MProEngAlertToneSeeker* iSeeker;
    };

class CProEngVTSeekerObserver : public CBase,
                                public MProEngAlertToneSeekerObserver
    {
    public:
        CProEngVTSeekerObserver(CSchedulerUtility& aSchedulerUtility):iSchedulerUtility(aSchedulerUtility){
        //iSchedulerUtility = aSchedulerUtility;
        }
        ~CProEngVTSeekerObserver() {};
        void HandleAlertToneListCompletedL( MDesCArray* aToneList )
            {
            RDebug::Print( _L( "ProEng Validation test VTSeekerObserver::HndlAlrtTnLstCmpltd" ) );
            delete aToneList;
            iSchedulerUtility.Stop();
           // CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleError( TInt /* aError */ )
            {
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }
    public:
        CSchedulerUtility& iSchedulerUtility;
    };

class CProEngVTObserver: public CBase,
                         public MProEngActiveProfileObserver,
                         public MProEngProfileActivationObserver,
                         public MProEngProfileObserver,
                         public MProEngProfileNameArrayObserver
    {
    public:
        CProEngVTObserver(CSchedulerUtility& aSchedulerUtility):iSchedulerUtility(aSchedulerUtility){
        //iSchedulerUtility = aSchedulerUtility;
        }
        ~CProEngVTObserver() {};

        void HandleActiveProfileModifiedL()
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle act prof modified" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleActiveProfileNotificationError(
                TInt /* aError */ )
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle act prof error" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleProfileActivatedL( TInt /* aProfileId */ )
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle act prof activated" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleProfileActivationNotificationError(
                TInt /* aError */ )
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle act prof activated error" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleProfileNameArrayModificationL()
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle name array notify" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleProfileNameArrayNotificationError(
                TInt /* aError */ )
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle name array notify error" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleProfileModifiedL( TInt /* aProfileId */ )
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle profile notify" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }

        void HandleProfileNotificationError(
                TInt /* aError */ )
            {
            RDebug::Print( _L( "ProEng Validation test VTObserver handle profile notify error" ) );
            iSchedulerUtility.Stop();
            //CSchedulerUtility::InstanceL()->Stop();
            }
    public:
        CSchedulerUtility& iSchedulerUtility;

    };



// FORWARD DECLARATIONS
class CProEngWrapAPI;


// CLASS DECLARATION

/**
*  CProEngWrapAPI test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CProEngWrapAPI) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProEngWrapAPI* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CProEngWrapAPI();
 

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    private:

        /**
        * C++ default constructor.
        */
        CProEngWrapAPI( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
        virtual TInt NewEngineL();
        virtual TInt NewEngineLC();
        virtual TInt NewEngineLCWithRFsL();
        virtual TInt NewNotifyHandlerL();
        virtual TInt RequestProfileActivationNotificationsL();
        virtual TInt CancelProfileActivationNotificationsL();
        virtual TInt RequestActiveProfileNotificationsL();
        virtual TInt CancelActiveProfileNotificationsL();
        virtual TInt RequestProfileNotificationsL();
        virtual TInt CancelProfileNotificationsL();
        virtual TInt RequestProfileNameArrayNotificationsL();
        virtual TInt CancelProfileNameArrayNotificationsL();
        virtual TInt CancelAllL();
        virtual TInt ActiveProfileLC();
        virtual TInt ActiveProfileL();
        virtual TInt ActiveProfileIdL();
        virtual TInt ProfileNameArrayLC();
        virtual TInt SetActiveProfileL();
        virtual TInt ProfileLC();
        virtual TInt ProfileL();
        
        virtual TInt MdcaCountL();
        void TestProfileNameArrayMdcaCountL(MProEngProfileNameArray& aNameArray);
        virtual TInt MdcaPointL();
        void TestProfileNameArrayMdcaPointL(MProEngProfileNameArray& aNameArray);
        virtual TInt ProfileIdL();
        void TestProfileNameArrayProfileIdL(MProEngProfileNameArray& aNameArray);
        virtual TInt FindByIdL();
        void TestProfileNameArrayFindByIdL(MProEngProfileNameArray& aNameArray);
        virtual TInt FindByNameL();
        void TestProfileNameArrayFindByNameL(MProEngProfileNameArray& aNameArray);
        
        virtual TInt ProfileNameL();
        void TestProfileSettingsProfileNameL(MProEngProfile& aProfile);
        virtual TInt ProfileTonesL();
        void TestProfileSettingsProfileTonesL(MProEngProfile& aProfile);
        virtual TInt ToneSettingsL();
        void TestProfileSettingsToneSettingsL(MProEngProfile& aProfile);
        virtual TInt AlertForL();
        void TestProfileSettingsAlertForL(MProEngProfile& aProfile);
        virtual TInt SetAlertForL();
        void TestProfileSettingsSetAlertForL(MProEngProfile& aProfile);
        virtual TInt IsSilentL();
        void TestProfileSettingsIsSilentL(MProEngProfile& aProfile);                
        virtual TInt CommitChangeL();
        void TestProfileSettingsCommitChangeL(MProEngProfile& aProfile);
        
        virtual TInt NameL();
        void TestProfileSettingsNameL(MProEngProfile& aProfile);
        virtual TInt IdL();
        void TestProfileSettingsIdL(MProEngProfile& aProfile);
        virtual TInt SetNameL();
        void TestProfileSettingsSetNameL(MProEngProfile& aProfile);
        
        virtual TInt RingingTone1L();
        void TestProfileSettingsRingingTone1L(MProEngProfile& aProfile);
        virtual TInt RingingTone2L();
        void TestProfileSettingsRingingTone2L(MProEngProfile& aProfile);
        virtual TInt MessageAlertToneL();
        void TestProfileSettingsMessageAlertToneL(MProEngProfile& aProfile);
        virtual TInt EmailAlertToneL();
        void TestProfileSettingsEmailAlertToneL(MProEngProfile& aProfile);
        virtual TInt VideoCallRingingToneL();
        void TestProfileSettingsVideoCallRingingToneL(MProEngProfile& aProfile);
        virtual TInt SetRingingTone1L();
        void TestProfileSettingsSetRingingTone1L(MProEngProfile& aProfile);
        virtual TInt SetRingingTone2L();
        void TestProfileSettingsSetRingingTone2L(MProEngProfile& aProfile);
        virtual TInt SetMessageAlertToneL();
        void TestProfileSettingsSetMessageAlertToneL(MProEngProfile& aProfile);
        virtual TInt SetEmailAlertToneL();
        void TestProfileSettingsSetEmailAlertToneL(MProEngProfile& aProfile);
        virtual TInt SetVideoCallRingingToneL();
        void TestProfileSettingsSetVideoCallRingingToneL(MProEngProfile& aProfile);
        
        virtual TInt RingingTypeL();
        void TestProfileSettingsRingingTypeL(MProEngProfile& aProfile);
        virtual TInt KeypadVolumeL();
        void TestProfileSettingsKeypadVolumeL(MProEngProfile& aProfile);        
        virtual TInt RingingVolumeL();
        void TestProfileSettingsRingingVolumeL(MProEngProfile& aProfile);
        virtual TInt VibratingAlertL();
        void TestProfileSettingsVibratingAlertL(MProEngProfile& aProfile);
        virtual TInt WarningAndGameTonesL();
        void TestProfileSettingsWarningAndGameTonesL(MProEngProfile& aProfile);
        virtual TInt TextToSpeechL();
        void TestProfileSettingsTextToSpeechL(MProEngProfile& aProfile);
        virtual TInt SetRingingTypeL();
        void TestProfileSettingsSetRingingTypeL(MProEngProfile& aProfile);
        virtual TInt SetKeypadVolumeL();
        void TestProfileSettingsSetKeypadVolumeL(MProEngProfile& aProfile);        
        virtual TInt SetRingingVolumeL();
        void TestProfileSettingsSetRingingVolumeL(MProEngProfile& aProfile);
        virtual TInt SetVibratingAlertL();
        void TestProfileSettingsSetVibratingAlertL(MProEngProfile& aProfile);
        virtual TInt SetWarningAndGameTonesL();
        void TestProfileSettingsSetWarningAndGameTonesL(MProEngProfile& aProfile);
        virtual TInt SetTextToSpeechL();
        void TestProfileSettingsSetTextToSpeechL(MProEngProfile& aProfile);
        
        virtual TInt NewAlertToneSeekerL();
        virtual TInt FetchAlertToneListL();
        virtual TInt CancelFetchL();
        
        virtual TInt NewEngineLWithRFsL();

        
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        CSchedulerUtility* iSchedulerUtility;
        RFs iFs;

    };

#endif      // PROENGWRAPAPI_H

// End of File
