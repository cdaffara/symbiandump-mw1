/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call forwarding CenRep-key observer implementation.
*
*/


#include "sysapcenrepcallforwardingobserver.h"
#include "SysApEtelConnector.h"
#include "SysApAppUi.h"
#include "SysApFeatureManager.h"
#include <startupdomainpskeys.h>

const TInt KCFFile=0x6F13;

// ========================== MEMBER FUNCTIONS ================================

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver* CSysApCenRepCallForwardingObserver::NewL()
// ----------------------------------------------------------------------------
//
CSysApCenRepCallForwardingObserver* CSysApCenRepCallForwardingObserver::NewL( CSysApAppUi& aSysApAppUi )
    {       
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::NewL" ) ) );
    CSysApCenRepCallForwardingObserver* self = new ( ELeave ) CSysApCenRepCallForwardingObserver( aSysApAppUi );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::CSysApCenRepCallForwardingObserver( CSysApAppUi& aSysApAppUi )
// ----------------------------------------------------------------------------
//
CSysApCenRepCallForwardingObserver::CSysApCenRepCallForwardingObserver( CSysApAppUi& aSysApAppUi  )
    : iSysApAppUi( aSysApAppUi ),
      iSession( NULL ),
      iCallForwardingHandler( NULL ),
      iCallForwardingStatus( KCFIndicatorUnknown ),
      iUiReady( EFalse )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::CSysApCenRepCallForwardingObserver" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::~CSysApCenRepCallForwardingObserver()
// ----------------------------------------------------------------------------
//
CSysApCenRepCallForwardingObserver::~CSysApCenRepCallForwardingObserver()
    {
    TRACES( RDebug::Print( _L("~CSysApCenRepCallForwardingObserver") ) );
    if ( iCallForwardingHandler )
        {
        iCallForwardingHandler->StopListening();
        }
    delete iCallForwardingHandler;
    delete iSession;
    delete iSatCFHandler;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::ConstructL()
// ----------------------------------------------------------------------------
//
void CSysApCenRepCallForwardingObserver::ConstructL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::ConstructL") ) );
    
    iSession = CRepository::NewL( KCRUidCtsyCallForwardingIndicator );
    
    iCallForwardingHandler = CCenRepNotifyHandler::NewL( *this, 
                                                         *iSession, 
                                                         CCenRepNotifyHandler::EBinaryKey, 
                                                         KCtsyUnconditionalCFStatus );

    iCallForwardingHandler->StartListeningL();
    
    iSatCFHandler = CSysApSatObserver::NewL( *this, KCFFile );
    
    // CF file in SIM contains up to date status of call diverts, so it is read every time when SIM has been changed.
    // Also, if voice mail box divert indication is not supported SIM values can be used in every boot, otherwise
    // we must rely on phone information because VMBX phone number is stored in application layer.
    if ( iSysApAppUi.StateOfProperty( KPSUidStartup, KPSSimChanged ) == ESimChanged 
        || ( UseSimCallDivertInfo() && !iSysApAppUi.SysApFeatureManager().VmbxCallDivertIconSupported() ) )
        {
        MapForwarding(); // initializes also iCallForwardingStatus
        }
    else
        {
        TInt indicatorStatus( 0 ); // initialized to 0 because CentRep may not update the value correctly after CRepository::Reset()
        TPckg<TInt> indicatorPckg(indicatorStatus);
        
        TInt err = iSession->Get( KCtsyUnconditionalCFStatus, indicatorPckg );
        
        if ( err == KErrNone )
            {
            iCallForwardingStatus = static_cast <TUnconditionalCFStatus>(indicatorStatus);
            TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::ConstructL(): iCallForwardingStatus=0x%x" ),
                           iCallForwardingStatus ) );
            DoConditionalMapForwarding();
            }
        else
            {
            TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::ConstructL(): err=%d" ), err ) );    
            
            // failed to read KCtsyUnconditionalCFStatus, as a last resort check whether SIM has some divert activated
            MapForwarding();    
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::DoConditionalMapForwarding()
// ----------------------------------------------------------------------------
//
void CSysApCenRepCallForwardingObserver::DoConditionalMapForwarding()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::DoConditionalMapForwarding(): iCallForwardingStatus=0x%x" ),
                           iCallForwardingStatus ) );
    
    RMobilePhone::TCFUIndicatorStatus callForwardingStatus;
    TInt err = GetSimCallForwardingStatus( callForwardingStatus );
    
    if( err == KErrNone )
        {
        iSimCFCurrentStatus = callForwardingStatus;
        }

    if ( iCallForwardingStatus == 0 ) // key has been resetted by CTSY
        {
        MapForwarding();
        }
    else if( UseSimCallDivertInfo() )
        {
        TBool validVoiceCF = 
            ( (iCallForwardingStatus & KCFVoiceForwardedToVoiceMailbox) ==
              (iSimCFCurrentStatus & RMobilePhone::KCFUIndicatorVoice) );
        
        TBool validVideoCF = 
            ( (iCallForwardingStatus & KCFVideoForwardedToVoiceMailbox) ==
              (iSimCFCurrentStatus & RMobilePhone::KCFUIndicatorData) );
        
        if( !validVoiceCF || !validVideoCF )
            {
            // If SIM and phone vmbx information is out of synch, do mapping.
            MapForwarding();
            }
        else if( !(iCallForwardingStatus & KCFVoiceForwardedToVoiceMailbox) &&
                 !(iCallForwardingStatus & KCFVideoForwardedToVoiceMailbox) )
            {
            // If phone does not have voice mailbox CFs, mapping can be done.
            MapForwarding();
            }
        }
    }
    
// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::GetSimCallForwardingStatus()
// ----------------------------------------------------------------------------
//
TInt CSysApCenRepCallForwardingObserver::GetSimCallForwardingStatus( RMobilePhone::TCFUIndicatorStatus& callForwardingStatus )
    {
    TInt indicatorStatus( RMobilePhone::KCFUIndicatorUnknown );
    TPckg<TInt> indicatorPckg(indicatorStatus);
    
    TInt err = iSession->Get( KCtsyCallForwardingIndicator, indicatorPckg );
    
    callForwardingStatus = static_cast <RMobilePhone::TCFUIndicatorStatus>(indicatorStatus);
    
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::GetSimCallForwardingStatus(): err %d, callForwardingStatus=0x%x" ),
                           err, callForwardingStatus ) );
    
    return err;
    }
        
// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::MapForwarding()
// ----------------------------------------------------------------------------
//
void CSysApCenRepCallForwardingObserver::MapForwarding()
    {
    RMobilePhone::TCFUIndicatorStatus callForwardingStatus;
    
    TInt err = GetSimCallForwardingStatus( callForwardingStatus );
    
    if ( err != KErrNone )
        {
        // update KCtsyUnconditionalCFStatus also if an error occurs because otherwise we might end up looping
        TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::MapForwarding(): err=%d" ), err ) );
        iCallForwardingStatus = KCFNoCallsForwarded;
        iSimCFCurrentStatus = RMobilePhone::KCFUIndicatorUnknown;
        }
    else        
        {
        iSimCFCurrentStatus = callForwardingStatus;
        if ( !AlsSupport() )
            {
            switch( callForwardingStatus )
                {
                case RMobilePhone::KCFUIndicatorVoice:
                case RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = KCFVoiceForwarded;   
                    break;
                case RMobilePhone::KCFUIndicatorData:
                case RMobilePhone::KCFUIndicatorData | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = KCFVideoForwarded;   
                    break;
                case RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorData:
                case RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorData | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus>( 
                            KCFVoiceForwarded | KCFVideoForwarded );   
                    break;
                default:
                    iCallForwardingStatus = KCFNoCallsForwarded; 
                    break;
                }
            }
        else
            {
            switch( callForwardingStatus )
                {
                case RMobilePhone::KCFUIndicatorVoice:
                case RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus> ( 
                            KCFVoiceForwarded | KCFForwardedOnLine1 );   
                    break;
                     
                case RMobilePhone::KCFUIndicatorData:
                case RMobilePhone::KCFUIndicatorData | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus> ( 
                            KCFVideoForwarded | KCFForwardedOnLine1 );   
                    break;
                case RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorData:
                case RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorData | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus>( 
                            KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 );
                    break;
                
                case RMobilePhone::KCFUIndicatorAuxVoice:
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus>( KCFForwardedOnLine2 );
                    break;
                
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorVoice:
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus>( 
                            KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                    break;
                    
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorData:
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorVoice | RMobilePhone::KCFUIndicatorData | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus>( 
                            KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                    break;
                
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorData:
                case RMobilePhone::KCFUIndicatorAuxVoice | RMobilePhone::KCFUIndicatorData | RMobilePhone::KCFUIndicatorFax:
                    iCallForwardingStatus = static_cast<TUnconditionalCFStatus>( 
                            KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2 );
                    break;
                                
                default:
                    iCallForwardingStatus = KCFNoCallsForwarded; 
                    break;
                }
            } 
        }
    
    TInt package = iCallForwardingStatus;
    TPckg<TInt> forwardingPckg( package );
    err = iSession->Set( KCtsyUnconditionalCFStatus, forwardingPckg );
    
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::MapForwarding(): err=%d, callForwardingStatus=0x%x, iCallForwardingStatus=0x%x" ),
                            err, callForwardingStatus, iCallForwardingStatus ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::AlsSupport()
// ----------------------------------------------------------------------------
//
TBool CSysApCenRepCallForwardingObserver::AlsSupport() const
    {
    TBool alsSupported(EFalse);
    CSysApEtelConnector* etelConnector = iSysApAppUi.EtelConnector();
    
    if ( etelConnector )
        {
        // Check ALS from PP and CSP only, because line information is sometimes unavailable even ALS is supported.
        alsSupported = ( etelConnector->AlsPpOnOff() || etelConnector->AlsCspOnOff() );
        }
    
    return alsSupported;
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::HandleNotifyBinary()
// ----------------------------------------------------------------------------
//
void CSysApCenRepCallForwardingObserver::HandleNotifyBinary( TUint32 aId, const TDesC8& aNewValue )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::HandleNotifyBinary(): aId=0x%x" ), aId ) );

    if ( aId == KCtsyUnconditionalCFStatus )
        {
        TInt indicatorStatus( 0 );
        TPckg<TInt> indicatorPckg(indicatorStatus);
        indicatorPckg.Copy( aNewValue );
        
        iCallForwardingStatus = static_cast <TUnconditionalCFStatus>(indicatorStatus);
        
        if ( iCallForwardingStatus == 0 ) // resetted by CTSY
            {
            MapForwarding();
            }
        else
            {
            // iSimCFCurrentStatus updated otherwise in MapForwarding
            RMobilePhone::TCFUIndicatorStatus callForwardingStatus;
            TInt err = GetSimCallForwardingStatus( callForwardingStatus );
            
            if( err == KErrNone )
                {
                iSimCFCurrentStatus = callForwardingStatus;
                }
            }
            
        if ( iUiReady ) // UpdateCallForwardingIndicatorsL must be called for the first time from CSysApAppUi
            {
            TRAP_IGNORE( UpdateCallForwardingIndicatorsL() );
            }
        }
        
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::HandleNotifyError()
// ----------------------------------------------------------------------------
//
void CSysApCenRepCallForwardingObserver::HandleNotifyError( TUint32 /* aId */, TInt /* error */, CCenRepNotifyHandler* /* aHandler */ )
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::HandleNotifyError()" ) ) );
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::SimFileChangedL()
// ----------------------------------------------------------------------------
void CSysApCenRepCallForwardingObserver::SimFileChangedL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::SimFileChangedL(): iCallForwardingStatus=0x%x, iSimCFCurrentStatus=0x%x" ),
                           iCallForwardingStatus, iSimCFCurrentStatus ) );
    
    if ( iCallForwardingStatus == 0 ) // key has been resetted by CTSY
        {
        MapForwarding();
        }
    else if( UseSimCallDivertInfo() ) 
        {
        RMobilePhone::TCFUIndicatorStatus callForwardingStatus;
        
        TInt err = GetSimCallForwardingStatus( callForwardingStatus );
        
        // If SIM CF-status has changed, update indicators.
        if( err == KErrNone && (callForwardingStatus != iSimCFCurrentStatus) )
            {
            MapForwarding();
            }
        }
    }

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::UpdateCallForwardingIndicatorsL()
// ----------------------------------------------------------------------------
void CSysApCenRepCallForwardingObserver::UpdateCallForwardingIndicatorsL()
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::UpdateCallForwardingIndicatorsL(): iCallForwardingStatus=0x%x" ),
                               iCallForwardingStatus ) );
        
    if ( !iUiReady )
        {
        iUiReady = ETrue; // CenRep notfications are allowed to update the indicators from now on
        }
    
    switch( iCallForwardingStatus )
        {
        case KCFVoiceForwarded:
        case KCFVideoForwarded:
        case KCFVoiceForwarded | KCFVideoForwarded:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOn );
            break;
        case KCFVoiceForwarded | KCFForwardedOnLine1:
        case KCFVideoForwarded | KCFForwardedOnLine1:
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine1, EAknIndicatorStateOn );
            break;
        case KCFForwardedOnLine2:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine2, EAknIndicatorStateOn );
            break;
        case KCFVoiceForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
        case KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedOnLine1 | KCFForwardedOnLine2:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnBothLines, EAknIndicatorStateOn );
            break;
        case KCFVideoForwardedToVoiceMailbox:
        case KCFVoiceForwardedToVoiceMailbox:
        case KCFVideoForwardedToVoiceMailbox | KCFVoiceForwarded:
        case KCFVoiceForwardedToVoiceMailbox | KCFVideoForwarded:
        case KCFVoiceForwarded | KCFVideoForwarded | KCFForwardedToVoiceMailbox:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnBothLines, EAknIndicatorStateOff );
            
            if ( iSysApAppUi.SysApFeatureManager().VmbxCallDivertIconSupported() )
                {
                iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOff );
                iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOn );    
                }
            else
                {
                iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOff );    
                iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOn );
                }                
            break;
        case KCFNoCallsForwarded:
        case KCFIndicatorUnknown:
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine1, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnLine2, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedOnBothLines, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorCallsForwardedToVoiceMailbox, EAknIndicatorStateOff );
            iSysApAppUi.SetIndicatorStateL( EAknIndicatorAllCallsForwarded, EAknIndicatorStateOff );
            break;
        default: 
            break;
        }
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::UpdateCallForwardingIndicatorsL(): end" ) ) );
    }    

// ----------------------------------------------------------------------------
// CSysApCenRepCallForwardingObserver::UseSimCallDivertInfo()
// ----------------------------------------------------------------------------

TBool CSysApCenRepCallForwardingObserver::UseSimCallDivertInfo() const
    {
    TRACES( RDebug::Print( _L("CSysApCenRepCallForwardingObserver::UseSimCallDivertInfo()" ) ) );
                               
    TBool useSimCfInfo(EFalse);
    
    CSysApEtelConnector* etelConnector = iSysApAppUi.EtelConnector();

    if ( etelConnector )
        {
        // check whether SIM stores divert information
        useSimCfInfo = etelConnector->SimCallDivertEfSupported();
        }    
    
    return useSimCfInfo;    
    }

// End of File

