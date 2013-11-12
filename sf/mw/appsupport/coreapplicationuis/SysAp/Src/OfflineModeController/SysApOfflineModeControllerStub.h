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
* Description:  The stub implementation of MSysApOfflineModeController.
*
*/


#ifndef SYSAPOFFLINEMODECONTROLLERSTUB_H
#define SYSAPOFFLINEMODECONTROLLERSTUB_H

// INCLUDES
#include <e32base.h>
#include "MSysApOfflineModeController.h"

// FORWARD DECLARATIONS

class CSysApAppUi;

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
    CSysApOfflineModeController();

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
};

#endif // SYSAPOFFLINEMODECONTROLLERSTUB_H

// End of File
