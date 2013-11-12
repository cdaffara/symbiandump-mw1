/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The implementation of MSysApBtSapController.
*
*/


#ifndef SYSAPBTSAPCONTROLLERIMPL_H
#define SYSAPBTSAPCONTROLLERIMPL_H

// INCLUDES
#include <e32base.h>
#include "MSysApBtSapController.h"
#include "MSysApPropertyResponder.h"

// FORWARD DECLARATIONS

class CSysApAppUi;
class CSysApSubscriber;
class CCoeEnv;

// CLASS DECLARATION
/**
*  CSysApBtSapController
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApBtSapController : 
              public CBase, 
              public MSysApBtSapController,
              public MSysApPropertyResponder
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApBtSapController* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    ~CSysApBtSapController();
    
    /**
    * Creates the device name string for BT Sap disconnect prompt.
    * @return The device name for disconnect prompt.
    */
    HBufC* GetBtSapDeviceNameL();

private:

    /**
    * Constructors.
    */         
    CSysApBtSapController( CSysApAppUi& aSysApAppUi );
    void ConstructL();

protected: // From MSysApBtSapController

    TBool BtSapEnabled();
    void  SimApplicationsClosed();
    TInt  Disconnect();
    void  SwitchingToOffline();

protected: // From MSysApPropertyResponder

    void HandlePropertyChangedL( const TUid& aCategory, const TUint aKey );
 
private:

    /**
     * Gets called after SAP Server disabling SAP
     * @param None
     * @return void
     */       
    void FinalizeDisconnect();

private:
    
    CSysApAppUi&        iSysApAppUi;
    CSysApSubscriber*   iBtSapSubscriber;    
    TBool               iBtSapEnabled;
    TBool               iSwitchingToOffline;
    TBool               iFinalizeDisconnectNeeded;
};

#endif // SYSAPBTSAPCONTROLLERIMPL_H

// End of File
