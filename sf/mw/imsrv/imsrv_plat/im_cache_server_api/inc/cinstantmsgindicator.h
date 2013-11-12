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
* Description:  base class declaration for accessing the cache information
*
*/



#ifndef __CINSTNATMSGINDICATOR_H
#define __CINSTNATMSGINDICATOR_H
// INCLUDES
#include <e32base.h>
#include <ecom/ecom.h>

//Constants
// interface uid for the instantmessagecache.
const TUid KINSTANTMSGINTERFACEUID={0x20026768};

// The general rule is that the (category) UID must be the same as the Security ID (the SID)
// of the process in which the code that is performing the define operation is running. 
// In effect, this forms a data cage, preventing a process from defining, or "occupying", 
// another process's property. 
// hence this is the uid of imcacheserver.
const TUid KIMStatusProperty={0x2001242A};
enum TIMStatusPropertyKeys
    {
    EIMStatusPropertyMessageCount,
    EIMStatusPropertyServiceId,
    EIMStatusPropertySenderId,
    EIMMultipleSender
    };

 /**
  * An interface for setting/resetting statuspane icon
  *
  * @since 5.0
  */
 class CInstantMsgIndicator: public CBase
     {
      public:
        
        static CInstantMsgIndicator* CreateImplementationL( TUid aUid );
       
        virtual ~CInstantMsgIndicator();
     public:
         
         /**
          * sets/resets the status pane indicator
          * @param aNewMessages, new message count
          * @param aSeriveId, service id.
          * @param aSenderId, message sender id.
          * @param aMultipleSender, count of number of P-2_p chats.
          */
 	   virtual void MessageInfoL(TInt aNewMessages, TInt aSeriveId,const TDesC& aSenderId,TInt aMultipleSenders) = 0;	
		 
     private:
         /**
          * Instance identifier key. When instance of an implementation is
          * created by ECOM framework, the framework will assign a UID for
          * it. The UID is used in the destructor to notify framework that
          * this instance is being destroyed and resources can be released.
          * Plugin implementations should leave this untouched, as the inline
          * destructor and constructor method take care of ECom releasing.
          */
         TUid iDtor_ID_Key; 
     };
 
 inline CInstantMsgIndicator* CInstantMsgIndicator::CreateImplementationL( TUid aUid )
     {
     TAny* ptr = REComSession::CreateImplementationL( aUid, _FOFF( CInstantMsgIndicator, iDtor_ID_Key ) );
     CInstantMsgIndicator* impl = static_cast<CInstantMsgIndicator*> (ptr);
     return impl;
     }

 inline CInstantMsgIndicator::~CInstantMsgIndicator()
     {
     REComSession::DestroyedImplementation( iDtor_ID_Key );
     }
 
 #endif //__CINSTNATMSGINDICATOR_H
