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
* Description:  An implementation of a configurable key translation map.
*
*/


// INCLUDES
#include "kefmapimpl.h"
#include <e32std.h>
#include <e32math.h>
#include <barsc.h>
#include <barsread.h>
#include <bautils.h>
#include "kefresource.h"
#include <AknKeyEventMap.rsg>
#include "keflogger.h"
#include "kefprovider.h"

// CONSTANTS
#define STRIP_MODS  &0x0000FFFF
#define GET_MODS    &0xFFFF0000

// Key event map resource file.
_LIT( KKefKeyTableRsc, "z:\\resource\\AknKeyEventMap.rsc" );

// Publish & Subscribe device mode category.
const TInt32 KUidWinservCategoryValue = 0x10003B20;

// Publish & Subscribe device mode category as TUid.
const TUid KUidWinservCategory = { KUidWinservCategoryValue };

// Always pass policy.
_LIT_SECURITY_POLICY_PASS(KAlwaysPassPolicy);

// Only system application can access.
_LIT_SECURITY_POLICY_S0(KSysapOnlyPolicy, 0x100058F3);

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CKefMapImpl::NewL
// -----------------------------------------------------------------------------
//
CKefMapImpl* CKefMapImpl::NewL()
    {
    CKefMapImpl* self = new (ELeave)CKefMapImpl();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::CKefMapImpl
// -----------------------------------------------------------------------------
//
CKefMapImpl::CKefMapImpl()
    : iDownEvents( KKefMaxComboKeys ), iUpEvents( KKefMaxComboKeys )
    {    
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::ConstructL
//
// Initializes member variables, reads the default keytables from a resource 
// file, instantiates P&S subscriber and opens the door for raw events.
// -----------------------------------------------------------------------------
//
void CKefMapImpl::ConstructL()
    {

    if ( KKefMaxDeviceMode < 1 || KKefMaxDeviceMode > 15)
        {
        __KEFLOGSTRING("CKefMapImpl::ConstructL ERROR: must have at least 1 and at most 15 device modes");
        User::Leave(KErrGeneral);
        }
    //NEW CODE
    iDelayedCombo = EFalse;
    iIsPartialMatch = EFalse;
    iCompleteDelayedComboMatch = EFalse;
    iPostPendingDownEvents = EFalse;
    //END NEW CODE
    iCurrentDeviceMode = KKefModeDefault;
    iKeyBeacon.iKeyCode = EKeyNull;
    iKeyBeacon.iEventType = 0;
    iDownTimer = CPeriodic::New( CActive::EPriorityLow );
    iCombos = new (ELeave) RArray<RArray<TUint32>*>( KKefMaxComboKeys );
    
    iRepeatRate = KKefRepeatRate;
    iKeyPollInterval = static_cast<TTimeIntervalMicroSeconds32>(1000000./iRepeatRate);
    
    iConsume = EFalse;
    iIsCanceled = EFalse;
    
    iScanCodeIJustGenerated = -1;
    iPostedDelayedScanCode = -1;
    
    iIsCombo = EFalse;
    
    // Initialize keymap.
    const TKefKeyEvent nullEvent = { EKeyNull, EKeyNull, 0 };
    for ( TUint j = 0; j < KKefKeyMapRows; j++ )
        {
        for ( TUint i = 0; i < KKefKeyMapColumns; i++ )
            {
            iKeyMap[i][j] = nullEvent;
            }
        }
    
    // Load the default configuration.
    TRAPD( ierr, InitializeKeyTableFromResourceL( KKefKeyTableRsc ) );
    if ( ierr != KErrNone )
        {
        __KEFLOGSTRING1( "Error initializing the key tables, %d", ierr );
        }

    if ( !IsKeyEventMapEnabled() )
        {
        return;
        }
    
    // Define Publish & Subscribe key
    TInt err = RProperty::Define( KUidWinservCategory, KKefPSDeviceMode, 
        RProperty::EInt, KAlwaysPassPolicy, KSysapOnlyPolicy );
    if ( err != KErrNone )
        {
        __KEFLOGSTRING1("CKefMapImpl::ConstrucL() Problem defining The Key %d", err);
        }

    // Subscribe to Publish & Subscribe keys
    err = KErrNone;
    CKefSubscriber *sub = NULL;
    
    sub = CKefSubscriber::NewL( *this, KUidWinservCategory , KKefPSDeviceMode  );
    if ( sub )
        {        
        err = iKefSubscribers.Append( sub );
        if ( err != KErrNone )
            {
            delete sub;
            __KEFLOGSTRING( "CKefMapImpl::ConstructL ERROR: device mode subscription failed." );
            // We could stop the boot here, but we can also continue, just without 
            // the device modes.
            }
        }
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::~CKefMapImpl
// -----------------------------------------------------------------------------
//
CKefMapImpl::~CKefMapImpl()
    {
    __KEFLOGSTRING("CKefMapImpl::Destructor entered.");
    
    if ( iCombos )
        {
        const TInt count = iCombos->Count();
        for ( TInt i = count - 1; i >= 0; i-- ) 
            {
            (*iCombos)[i]->Close();
            delete (*iCombos)[i];
            iCombos->Remove(i);
            }
        iCombos->Close();
        delete iCombos;
        }
    
    delete iDownTimer;

    iUpEvents.Close();   
    iDownEvents.Close();
    iRepeatRates.Close();
    
    iKefSubscribers.ResetAndDestroy();
    iKefSubscribers.Close();
    
    iRawEventsNotPosted.Close();
    }


// -----------------------------------------------------------------------------
// CKefMapImpl::SetProvider
// -----------------------------------------------------------------------------
//    
void CKefMapImpl::SetProvider( MKefProvider& aProvider )
    {
    iProvider = &aProvider;
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::IsKeyEventMapEnabled
// -----------------------------------------------------------------------------
//
TBool CKefMapImpl::IsKeyEventMapEnabled() const
    {
    return iKeyEventMapEnabled;
    }
    
// -----------------------------------------------------------------------------
// CKefMapImpl::OfferRawEvent
// 
// This is where the raw events end up in, and
// where we have to decide what to do with them.
// -----------------------------------------------------------------------------
//
TBool CKefMapImpl::OfferRawEvent( const TRawEvent& aRawEvent, TBool aSilentEvent )    
    {
    __ASSERT_ALWAYS( iProvider, User::Invariant() );
    
    const TBool downEvent = ( aRawEvent.Type() == TRawEvent::EKeyDown );
    const TBool upEvent = ( aRawEvent.Type() == TRawEvent::EKeyUp );
    //NEW CODE
    iCompleteDelayedComboMatch = EFalse;
    //END NEW CODE
    //
    // Raw events created by this CKefMapImpl itself: pass them on.
    //
    if ( ( downEvent || upEvent ) && 
         ( aRawEvent.ScanCode() == iScanCodeIJustGenerated ) )
        {
        if ( downEvent )
            {
            // Add this scan code to iUpEvents so we can generate
            // the appropriate raw UPs when the key press is finished.
            __KEFLOGSTRING1("CKefMapImpl::OfferRawEvent adding %x to iUpEvents", iScanCodeIJustGenerated);
            TInt err = iUpEvents.Append( iScanCodeIJustGenerated );
            if ( err != KErrNone )  // never happened
                {
                __KEFLOGSTRING("CKefMapImpl::OfferRawEvent ERROR appending up event!");
                }        
            }
    
        if ( upEvent )
            {
            // would be nice to .Remove() elements from iUpEvents here,
            // but that'd require another call to ResolveMapping(),
            //  -- .Remove() in CancelProcessingKeyPress() instead.
            }
            
        iScanCodeIJustGenerated = -1;
        return EFalse;  // not further processing for scan codes iJustGenerated.
        } 
    
    //NEW CODE
    if ( ( downEvent || upEvent ) && 
         ( aRawEvent.ScanCode() == iPostedDelayedScanCode ) )
        { 
        iPostedDelayedScanCode = -1;
        return EFalse;  // not further processing for scan codes iJustGenerated.
        }
    //END NEW CODE      
        
    TInt err = KErrNone;
    iConsume = EFalse;  // very important to set this here.
    
    // log arrays
#ifdef _KEF_LOGGING	
    for ( TInt i = 0 ; i < iDownEvents.Count() ; i++ )
        {
        __KEFLOGSTRING2("CKefMapImpl::OfferRawEvent iDownEvents[%d] = %x", i, iDownEvents[i] );
        }
    for ( TInt i = 0 ; i < iUpEvents.Count() ; i++ )
        {
        __KEFLOGSTRING2("CKefMapImpl::OfferRawEvent iUpEvents[%d] = %x", i, iUpEvents[i] );
        }
#endif // _KEF_LOGGING	
    
    // Down events
    if ( downEvent && aRawEvent.ScanCode() >= 0 )
        {
        iIsDownEvent = ETrue;
        iCurrentScanCode = aRawEvent.ScanCode(); 
        iPostMeOnUp.iKeyCode = EKeyNull;
        iPostMeOnUp.iEventType = 0;
        __KEFLOGSTRING3( "CKefMapImpl::OfferRawEvent KeyDown scancode=%x mods=%x, we're in devicemode=%d", 
            iCurrentScanCode STRIP_MODS, iCurrentScanCode GET_MODS, iCurrentDeviceMode);
        
        // Combo support
        if ( iDownEvents.Count() < KKefMaxComboKeys )
            {
            err = iDownEvents.Append( aRawEvent.ScanCode() STRIP_MODS );
            if ( err != KErrNone )  // never happened
                {
                __KEFLOGSTRING("CKefMapImpl::OfferRawEvent ERROR appending down event!");
                }                
            }
        else 
            {
            // User is trying to press more simultaneous keys than supported.
            // We have no problem with that, just ignore it.
            }
        
        // Resolve mapping for this event and set is as the beacon.
        iKeyBeacon = ResolveMapping();

        // Launch a CPeriodic so the duration of the keypress can be known.  
        // There's only one timer; for combos, it measures the time starting
        // from the first DOWN event of the combo.  If usability troubles appear,
        // try re-starting the timer for each participating key in a combo
        // (by removing the test against iDownEvents.Count())
        //NEW CODE
        if ( iDownEvents.Count() < 2 && !iDelayedCombo)
        //END NEW CODE        
            {            
            iKeyPressDuration = 0.0;
            iDownTimer->Cancel();
            iIsCanceled = EFalse; // set this before Start()
            iDownTimer->Start( iKeyPollInterval, iKeyPollInterval, 
                TCallBack( &KeyPressDuration, (TAny*) this ) );
            }

        if ( iKeyBeacon.iKeyCode > KKefKeyDisabled )
            {
            if ( ( iKeyBeacon.iEventType & KKefIsLongOnly ) )
                {
                // delayed posting of the short event when KKefIsLongOnly
                iPostMeOnUp = iKeyBeacon;  
                return iConsume;      // notice!  we're finished with this event.
                }//this might be part of special combo key event
            //NEW CODE
            else if(iDelayedCombo && !iCompleteDelayedComboMatch)
                {
                    __KEFLOGSTRING("CKefMapImpl::OfferRawEvent Delayed Combo; downevent pending");
                 //delay posting of down event(s) in case this is part of the combo
                 return iConsume;      // notice!  we're finished with this event.   
                }
            //END NEW CODE
            else
                {
                if ( ( iKeyBeacon.iEventType & KKefIsShortKey ) == EFalse )
                    {
                    PostEvent( EKefPostRaw );
                    }
                else 
                    {
                    PostEvent( EKefPostKey );
                    }
                //NEW CODE    
                if(iCompleteDelayedComboMatch)
                    {
                        TInt existsAlready(-1);
                        __KEFLOGSTRING("CKefMapImpl::OfferRawEvent Delayed Combo Complete. Key event posted");
                      //Events for keys part of completed special combo are NOT posted. Ever.
                      for(TInt i=0; i<iDownEvents.Count(); i++)
                        {   
                            existsAlready = iRawEventsNotPosted.Find( iDownEvents[i] );
                            //However, do NOT add same events more than once.
                            //When user is pressing the second key of a combo multiple times and holding the first down
                            //we'd have the first key of the combo umpty times on the list.
                            if(existsAlready == KErrNotFound)
                                iRawEventsNotPosted.Append( iDownEvents[i] );
                        }
                            
                      iIsPartialMatch = EFalse;
                      iPostPendingDownEvents = EFalse;
                      iCompleteDelayedComboMatch = EFalse;
                    }
                //END NEW CODE  
                }
            }
         //NEW CODE
         if(iPostPendingDownEvents)
                {   //This is not a special combo. Post delayed downevents.
                    __KEFLOGSTRING("CKefMapImpl::OfferRawEvent Delayed Combo ABORT. Post pending events.");
                    iDelayedCombo = EFalse;
                    iIsPartialMatch = EFalse;
                    TInt lastBeforeCurrentEvent = iDownEvents.Count();
                    TInt eventNotPostedIndex(-1);
                    //The current downevent will be posted by the window server (iConsume is EFalse).
                    for(TInt i=0; i<lastBeforeCurrentEvent; i++)
                        {
                            eventNotPostedIndex = KErrNotFound;
                            if(iRawEventsNotPosted.Count())                           
                                eventNotPostedIndex = iRawEventsNotPosted.Find( iDownEvents[i] );
                            if(eventNotPostedIndex == KErrNotFound)
                                {
                                    
                                    iPostedDelayedScanCode = iDownEvents[i];
                                    TRawEvent rawEvent;
                                    rawEvent.Set( TRawEvent::EKeyDown, iPostedDelayedScanCode );
                                    __KEFLOGSTRING1("CKefMapImpl::OfferRawEvent Posted pending event scancode=%x",rawEvent.ScanCode());
                                    iProvider->KefPostRawEvent( rawEvent );
                                }
                        }          
                    iPostPendingDownEvents = EFalse;
                }
            //END NEW CODE       
        
        // Emit a keyclick.
        if ( !aSilentEvent && iKeyBeacon.iKeyCode != KKefKeyDisabled )
            {
            if ( iKeyBeacon.iKeyCode > KKefKeyDisabled )
                {
                if (iDownEvents.Count() > 1)
                	{	//Make sure the key sound is played for the last key of a combo
                		iProvider->KefGenerateKeySound( iCurrentScanCode STRIP_MODS );
                	}
                else
                	{	//Otherwise send the mapped single key
                		iProvider->KefGenerateKeySound( iKeyBeacon.iKeyCode STRIP_MODS );
                	}
                }
            else
                {
                iProvider->KefGenerateKeySound( aRawEvent.ScanCode() STRIP_MODS );
                }
            }
        
        return iConsume; // this was set by ResolveMapping
        }
    
    //  Up events    
    if ( upEvent )
        {
        iIsDownEvent = EFalse;               
        iCurrentScanCode = aRawEvent.ScanCode(); 
        __KEFLOGSTRING1("CKefMapImpl::OfferRawEvent KeyUp scancode=%x",aRawEvent.ScanCode());
        
        // Check whether this up event finishes a keypress we've been handling.
        TInt finishesKeyPress = 
            iDownEvents.Find( aRawEvent.ScanCode() STRIP_MODS );
        __KEFLOGSTRING1("CKefMapImpl::OfferRawEvent finishesKeypress index %d", finishesKeyPress);
        __KEFLOGSTRING1("CKefMapImpl::OfferRawEvent Duration =%f",iKeyPressDuration);
        
        // We got an UP for a known DOWN

        if ( finishesKeyPress != KErrNotFound) 
            {

                 
            if ( iPostMeOnUp.iKeyCode > KKefKeyDisabled && !iIsLongKeyPress  )
                {
                iKeyBeacon = iPostMeOnUp;
                if ( ( iKeyBeacon.iEventType & KKefIsShortKey ) == EFalse )
                    {
                    PostEvent( EKefPostRaw );
                    }
                else 
                    {
                    PostEvent( EKefPostKey );
                    }
                iPostMeOnUp.iKeyCode = EKeyNull;
                iPostMeOnUp.iEventType = 0;
                }
            
            //NEW CODE
            if(iDelayedCombo )
                {
                    TInt lastDownEventIndex = iDownEvents.Count() - 1;
                    TInt eventNotPostedIndex(-1);
                    TInt lastEvent = iDownEvents[lastDownEventIndex]; //DEBUG VARIABLE
                    //Up event for some other key than the last pressed; handling of special event aborted.
                    if(finishesKeyPress != lastDownEventIndex)
                        {
                            __KEFLOGSTRING("CKefMapImpl::OfferRawEvent: Not the last key of DK. ABORT by UP event.");
                            iDelayedCombo = EFalse;
                            iIsPartialMatch = EFalse;
                            iCompleteDelayedComboMatch = EFalse;
                            iPostPendingDownEvents = ETrue;
                            //Check if current UP event is in the "not to be posted" list
                            eventNotPostedIndex = iRawEventsNotPosted.Find( iCurrentScanCode STRIP_MODS );
                            if(eventNotPostedIndex != KErrNotFound)
                              {
                                iDownEvents.Remove( finishesKeyPress ); 
                                iRawEventsNotPosted.Remove(eventNotPostedIndex);
                                iConsume = ETrue; //This UP event is not sent either...
                              }
                            //Post pending down events
                            __KEFLOGSTRING("CKefMapImpl::OfferRawEvent: Post pending events.");
                            for(TInt i=0; i<iDownEvents.Count(); i++)
                                {
                                    eventNotPostedIndex = iRawEventsNotPosted.Find( iDownEvents[i] STRIP_MODS );
                                    //Event is not in the "not to be posted" list; send it.
                                    if(eventNotPostedIndex == KErrNotFound)
                                        {
                                            iPostedDelayedScanCode = iDownEvents[i];
                                            TRawEvent rawEvent;
                                            rawEvent.Set( TRawEvent::EKeyDown, iPostedDelayedScanCode );
                                            iProvider->KefPostRawEvent( rawEvent );
                                            eventNotPostedIndex = - 1;
                                        }
      
                                }
                                          
                            iPostPendingDownEvents = EFalse;
                        }
                     else //Last key of a special combo or a candidate.Continue processing as special combo.
                        {
                            __KEFLOGSTRING("CKefMapImpl::OfferRawEvent: The last key of DK.");
                            eventNotPostedIndex = iRawEventsNotPosted.Find( iCurrentScanCode STRIP_MODS );
                            //Last key of a partial match 
                            if(eventNotPostedIndex == KErrNotFound)
                                {
                                  __KEFLOGSTRING("CKefMapImpl::OfferRawEvent: Last key not on NO list. Post downevent.");
                                  iPostedDelayedScanCode = iDownEvents[finishesKeyPress];
                                  iDownEvents.Remove( finishesKeyPress ); 
                                  TRawEvent rawEvent;
                                  rawEvent.Set( TRawEvent::EKeyDown, iPostedDelayedScanCode );
                                  //Post DOWN event. UP event will be posted by Window Server since iConsume is False.
                                  iProvider->KefPostRawEvent( rawEvent );
                                }
                            else //Part of a completed special event. Do not post events associated with this key.
                                { 
                                    __KEFLOGSTRING("CKefMapImpl::OfferRawEvent: Last key on NO list. Don't post events.");  
                                  iDownEvents.Remove( finishesKeyPress ); 
                                  iRawEventsNotPosted.Remove(eventNotPostedIndex);
                                  iConsume = ETrue;
                                }  
   
                        }

                }
            else
                {
                     TInt eventNotPostedIndex = iRawEventsNotPosted.Find( aRawEvent.ScanCode() STRIP_MODS );
                     //This has been part of a sent special combo; do not send raw event.
                     if(eventNotPostedIndex != KErrNotFound)
                         {
                           iRawEventsNotPosted.Remove(eventNotPostedIndex);
                           iDownEvents.Remove( finishesKeyPress );
                           iConsume = ETrue;
                         }
                 //END NEW CODE
                     else //Business as usual.
                        {
                            //
                            iKeyBeacon = ResolveMapping();
                            // Remove key from the down array
                            iDownEvents.Remove( finishesKeyPress );
                            
                            // if it is mapped on the up array -> generate the up event
                            TInt upKeyPressIndex = KErrNotFound;
                            for ( TInt i = 0 ; i < iUpEvents.Count() ; i++ )
                                {
                                if ( (iUpEvents[i] STRIP_MODS) == ( iKeyBeacon.iKeyCode STRIP_MODS ) )
                                    {
                                    upKeyPressIndex = i;
                                    break;
                                    }
                                }
                            
                            if ( upKeyPressIndex != KErrNotFound )
                                {
                                iScanCodeIJustGenerated = iUpEvents[upKeyPressIndex];
                                TRawEvent rawEvent;
                                rawEvent.Set( TRawEvent::EKeyUp, iScanCodeIJustGenerated );
                                __KEFLOGSTRING1("CKefMapImpl::OfferRawEvent POST RAW UP-EVENT for %x",iScanCodeIJustGenerated);
                                iProvider->KefPostRawEvent( rawEvent ) ;
                                iUpEvents.Remove(upKeyPressIndex);
                                iConsume = ETrue;
                                }
                        }
                        
                }
                
                    
            if ( !iDownEvents.Count() )
                 {
                  CancelProcessingKeyPress();
                 }

            }
         else // finishesKeyPress == KErrNotFound
            {

                    // probably a hardware/driver error, in any case we're
                    // confused now.
                    __KEFLOGSTRING("CKefMapImpl::OfferRawEvent INPUT IS CONFUSED: cancel key processing");            
                    CancelProcessingKeyPress();     
            }
        }
    
    return iConsume;
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::CancelProcessingKeyPress
// -----------------------------------------------------------------------------

void CKefMapImpl::CancelProcessingKeyPress()
    {
    iDownTimer->Cancel();
    iIsCanceled = ETrue;
    iDownEvents.Reset();
    iRawEventsNotPosted.Reset();
    
    // be sure to send out all raw upevents or CKeyTranslator
    // will think the key is still being pressed.
    TRawEvent rawEvent;    
    while ( iUpEvents.Count() > 0 )
        {
        iScanCodeIJustGenerated = iUpEvents[0];
        rawEvent.Set( TRawEvent::EKeyUp, iScanCodeIJustGenerated );
        __KEFLOGSTRING1("CKefMapImpl::CancelProcessingKeyPress POST RAW UP-EVENT for %x",iScanCodeIJustGenerated);
        iProvider->KefPostRawEvent( rawEvent ) ;
        iUpEvents.Remove(0);
        }
        
    iUpEvents.Reset();
    iRepeats = 0;
    iKeyPressDuration = 0.0;
    iIsLongKeyPress = EFalse;                
    __KEFLOGSTRING1("CKefMapImpl::CancelProcessingKeyPress Last beacon sent: %x", iKeyBeacon.iKeyCode);
    iKeyBeacon.iKeyCode = EKeyNull;
    iKeyBeacon.iEventType = 0;
    //NEW CODE
    iDelayedCombo = EFalse;
    iIsPartialMatch = EFalse;
    iCompleteDelayedComboMatch = EFalse;            
    iPostPendingDownEvents = EFalse;   
    //END NEW CODE                     
    }
    

// -----------------------------------------------------------------------------
// CKefMapImpl::HandlePropertyChangedL
//
// Callback for Publish & Subscribe key events, it tracks
// the device mode changes for internal use.
// -----------------------------------------------------------------------------
//
void CKefMapImpl::HandlePropertyChangedL( 
        const TUid& aCategory, 
        const TUint aKey )
    {
    TInt keyValue( 0 );
    TInt err = KErrNone;
    err = RProperty::Get( aCategory, aKey, keyValue );
    if ( err == KErrNone )
        {
        switch( aKey )
            {
            case KKefPSDeviceMode:
                {
                __KEFLOGSTRING1("CKefMapImpl::HandlePropertyChanged VALUE: %d",aKey);
                iCurrentDeviceMode = keyValue;
                
                const TInt count = iRepeatRates.Count();
                TUint16 currentMode = iCurrentDeviceMode; 
                for ( TInt ii = 0; ii < count; ii++ )
                    {
                    const TKefKeyRepeatRate& repeatRate = iRepeatRates[ ii ];
                    
                    if ( ( currentMode & repeatRate.iModeMask ) == 
                         repeatRate.iMode )
                        {
                        SetRepeatRate( repeatRate.iRate );
                        ii = count; // exit the loop.
                        }
                    }
                }
                break;
            default:
                {
                __KEFLOGSTRING1("CKefMapImpl::HandlePropertyChanged GOT A P&S VALUE: %d",aKey);
                }
                break;
            }
        }
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::InitializeKeyTableFromResourceL
// -----------------------------------------------------------------------------
//
void CKefMapImpl::InitializeKeyTableFromResourceL(
        const TDesC& aConfigFile ) 
    {
    // Get a handle for the resource file.
    RFs fsSession;
    CleanupClosePushL( fsSession );
    TInt err = fsSession.Connect();
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
    RResourceFile resourceFile;
    CleanupClosePushL( resourceFile );
    
    // Make sure we have the resource file.
    if ( BaflUtils::FileExists( fsSession, aConfigFile ) )
        {
        resourceFile.OpenL( fsSession, aConfigFile );
        }
    else
        {
        __KEFLOGSTRING( "CKefMapImpl::InitializeKeyTableFromResourceL: no configuration file!");
        CleanupStack::PopAndDestroy(); // CleanupClosePushL.
        CleanupStack::PopAndDestroy(); // CleanupClosePushL.
        return;
        }
    
    // Confirm signature of the resource file.
    resourceFile.ConfirmSignatureL( 0 );
    
    // Now just get the resource chunk into a heap buffer and give it to a 
    // TResourceReader.
    HBufC8* res;
    res = resourceFile.AllocReadLC( R_AVKON_KEY_EVENT_MAP );
    
    TResourceReader theReader;
    theReader.SetBuffer( res );
    
    // Check version field.
    const TUint16 version = theReader.ReadUint16();
    if ( version != KKefKeyEventVersion1 )
        {
        __KEFLOGSTRING( "CKefMapImpl::InitializeKeyTableFromResourceL: key event map disabled!");
        CleanupStack::PopAndDestroy( res );
        CleanupStack::PopAndDestroy(); // CleanupClosePushL.
        CleanupStack::PopAndDestroy(); // CleanupClosePushL.
        return;
        }
    
    iKeyEventMapEnabled = ETrue;
    theReader.ReadUint16(); // read flags, not in use yet.

    // Read repeat rates.
    const TUint count = theReader.ReadUint16();
    for ( TInt idx = 0; idx < count; idx++ )
        {
        TUint16 mode = theReader.ReadUint16();
        TUint16 modeMask = theReader.ReadUint16();
        TUint8 rate = theReader.ReadUint8();
        
        const TKefKeyRepeatRate repeatRate = 
            {
            mode,
            modeMask,
            rate
            };
        
        User::LeaveIfError( iRepeatRates.Append( repeatRate ) );
        }

    // Parses the resource data
    CKefResourceArray* keyResourceArray = CKefResourceArray::NewLC();
    CArrayPtrFlat<CKefResource>* keyEvents = NULL;
    keyEvents = keyResourceArray->GetKeyEventsL( theReader );
    
    // At this point we have all the key event info; just put it in its place
    // in iKeyMap and iCombos.
    for ( TUint i = 0; i < (TUint)keyEvents->Count(); i++ )
        {
        CKefResource* keyEvent = keyEvents->At(i);
        
        if ( keyEvent->GetScanCodes().Count() > 0 )
            {
            if ( (keyEvent->GetKeyCodes().Count() == keyEvent->GetDeviceModes().Count())
                 && (keyEvent->GetKeyCodes().Count() == keyEvent->GetEventTypes().Count()))
                {
                for ( TUint j = 0; 
                      j < (TUint)keyEvent->GetKeyCodes().Count(); 
                      j++ )
                    {            
                    TUint16 thisMode = keyEvent->GetDeviceModes().At(j);
                    RArray<TUint> theseModes;
                    CleanupClosePushL( theseModes );
                    
                    ResolveDeviceModeL( 
                        theseModes, 
                        keyEvent->GetDeviceModes().At(j) );
                    
                    TUint16 thisType = keyEvent->GetEventTypes().At(j);
                    for ( TUint k = 0; k < (TUint)theseModes.Count(); k++ )
                        {
                        TKefKeyDefinition thisKey;
                        thisKey.iScanCodes = &(keyEvent->GetScanCodes());
                        thisKey.iDeviceMode = theseModes[k];
                        thisKey.iKeyCode = keyEvent->GetKeyCodes().At(j);
                        thisKey.iEventType = thisType;
#ifdef RD_TACTILE_FEEDBACK
                        thisKey.iFeedbackType = keyEvent->GetFeedbackType();
#endif // RD_TACTILE_FEEDBACK
                        
                        TRAPD( error, SetKeyMappingL( thisKey ) );
                        if (error != KErrNone)
                            {
                            __KEFLOGSTRING( "CKefMapImpl::InitializeKeyTableFromResourceL ERROR setting a mapping");
                            }
                        }
                    
                    CleanupStack::PopAndDestroy( &theseModes );
                    }
                }
            else
                {
                // Keyboard configuration is erroneous!
                // When multiple keycodes have been defined, they must have
                // an associated device mode, etc.
                __KEFLOGSTRING( "CKefMapImpl::InitializeKeyTableFromResourceL ERROR: inconsistent data in KEY_EVENT" );
                User::Leave( KErrGeneral );
                }
            }
        else 
            {
            // Error. No scancodes for the key event.
            User::Leave( KErrGeneral );
            }    
        }
    
    // All done! The maps are now ready for use!
    CleanupStack::PopAndDestroy( 4 ); // keyResourceArray, res, 
                                      // &resourceFile, &fsSession
    }
    
// -----------------------------------------------------------------------------
// CKefMapImpl::SetRepeatRate
// -----------------------------------------------------------------------------
//
void CKefMapImpl::SetRepeatRate( TUint8 aRepeatRate )
    {
    if ( iRepeatRate > 0 && iRepeatRate < 32 )
        {        
        iRepeatRate = aRepeatRate;
        }
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::SetKeyMappingL
//
// Sets the keycodes in iKeyMap and iCombos for one device mode.
// -----------------------------------------------------------------------------
//
void CKefMapImpl::SetKeyMappingL( const TKefKeyDefinition& aKey )
    {
    if ( aKey.iScanCodes->Count() == 1 )
        {        
        // Normal (=single key) mapping
        if ( (aKey.iEventType & KKefTypeShort) == KKefTypeShort )
            {
            // handle anykey definition for single presses.
            if ( aKey.iScanCodes->At(0) == KKefAnyKey)
                {
                for (TUint j = 0; j < KKefMaxScanCode; j++) 
                    {
                    iKeyMap[j][aKey.iDeviceMode].iShortPress = 
                        aKey.iKeyCode;
                    iKeyMap[j][aKey.iDeviceMode].iLongPress = 
                        aKey.iKeyCode;
                    iKeyMap[j][aKey.iDeviceMode].iEventType |=
                        ResolveEventType ( aKey.iEventType );
#ifdef RD_TACTILE_FEEDBACK
                    iKeyMap[j][aKey.iDeviceMode].iFeedbackType = 
                        aKey.iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
                    }
                }
            else 
                {                                
                // default is to set iLongPress to same as iShortPress, which in 
                // practice means that the default behavior for long keypresses is
                // to repeat.
                iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iShortPress = 
                    aKey.iKeyCode;
                iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iLongPress = 
                    aKey.iKeyCode;
                iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iEventType |= 
                    ResolveEventType( aKey.iEventType );
#ifdef RD_TACTILE_FEEDBACK
                iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iFeedbackType = 
                    aKey.iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
                }
            }
        else if ( ((aKey.iEventType & KKefTypeLong) == KKefTypeLong) ||
                  ((aKey.iEventType & KKefTypeLongOnly) == KKefTypeLongOnly))
            {
            iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iLongPress  = 
                aKey.iKeyCode;
            iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iEventType |= 
                ResolveEventType( aKey.iEventType );
#ifdef RD_TACTILE_FEEDBACK
            iKeyMap[aKey.iScanCodes->At(0)][aKey.iDeviceMode].iFeedbackType = 
                aKey.iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
            }
        else
            {
            // Ignored.
            }
        }
    else
        {
        // Combo mapping
        RArray<TUint32>* newCombo = 
            new (ELeave) RArray<TUint32>( KKefMaxComboKeys );
        TUint i;
        TInt err = KErrNone;
        for ( i = 0; i < aKey.iScanCodes->Count(); i++ ) 
            {           
            err = newCombo->Append( aKey.iScanCodes->At(i) );
            if ( err != KErrNone ) 
                {
                User::Leave( err );  // we're trapped, just bail out.
                }
            }
        
        // For combos, the device mode information is encoded in to the
        // upper 16 bits of the Key Code value.    
        TUint32 keyMode = aKey.iKeyCode + ( aKey.iDeviceMode << 16 );
        newCombo->Insert( keyMode, 0 );
       
        TUint32 eventType = ResolveEventType( aKey.iEventType );
        newCombo->Insert( eventType, 1 );

        err = iCombos->Append( newCombo );
        if ( err != KErrNone )
            {
            newCombo->Close();
            delete newCombo;
            User::Leave( err );
            }  
        }
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::ResolveEventType
//
// A conversion between the eventtype representation in the resource file
// and iKeyMap.
// -----------------------------------------------------------------------------

TUint8 CKefMapImpl::ResolveEventType( TUint8 aEventType )
    {
    TUint8 result = 0;
    
    if ( (aEventType & (KKefTypeLongOnly)) == KKefTypeLongOnly )
        {
        result |= KKefIsLongOnly;
        result |= KKefIsLongPress;
        if ( (aEventType & KKefTypeKey) )
            {
            result |= KKefIsLongKey;
            }
        }
    else if ( (aEventType & KKefTypeLong) == KKefTypeLong )
        {
        result |= KKefIsLongPress;
        if ( (aEventType & KKefTypeKey) == KKefTypeKey )
            {
            result |= KKefIsLongKey;
            
            // This is needed if you want continious events from your
            // combo mapping, i.e. hold down the modifier and press
            // the other key several times to obtain the wanted result
            //
            if ( (aEventType & KKefTypeShort) == KKefTypeShort )
                {
                result |= KKefTypeShort;
                }
            
            }
        
        }
    else if ( (aEventType & KKefTypeShort) == KKefTypeShort )
        {
        result = 0; // to be 101% sure
        if ( (aEventType & KKefTypeKey) == KKefTypeKey )
            {
            result |= KKefIsShortKey;
            //NEW CODE
            //In case this is a special combo event.
            if ( (aEventType & KKefTypeDelayedCombo) == KKefTypeDelayedCombo )
            	{
            	result |= KKefIsDelayedOnCombo;
            	}
            //END NEW CODE
            }
        }

    return result;
    }
    
// -----------------------------------------------------------------------------
// CKefMapImpl::ResolveDeviceModeLC
//
// Given a TUint specifying a device mode, this function figures out 
// which rows of the iKeyMap are affected by a mapping. Only used in 
// setting up the key tables, not when querying data from them (just use 
// iCurrentDeviceMode then).
// -----------------------------------------------------------------------------
//
void CKefMapImpl::ResolveDeviceModeL( 
        RArray<TUint>& aModes, 
        TUint16 aMode )
    {
    TInt err;   
    TUint16 myMode = aMode;
    
    for ( TUint k = 0; k < KKefKeyMapRows; k++ )
        {
        // The loop index is now tested against aMode to see whether
        // there should be a non-null value in iKeyMap.
        
        if ( k == myMode )
            {            
            err = aModes.InsertInOrder( k );
            if ( err != KErrNone && err != KErrAlreadyExists )
                {
                User::Leave( err );  // we can't initialize the table.  fail.
                }
            }
        }
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::GetKeyMapping
//
// Returns mappings from iKeyMap.
// -----------------------------------------------------------------------------
//
TKefKeyBeacon CKefMapImpl::GetKeyMapping( 
        TUint16 aScanCode, 
        TUint16 aDeviceMode )
    {
#ifndef RD_TACTILE_FEEDBACK
    TKefKeyBeacon mapValue = { EKeyNull, 0 };
#else
    TKefKeyBeacon mapValue = { EKeyNull, 0, 0 };
#endif // RD_TACTILE_FEEDBACK

        if ( aScanCode < KKefKeyMapColumns && aDeviceMode < KKefKeyMapRows )
            {
            // Look up the mode specific value, if any
            if ( iIsLongKeyPress == EFalse )
                {
                mapValue.iKeyCode = iKeyMap[aScanCode][aDeviceMode].iShortPress;
                }
            else
                {
                mapValue.iKeyCode = iKeyMap[aScanCode][aDeviceMode].iLongPress;
                }

            mapValue.iEventType = iKeyMap[aScanCode][aDeviceMode].iEventType;        
#ifdef RD_TACTILE_FEEDBACK
            mapValue.iFeedbackType = iKeyMap[aScanCode][aDeviceMode].iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
                
            // With this, we always return the default mapping (if any)
            // unless a mode-specific mapping was found.  
            if ( !mapValue.iKeyCode )
                {
                if ( iIsLongKeyPress == EFalse )
                    {
                    mapValue.iKeyCode = iKeyMap[aScanCode][ KKefModeDefault ].iShortPress; 
                    }
                else
                    {
                    mapValue.iKeyCode = iKeyMap[aScanCode][ KKefModeDefault ].iLongPress;
                    }
                mapValue.iEventType = iKeyMap[aScanCode][ KKefModeDefault ].iEventType;        
#ifdef RD_TACTILE_FEEDBACK
                mapValue.iFeedbackType = iKeyMap[aScanCode][KKefModeDefault].iFeedbackType;
#endif // RD_TACTILE_FEEDBACK
                }
            }

    return mapValue;
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::GetComboMapping
//
// Returns mappings from iCombos.
// -----------------------------------------------------------------------------
//
TKefKeyBeacon CKefMapImpl::GetComboMapping( 
        RArray<TUint32>& aComboCandidate )
    {
    TKefKeyBeacon mapValue = { EKeyNull, 0 };        
    
    TUint i;
    TUint j;
    //NEW CODE
    TUint noOfMatches(0);
    TBool partialMatch(EFalse);
    //END NEW CODE
    
    // First we check the first index of aComboCandidate
    // against the second index (= first scancode) of all combo
    // arrays in iCombos. If they match, then the i'th combo
    // array in iCombos could match aComboCandidate.
    //
    // iCombos[i][0] contains the key code that is to be returned for a 
    // succesful mapping, which introduces the offset-by-one in the following 
    // loops/lookups.also included in [i][0] are the device mode info and the 
    // event type info, both encoded into the upper 16 bits of iCombos[i][0].
    
    RArray<TUint> possibleMatch;  //  iCombos indices that might match
    for (i = 0; i < iCombos->Count(); i++) 
        {
        if ( (*(*iCombos)[i])[2] == aComboCandidate[0])
            {
            if((((*iCombos)[i])->Count()-2) == aComboCandidate.Count() )
                possibleMatch.Append(i);
            //NEW CODE
            //This might still be a potential match...
            if(iIsDownEvent)
                {
                    if(iDelayedCombo && (((((*iCombos)[i])->Count()-2) > aComboCandidate.Count())))
                        possibleMatch.Append(i);
                }           
            //END NEW CODE
            __KEFLOGSTRING1("CKefMapImpl::GetComboMapping ComboCandidate might hit iCombos[%d]",i);
            }                    
        }
    
    // If we have possibleMatches, then we just check whether any of
    // them matches aComboCandidate precisely. For the first match,
    // we return the keyvalue from the first element of the matching
    // combo array.
    if ( possibleMatch.Count() )
        {
        TInt8 isAMatch = -1;
        for ( i = 0; i < possibleMatch.Count() && isAMatch < 0; i++ ) 
            {
            // If isAMatch doesn't go to -1 within this loop, 
            // then we got a match.
            isAMatch = possibleMatch[i]; 
            for ( j = 2; j < (*(*iCombos)[possibleMatch[i]]).Count(); j++ ) 
                {
                // Try to find all scan codes of the current event 
                // in a combo definition.
                // NOTE: the order of the remaining scancodes is not matched,
                // as long as the first scancode is OK, the others can come in 
                // any order.
                TInt err = KErrNotFound;
                for ( TInt f = (*(*iCombos)[possibleMatch[i]]).Count()-1; f >= 0 ; f-- )
                    {
                    if ( (*(*iCombos)[possibleMatch[i]])[f] == aComboCandidate[j-2] )
                        {
                        err = f;
                        //NEW CODE
                        noOfMatches++; //record each hit
                        //END NEW CODE
                        break;
                        }
                    }
                
                
                // err < 2 must be discarded, because the first two elements are
                // used for device mode & event type data.
                if ( err == KErrNotFound || err < 2)
                    {
                    // If a scan code cannot be found, this possibleMatch is 
                    // not a real match.
                    isAMatch = -1;
                    //NEW CODE
                    noOfMatches --; //remove this "hit".
                    //END NEW CODE
                    }
                    //NEW CODE
                if ((j-2) == aComboCandidate.Count())
                    {
                        if(noOfMatches == aComboCandidate.Count())
                            partialMatch = ETrue;
                        break;//can happen when looking for a partial match as we took in combos
                          //with more scancodes than in the candidate array. Must exit or we go out of bounds
                          // of the candidate arrray.
                    }
                    //END NEW CODE
                }
            
            // If a matching combination of keys has been 
            // found we need to check that the combo type (long/short) and 
            // the active device mode for this combo match those of the current
            // event.

              TUint16 comboMode = ((*(*iCombos)[possibleMatch[i]])[0] GET_MODS) >> 16;
              TUint16 comboType = ((*(*iCombos)[possibleMatch[i]])[1]);

            
            
            // If ..
            //    a matching set of scan codes (or a partial match) was found,
            //    but it fails the long/short test,
            //    or it's defined for another device mode (default is always accepted).
            if ( isAMatch > -1 && 
                 ( iIsLongKeyPress && (!(comboType & KKefIsLongPress)  ) || 
                   ( ( iCurrentDeviceMode != comboMode ) && ( comboMode != KKefModeDefault ) ) ) )
                {
                isAMatch = -1;
                partialMatch = EFalse; //This wasn't even a partial match after all...
                }
            
            }
        
        if ( isAMatch > -1  && isAMatch < iCombos->Count() )
            {
            // The correct return value can be found from the isAMatch'th 
            // combo-mapping's first element.
            mapValue.iKeyCode = (*(*iCombos)[isAMatch])[0] STRIP_MODS;
            mapValue.iEventType = (*(*iCombos)[isAMatch])[1];
            __KEFLOGSTRING1("CKefMapImpl::GetComboMapping ComboCandidate DID hit iCombos[%d]", isAMatch);
            __KEFLOGSTRING1("CKefMapImpl::GetComboMapping mapValue = %d", mapValue.iKeyCode);
            partialMatch = EFalse; //Since complete match was found there's no need for a partial match, even if one was found.
            }
        }
    else 
        {
        mapValue.iKeyCode = EKeyNull;
        mapValue.iEventType = 0 ;
        }
    possibleMatch.Close();
    //NEW CODE
    iIsPartialMatch = partialMatch;
    //END NEW CODE
    return mapValue;
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::ResolveMapping
// 
// A little macro for figuring out how to map the current scan code.  
// -----------------------------------------------------------------------------
//
TKefKeyBeacon CKefMapImpl::ResolveMapping()
    {
    TKefKeyBeacon keyBeacon = 
    {
        EKeyNull, 0 
#ifdef RD_TACTILE_FEEDBACK
        , 0
#endif // RD_TACTILE_FEEDBACK
    };
    
    if ( iDownEvents.Count() > 1 && iDownEvents.Count() <= KKefMaxComboKeys ) 
        {
        // check that the current key is not disabled in this mode
        // if it is, then any combo containing this key must be 
        // blocked.
        TKefKeyBeacon tmp = GetKeyMapping( 
            iCurrentScanCode STRIP_MODS, iCurrentDeviceMode);
        
        if (tmp.iKeyCode != KKefKeyDisabled)
            {
            
            //Some products have two keys mapped to same scancode. It's not much use to try to check for combo
            //for two identical downevents as we'd just send the first potential combo event in the combo event list.    
            if(iDownEvents[0] != iDownEvents[1]) 
                {
                    keyBeacon = GetComboMapping( iDownEvents );
                }
           

            
            if ( (keyBeacon.iKeyCode == EKeyNull))
                {
                //NEW CODE 
                if((iDelayedCombo && !iIsPartialMatch) || !iDelayedCombo)
                    {                       
                //END NEW CODE 
                       __KEFLOGSTRING( "CKefMapImpl::ResolveMapping Combo not found. Trying to get single." );
                       keyBeacon = GetKeyMapping( iCurrentScanCode STRIP_MODS, iCurrentDeviceMode );
                //NEW CODE 
                    if(iDelayedCombo)
                        {
                            iDelayedCombo = EFalse;
                            iPostPendingDownEvents = ETrue; //spec combo has been aborted; send all pending down events.
                        }
                       
                    }
                 iCompleteDelayedComboMatch = EFalse;
                 
                }
            else 
                {   //Combo found; if the flag is up then a special event is ready.
                
                    iIsCombo = ETrue;
                    if(iDelayedCombo)
                        {
                            __KEFLOGSTRING("CKefMapImpl::ResolveMapping:handling Delayed Combo FOUND.");
                            iIsPartialMatch = EFalse; //lower this flag just to be sure
                            iCompleteDelayedComboMatch = ETrue; 
                        }
                       
                } 
               //END NEW CODE 
            }
        else
            {
            __KEFLOGSTRING("CKefMapImpl::ResolveMapping Ignoring combo because component is disabled");
            keyBeacon.iKeyCode = KKefKeyDisabled;
            keyBeacon.iEventType = 0;
            }
        }
    else if ( iDownEvents.Count() == 1 )
        { //NEW CODE                        
            iCompleteDelayedComboMatch = EFalse;
            //There's not much use to start searching for special combos if this is an UP event.
            if(iIsDownEvent)
                keyBeacon = GetFirstKey( iCurrentScanCode STRIP_MODS );
            
            if((keyBeacon.iKeyCode != EKeyNull) && (keyBeacon.iEventType & KKefIsDelayedOnCombo))
                {
                 __KEFLOGSTRING("CKefMapImpl::ResolveMapping:handling Delayed Combo first key.");
                 iDelayedCombo = ETrue;
                }
            else
           //END NEW CODE  //not the first key of a special combo, get normal mapping.
                keyBeacon = GetKeyMapping( iCurrentScanCode STRIP_MODS, iCurrentDeviceMode );
         
        }
    
    // Always consume succesful mappings, including EKeyDisabled
    if ( keyBeacon.iKeyCode != EKeyNull )
        {
        iConsume = ETrue;
        __KEFLOGSTRING1( "CKefMapImpl::ResolveMapping mapping found: %x", keyBeacon.iKeyCode );
        }
    else
        {
        __KEFLOGSTRING( "CKefMapImpl::ResolveMapping mapping not found" );
        }
    
    return keyBeacon;
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::KeyPressDuration
//
// Callback for iDownTimer, used to distinguish between short and 
// long key presses and to handle repeats.
// -----------------------------------------------------------------------------
//
TInt CKefMapImpl::KeyPressDuration( TAny* aSelf )
    {
    if (aSelf != NULL)
        {
        CKefMapImpl* self = reinterpret_cast<CKefMapImpl*>(aSelf);
        
        self->iKeyPressDuration += ((TReal32)self->iKeyPollInterval.Int())/1e6;
        
        // Distinguish between short and long key presses.
        if ( self->iKeyPressDuration < KKefKeyLongThreshold )
            {
            // nothing to do.  if combo duration measurement is changed
            // so that each participating key resets the timer, then
            // un-comment the following lines:
            // self->iIsLongKeyPress = EFalse;
            // self->iKeyBeacon = self->ResolveMapping();
            }
        else 
            {            
            // Keypress turned out to be a long one
            if ( ! self->iIsLongKeyPress && ! self->iIsCanceled ) 
                {
                self->iIsLongKeyPress = ETrue;
                self->iKeyBeacon = self->ResolveMapping();
    
                // post the KKefTypeLongOnly event
                if ( self->iKeyBeacon.iKeyCode > KKefKeyDisabled
                    && (self->iKeyBeacon.iEventType & KKefIsLongPress))
                    {
                    if ( (self->iKeyBeacon.iEventType & KKefIsLongKey ) ) 
                        {
                        self->PostEvent( EKefPostKey );
                        }
                    else  
                        {
                        if ( self->iKeyBeacon.iEventType & KKefIsLongOnly )
                            {
                            self->PostEvent( EKefPostRaw );
                            }
                        else
                            {
                            __KEFLOGSTRING("CKefMapImpl::KeyPressDuration - LONG PRESS JUST DETECTED; RAW repeat not sent (CKeyTranslator handles that)");
                            }
                        }
                    }
                }
            }
        
        // Handle repeats.
        if ( self->iKeyPressDuration >= KKefKeyLongThreshold && 
             self->iKeyBeacon.iKeyCode > KKefKeyDisabled ) 
            {
            if ( ( self->iKeyBeacon.iEventType & KKefIsLongKey ) )
                {
                //Make sure that the mapping is re-checked otherwise the iRepeats will grow until the last combo key is released
                TKefKeyBeacon oldBeacon(self->iKeyBeacon);
                
                // Call resolve mapping to check if one of the keys has been released
                self->iKeyBeacon = self->ResolveMapping();
                // If no change in mapping increase number of repeats
                // otherwise reset the iRepeats variable
                if ( self->iKeyBeacon.iKeyCode == oldBeacon.iKeyCode &&
                     self->iKeyBeacon.iEventType == oldBeacon.iEventType )
                    {
                    self->iRepeats++;    
                    }
                else
                    {
                    self->iRepeats = 0;
                    }
                  // These conditions needs to be re-checked, since iKeyBeacon may have changed above                     
                if ( self->iKeyBeacon.iKeyCode > KKefKeyDisabled
                    && (self->iKeyBeacon.iEventType & KKefIsLongPress))
                    {
                     if ( (self->iKeyBeacon.iEventType & KKefIsLongKey ) ) 
                        {
                        self->PostEvent( EKefPostKey );
                        }
                     else  
                        {
                        __KEFLOGSTRING("CKefMapImpl::KeyPressDuration - KKefIsLongKey = 1; RAW repeat not sent (CKeyTranslator handles that)");    
                        }
                        
                    }                    
                }
            else
                {
                __KEFLOGSTRING("CKefMapImpl::KeyPressDuration - KKefIsLongKey = 0; RAW repeat not sent (CKeyTranslator handles that)");   
                }
            }
        
        if ( self->iIsCanceled )
            {
            self->iDownTimer->Cancel();
            }
        }
    return KErrNone;
    } 

// -----------------------------------------------------------------------------
// CKefMapImpl::PostEvent
// -----------------------------------------------------------------------------

void CKefMapImpl::PostEvent(TUint aType) 
    {
    switch ( aType )
        {
        case EKefPostKey:
            TKeyEvent keyEvent;
            keyEvent.iCode = iKeyBeacon.iKeyCode;
            if(!iIsCombo)
                keyEvent.iScanCode = iCurrentScanCode STRIP_MODS;
            else// Discard the scan code in favour of the key code as combo events don't have a unabiguous scancode
                keyEvent.iScanCode = EStdKeyNull;
            keyEvent.iRepeats = iRepeats;
            keyEvent.iModifiers = iCurrentScanCode GET_MODS; 
            iProvider->KefPostKeyEvent(keyEvent);
#ifdef RD_TACTILE_FEEDBACK
            // tactile feedback requested in case of key events
            iProvider->KefGenerateFeedback( iKeyBeacon.iFeedbackType );
#endif // RD_TACTILE_FEEDBACK
            __KEFLOGSTRING1( "CKefMapImpl::PostEvent() POST KEY code=%x", keyEvent.iCode);
            iIsCombo = EFalse;
            break;        
        
        case EKefPostRaw:
        default:
            TRawEvent rawEvent;
            rawEvent.Set(TRawEvent::EKeyDown, (iCurrentScanCode GET_MODS) + iKeyBeacon.iKeyCode );
            iScanCodeIJustGenerated = rawEvent.ScanCode();
            __KEFLOGSTRING1( "CKefMapImpl::PostEvent() POST RAW scancode=%x", rawEvent.ScanCode());
            iProvider->KefPostRawEvent( rawEvent );
            break;
        }
    iConsume = ETrue;
    }
    
    
// -----------------------------------------------------------------------------
// KefMapFactory::CreateKefMapL
// Factory functoin to create instance out of CKefMap(Impl).
// -----------------------------------------------------------------------------
//
EXPORT_C CKefMap* KefMapFactory::CreateKefMapL()
    {
    return CKefMapImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CKefMapImpl::GetFirstKey
// -----------------------------------------------------------------------------
//NEW CODE    
TKefKeyBeacon CKefMapImpl::GetFirstKey(TUint32 aComboFirstCodeCandidate )
    {
    TKefKeyBeacon mapValue = { EKeyNull, 0 };        
    
    TUint i;
    
    // We check the aComboCandidate
    // against the second index (= first scancode) of all combo
    // arrays in iCombos. If they match, then the i'th combo
    // array in iCombos could match aComboCandidate.
    //
    // iCombos[i][0] contains the key code that is to be returned for a 
    // succesful mapping, which introduces the offset-by-one in the following 
    // loops/lookups.also included in [i][0] are the device mode info.  
    // Event type info, is encoded into iCombos[i][1].
    
    TInt possibleMatch(-1);  //  iCombos index that might match
    for (i = 0; i < iCombos->Count(); i++) 
        {
        if ( (*(*iCombos)[i])[2] == aComboFirstCodeCandidate) 
            {
            //Check device mode and event type 
    		TUint16 comboMode = ((*(*iCombos)[i])[0] GET_MODS) >> 16; 
    		TUint16 comboType = ((*(*iCombos)[i])[1]);
    		
    		if((comboType & KKefIsDelayedOnCombo)) //We've found a special combo
    		    {
    		        __KEFLOGSTRING("CKefMapImpl::GetFirstKey: IS a special event");
    		        if((iCurrentDeviceMode != comboMode) && (comboMode != KKefModeDefault))
    		            {
    		                //but it is not for correct devicemode
    		                 __KEFLOGSTRING("CKefMapImpl::GetFirstKey Wrong devicemode");
    		            }
    		        else
    		            {
    		               possibleMatch = i;
                            __KEFLOGSTRING1("CKefMapImpl::GetFirstKey SPECIAL ComboCandidate might hit iCombos[%d]",i);
        	               break;  
    		            }
    		    }
                        
            }                    
        }
	if ( possibleMatch >= 0 ) //We have apossible match
	{
	    
		mapValue.iKeyCode = (*(*iCombos)[possibleMatch])[0] STRIP_MODS;
        mapValue.iEventType = (*(*iCombos)[possibleMatch])[1]; 

	}
	return mapValue;
  }
//END NEW CODE  
// End of File
