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
* Description: STIF testclass declaration
*
*/

#ifndef XMPPSETTINGS_API_H
#define XMPPSETTINGS_API_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( Kxmppsettings_apiLogPath, "\\logs\\testframework\\xmppsettings_api\\" ); 
// Log file
_LIT( Kxmppsettings_apiLogFile, "xmppsettings_api.txt" ); 
_LIT( Kxmppsettings_apiLogFileWithTitle, "xmppsettings_api_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CXmppSettingsApi;
class Cxmppsettings_api;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cxmppsettings_api test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cxmppsettings_api) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cxmppsettings_api* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cxmppsettings_api();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        Cxmppsettings_api( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

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
        
        void ResetRepository();
        virtual TInt Test_CreateSettingsRecordL( CStifItemParser& aItem );
        virtual TInt Test_DefaultSettingsRecord( CStifItemParser& aItem );
        virtual TInt Test_GetSettingsRecordIdArray(CStifItemParser& aItem );
        virtual TInt Test_SetParamInt(CStifItemParser& aItem );
        virtual TInt Test_SetParamLDes(CStifItemParser& aItem );
        virtual TInt Test_GetParamInt(CStifItemParser& aItem );
        virtual TInt Test_GetParamLDes(CStifItemParser& aItem );
        virtual TInt Test_RemoveSettingsRecord(CStifItemParser& aItem );
        virtual TInt Test_RemoveParam(CStifItemParser& aItem );
       
        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
        TInt iXmppSettingId;
           TInt iXmppSnapId;
           TInt iXmppServerPort;
           TInt iMessageToneVolume;
           TInt iLastRegistrationError;

           TBool iPublishOnThePhoneStatus;
           TBool iRoamingWarning;
           TBool iAllowBuddyReq;
           TBool iAutoLogin;
           TBool iDefaultProfile;

           HBufC* iMessageTone;
           HBufC* iSnapName;
           HBufC* iSettingsName;
           HBufC* iStackVersion;
           HBufC* iXmppServerAddress;
           HBufC* iUsername;
           HBufC* iPassword;
           HBufC* iConnectionServer1;
           HBufC* iConnectionServer2;
           TUint32 iSettingId;
           RArray<TUint32> iSettingIdArray;
           
           CXmppSettingsApi* iApi;

           


    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // XMPPSETTINGS_API_H

// End of File
