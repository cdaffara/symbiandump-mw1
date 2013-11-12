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
* Description:  Data bearer change observer.
*
*/


#ifndef SYSAPETELDATABEAREROBSERVER_H
#define SYSAPETELDATABEAREROBSERVER_H

#include <e32base.h>
#include <etelmm.h>
#include <etelpckt.h>

class CSysApAppUi;

/**
*  An active object for observing changes in data bearer.
*  
*  This object subscribes to ETel's packet dynamic capability changes
*  and notifies SysAp's application class about them.
*
*  @lib Noe
*  @since S60 3.1
*/
class CSysApEtelDataBearerObserver : public CActive
    {

public:

    /**
    * Symbian two-phased constructor.
    *
    * @since S60 3.1
    * @param aSysApAppUi SysAp application class object
    * @param aPhone      ETel mobile phone
    */
    static CSysApEtelDataBearerObserver* NewL( CSysApAppUi& aSysApAppUi, RMobilePhone& aPhone );

    /**
    * Destructor
    */
    virtual ~CSysApEtelDataBearerObserver();

    /**
    * Returns whether current data bearer is HSDPA.
    *
    * @since S60 3.1
    * @return TBool ETrue, if current data bearer is HSDPA. 
    */
    TBool IsBearerHsdpa() const;

protected:
    
    // from base class CActive
    void RunL();
    void DoCancel();

private:

    /**
    * C++ default constructor.
    * Not implementation.
    */ 
    CSysApEtelDataBearerObserver();

    /**
    * Constructor.
    */
    CSysApEtelDataBearerObserver( CSysApAppUi& aSysApAppUi, RMobilePhone& aPhone );
    
    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

    /**
    * Orders dynamic capability change notification from ETel.
    *
    * @since S60 3.1
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
    * Packet service information.
    */
    RPacketService iPacketService;
    
    /**
    * Packet service dynamic capabilities.
    */
    RPacketService::TDynamicCapsFlags iDynCaps;
    
    /**
    * If ETrue, there's an active HSDPA connection.
    */
    TBool iHsdpaActive;
    };

#endif // SYSAPETELDATABEAREROBSERVER_H
