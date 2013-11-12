/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Animation plugin implementation file
*       
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <e32cmn.h>
#include <mediakeysinternalpskeys.h>
#include <mmkeybearerscancodes.h>
#include <remconcoreapi.h>
#include <uikon.hrh>

#include "KeyPublisher.h"

// CONSTANTS

const TInt KScanCodeMask = 0xFFFF;

#if defined(__WINS__)
const TInt KVolumeUpKeyEmulator   = 0xAC;
const TInt KVolumeDownKeyEmulator = 0xAD;
#endif
// Change these values once peoblem with keys is fixed
const TInt KVolumeUpKeyHardware   = EStdKeyIncVolume;   //0xa3; 
const TInt KVolumeDownKeyHardware = EStdKeyDecVolume;  //0xa2; 

const TInt KMusicPlayHarware     = EStdKeyApplication2;
const TInt KMusicStopHarware     = EStdKeyApplication3;
const TInt KMusicForwardHardware = EStdKeyApplication4;
const TInt KMusicRewindHardware  = EStdKeyApplication5;

// Scan codes in this table are blocked by the Key Publisher Anim plugin
// Table KBlockedKeyCodes[] and KBlockedKeyCodesEvents[][3] must have same 
// number of rows and match correspondingly
const TInt KBlockedKeyCodes[]=
    {
#if defined(__WINS__)    
    /*0*/KVolumeUpKeyEmulator,
    /*1*/KVolumeDownKeyEmulator,
#endif
    /*2*/KVolumeUpKeyHardware,
    /*3*/KVolumeDownKeyHardware,

    /*4*/ KMusicPlayHarware,    
    /*5*/ KMusicStopHarware,    
    /*6*/ KMusicForwardHardware,
    /*7*/ KMusicRewindHardware 

    };
const TInt KBlockedKeyCodesEvents[][3]=
    {     /* Press - 0 */       /* Release - 1 */      /* Click - 2 */
#if defined(__WINS__)    
    /*0*/{KPSVolumeUpPressed,   KPSVolumeUpReleased,   KPSVolumeUpClicked },
    /*1*/{KPSVolumeDownPressed, KPSVolumeDownReleased, KPSVolumeDownClicked },
#endif
    /*2*/{KPSVolumeUpPressed,   KPSVolumeUpReleased,   KPSVolumeUpClicked },
    /*3*/{KPSVolumeDownPressed, KPSVolumeDownReleased, KPSVolumeDownClicked }, 

    /*4*/{EPlayPressed,         EPlayReleased,         EPlayClicked },
    /*5*/{EStopPressed,         EStopReleased,         EStopClicked },
    /*6*/{EFastForwardPressed,  EFastForwardReleased,  EForwardClicked },
    /*7*/{ERewindPressed,       ERewindReleased,       EBackwardClicked }

    };

const TInt KBlockedKeyCodeTableSize = sizeof(KBlockedKeyCodes) / sizeof(TInt);

const TInt KTimerMilliSecond = 1000; // in micro seconds

_LIT_SECURITY_POLICY_C1(KKeyPublisherReadPolicy,  ECapability_None);
_LIT_SECURITY_POLICY_C1(KKeyPublisherWritePolicy, ECapabilityWriteDeviceData);

// Following constants can subject change if duration is not suitable

const TInt KFirstTimerExpiryInterval = 600 * KTimerMilliSecond; // 600mSec
// For second time onwards ,the duration of the time interval, is below
// Should not be grater than 1 Minute
const TInt KTimerExpiryInterval      = 500 * KTimerMilliSecond; // 500mSec

// -----------------------------------------------------------------------
// CKeyPublisher::CKeyPublisher
// Description: Constuctor
// -----------------------------------------------------------------------
//
CKeyPublisher::CKeyPublisher()
: iTimerStarted (EFalse) ,iPressEventSent(EFalse) ,iReleaseEventSent(ETrue),
  iTimerCount(0), iConnected(EFalse)
    {
    }
// -----------------------------------------------------------------------
// CKeyPublisher::~CKeyPublisher
// Description: Destructor
// -----------------------------------------------------------------------
//
CKeyPublisher::~CKeyPublisher()
    {
    
    iSoundSession.Close();
    
    if (iFunctions)
        {
        //switch OFF the animation raw event receiving 
        iFunctions->GetRawEvents(EFalse);
        }
    if(iTimer)
        {
        delete iTimer;
        }
    }
// -----------------------------------------------------------------------
// CKeyPublisher::ConstructL   (part of CWindowAnim)
// Description: Enable the Raw Event handling from Win server. and 
//              create timer (not starting)
// -----------------------------------------------------------------------
//
void CKeyPublisher::ConstructL(TAny* /*aArgs*/, TBool /*aHasFocus*/)
    {
    
    /* iFunction: protected member of CAnim /CWindowAnim
     * Pointer to a class containing functions implemented by the window 
     * server. These are available to any CAnim derived class. Note that 
     * this value is automatically set for you by the animation framework. 
     * we do not need to assign a value to this pointer.
     */
    
    /* This function switches animation raw event handling on and off. If 
     * raw event handling is switched on(ETrue), then raw events, 
     * e.g. pen events, key presses, power events etc., are all offered 
     * to the animation event handling code - MEventHandler::OfferRawEventL().
     */ 
    iFunctions->GetRawEvents(ETrue);
    
    TInt error = iSoundSession.Connect();
    if (error == KErrNone)
        {
        iConnected = ETrue;
		}
    
    // Timer for implementing Pressed/Released/Clicked feature.
    iTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    
    //Define the P&S for Volume keys and Media Keys
    RProperty::Define(KPSUidMediaKeysEventNotifier, KMediaKeysVolumeKeyEvent, 
           RProperty::EInt, KKeyPublisherReadPolicy, KKeyPublisherWritePolicy);
    RProperty::Define(KPSUidMediaKeysEventNotifier, KMediaKeysControlKeyEvent,
           RProperty::EInt, KKeyPublisherReadPolicy, KKeyPublisherWritePolicy);
    // Define Vol Sync Notifier in P & S
    RProperty::Define(KPSUidMediaKeysEventNotifier,KMediaKeysCurrentVolume, 
           RProperty::EInt, KKeyPublisherReadPolicy, KKeyPublisherWritePolicy);
    // Define P&S Key for Volume keys from Accessory device
    RProperty::Define(KPSUidMediaKeysEventNotifier,KMediaKeysAccessoryVolumeEvent, 
           RProperty::EInt, KKeyPublisherReadPolicy, KKeyPublisherWritePolicy);
    }
// -----------------------------------------------------------------------
// CKeyPublisher::IsBlockedKeyCode 
// Description: Search the list of blocked keys by comparing scan codes
// return : ETrue if key is blocked, else EFalse
// -----------------------------------------------------------------------
//
TBool CKeyPublisher::IsBlockedKeyCode(TInt aScanCode)
    {
    for (TInt index=0; index < KBlockedKeyCodeTableSize; index++)
        {
        if (aScanCode == KBlockedKeyCodes[index])
            {
            // Found the Key 
            return ETrue;
            }
        }
    // Not found
    return EFalse;
    }
// -----------------------------------------------------------------------
// CKeyPublisher::GetEnumVal
// Description: Based on the scancode and event, find the Enum value(which
//              are declared in the TRemConCoreApiButtonAction).
//              MMBearerPlugin will decript this enum value.
// return : 
// -----------------------------------------------------------------------
//
TInt CKeyPublisher::GetEnumVal(TInt aScanCode, 
                               TRemConCoreApiButtonAction aEvent)
    {
    TInt result = KPSVolumeNoKey;
    TInt index;

    // Find the Index of the Scan code in the KBlockedKeyCodes[] 
    // and use the same index in KBlockedKeyCodesEvents[][] 
    // for getting enum values
    for (index=0; index < KBlockedKeyCodeTableSize; index++)
        {
        if (aScanCode == KBlockedKeyCodes[index])
            {
            break;
            }
        }

    if (index == KBlockedKeyCodeTableSize)
    {
        // Not found 
        return result;
    }

    result = KBlockedKeyCodesEvents[index][aEvent];
    return result;
    }
// -----------------------------------------------------------------------
// CKeyPublisher::PublishEvent
// Description: Publish the event to P&S based on the Scancode, event
// return : 
// -----------------------------------------------------------------------
//
void CKeyPublisher::PublishEvent(TInt aScanCode, 
                                 TRemConCoreApiButtonAction aEvent)
    {
    TInt uidMMKey;
    
    if ( 
#if defined(__WINS__)        
         (aScanCode == KVolumeUpKeyEmulator)   ||
         (aScanCode == KVolumeDownKeyEmulator) ||
#endif         
         (aScanCode == KVolumeUpKeyHardware)   ||
         (aScanCode == KVolumeDownKeyHardware)  
       )
        {
        uidMMKey = KMediaKeysVolumeKeyEvent; // Volume Keys - Up / down
        }
    else
        {
        uidMMKey = KMediaKeysControlKeyEvent; //Media Keys - play ,stop ,etc
        }

    iProperty.Set(KPSUidMediaKeysEventNotifier,
                  uidMMKey,
                  GetEnumVal(aScanCode, aEvent));

    }   
    
// -----------------------------------------------------------------------
// CKeyPublisher::OfferRawEvent
// Description: This function starts getting called when animation raw 
//              event is switched on by calling GetRawEvents(ETrue)
// return : ETrue  - if key is handled and blocks it to applications
//          EFalse - if not handled here, Apps/other anim plugins will 
//               receive this key event.
// -----------------------------------------------------------------------
//
TBool CKeyPublisher::OfferRawEvent(const TRawEvent &aRawEvent)
    {
    TBool returnValue = EFalse;        // Key not blocked
    TInt scan;
    
    switch (aRawEvent.Type())
        {
        case TRawEvent::EKeyDown:
            {
            scan = aRawEvent.ScanCode() & KScanCodeMask;
            
            if (IsBlockedKeyCode(scan))
                {
                if (iConnected)
                    {
                    iSoundSession.KeyPressed(scan);  	
                    }
                
                if ((! iTimerStarted) && (iReleaseEventSent))
                    {
                    
                    if ((scan != KMusicForwardHardware) && (scan != KMusicRewindHardware))
                    {
	                    // Send Clicked Event
	                    PublishEvent(scan,ERemConCoreApiButtonClick);
                    }

                    //Start Timer
                    iTimer->Start(KFirstTimerExpiryInterval,
                                  KTimerExpiryInterval, 
                                  TCallBack(TimerCallback, this));
                    iTimerStarted  = ETrue;
                    iPrevScanCode    = scan;
                    iPressEventSent = EFalse;
                    returnValue     = ETrue; // Block   
                    iReleaseEventSent = EFalse; // For blocking multiple key
                                                // presses
                    }
                }
            }
            break;
        case TRawEvent::EKeyUp:
            {
            scan = aRawEvent.ScanCode() & KScanCodeMask;

            if ((iPrevScanCode == scan) && IsBlockedKeyCode(scan))
                { 
            
                // Clicked Scenario
                if ( iPressEventSent == EFalse ) 
                    {
                    
                    if ((iPrevScanCode == KMusicForwardHardware) ||  
                        (iPrevScanCode == KMusicRewindHardware))
                    {
	                    // Send Clicked Event
	                    PublishEvent(iPrevScanCode, ERemConCoreApiButtonClick);
                    }
                    
                    // Since Click event has been sent when pressed down,
                    // (except Frwd & Rewind)
                    // Just stop the timer
                    if (iTimerStarted)
                        {
                        iTimer->Cancel();
                        iTimerStarted  = EFalse;
                        }
                    iReleaseEventSent = ETrue;
                    iPrevScanCode     = 0;
                    returnValue       = ETrue; //Block it
                    }
                else  // Released Scenario
                    {
                    PublishEvent(scan,ERemConCoreApiButtonRelease);

                    iPrevScanCode   = 0;
                    iPressEventSent = EFalse;
                    returnValue     = ETrue; //Block it
                    iReleaseEventSent = ETrue;

                    }
                    
                } // end of if ((iPrevScanCode == scan) && IsBlockedK...
            }
            break;
        default:
            break;
        }
    return returnValue; // to allow the event to be passed back to app
    }
// -----------------------------------------------------------------------
// CKeyPublisher::HandleTimerEvent
// Description: When Timer is expired, this function is called from 
//              callback function. If the timer is expired for the first
//              time then KeyPressed event is published to P&S. And 
//              second and further expires, only count is incremented.
//              Once the count reaches some predefined value, Released 
//              Key event is posted to P&S. and timer is switched off.
// return : 
// -----------------------------------------------------------------------
//
void CKeyPublisher::HandleTimerEvent()
    {
    
    // Send Pressed Key Event
    PublishEvent(iPrevScanCode,ERemConCoreApiButtonPress);  
    iPressEventSent = ETrue;
    iTimer->Cancel();
    iTimerStarted  = EFalse;

    }

// -----------------------------------------------------------------------
// CKeyPublisher::TimerCallback
// Description: When Timer is expired, this function is called 
// return : 
// -----------------------------------------------------------------------
//
TInt CKeyPublisher::TimerCallback(TAny* aThis)
    {
    
    //STATIC_CAST(CKeyPublisher*,aThis)->HandleTimerEvent();
    static_cast<CKeyPublisher*>(aThis)->HandleTimerEvent();
    return 0;
    
    }

// -----------------------------------------------------------------------
// CKeyPublisher::Redraw   (part of CWindowAnim - pure virtual function) 
// Description: 
// -----------------------------------------------------------------------
//
void CKeyPublisher::Redraw()
    {
    //Pass
    }
// -----------------------------------------------------------------------
// CKeyPublisher::FocusChanged   (part of CWindowAnim - pure virtual function)
// Description: 
// -----------------------------------------------------------------------
//
void CKeyPublisher::FocusChanged(TBool /*aState*/)
    {
    //Pass
    }
// -----------------------------------------------------------------------
// CKeyPublisher::CommandReplyL  (part of CAnim - pure virtual function)
// Description: 
// -----------------------------------------------------------------------
//
TInt CKeyPublisher::CommandReplyL(TInt /*aOpcode*/, TAny* /*aArgs*/)
    {
    //Pass
    return KErrNone;
    }
// -----------------------------------------------------------------------
// CKeyPublisher::Command  (part of CAnim - pure virtual function)
// Description: 
// -----------------------------------------------------------------------
//
void CKeyPublisher::Command(TInt /*aOpcode*/, TAny* /*aArgs*/)
    {
    //Pass
    }
// -----------------------------------------------------------------------
// CKeyPublisher::Animate  (part of CAnim - pure virtual function)
// Description: 
// -----------------------------------------------------------------------
//
void CKeyPublisher::Animate(TDateTime* /*aDateTime*/)
    {
    //Pass
    }

// End of File
