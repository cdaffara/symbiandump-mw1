/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The stub implementation of MSysApBtSapController.
*
*/


#ifndef SYSAPBTSAPCONTROLLERSTUB_H
#define SYSAPBTSAPCONTROLLERSTUB_H

// INCLUDES
#include <e32base.h>
#include "MSysApBtSapController.h"

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApBtSapController
*  
*  @lib   sysap
*  @since 3.0
*/

class CSysApBtSapController : public CBase, public MSysApBtSapController
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
    * Constructor.
    */         
    CSysApBtSapController();

protected: // From MSysApBtSapController
    
    TBool BtSapEnabled();
    void  SimApplicationsClosed();
    TInt  Disconnect();
    void  SwitchingToOffline();
};

#endif // SYSAPBTSAPCONTROLLERSTUB_H

// End of File
