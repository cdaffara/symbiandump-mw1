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
* Description:  Observes audio output routing changes
*
*/


#ifndef SYSAPAUDIOROUTINGONBSERVER_H
#define SYSAPAUDIOROUTINGONBSERVER_H

#include <MTelephonyAudioRoutingObserver.h>
class CSysApAppUi;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CSysApAudioRoutingObserver : public CBase,
                                   public MTelephonyAudioRoutingObserver
    {

public:

    /**
    * Symbian two-phased constructor.
    * 
    * @param aSysApAppUi    reference to System Application's application UI class
    */
    static CSysApAudioRoutingObserver* NewL( CSysApAppUi& aSysApAppUi );
    
    /**
    * Destructor.
    */
    virtual ~CSysApAudioRoutingObserver();
    
    /**
    * Returns whether audio output is routed to IHF.
    * 
    * @since S60 3.2
    * @return   TBool   ETrue audios routed to IHF
    *                   EFalse audios not routed to IHF.
    */
    TBool IhfEnabled() const;

private: // from MTelephonyAudioRoutingObserver
    /**
    * Signals that available audio outputs have changed.
    *
    * @since S60 3.2
    * @param CTelephonyAudioRouting& aTelephonyAudioRouting reference.
    * @see MTelephonyAudioRoutingObserver::AvailableOutputsChanged
    */
    virtual void AvailableOutputsChanged( CTelephonyAudioRouting& aTelephonyAudioRouting);

    /**
    * Signals that current output has changed. 
    *
    * @since 60 3.2
    * @param CTelephonyAudioRouting& aTelephonyAudioRouting reference.
    * @see MTelephonyAudioRoutingObserver::OutputChanged
    */
    virtual void OutputChanged( CTelephonyAudioRouting& aTelephonyAudioRouting);

    /**
    * Signals that CTelephonyAudioRouting::SetOutputL request issued by this
    * application (on CTelephonyAudioRouting instance) was serviced by the
    * Adaptation.
    *
    * @since S60 3.2
    * @param CTelephonyAudioRouting& aTelephonyAudioRouting reference.
    * @param aError KErrNone if output requested by application is set, else
    *        system-wide error code.
    * @see MTelephonyAudioRoutingObserver::SetOutputComplete
    */
    virtual void SetOutputComplete( CTelephonyAudioRouting& aTelephonyAudioRouting, TInt aError);

private:

    /**
    * C++ default constructor.
    */
    CSysApAudioRoutingObserver();

    /**
    * Constructor.
    * 
    * @param aSysApAppUi    reference to System Application's application UI class
    */
    CSysApAudioRoutingObserver( CSysApAppUi& aSysApAppUi );
    
    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

private: // data

    /**
    * Telephony audio routing instance.
    * Own.
    */
    CTelephonyAudioRouting* iTelephonyAudioRouting;

    /**
    * System Application application UI class.
    */
    CSysApAppUi& iSysApAppUi;

    };


#endif // SYSAPAUDIOROUTINGONBSERVER_H
