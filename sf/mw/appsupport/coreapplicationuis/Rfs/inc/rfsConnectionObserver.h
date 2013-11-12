/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
*  
*/


#ifndef __RFSPDPOBSERVER_H
#define __RFSPDPOBSERVER_H

#include <hbdeviceprogressdialogsymbian.h>

// P&S KEYS FROM SIP & PDP CONNECTION
#include <e32property.h>
#include <pdpcontextmanagerpskeys.h>
#include <sipsystemstatemonitorpskeys.h>

#include "rfscontimer.h"

class CRfsConTimer;

enum TRfsConnectionCloseState
    {
    ESipConnectionClose,
    EPdpConnectionClose // this should be the last enum
    };

NONSHARABLE_CLASS( CRfsConnectionObserver ): public CActive,public MHbDeviceProgressDialogObserver
    {      
public:
    
    /**
     * Two-phased constructor
     */
    static CRfsConnectionObserver* NewLC();

    /**
     * Destructor
     */
    virtual ~CRfsConnectionObserver();
    
    void ProgressDialogCancelled(const CHbDeviceProgressDialogSymbian* iDialog);
                    
    void ProgressDialogClosed(const CHbDeviceProgressDialogSymbian *  iDialog) ;

        
public:
    /**
     * New methods
     */

    /**
     * Closes PDP connection.
     * A wait dialog is shown while the connection is torn down.
     *
     * @since S60 v3.1
     * @return ETrue if connection was succesfully closed
     */
    TBool CloseAlwaysOnConnectionL();
        
    /**
     * Re-opens connection in case user does not confirm RFS.
     * 
     * @since S60 v3.1
     */
    void ReOpenPDPConnection();
    void ReportRfsFailureToSip();
    void ReportRfsCompletionToSip();
     /**
      * Subscribes PDP property
      *
      * @since S60 v3.1
      */
     void Subscribe();
         

    
protected:
    
    /**
     * From base class CActive
     */
    void DoCancel();
         
    /**
     * From CActive
     *
     * @since S60 v3.1
     */
    void RunL();
    
    
private:
    
    /**
     * New methods
     */

        
    /**
     * Dismisses closing connections dialog
     *
     * @since S60 v3.1
     */
    void DismissWaitDialog();
    
    /**
     * 2nd phase constructor
     *
     * @since S60 v3.1
     */
    void ConstructL();
    
    /**
     * C++ constructor
     *
     * @since S60 v3.1
     */
    CRfsConnectionObserver();
    
    /**
     * Returns information whether PDP context's Always 
     * online feature is enabled or not
     */
    TInt IsAlwaysOnFeatureEnabledL();

private: 

    /**
     * Own: Wait dialog
     */
     CHbDeviceProgressDialogSymbian* iDialog ;
       /**
     * Own: PDP property
     */
    RProperty iPDPProperty;
    
    /**
     * Own: SIP property
     */
    RProperty iSIPProperty;
    
    /**
     * Following are status flags used to synchronize the code
     * of publishing the active connection close information to 
     * SIP and PDP. Also they are used to decide the proper execution 
     * of the code in the four cases recognized and can be possible 
     * for the devices. These four cases are
     * 
     * CASE 1: SIP Key defined, PDP Key not defined
     * CASE 2: SIP Key defined, PDP key defined
     * CASE 3. SIP key not defined, PDP key not defined
     * CASE 4: SIP key not defined, PDP key defined  
     * 
     * N.B The presence of definition the P&S keys gives us the information
     * whether there will be potential active connections ON.
     * In that case we need to do the following
     *  1. Close all active connections
     *  2. Meanwhile display a WAIT FOR dialog to the user which he can CANCEL at any moment
     *  3. If failure occurs or if user presses the Cancel Key then cancel the RFS operation  
     */
    TBool iAllConnectionClosed;
    TBool iIsWaitForDialogExecuted;
    TBool iIsSIPConnectionsPresent;
    TBool iIsDialogNeedToBeDisplayed;
    TBool iIsSipInformedForClosingAllConnection;
    TBool iIsPDPInformedforClosingAllConnection;
    TBool iIsClosingConnectionsApplicable;
    //for synchronous dialog handling
    CActiveSchedulerWait *iWait;
    
    
public:
    TRfsConnectionCloseState iState;
    TInt  iIsPDPFeatureEnabled;        
    CRfsConTimer* iRfsConTimer;
    };
    
#endif    //RFSPDPOBSERVER_H
