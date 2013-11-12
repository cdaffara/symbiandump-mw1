/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*             Declaration of class CMMKeyBearerObserver
*   
*
*/


#ifndef __MMKEYBEAREROBSERVERPS_H__
#define __MMKEYBEAREROBSERVERPS_H__

// INCLUDE
#include <e32svr.h>
#include <e32property.h>

// DATA TYPES
enum TObserverKeyType
{
    ESideVolumeKeys = 0,
    EMediaKeys,
    EAccessoryVolumeKeys,
    EFileTransferStatus
};

// CLASS DECLARATION
/**
*  MCallBackReceiver
*     Used to call functions of CMMKeyBearer from CMMKeyBearerObserver
*     On receiving event from the P & S, CMMKeyBearerObserver calls 
*     this function
*  @lib MMKeyBearer.dll
*  @since 3.0
*/
class MCallBackReceiver
    {
public:
    /**
    Called when event has arrived from P&S.
    @param aEnumValue: Enum value representing the key and action.
           aKeyType : will indicate whether it is Volume key or Media key
    */
    virtual void ReceivedKeyEvent(TInt aEnumValue, TInt aKeyType) = 0;
    };

/**
*  CMMKeyBearerObserver
*     This class inplements the Active Object. There will be two instance 
*     of this ,one for handling Volume keys and other for Media Keys
*  @lib MMKeyBearer.dll
*  @since 3.0
*/
class CMMKeyBearerObserver: public CActive
    {
    public: // Constructors and destructor
        /**
        * Creates instance of the CMMKeyBearerObserver class.
        * @param aCallBack : reference of the class which implements 
        *                    MCallBackReceiver for handling call back 
        *                    functions
        * @param aKeyType : Indiacates whether it is side volume key or 
        *                   media key
        * @return - Instance of the CMMKeyBearerObserver class.
        */
        static CMMKeyBearerObserver* NewL(MCallBackReceiver& aCallback, 
                                          TInt aKeyType);
        /**
        * Destructor.
        */
        virtual ~CMMKeyBearerObserver();    
    
    public: // Base class functions - CActive
        /** @see CActive::Start() */
        TInt Start();
        
        /** @see CActive::Stop() */
        void Stop();

       /**
        * Gets the current value.
        * @return - The current value.
        */
        TInt GetValue();

    private: // constructors
        /**
        * default constructor.
        * @param aCallback : reference to callback class
        */
        CMMKeyBearerObserver(MCallBackReceiver& aCallback);
        
        /**
        * Two - phase construction.
        */
        void ConstructL(TInt aKeyType);
        
    private: // Base class functions - CActive    
        /** @see CActive::RunL() */
        void RunL();
        /** @see CActive::DoCancel() */
        void DoCancel();
        
    private: // Data
        
        RProperty            iProperty;  // For P & S
        MCallBackReceiver&   iCallback;
        TInt                 iKeyType;   // Vol key or media Key
    };

#endif // __MMKEYBEAREROBSERVERPS_H__

// End of file
