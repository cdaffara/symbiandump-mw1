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
* Description:  A configurable key translation table class.
*
*/



#ifndef KEFMAPIMPL_H
#define KEFMAPIMPL_H

// INCLUDES
#include <e32base.h>
#include <w32adll.h>
#include <keffactory.h>
#include "kefmapinternal.hrh"
#include "kefsubscriber.h"

// DATA TYPES

enum TKefPost
    {
    EKefPostRaw = 0,
    EKefPostKey = 1
    };

/**
* Flag for short key event.
* @since Series 60 3.2
*/
#define KKefIsShortKey 0x01

/**
* Flag for long key event.
* @since Series 60 3.2
*/
#define KKefIsLongKey 0x02

/**
* Flag for long key press.
* @since Series 60 3.2
*/
#define KKefIsLongPress 0x04

/**
* Flag for long key event only.
* @since Series 60 3.2
*/
#define KKefIsLongOnly 0x08

/**
* Flag for special combo key event; no down or up events sent for the combo keys.
* @since Series 60 3.2
*/
#define KKefIsDelayedOnCombo 0x10

/**
* Structure encapsulating key event type.
* @since Series 60 3.2
*/
struct TKefKeyEvent
    {
    TUint16 iShortPress;
    TUint16 iLongPress;
    TUint8  iEventType;
#ifdef RD_TACTILE_FEEDBACK
    TUint16 iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
    //
    // iEventType does not contain the KKefType-flags directly.  Instead
    // the necessary information is bitflagged using the KKefIs* flags, 
    // as follows:
    //
    // bit 0: 0 when Short keypress is sent as raw, 
    //        1 when it is sent as key (KKefIsShortKey)
    // bit 1: 0 when Long keypress is sent as raw, 
    //        1 when it is sent as key  (KKefIsLongKey)
    // bit 2: 0 when the scancode has no long keypresses defined 
    //        (required for combos to work) (KKefIsLongPress)
    // bit 3: 0 when the short keypress before a long one can be generated,
    //        1 when it must be delayed until press is long (KKEfIsLongOnly)
    // bit 4: reserved
    // bit 5: reserved
    // bit 6: reserved
    // bit 7: reserved
    //
    // This means that the default event type (=0) is 
    // KKefTypeShort | KKefTypeRaw, with successful mappings sent out 
    // immediately on the down event.
    };

/**
* Encapsulates key event data during initialization of the key table.
* @since Series 60 3.2
*/
struct TKefKeyDefinition
    {
    CArrayFixFlat<TUint16>* iScanCodes;
    TUint16 iKeyCode;
    TUint16 iDeviceMode;
    TUint8 iEventType;
#ifdef RD_TACTILE_FEEDBACK
    TUint16 iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
    };

/**
* Key Beacon contains the on-time information necessary for posting
* out correct kind of events.
* @since Series 60 3.2
*/
struct TKefKeyBeacon
    {
    TUint16 iKeyCode;
    TUint8  iEventType;
#ifdef RD_TACTILE_FEEDBACK
    TUint16 iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
    };

/**
* Structure containing repeat rate definition.
*/
struct TKefKeyRepeatRate
    {
    TUint16 iMode;
    TUint16 iModeMask;
    TUint8 iRate;
    };
        

class MKefProvider;
        
// CLASS DECLARATION

/**
* CKefMapImpl is the main class of the Key Event Framework.
*
* It is responsible for configuring the keyboard mapping table(s) from
* resource files; listening to raw events from the device; performing
* the key translation and subsequently passing the resolved event for 
* Window Server, which further passes it to interested clients.
*
* @lib KeyEventFw.lib
* @since Series 60 3.2
*/
NONSHARABLE_CLASS( CKefMapImpl )
    : public CKefMap,
      public MKefPropertyResponder
    {
    public: // Construction and destruction

        /**
         * 2-phased contructor
         */
        static CKefMapImpl* NewL();

        /**
        * Destructor.
        */
        ~CKefMapImpl();

    public: // Functions from base classes

        /**
         * From CKefMap
         * @see keffactory.h
         */
        void SetProvider( MKefProvider& aProvider );
        
        /**
         * From CKefMap
         * @see keffactory.h
         */
        TBool IsKeyEventMapEnabled() const;
        
        /**
         * From CKefMap
         * @see keffactory.h
         */
        TBool OfferRawEvent( const TRawEvent& aRawEvent, TBool aSilentEvent );        

        /**
        * From MKefPropertyResponder, handles changes in monitored 
        * Publish & Subscribe variables.
        *
        * @param aCategory Category of the variable that was modified.
        * @param aKey Key of the variable that was modified.
        */
        virtual void HandlePropertyChangedL( 
            const TUid& aCategory, 
            const TUint aKey );   

    private:

        /**
        * Loads a resource file and sets up the translation table.
        * @param aConfigFile Name of the configuration resource file
        */
        void InitializeKeyTableFromResourceL( 
            const TDesC& aConfigFile );

        /**
        * Sets the repeat rate for long key presses.
        * @param aRepeatRate repeat rate.
        */
        void SetRepeatRate( TUint8 aRepeatRate );

        /**
        * Sets a key mapping for scan code(s) in a given devicemode. 
        * @param aKey A struct containing all information to set up 
        *        the mapping.    
        */
        void SetKeyMappingL( const TKefKeyDefinition& aKey );

        /**
        * This method creates an array containing indices of all device 
        * modes (iKeyMap rows) that are affected by the processed mapping.
        * @param aModes it will contain array of modes.
        * @param aMode device mode identifier (from the resource file).
        */
        void ResolveDeviceModeL( RArray<TUint>& aModes, TUint16 aMode );

        /**
        * This method converts the eventtype-flags (KKefType*) used in the resource file
        * into TKefKeyEvent::iEventType.  Notice that the resource file
        * definitions are translated into KKefIs* flags.
        *
        * @param aEventType Event type indentifier (from the resource file).
        * @return event type data in a format that can be put in TKefKeyEvent.
        */
        TUint8 ResolveEventType( TUint8 aEventType );
    
        /**
        * Performs a lookup on key map to find out the correct
        * mapping for a (single) scan code in a given device mode
        * @param aScanCode The scan code that is to be looked up.
        * @param aDeviceMode The device mode for which the mapping is done.
        * @returns result of the mapping
        */
        TKefKeyBeacon GetKeyMapping( TUint16 aScanCode, TUint16 aDeviceMode );

        /**
        * Performs a search on combination key mapping to find out the correct
        * mapping for a combination of scan codes, if any.
        * @param aComboCandidate reference to iDownEvents.
        * @returns result of the mapping
        */
        TKefKeyBeacon GetComboMapping( RArray<TUint32>& aComboCandidate );

        /**
        * Resolves correct keycode for the last keypress / combo that 
        * came in / is coming in.
        * @return result of the mapping.
        */
        TKefKeyBeacon ResolveMapping();

        /**
        * A callback for the CPeriodic timer that tracks the length of the
        * keypress,and subsequently the amount of repeats (if any).
        *
        * @param aMyPtr a pointer to an instance of this class.
        * @return 1 if the callback should be called again, 0 otherwise.
        */
        static TInt KeyPressDuration( TAny *aMyPtr );
        
        /**
        * Helper function for posting the re-mapped event.
        *
        * @param aType type of event to send;
        *         default is TKefPost::ERaw for a raw event
        *         TKefPost::Ekey posts a key event
        */
        
        void PostEvent(TUint aType = 0);

        /**
        * A macro that stops the timer and resets all variables.
        */
        void CancelProcessingKeyPress();
        
        TKefKeyBeacon GetFirstKey(TUint32 aComboFirstCodeCandidate );
    
    private:
    
        /**
        * Constructor.
        */
        CKefMapImpl();

        /**
        * 2nd phase constructor.
        */
        void ConstructL();

    
        /**
        * This is the 'main' translation table that corresponds to a 
        * M x N matrix, where M is the number of scan codes, and N is 
        * the number of device modes.
        *
        * Each device mode is identified by a bit-encoded integer, and each
        * device mode's key mapping can be found from a separate row in 
        * iKeyMap.
        *
        * The first row, ie. iKeyMap[i][0] contains the 'Default' mode mapping.
        * If this mapping is not EKeyNull (the initial value), then it is 
        * returned regardless of what the actual device mode is, IF non-EKeyNull
        * mapping for that mode is not found.  If it is EKeyDisabled, then the 
        * raw event is consumed but nothing is passed on.
        */
        TKefKeyEvent iKeyMap[ KKefKeyMapColumns ][ KKefKeyMapRows ];
    
        /**
        * This array of arrays of integers stores combined keypresses. For 
        * each combo,  *(*iCombos)[i], there's an array whose first two
        * elements contain the key code AND the device mode this 
        * combo is active in, AND the event type (long or short keypress). 
        * Subsequent elements contain the participating scan codes.
        *
        * Information about the device mode and the event type is encoded into
        * the higher 16 bits of the keycode value.
        */
        RArray<RArray<TUint32>*>* iCombos;
    
        /**
        * Support for combined keypresses, this array accumulates down events 
        * until an up event comes and resets the array. The contents of this 
        * array are matched against each element of iCombos.
        */
        RArray<TUint32> iDownEvents;

        /**
        * Support for sending out raw events, this array accumulates the
        * results for mapped DOWN raw events.  Then when an UP raw event
        * is got from the keyb driver, this array is used to make sure that 
        * for every event that was sent out as raw DOWN there will 
        * also be the raw UP.
        */
        RArray<TUint32> iUpEvents;

        /**
        * Repeat rate definitions.
        */
        RArray< TKefKeyRepeatRate > iRepeatRates;
        
        /**
        * Contains the character value to be sent in PostKeyEvent, if any.
        * Possible values:
        *  EKeyNull: default, disables PostEvent.
        *  KKefKeyDisabled: disables PostEvent, and indicates that 
        *  the current raw event should be consumed.
        *  any other value: the key code to be sent in PostEvent().
        */
        TKefKeyBeacon iKeyBeacon;
    
        /**
        * EFalse = key, ETrue = raw
        *
        * This variable is currently OBSOLETE; i'm leaving it in nonetheless,
        * so someone remembers to add KKefTypeRepeatable and associated functionality.
        *
        */
        TBool iSendRepeats; 

        /**
        * Scan code of the last incoming down or up type raw event, 
        * excluding raw events generated by this class.
        */
        TUint32 iCurrentScanCode;

        /**
        * The device mode.  This is updated via a P&S key.
        */ 
        TUint16 iCurrentDeviceMode;

        /**
        * Timer for the latest incoming down type raw event    
        */
        CPeriodic* iDownTimer;

        /**
        * Interval to poll.
        */
        TTimeIntervalMicroSeconds32 iKeyPollInterval;

        /**
        * Key press duration.
        */
        TReal32 iKeyPressDuration;
        
        /**
        * Support for key presses that must not generate
        * any event until the key press type is fully resolved.
        * Currently this is used for KKefTypeLongOnly.
        */
        TKefKeyBeacon iPostMeOnUp;

        /**
        * Safeguard against active scheduler problems.
        */
        TBool iIsCanceled; 

        /**
        * ETrue when iDownTimer > KLongPressThreshold.
        */
        TBool iIsLongKeyPress;

        /**
        * For ignoring self-generated raw events.
        */
        TInt iScanCodeIJustGenerated;

        /**
        * Key repeat rate.
        */
        TUint8 iRepeatRate;
        
        /**
        * Amount of repeats.
        */
        TInt32 iRepeats;

        /**
        * Boolean flag indicating whether the processed event 
        * should be consumed
        */
        TBool iConsume;
    
        /**
        * Owned array of Publish & Subscribe variable monitors.
        */
        RPointerArray< CKefSubscriber > iKefSubscribers;
    
        /**
        * Not owned event generator privider.
        */
        MKefProvider* iProvider;
        
        // ETrue if key event map is enabled.
        TBool iKeyEventMapEnabled;
        
        // ETrue if a special combo event is being handled
        TBool iDelayedCombo;
        
        TBool iPostPendingDownEvents;
        
        TBool iIsPartialMatch;
        
        TBool iCompleteDelayedComboMatch;
        
        TBool iIsDownEvent;
        
        TBool iIsCombo;
        
        RArray<TUint32> iRawEventsNotPosted;
        
        TInt iPostedDelayedScanCode;
      
    };

#endif // KEFMAPIMPL_H

// End of File
