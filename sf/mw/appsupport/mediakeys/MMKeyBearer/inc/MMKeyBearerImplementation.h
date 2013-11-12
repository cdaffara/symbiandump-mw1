/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Declaration of class CMMKeyBearer.
*       
*/


#ifndef __MMKEYBEARERIMPLEMENTATION_H__
#define __MMKEYBEARERIMPLEMENTATION_H__

// INCLUDE
#include <e32base.h>
#include <c32comm.h>
#include <remcon/remconbearerplugin.h>
#include <remcon/remconbearerinterface.h>
#include <remcon/messagetype.h>
#include <keyguardaccessapi.h>



#include "MMKeyBearerObserverPS.h"

//CONSTANTS
const TInt KDataBufferSize = 10;

/**
*  CMMKeyBearer
*     Implementation of the Rem Con Bearer Plugin
*  @lib MMKeyBearer.dll
*  @since 3.0
*/
class CMMKeyBearer : public CRemConBearerPlugin,
                     public MRemConBearerInterface,
                     public MCallBackReceiver
    {
    public:
        /**
        * Creates instance of the CMMKeyBearer class.
        *
        * @param  TBearerParams: required for the CRemConBearerPlugin
        * @return : Instance of the CMMKeyBearer
        */
        static CMMKeyBearer* NewL(TBearerParams& aParams);

        /**
        * Destructor.
        */    
        virtual ~CMMKeyBearer();

    private:  // constructors
        /**
        * default constructor.
        * @param aParams: BearerPlugin Params
        */
        CMMKeyBearer(TBearerParams& aParams);
        
        /**
        * Two - phase construction.
        */
        void ConstructL();

    private: // from CRemConBearerPlugin
        /** @see CRemConBearerPlugin::GetInterface() */
    	TAny* GetInterface(TUid aUid);

    private: // from MRemConBearerInterface
    
        /** @see MRemConBearerInterface::GetResponse() */
        TInt GetResponse(TUid& aInterfaceUid, 
            TUint& aTransactionId, 
            TUint& aOperationId, 
            RBuf8& aData, 
            TRemConAddress& aAddr);
            
        /** @see MRemConBearerInterface::SendCommand() */            
        TInt SendCommand(TUid aInterfaceUid, 
            TUint aCommand, 
            TUint aTransactionId,  
            RBuf8& aData, 
            const TRemConAddress& aAddr);
            
        /** @see MRemConBearerInterface::GetCommand() */            
        TInt GetCommand(TUid& aInterfaceUid, 
            TUint& aTransactionId, 
            TUint& aCommand, 
            RBuf8& aData, 
            TRemConAddress& aAddr);
            
        /** @see MRemConBearerInterface::SendResponse() */
        TInt SendResponse(TUid aInterfaceUid, 
            TUint aOperationId, 
            TUint aTransactionId, 
            RBuf8& aData, 
            const TRemConAddress& aAddr);

        /** @see MRemConBearerInterface::ConnectRequest() */
        void ConnectRequest(const TRemConAddress& aAddr);
        
        /** @see MRemConBearerInterface::DisconnectRequest() */        
        void DisconnectRequest(const TRemConAddress& aAddr);
        
        /** @see MRemConBearerInterface::ClientStatus() */        
        void ClientStatus(TBool aControllerPresent, TBool aTargetPresent);
        
        /** @see MRemConBearerInterface::SecurityPolicy() */        
        TSecurityPolicy SecurityPolicy() const;
        
    private: // from MCallBackReceiver
    
        /** @see MCallBackReceiver::ReceivedKeyEvent() */            
        void ReceivedKeyEvent(TInt aScanCode, TInt aKeyType);
        
    private:
        /**
        * SetSVKOperationIdAndData
        * Assigns the Operation Id and Data field based on the aEnumValue
        * for SVK keys
        */
        void SetSVKOperationIdAndData(TInt aEnumValue);
        
        /**
        * SetMediaKeyOperationIdAndData
        * Assigns the Operation Id and Data field based on the aEnumValue
        * for Media Keys
        */
        void SetMediaKeyOperationIdAndData(TInt aEnumValue);

    private: // Data

        // Last received and corrected-decoded message.
        TUid iInterfaceUid;
        TUint iOperationId;

        TBuf8<KDataBufferSize> iData; 
        
        TUint iTransactionId;
        
        CMMKeyBearerObserver* iMMKeyBearerObserver;
        CMMKeyBearerObserver* iMediaKeyObserver;
        CMMKeyBearerObserver* iAccessoryVolKeyObserver;
        CMMKeyBearerObserver* iUSBFileTransferObserver;

        TInt iUSBFileTransfer;
        // For posting event to P&S 
        RProperty iProperty;
        
        // For keylock interaction
        CKeyguardAccessApi* iKeyguardAccess;
        };

#endif // __MMKEYBEARERIMPLEMENTATION_H__
