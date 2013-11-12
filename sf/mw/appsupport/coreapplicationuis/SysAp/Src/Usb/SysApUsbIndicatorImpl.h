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
* Description:  The implementation of MSysApUsbIndicator.
*
*/


#ifndef SYSAPUSBINDICATORIMPL_H
#define SYSAPUSBINDICATORIMPL_H

// INCLUDES
#include <e32base.h>
#include "MSysApUsbIndicator.h"
#include <usbman.h>
#include <avkon.hrh>

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApUsbIndicator
*  
*  @lib   sysap
*  @since 2.1 
*/

class CSysApUsbIndicator : public CActive, public MSysApUsbIndicator
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApUsbIndicator* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    ~CSysApUsbIndicator();

public: // from CActive
    /**
    *	@param none
    *	@return none
    */    
    void RunL();
    
    /**
    *	@param aError the error returned
    *	@return error
    */
    TInt RunError(TInt aError);

    /**
    *	@param none
    *	@return none
    */    
    void DoCancel();

private:

    /**
    * Constructor.
    */         
    CSysApUsbIndicator( CSysApAppUi& aSysApAppUi );

    /**
    * Two-phased constructor.
    * @param None
    * @return void
    */
    void ConstructL();  

private:

    CSysApAppUi&         iSysApAppUi;
    RUsb                 iUsbMan;
    TUsbDeviceState      iDeviceState;     // current state of USB
    TBool                iIndicatorOn;  // current state of indicator
    
};

#endif // SYSAPUSBINDICATORIMPL_H

// End of File
