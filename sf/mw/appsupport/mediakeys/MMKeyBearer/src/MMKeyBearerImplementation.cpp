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
* Description:  Bearer plugin API Implementation file
*
*/


// INCLUDE FILES
#include <e32def.h>
#include <remcon/remconbearerobserver.h>
#include <remcon/remconconverterplugin.h>
#include <remconaddress.h>
#include <remconcoreapi.h>
#include <mediakeysinternalpskeys.h>
#include <UsbWatcherInternalPSKeys.h>
#include <usbpersonalityids.h>
#include <PSVariables.h>   // Property values
#include <keyguardaccessapi.h>

// Include this once it is exported
// #include <RemConExtensionApi.h>

#include "MMKeyBearerImplementation.h"
#include "MMKeyBearerObserverPS.h"
#include "mmkeybearerscancodes.h"
#include "trace.h"

#define SET_MEDIAKEY_OPID(eNum, opId, remconEvent) \
case eNum:                                          \
	iOperationId = opId;                            \
    iData.Format(KFormatString,remconEvent);        \
    break;

//Constants
_LIT8(KFormatString,"%c");

// ---------------------------------------------------------
// CMMKeyBearer::NewL()
// Description: Factory function.
// return: Ownership of a new CMMKeyBearer.
// ---------------------------------------------------------
//
CMMKeyBearer* CMMKeyBearer::NewL(TBearerParams& aParams)
    {
    FUNC_LOG;

    CMMKeyBearer* self = new(ELeave) CMMKeyBearer(aParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------
// CMMKeyBearer::~CMMKeyBearer()
// Destructor.
// ---------------------------------------------------------
//
CMMKeyBearer::~CMMKeyBearer()
    {
    FUNC_LOG;

    delete iMMKeyBearerObserver;
    delete iMediaKeyObserver;
    delete iAccessoryVolKeyObserver;
    delete iUSBFileTransferObserver;
    delete iKeyguardAccess;
    }

// ---------------------------------------------------------
// CMMKeyBearer::CMMKeyBearer()
// construction.
// ---------------------------------------------------------
//
CMMKeyBearer::CMMKeyBearer(TBearerParams& aParams)
:   CRemConBearerPlugin(aParams),
    iUSBFileTransfer(KUsbWatcherSelectedPersonalityNone)
    {
    FUNC_LOG;
    
    //Pass
    }

// ---------------------------------------------------------
// CMMKeyBearer::ConstructL()
// 2nd-phase construction.
// ---------------------------------------------------------
//
void CMMKeyBearer::ConstructL()
    {
    FUNC_LOG;

    // throw an indication up to RemCon.
    TRemConAddress addr;
    addr.BearerUid() = Uid();
    TInt err = Observer().ConnectIndicate(addr);
    iKeyguardAccess = CKeyguardAccessApi::NewL();

    // Start Active object for listening key events from P&S
    TRAP_AND_LEAVE(
        iMMKeyBearerObserver = CMMKeyBearerObserver::NewL(*this, ESideVolumeKeys),
        "Volume key observer init failed" );

    TRAP_AND_LEAVE(
        iMediaKeyObserver = CMMKeyBearerObserver::NewL(*this, EMediaKeys),
        "Media key observer init failed" );

    TRAP_AND_LEAVE(
        iAccessoryVolKeyObserver = CMMKeyBearerObserver::NewL(*this, EAccessoryVolumeKeys),
        "Accessory key observer init failed" );

    TRAP_AND_LEAVE(
        iUSBFileTransferObserver = CMMKeyBearerObserver::NewL(*this, EFileTransferStatus),
        "USB observer init failed" );
    }
// ---------------------------------------------------------
// CMMKeyBearer::ConnectRequest()
// Rem Con server send connect request to Bearer Plugin
// ---------------------------------------------------------
//
void CMMKeyBearer::ConnectRequest(const TRemConAddress& /*aAddr*/)
    {
    FUNC_LOG;

    // Pass
    }
// ---------------------------------------------------------
// CMMKeyBearer::DisconnectRequest()
// Rem Con server send disconnect request to Bearer Plugin
// ---------------------------------------------------------
//
void CMMKeyBearer::DisconnectRequest(const TRemConAddress& /*aAddr*/)
    {
    FUNC_LOG;

    // Pass
    }
// ---------------------------------------------------------
// CMMKeyBearer::SendResponse()
//
// ---------------------------------------------------------
//
TInt CMMKeyBearer::SendResponse(TUid /* aInterfaceUid */,
        TUint /*aOperationId*/,
        TUint /*aTransactionId*/, // we don't care about this transaction ID
        RBuf8&  aData ,
        const TRemConAddress& /*aAddr*/)
    {
    FUNC_LOG;

    aData.Close();
    return KErrNone;
    }
// ---------------------------------------------------------
// CMMKeyBearer::SendCommand()
//
// ---------------------------------------------------------
//
TInt CMMKeyBearer::SendCommand(TUid /* aInterfaceUid */ ,
        TUint /* aOperationId */,
        TUint /* aTransactionId */,
        RBuf8& /* aData */,
        const TRemConAddress& /*aAddr*/)
    {
    FUNC_LOG;

    return KErrNone;
    }
// ---------------------------------------------------------
// CMMKeyBearer::GetResponse()
//
// ---------------------------------------------------------
//
TInt CMMKeyBearer::GetResponse(TUid& /* aInterfaceUid */,
        TUint& /* aTransactionId*/,
        TUint& /* aOperationId */,
        RBuf8& /* aData */,
        TRemConAddress& /*aAddr */)
    {
    FUNC_LOG;

    return KErrNone;
    }
// ---------------------------------------------------------
// CMMKeyBearer::GetCommand()
//
// ---------------------------------------------------------
//
TInt CMMKeyBearer::GetCommand(TUid& aInterfaceUid ,
        TUint& aTransactionId,
        TUint& aOperationId,
        RBuf8& aData,
        TRemConAddress& aAddr)
    {
    FUNC_LOG;

    aInterfaceUid = iInterfaceUid;
    aOperationId = iOperationId;

    // Pass ownership of this to RemCon.
    TRAPD_ERR( err, aData.CreateL(iData) );
    ERROR( err, "Remcon ownership change failed" );

    aAddr.BearerUid() = Uid();
    aAddr.Addr() = KNullDesC8();

    aTransactionId = iTransactionId;

    return KErrNone;
    }
// ---------------------------------------------------------
// CMMKeyBearer::SetSVKOperationIdAndData()
//
// ---------------------------------------------------------
//
void CMMKeyBearer::SetSVKOperationIdAndData(TInt aEnumValue)
    {
    FUNC_LOG;

    INFO_1( "SetSVKOperationIdAndData: enumValue = %d", aEnumValue );

    switch (aEnumValue)
        {
        case KPSVolumeUpPressed:
            iOperationId = ERemConCoreApiVolumeUp;
            iData.Format(KFormatString,ERemConCoreApiButtonPress);
            break;
        case KPSVolumeUpReleased:
            iOperationId = ERemConCoreApiVolumeUp;
            iData.Format(KFormatString,ERemConCoreApiButtonRelease);
            break;
        case KPSVolumeUpClicked:
            iOperationId = ERemConCoreApiVolumeUp;
            iData.Format(KFormatString,ERemConCoreApiButtonClick);
            break;
        case KPSVolumeDownPressed:
            iOperationId = ERemConCoreApiVolumeDown;
            iData.Format(KFormatString,ERemConCoreApiButtonPress);
            break;
        case KPSVolumeDownReleased:
            iOperationId = ERemConCoreApiVolumeDown;
            iData.Format(KFormatString,ERemConCoreApiButtonRelease);
            break;
        case KPSVolumeDownClicked:
            iOperationId = ERemConCoreApiVolumeDown;
            iData.Format(KFormatString,ERemConCoreApiButtonClick);
            break;

        case KPSVolumeNoKey:  // This should never execute
        default:
            iOperationId = aEnumValue;
            iData.Format(KFormatString,ERemConCoreApiButtonClick);
            break;

        }

    }
// ---------------------------------------------------------
// CMMKeyBearer::SetMediaKeyOperationIdAndData()
//
// ---------------------------------------------------------
//
void CMMKeyBearer::SetMediaKeyOperationIdAndData(TInt aEnumValue)
    {
    FUNC_LOG;

    INFO_1( "SetMediaKeyOperationIdAndData: enumValue = %d", aEnumValue );

    switch (aEnumValue)
        {

        SET_MEDIAKEY_OPID (EPlayReleased, ERemConCoreApiPausePlayFunction,
                           ERemConCoreApiButtonRelease);
        SET_MEDIAKEY_OPID (EPlayPressed,  ERemConCoreApiPausePlayFunction,
                           ERemConCoreApiButtonPress);
        SET_MEDIAKEY_OPID (EPlayClicked,  ERemConCoreApiPausePlayFunction,
                           ERemConCoreApiButtonClick);

        SET_MEDIAKEY_OPID (EStopReleased, ERemConCoreApiStop,
                           ERemConCoreApiButtonRelease);
        SET_MEDIAKEY_OPID (EStopPressed,  ERemConCoreApiStop,
                           ERemConCoreApiButtonPress);
        SET_MEDIAKEY_OPID (EStopClicked,  ERemConCoreApiStop,
                           ERemConCoreApiButtonClick);

        SET_MEDIAKEY_OPID (EForwardReleased, ERemConCoreApiForward,
                           ERemConCoreApiButtonRelease);
        SET_MEDIAKEY_OPID (EForwardPressed,  ERemConCoreApiForward,
                           ERemConCoreApiButtonPress);
        SET_MEDIAKEY_OPID (EForwardClicked,  ERemConCoreApiForward,
                           ERemConCoreApiButtonClick);

        SET_MEDIAKEY_OPID (ERewindReleased, ERemConCoreApiRewind,
                           ERemConCoreApiButtonRelease);
        SET_MEDIAKEY_OPID (ERewindPressed,  ERemConCoreApiRewind,
                           ERemConCoreApiButtonPress);
        SET_MEDIAKEY_OPID (ERewindClicked,  ERemConCoreApiRewind,
                           ERemConCoreApiButtonClick);

        SET_MEDIAKEY_OPID (EFastForwardReleased, ERemConCoreApiFastForward,
                           ERemConCoreApiButtonRelease);
        SET_MEDIAKEY_OPID (EFastForwardPressed,  ERemConCoreApiFastForward,
                           ERemConCoreApiButtonPress);
        SET_MEDIAKEY_OPID (EFastForwardClicked,  ERemConCoreApiFastForward,
                           ERemConCoreApiButtonClick);

        SET_MEDIAKEY_OPID (EBackwardReleased, ERemConCoreApiBackward,
                           ERemConCoreApiButtonRelease);
        SET_MEDIAKEY_OPID (EBackwardPressed,  ERemConCoreApiBackward,
                           ERemConCoreApiButtonPress);
        SET_MEDIAKEY_OPID (EBackwardClicked,  ERemConCoreApiBackward,
                           ERemConCoreApiButtonClick);


        // This should never execute
        default:
            iOperationId = aEnumValue;
            iData.Format(KFormatString,ERemConCoreApiButtonClick);
            break;

        }
    }
// ---------------------------------------------------------
// CMMKeyBearer::ReceivedKeyEvent()
//
// ---------------------------------------------------------
//
void CMMKeyBearer::ReceivedKeyEvent(TInt aEnumValue, TInt aKeyType)
    {
    FUNC_LOG;

    INFO_3( "Received key: enumValue = %d, keyType = %d, usbFileTransfer = %d",
        aEnumValue, aKeyType, iUSBFileTransfer );

    // Mediakeys must be disabled when MTP (Music Transfer) is happening.
    if (aKeyType == EFileTransferStatus)
        {
        iUSBFileTransferObserver->Start(); // Ensure first that no status change gets missed
        iUSBFileTransfer = iUSBFileTransferObserver->GetValue(); // Read the MTP status
        return;
        }
    if ((aKeyType == EMediaKeys) &&
        (iUSBFileTransfer == KUsbPersonalityIdMTP ||
         iUSBFileTransfer == KUsbPersonalityIdPCSuiteMTP))
        {
        iMediaKeyObserver->Start();
        return;
        }

    // Check for keypadlock if the events are from device keypad
    // If events are from accessory device,then do not check for keypadlock
    if (aKeyType != EAccessoryVolumeKeys && aKeyType != ESideVolumeKeys )
        {
               
        TInt err=iKeyguardAccess->ShowKeysLockedNote();

        if (err==KErrNone)
            {
            // Device is locked , Discard the key event

            //Start the listener once again
            
            if (aKeyType == EMediaKeys)
                {
                iMediaKeyObserver->Start();
                }

            return;
            }
        }

    /* To keep the background light on */
    User::ResetInactivityTime();

    // 1. Interface ID is required when Getcommand is called
    iInterfaceUid = TUid::Uid(KRemConCoreApiUid); // from Remconcoreapi.h

    // 2. Transaction ID
    iTransactionId = Observer().NewTransactionId();

    // 3. Addr
    TRemConAddress addr;
    addr.BearerUid() = Uid();
    addr.Addr() = KNullDesC8();

    // 4 & 5. Operation ID and Data
    if ((aKeyType == ESideVolumeKeys) || (aKeyType == EAccessoryVolumeKeys))
        {
        SetSVKOperationIdAndData(aEnumValue);
        }
    else
        {
        SetMediaKeyOperationIdAndData(aEnumValue);
        }

    // Msgtype  is ERemConCommand
    TInt aError = Observer().NewCommand(addr);

    //Start the listener once again
    if (aKeyType == ESideVolumeKeys)
        {
        iMMKeyBearerObserver->Start();
        }
    else if (aKeyType == EMediaKeys)
        {
        iMediaKeyObserver->Start();
        }
    else if (aKeyType == EAccessoryVolumeKeys)
        {
        iAccessoryVolKeyObserver->Start();
        }
    }
// ---------------------------------------------------------
// CMMKeyBearer::SecurityPolicy()
//
// ---------------------------------------------------------
//
TSecurityPolicy CMMKeyBearer::SecurityPolicy() const
    {
    FUNC_LOG;

    return TSecurityPolicy(ECapabilityLocalServices);
    }
// ---------------------------------------------------------
// CMMKeyBearer::ClientStatus()
//
// ---------------------------------------------------------
//
void CMMKeyBearer::ClientStatus(TBool /*aControllerPresent*/,
                                TBool /*aTargetPresent*/)
    {
    FUNC_LOG;

    //Pass
    }
// ---------------------------------------------------------
// CMMKeyBearer::GetInterface()
//
// ---------------------------------------------------------
//
TAny* CMMKeyBearer::GetInterface(TUid aUid)
    {
    FUNC_LOG;

    TAny* ret = NULL;
    if ( aUid == TUid::Uid(KRemConBearerInterface1) )
        {
        ret = reinterpret_cast<TAny*>(
                    static_cast<MRemConBearerInterface*>(this)
                    );
        }
    return ret;

    }


// End of file
