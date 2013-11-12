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
*       Declaration of class CKeyPublisher.
*       
*
*/

#ifndef __KEYPUBLISHER_H__
#define __KEYPUBLISHER_H__

// INCLUDE
#include <w32adll.h>
#include <e32property.h>
#include <remconcoreapi.h>
#include "KeyPublisherSoundServerSession.h"

class RKeyPublisherSoundServerSession;
/**
*  CKeyPublisher
*  Used by the Window server. This class implements the Raw Key event 
*  handling feature. and publishes to P&S .
*
*  @lib KeyPublisherPlugin.dll
*  @since  3.0
*/
NONSHARABLE_CLASS(CKeyPublisher) : public CWindowAnim
    {
public:   // Constructor and Destructor
    /**
    * Constructor.
    */
    CKeyPublisher();

    /**
    * Destructor.
    */
    virtual ~CKeyPublisher();

public: // New Functions
    /**
    * HandleTimerEvent
    * When Timer is expired, this function is called from callback function. 
    * @since 3.0
    * @param None
    * @return None
    */
    void HandleTimerEvent();
        
public: // Base Class Functions - CAnim

    /** @see CAnim::CommandReplyL(TInt aOpcode, TAny *aArgs) */
    TInt CommandReplyL(TInt aOpcode, TAny *aArgs);
    
    /** @see CAnim::Command(TInt aOpcode, TAny *aArgs) */
    void Command(TInt aOpcode, TAny *aArgs);
    
    /** @see CAnim::Animate(TDateTime *aDateTime) */
    void Animate(TDateTime *aDateTime);

public: // Base Class Functions - CWindowAnim

    /** @see CWindowAnim::ConstructL(TAny *aArgs, TBool aHasFocus) */
    void ConstructL(TAny *aArgs, TBool aHasFocus);
    
    /** @see CWindowAnim::Redraw() */    
    void Redraw();
    
    /** @see CWindowAnim::FocusChanged(TBool aState) */    
    void FocusChanged(TBool aState);
    
public: // Base Class Functions - MEventHandler    

    /** @see MEventHandler::OfferRawEvent(const TRawEvent &aRawEvent) */    
    TBool OfferRawEvent(const TRawEvent &aRawEvent);
    
private:  

    /**
    * IsBlockedKeyCode () : Compares the scancode with the list of 
    * blocked keys and returns ETrue if key is blocked, else EFalse
    */
    TBool IsBlockedKeyCode(TInt aScanCode);
    
    /**
    * TimerCallback () : this is the callback function called from timer.
    */
    static TInt TimerCallback(TAny* aThis);

    /**
    * GetEnumVal() : Function returns the appropriate CoreApi enumarated
    * value for the given ScanCode and Event
    */
    TInt GetEnumVal(TInt aScanCode, TRemConCoreApiButtonAction aEvent);
    
    /**
    * PublishEvent() : Publish the Event to P&S,
    */
    void PublishEvent(TInt aScanCode, TRemConCoreApiButtonAction aEvent);
        
private:     // Data
    // For posting event to P&S 
    RProperty iProperty;
    
    // For getting timer ticks/events 
    CPeriodic* iTimer;
    
    // To store the scan code when key is pressed. This is used when key 
    // is released.
    TInt iPrevScanCode;
    
    // To keep track whether timer is already started or not.
    TBool iTimerStarted;
    
    // To know whether Pressed event sent or not.
    TBool iPressEventSent; 
    
    // To know whether Released event sent or not- blocking simultaneous key press
    TBool iReleaseEventSent; 
    
    // How many times Timer expired.
    TInt iTimerCount;
    
    // For sending sound event
    RKeyPublisherSoundServerSession iSoundSession;
    
    // To know whether it is connected to sound server or not
    TBool iConnected;
    };


#endif  // __KEYPUBLISHER_H__

// End of File
