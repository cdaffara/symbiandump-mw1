/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Message waiting CenRep-key observer implementation.
*
*/


#include "sysapcenrepmsgwaitingobserver.h"
#include "SysApAppUi.h"
#include "SysApEtelConnector.h"

static const TInt KCenRepKeyUninitialized = -1;

// ========================== LOCAL FUNCTIONS ================================
static TBool BitFieldChanged( TInt aNewValue, TInt aOldValue, TInt aBitField )
    {
    return ( (aNewValue & aBitField) != (aOldValue & aBitField) );
    }

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver* CSysApCenRepMsgWaitingObserver::NewL()
// ----------------------------------------------------------------------------
//
CSysApCenRepMsgWaitingObserver* CSysApCenRepMsgWaitingObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::NewL" ) ) );
    CSysApCenRepMsgWaitingObserver* self = new ( ELeave ) CSysApCenRepMsgWaitingObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::CSysApCenRepMsgWaitingObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------
//
CSysApCenRepMsgWaitingObserver::CSysApCenRepMsgWaitingObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi ),
      iSession( NULL ),
      iMessageWaitingHandler( NULL ),
      iMessageWaitingDisplayStatus( 0 ),
      iOldDisplayStatus( 0 ),
      iUiReady( EFalse )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::CSysApCenRepMsgWaitingObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::~CSysApCenRepMsgWaitingObserver()
// ----------------------------------------------------------------------------
//
CSysApCenRepMsgWaitingObserver::~CSysApCenRepMsgWaitingObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepMsgWaitingObserver") ) );
    if ( iMessageWaitingHandler )
        {
        iMessageWaitingHandler->StopListening();
        }
    delete iMessageWaitingHandler;
    delete iSession;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::ConstructL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::ConstructL") ) );
    
    iSession = CRepository::NewL( KCRUidCtsyMessageWaitingIndicator );
    
    iMessageWaitingHandler = CCenRepNotifyHandler::NewL( *this, 
                                                         *iSession, 
                                                         CCenRepNotifyHandler::EIntKey, 
                                                         KCtsyMessageWaitingDisplayStatus );

    iMessageWaitingHandler->StartListeningL();
                                    
    TInt err = iSession->Get( KCtsyMessageWaitingDisplayStatus, iMessageWaitingDisplayStatus );
    
    if ( err == KErrNone )
        {
        TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::ConstructL(): iMessageWaitingDisplayStatus=0x%x" ),
                               iMessageWaitingDisplayStatus ) );
        
        if ( iMessageWaitingDisplayStatus == KCenRepKeyUninitialized )
            {
            iMessageWaitingDisplayStatus = 0;
            }
        }
    else
        {
        TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::ConstructL(): err=%d" ), err ) );    
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::HandleNotifyInt()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::HandleNotifyInt( TUint32 aId, TInt aNewValue )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::HandleNotifyInt(): aId=0x%x, aNewValue=0x%x, iUiReady=%d" ), 
                            aId, aNewValue, iUiReady ) );
    
    if ( aId == KCtsyMessageWaitingDisplayStatus )
        {
        iOldDisplayStatus = iMessageWaitingDisplayStatus;
        
        iMessageWaitingDisplayStatus = ( aNewValue == KCenRepKeyUninitialized ? 0 : aNewValue );
 
        if ( iUiReady )
            {
            TRAPD( err, UpdateMessageWaitingIndicatorsIfNeededL() );
            
            if ( err != KErrNone )
                {
                TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::HandleNotifyInt(): err=%d"), err ) );
                iMessageWaitingDisplayStatus = 0; // forced update on next notification
                }
            }
        }
    }


// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::HandleNotifyError()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::HandleNotifyError()" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::UpdateMessageWaitingIndicatorsL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::UpdateMessageWaitingIndicatorsL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::UpdateMessageWaitingIndicatorsL(): iMessageWaitingDisplayStatus=0x%x, iUiReady=%d" ), 
                            iMessageWaitingDisplayStatus, iUiReady ) );
    
    if ( !iUiReady )
        {
        iUiReady = ETrue; // start updating indicators according to notifications    
        }
    
    // Update requested from outside of this class, forcibly update all indicators
    UpdateVoiceMailWaitingIndicatorsL();
    UpdateFaxMessageWaitingIndicatorL();
    UpdateEmailMessageWaitingIndicatorL();
    UpdateOtherMessageWaitingIndicatorL();
    
    TRACES( RDebug::Print( _L("CSysApCenRepMsgWaitingObserver::UpdateMessageWaitingIndicatorsL(): end" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::UpdateMessageWaitingIndicatorsIfNeededL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::UpdateMessageWaitingIndicatorsIfNeededL() const
    {
    if ( BitFieldChanged( iMessageWaitingDisplayStatus, iOldDisplayStatus, RMobilePhone::KDisplayVoicemailActive|RMobilePhone::KDisplayAuxVoicemailActive ) )
        {
        UpdateVoiceMailWaitingIndicatorsL();
        }
    
    if ( BitFieldChanged( iMessageWaitingDisplayStatus, iOldDisplayStatus, RMobilePhone::KDisplayFaxActive ) )
        {
        UpdateFaxMessageWaitingIndicatorL();
        }
    
    if ( BitFieldChanged( iMessageWaitingDisplayStatus, iOldDisplayStatus, RMobilePhone::KDisplayEmailActive ) )
        {
        UpdateEmailMessageWaitingIndicatorL();
        }
        
    if ( BitFieldChanged( iMessageWaitingDisplayStatus, iOldDisplayStatus, RMobilePhone::KDisplayOtherActive ) )
        {
        UpdateOtherMessageWaitingIndicatorL();
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::UpdateFaxMessageWaitingIndicatorL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::UpdateFaxMessageWaitingIndicatorL() const
    {
    if ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayFaxActive )
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorFaxMessage, EAknIndicatorStateOn );    
        }
    else
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorFaxMessage, EAknIndicatorStateOff );
        }        
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::UpdateEmailMessageWaitingIndicatorL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::UpdateEmailMessageWaitingIndicatorL() const
    {
    
    if ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayEmailActive )
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorRemoteMailboxEmailMessage, EAknIndicatorStateOn );
        }
    else
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorRemoteMailboxEmailMessage, EAknIndicatorStateOff );    
        }        
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::UpdateOtherMessageWaitingIndicatorL()
// ----------------------------------------------------------------------------
//    
void CSysApCenRepMsgWaitingObserver::UpdateOtherMessageWaitingIndicatorL() const   
    {
    if ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayOtherActive )
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorOtherMessage, EAknIndicatorStateOn );
        }
    else
        {
        iSysApAppUi.SetIndicatorStateL( EAknIndicatorOtherMessage, EAknIndicatorStateOff );
        }        
    }

// ----------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::UpdateVoiceMailWaitingIndicatorsL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepMsgWaitingObserver::UpdateVoiceMailWaitingIndicatorsL() const
    {
    
    switch( ResolveVoiceMailState() )
        {
        case EWaiting:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaiting, EAknIndicatorStateOn );
            break;
        case EWaitingOnLine1:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaiting, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine1, EAknIndicatorStateOn );
            break;
        case EWaitingOnLine2:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaiting, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine2, EAknIndicatorStateOn );
            break;
        case EWaitingOnBothLines:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaiting, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnBothLines, EAknIndicatorStateOn );
            break;
        case ENoVoiceMails:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaitingOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorVoiceMailWaiting, EAknIndicatorStateOff );
            break;
        default:
            break;
        }
    
    }
    
// ---------------------------------------------------------------------------
// CSysApCenRepMsgWaitingObserver::ResolveVoiceMailState()
// ---------------------------------------------------------------------------
//
CSysApCenRepMsgWaitingObserver::TVoiceMailWaitingState CSysApCenRepMsgWaitingObserver::ResolveVoiceMailState() const     
    {
    TVoiceMailWaitingState voiceMailState(ENoVoiceMails);
    
    TBool alsSupported(EFalse);
    
    CSysApEtelConnector* etelConnector = iSysApAppUi.EtelConnector();
    
    if ( etelConnector )
        {
        alsSupported = etelConnector->AlsSupported();
        }
    
    if ( alsSupported ) // line specific indicator is shown if ALS is supported
        {
        if ( ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayAuxVoicemailActive )
             && ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayVoicemailActive ) )
            {
            voiceMailState = EWaitingOnBothLines;
            }
        else if ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayVoicemailActive )
            {
            voiceMailState = EWaitingOnLine1;
            }
        else if ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayAuxVoicemailActive )
            {
            voiceMailState = EWaitingOnLine2;
            }
        }
    else
        {
        if ( iMessageWaitingDisplayStatus & RMobilePhone::KDisplayVoicemailActive )
            {
            voiceMailState = EWaiting;
            }
        }

    return voiceMailState;
    }
    
// End of File

