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
* Description:  CSysApEtelNetworkInfoObserver.h
*
*/


#ifndef SYSAPETELNETWORKINFOOBSERVER_H
#define SYSAPETELNETWORKINFOOBSERVER_H


#include <e32base.h>
#include <etelmm.h>

class CSysApAppUi;
class RMmCustomAPI;

/**
*  An active object for observing changes in network information.
*  
*  This object subscribes to ETel's network information notification
*  and calls methods of CSysApAppUi for reflecting changes in SysAp's behaviour.
*
*  @lib None
*  @since S60 3.2
*/
class CSysApEtelNetworkInfoObserver : public CActive
    {

public:

    /**
    * Symbian two-phased constructor.
    *
    * @since S60 3.2
    * @param aSysApAppUi SysAp application class object
    * @param aPhone      ETel mobile phone
    * @param aCustomApi  multimode custom API
    */
    static CSysApEtelNetworkInfoObserver* NewL( CSysApAppUi& aSysApAppUi, 
                                                RMobilePhone& aPhone, 
                                                RMmCustomAPI& aCustomApi );

    /**
    * Destructor
    */
    virtual ~CSysApEtelNetworkInfoObserver();

    /**
    * Returns whether network supports HSDPA.
    *
    * @since S60 3.2
    * @return TBool ETrue, if the current network cell is HSDPA capable.
    */
    TBool HsdpaAvailable() const;

protected:
    
    // from base class CActive
    void RunL();
    void DoCancel();

private:

    /**
    * C++ default constructor.
    * No implementation.
    */ 
    CSysApEtelNetworkInfoObserver();

    /**
    * Constructor.
    */
    CSysApEtelNetworkInfoObserver( CSysApAppUi& aSysApAppUi, 
                                   RMobilePhone& aPhone, 
                                   RMmCustomAPI& aCustomApi );
    
    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

    /**
    * Orders network information change notification from ETel.
    *
    * @since S60 3.2
    */ 
    void OrderNotification();

private: // data

    /**
    * Reference to SysAp application class.
    */
    CSysApAppUi& iSysApAppUi;
    
    /**
    * Reference to shared ETel mobile phone instance.
    */
    RMobilePhone& iPhone;
    
    /**
    * Reference to shared Custom API instance.
    */
    RMmCustomAPI& iCustomApi;
    
    /**
    * Network information.
    */
    RMobilePhone::TMobilePhoneNetworkInfoV5 iNetInfo;
    
    /**
    * Descriptorized network information.
    */
    RMobilePhone::TMobilePhoneNetworkInfoV5Pckg iNetInfoPckg;
    
    /**
    * If ETrue, the current cell is HSDPA capable.
    */
    TBool iHsdpaAvailable;
    };

#endif // SYSAPETELNETWORKINFOOBSERVER_H
