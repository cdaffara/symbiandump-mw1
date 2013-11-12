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
* Description: XIMP Framework Test Code 
*
*/


#ifndef __T_MPRESENCECACHEREADER_H__
#define __T_MPRESENCECACHEREADER_H__

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>
#include <EUnitDecorators.h>
#include <presencecachereader.h>
#include <presencecachewriter.h>
#include <prescachereadhandler.h>
#include <prescachewritehandler.h>

#include <e32cmn.h>

class MPresenceBuddyInfo;
class MPresenceFeeder;


//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION
/**
 * TODO Auto-generated EUnit test suite
 *
 */
NONSHARABLE_CLASS( T_PresenceCache )
	: public CEUnitTestSuiteClass,
	  public MPresCacheReadHandler,
	  public MPresCacheWriteHandler
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static T_PresenceCache* NewL();
        static T_PresenceCache* NewLC();
        /**
         * Destructor
         */
        ~T_PresenceCache();
        
    public:     // from handlers
        void HandlePresenceWriteL(TInt aErrorCode);    
        void HandlePresenceReadL(TInt aErrorCode,
            MPresenceBuddyInfoList* aPresenceBuddyInfoList);
        void HandlePresenceNotificationL(TInt aErrorCode,
                            MPresenceBuddyInfo* aPresenceBuddyInfo);            
            
    private:    // Constructors and destructors

        T_PresenceCache();
        void ConstructL();

    private:    // New methods

         void SetupL();
        
         void Teardown();
         
         void T__PresenceInfoL();
        
         void T_WriteReadPresenceAsyncL();
        
         void T_DeletingAndCancellingL();
         
         void T_PerformanceL();
         
         void T_RunAllL();
         
         void T_TestFeeder();
        
         void T_SubscribeBuddy1Response();
         
         void T_SubscribeBuddyAndUnsubscribe(); 
         
         void T_SubscribeBuddyManyResponses();
         
         void T_DeleteBeforeNotify();
         
       
    private:    // utilities
         MPresenceInfo* MakePresenceInfoLC();
         
         MPresenceBuddyInfo* MakeBuddyPresenceInfoLC(
                                         const TDesC& id, const TDesC& text);
                                         
         void T_DeleteAllCacheL();
         
         void T_MakePerformancePreconditionsL();
         
         MPresenceBuddyInfo* T_MakePerformanceBuddyLC(const TDesC& id);                  
         
         inline void WaitComplete();
        

    private:    // Data
		
        MPresenceCacheReader* iMPresenceCacheReader;
        MPresenceCacheWriter* iMPresenceCacheWriter;
        
        MPresenceFeeder* iPresenceFeeder;
        
        
        MPresenceBuddyInfoList* iPresBuddyInfoList;
        
        EUNIT_DECLARE_TEST_TABLE;
        TUid iUid;
        
        TBool iCoverageTesting;
        
        // we will record the error from handler here
        TInt iErrorFromHandler;
		
		//OWN: Active scheduler wait and Cb received indication
        CActiveSchedulerWait iWait;
        TBool iCbReceived;
        
        // expected status of next notification
        TInt iExpectedNotifyStatus; 
        // How many notifications are waicted until test is done.
        TInt iNbrNotifys;
        // Expected content of notification
        // OWN
        MPresenceBuddyInfo* iExpectedNotifyContent;        
        
    };
    
inline void T_PresenceCache::WaitComplete()
    {
    iCbReceived = EFalse;
    if( !iCbReceived )
        {
        iWait.Start();
        }
    }
    
#endif      //  __T_MPRESENCECACHEREADER_H__

// End of file
