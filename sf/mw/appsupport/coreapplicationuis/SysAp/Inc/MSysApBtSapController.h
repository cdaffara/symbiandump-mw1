/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MSysApBtSapController class definition. A virtual class.
*
*/


#ifndef SYSAPBTSAPCONTROLLER_H
#define SYSAPBTSAPCONTROLLER_H

// CLASS DECLARATION
/**
*  MSysApBtSapController
*  
*  @lib   sysap
*  @since 3.0 
*/

class CSysApAppUi;
class CCoeEnv;

class MSysApBtSapController
{
public:

    /**
    * Destructor.
    */
    virtual ~MSysApBtSapController() {};

    /**
    * Check BT SAP status
    * @param None
    * @return TBool : BT SAP enabled/disabled, always EFalse if BT SAP not supported
    */	
    virtual TBool BtSapEnabled() = 0;

    /**
    * CSysAppUi notifies CSysApBtSapController when closing the applications has been completed
    * @param None
    * @return TInt : error
    */	
    virtual void SimApplicationsClosed() = 0;

    /**
    * Disconnects the remote device and disables BT SAP
    * @param None
    * @return TInt : error
    */	
    virtual TInt Disconnect() = 0;

    /**
    * Notifies MSysApBtSapController that Offline Mode has been activated while BT SAP is active.
    * @param None
    * @return void
    */	
    virtual void SwitchingToOffline() = 0;
    
    /**
    * Creates the device name string for BT Sap disconnect prompt.
    * @return Device name string for disconnect prompt.
    */
    virtual HBufC* GetBtSapDeviceNameL() = 0;
    
    
};

MSysApBtSapController* CreateSysApBtSapControllerL( CSysApAppUi& aSysApAppUi );

#endif // SYSAPBTSAPCONTROLLER_H

// End of File
