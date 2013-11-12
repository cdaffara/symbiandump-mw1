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
* Description:  CSysApCenRepCallForwardingObserver class definition. 
*
*/


#ifndef SYSAPCENREPCALLFORWARDINGOBSERVER_H
#define SYSAPCENREPCALLFORWARDINGOBSERVER_H

#include <e32base.h>
#include <ctsydomaincrkeys.h>
#include <centralrepository.h>
#include <cenrepnotifyhandler.h>
#include <etelmm.h>
#include "SysApSatObserver.h"

class CSysApAppUi;

/**
*  CSysApCenRepCallForwardingObserver class observes for changes in
*  call forwarding status CenRep key. In addition SIM file change
*  events are monitored via SAT interface.
*
*  @lib None.
*  @since S60 3.2
*/

class CSysApCenRepCallForwardingObserver : public CBase, 
                                           public MCenRepNotifyHandlerCallback,
                                           public MSysApSatObserver
    {
public:
    /**
    * Symbian two-phased constructor.
    *
    * @param aSysApAppUi reference to SysAp's application class
    */
    static CSysApCenRepCallForwardingObserver* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    virtual ~CSysApCenRepCallForwardingObserver();

    /**
    * Updates the call forwarding indicators.
    *
    * @since S60 3.2
    */ 
    void UpdateCallForwardingIndicatorsL();

public:
    // From MCenRepNotifyHandlerCallback
    void HandleNotifyBinary( TUint32 aId, const TDesC8& aNewValue );
    void HandleNotifyError( TUint32 aId, TInt error, CCenRepNotifyHandler* aHandler );
    // From MSysApSatObserver
    void SimFileChangedL();

private:

    /**
    * Symbian 2nd-phase constructor.
    */
    void ConstructL();

    /**
    * C++ default constructor.
    * Prohibited.
    */
    CSysApCenRepCallForwardingObserver();
    
    /**
    * Constructor.
    *
    * @param aSysApAppUi reference to SysAp's application class
    */         
    CSysApCenRepCallForwardingObserver( CSysApAppUi& aSysApAppUi );

    
    /** 
    * Copy constructor.
    * Prohibited.
    */
    CSysApCenRepCallForwardingObserver( const CSysApCenRepCallForwardingObserver& );

    /**
    * Assigment operator
    * Prohibited.
    */
    CSysApCenRepCallForwardingObserver& operator= ( const CSysApCenRepCallForwardingObserver& );
    
    /**
    * Maps call forwarding values stored in SIM to uncondtional CF values.
    *
    * @since S60 3.2
    */
    void MapForwarding();
    
    /**
    * Checks ALS support from product profile and SIM
    * 
    * @since S60 3.2
    * @return TBool ETrue if ALS is supported
    */
    TBool AlsSupport() const;
    
    /**
    * Returns whether SIM supports storing call diverts and
    * whether it should be used for determining indicator
    * status.
    */
    TBool UseSimCallDivertInfo() const;
    
    /**
    * Checks whether mapping of call forwarding values needed to uncondtional CF values.
    *
    * @since S60 3.2
    */
    void DoConditionalMapForwarding();
    
    /**
    * Returns call forwarding status stored by common tsy.
    *
    * @since S60 3.2
    */
    TInt GetSimCallForwardingStatus( RMobilePhone::TCFUIndicatorStatus& callForwardingStatus );

private:
    
    /**
    * SysAp's application class.
    */
    CSysApAppUi& iSysApAppUi;
    
    /**
    * CenRep session.
    * Own.
    */ 
    CRepository* iSession;
    
    /**
    * CenRep notification handler.
    * Own.
    */
    CCenRepNotifyHandler* iCallForwardingHandler;
    
    /**
    * Call forwarding status
    */
    TUnconditionalCFStatus iCallForwardingStatus;
    
    /**
    * Set to ETrue when indicators can be updated to UI.
    */
    TBool iUiReady;
    
    /**
    * Current CF status stored by SIM.
    */
    RMobilePhone::TCFUIndicatorStatus iSimCFCurrentStatus;
    
    /**
    * SIM CF file refresh handler.
    */
    CSysApSatObserver* iSatCFHandler;
    };

#endif      // SYSAPCENREPCALLFORWARDINGOBSERVER_H
            
// End of File
