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
* Description:  MSysApOfflineModeController class definition. A virtual class.
*
*/


#ifndef SYSAPOFFLINEMODECONTROLLER_H
#define SYSAPOFFLINEMODECONTROLLER_H

// CLASS DECLARATION
/**
*  MSysApOfflineModeController
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApAppUi;

class MSysApOfflineModeController
{
public:

    /**
    * Destructor.
    */
    virtual ~MSysApOfflineModeController() {};

    /**
    * Returns the status of offline mode
    * @param None
    * @return TBool
    */
    virtual TBool OfflineModeActive() = 0;

    /**
    * Handles switch from online to offline. Called by 
    * CSysApAppUi::HandleProfileChangedL().
    * @param None
    * @return void
    */
    virtual void SwitchFromOnlineToOfflineModeL() = 0;
    
    /**
    * Handles switch from offline to online. Called by 
    * CSysApAppUi::HandleProfileChangedL().
    * @param None
    * @return void
    */
    virtual void SwitchFromOfflineToOnlineModeL() = 0;

    /**
    * Checks both DOS and S60 offline mode statuses in startup. If
    * they differ, the device will be set to the mode indicated
    * by DOS. Called by
    * CSysApAppUi::DoSwStateNormalConstructionL()
    * @param None
    * @return void
    */	
    virtual void CheckOfflineModeInitialStatusesL() = 0;

    /**
    * To notify MSysApOfflineModeController not to activate
    * BT when going back on-line.
    * @param None
    * @return void
    */	
    virtual void DoNotActivateBt() = 0;

    /**
    * Checks whether BT should be activated.
    * @param None
    * @return void
    */	
    virtual TBool MustBtBeActivated() = 0;

    /**
    * Checks whether it's ok to go back online.
    * @param None
    * @return void
    */	
    virtual void GoOnlineIfOkL() = 0;

    /**
    * Notifies MSysApOfflineModeController not to activate RF e.g. during
    * BT SAP connection.
    * @param None
    * @return void
    */	
    virtual void DoNotActivateRF() = 0;

};

MSysApOfflineModeController* CreateSysApOfflineModeControllerL( CSysApAppUi& aSysApAppUi );

#endif // SYSAPOFFLINEMODECONTROLLER_H

// End of File
