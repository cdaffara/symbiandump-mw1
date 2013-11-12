/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApEtelConnector class definition.
*
*/


#ifndef SYSAPETELCONNECTOR_H
#define SYSAPETELCONNECTOR_H

//  INCLUDES
#include <e32base.h>
#include <rmmcustomapi.h>
#include <etel.h>
#include "SysApAppUi.h"

// FORWARD DECLARATIONS
class CSysApEtelNetworkStatusNspsObserver;
class CSysApEtelSmsStoreObserver;
class CSysApEtelNetworkBarObserver;
class CSysApEtelDataBearerObserver;
class CSysApEtelNetworkInfoObserver;


//CONSTANTS

// CLASS DECLARATION
/**
*  CSysApEtelConnector
*  This class is responsible for creating the ETel and Custom API connection.
*
*  @lib   sysap
*  @since 1.0
*/

class CSysApEtelConnector : public CBase
    {
    public:  // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CSysApEtelConnector* NewL( CSysApAppUi& aSysApAppUi );
        
        /**
        * Destructor.
        */
        virtual ~CSysApEtelConnector();
        
    public:

        void CommandNetCsWakeupOnNsps();
        
        /**
        * Get amount of signal bars to display.
        *
        * @return Number of signal bars to display.
        */
        TInt GetSignalBars() const;
        
        /**
        * Get pointer to RTelServer
        *
        * @return pointer to RTelServer
        */
        RTelServer* GetTelServer();
        
        /**
        * Updates voice mail waiting indicators.
        *
        * @since S60 3.2
        */
        void UpdateVoiceMailWaitingIndicatorsL();
        
        /**
        * Returns whether ALS (Alternate line service) is supported.
        *
        * @since S60 3.2
        * @return ETrue if ALS is supported, otherwise EFalse.
        */
        TBool AlsSupported() const;

        /**
        * Returns whether active data bearer is HSDPA.
        *
        * @since S60 3.1
        * @return TBool ETrue, if data bearer is HSDPA.
        */
        TBool IsBearerHsdpa() const;
        
        /**
        * Returns whether network supports HSDPA.
        *
        * @since S60 3.2
        * @return TBool ETrue or EFalse depending on network cell HSDPA capability.
        *               EFalse always if the device does not support HSDPA availability indication.
        */
        TBool HsdpaAvailable() const;

        /**
        * Returns whether ALS is supported in product profile.
        */
        TBool AlsPpOnOff() const;

        /**
        * Returns whether ALS is supported in customer service profile.
        */
        TBool AlsCspOnOff() const;

        /**
        * Checks whether SIM card's SMS store is full.
        *
        * @since S60 3.2
        * @return TBool ETrue, if the store is full.
        */
        TBool IsSimSmsStoreFull() const;

        /**
        * Starts async SIM card's SMS store status read. This is done only once in startup to
        * check if store full indication should be shown or not.
        *
        * @since S60 3.2
        */
        void ReadSimSmsStoreStatusInStartup();
        
        /**
        * Returns whether SIM stores call divert information.
        *
        * @since S60 3.2
        */
        TBool SimCallDivertEfSupported() const;

    private:
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApEtelConnector();

        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @return void
        */        
        CSysApEtelConnector( CSysApAppUi& aSysApAppUi );

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CSysApEtelConnector( const CSysApEtelConnector& );
        
        // Prohibit assigment operator
        CSysApEtelConnector& operator= ( const CSysApEtelConnector& );

        /**
        * Returns whether ALS is supported by SIM.
        */
        TBool AlsSimSupport() const;

    private:    // Data
        CSysApEtelNetworkStatusNspsObserver*    iSysApEtelNetworkStatusNspsObserver;

        RTelServer                              iEtelServer;
        RMobilePhone                            iPhone;
        RMmCustomAPI                            iCustomAPI;
        CSysApAppUi&                            iSysApAppUi;
        CSysApEtelNetworkBarObserver*           iSysApEtelNetworkBarObserver;
        RMobileSmsStore                         iSmsStore;
        RMobileSmsMessaging                     iSmsMessaging;
        CSysApEtelSmsStoreObserver*             iSysApEtelSmsStoreObserver;
        CSysApEtelDataBearerObserver* iSysApEtelDataBearerObserver;
        CSysApEtelNetworkInfoObserver* iSysApEtelNetworkInfoObserver;
    };

#endif      // SYSAPETELCONNECTOR_H
            
// End of File

