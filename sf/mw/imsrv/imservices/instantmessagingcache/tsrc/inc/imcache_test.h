/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/



#ifndef IMCACHE_TEST_H
#define IMCACHE_TEST_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
// imcache
#include "mimcacheeventhandler.h"
#include "mimcacheeventhandler.h"
#include <imcachedefs.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
// Logging path
_LIT( Kimcache_testLogPath, "\\logs\\testframework\\imcache_test\\" ); 
// Log file
_LIT( Kimcache_testLogFile, "imcache_test.txt" ); 
_LIT( Kimcache_testLogFileWithTitle, "imcache_test_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class Cimcache_test;
class CIMCacheFactory; 
class MIMCacheEventHandler;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  Cimcache_test test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(Cimcache_test) : public CScriptBase,
									  public MIMCacheEventHandler
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static Cimcache_test* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~Cimcache_test();

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
        Cimcache_test( CTestModuleIf& aTestModuleIf );

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

        
        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove
        
        virtual TInt CreateSameUpdaterL( CStifItemParser& aItem );
        
        virtual TInt CreateDifferentUpdaterL( CStifItemParser& aItem );
        
        virtual TInt CreateSameAccessorL( CStifItemParser& aItem );
               
        virtual TInt CreateDifferentAccessorL( CStifItemParser& aItem );
        
        virtual TInt StartNewConversationL( CStifItemParser& aItem ) ;
        
        virtual TInt AppendSendMessageL( CStifItemParser& aItem ) ;
        
        virtual TInt AppendRecieveMessageL( CStifItemParser& aItem ) ;
        
        virtual TInt FetchExistingMessageL( CStifItemParser& aItem );
        
        virtual TInt DeactivateConversationL( CStifItemParser& aItem );

        virtual TInt CloseConversationL( CStifItemParser& aItem );
        
        virtual TInt GetUnreadMsgCountL(CStifItemParser& aItem) ;
        
        virtual TInt GetNewMsgNotificationtL(CStifItemParser& aItem) ;
        
        virtual TInt ConversationUnExistTestL(CStifItemParser& aItem) ;
        
        virtual TInt ConversationExistTestL(CStifItemParser& aItem) ;
        
        virtual TInt AppendMessageL( CStifItemParser& aItem);
        
 	 	virtual TInt GetAllUnreadMsgCountL( CStifItemParser& aItem);

      
        virtual TInt GetChatListTestL( CStifItemParser& aItem);
        
 	 	virtual TInt AccessorCloseConversationTestL( CStifItemParser& aItem);

          

    public:     // MIMCacheEventHandler
    
    /**
    	* Gets called when data update is available.
    	* Client can then decide wether to use the old one
    	* or the updated one.
    	* @param aNewMessage, new message details
       	*/
         void HandleIMCacheEventL(TIMCacheEventType aEvent, TAny* aChatMessage = NULL ) ;
        
          	
    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
     
     	CActiveSchedulerWait iWait;
     	
     	TInt iMessageFetch;
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

#endif      // IMCACHE_TEST_H

// End of File
