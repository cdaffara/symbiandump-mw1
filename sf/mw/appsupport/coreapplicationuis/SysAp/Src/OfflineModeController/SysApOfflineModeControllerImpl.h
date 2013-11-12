/*
 * Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * CSysApOfflineModeController controls the offline mode.
 * It turns OFF RF, BT and WLAN when offline mode is enabled and turns them ON when
 * Offline mode is disabled
 *
 */


#ifndef SYSAPOFFLINEMODECONTROLLERIMPL_H
#define SYSAPOFFLINEMODECONTROLLERIMPL_H

// INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include "MSysApOfflineModeController.h"

// FORWARD DECLARATIONS

class CSysApAppUi;
class CSysApCenRepOfflineModeObserver;

// CLASS DECLARATION
/**
*  CSysApOfflineModeController
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApOfflineModeController : public CBase, public MSysApOfflineModeController
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApOfflineModeController* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    ~CSysApOfflineModeController();

private:

    /**
    * Constructor.
    */         
    //CSysApOfflineModeController();

    /**
    * Constructor.
    */         
    CSysApOfflineModeController( CSysApAppUi& aSysApAppUi );

protected: // From MSysApOfflineModeController

    /**
    * Returns the status of offline mode
    * @param None
    * @return TBool
    */
    TBool OfflineModeActive();

    /**
    * Handles switch from online to offline. Called by 
    * CSysApAppUi::HandleProfileChangedL().
    * @param None
    * @return void
    */
    void SwitchFromOnlineToOfflineModeL();
    
    /**
    * Handles switch from offline to online. Called by 
    * CSysApAppUi::HandleProfileChangedL().
    * @param None
    * @return void
    */
    void SwitchFromOfflineToOnlineModeL();

    /**
    * Checks both DOS and S60 offline mode statuses in startup. If
    * they differ, the device will be set to the mode indicated
    * by DOS. Called by
    * CSysApAppUi::DoSwStateNormalConstructionL()
    * @param None
    * @return void
    */	
    void CheckOfflineModeInitialStatusesL();

    /**
    * To notify MSysApOfflineModeController not to activate
    * BT when going back on-line.
    * @param None
    * @return void
    */	
    void DoNotActivateBt();

    /**
    * Checks whether BT should be activated.
    * @param None
    * @return void
    */	
    TBool MustBtBeActivated();

   /**
    * Checks whether it's ok to go back online.
    * @param None
    * @return void
    */	
    void GoOnlineIfOkL();

    /**
    * Notifies MSysApOfflineModeController not to activate RF e.g. during
    * BT SAP connection.
    * @param None
    * @return void
    */	
    void DoNotActivateRF();

private:

    /**
    * Two-phased constructor.
    * @param None
    * @return void
    */
    void ConstructL();

    /**
    * Returns ETrue if network connections allowed
    * @param None
    * @return TBool
    */
    TBool GetNetworkConnectionAllowed();

    /**
    * Sets the status of the "BT active" setting
    * @param TBool
    * @return void
    */
    void SetBtActiveBeforeOfflineMode( TBool aBtActive );

    /**
    * Returns ETrue if BT was active before Offline Mode.
    * @param None
    * @return TBool
    */
    TBool BtActiveBeforeOfflineMode();

private:

    CSysApAppUi&        iSysApAppUi;
	
    /**
    * Instance of the Offline mode observer. Listens to offline mode changes
	*/	
    CSysApCenRepOfflineModeObserver*         iSysApCenRepOfflineModeObserver;

    TBool               iOfflineModeActive;
    TBool               iDoNotActivateRF;
};

#endif // SYSAPOFFLINEMODECONTROLLERIMPL_H

// End of File
