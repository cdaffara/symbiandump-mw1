/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The implementation of MSysApBtController.
*
*/


#ifndef SYSAPBTCONTROLLERIMPL_H
#define SYSAPBTCONTROLLERIMPL_H

// INCLUDES
#include <e32base.h>
#include <btengsettings.h>
#include "MSysApBtController.h"

// FORWARD DECLARATIONS

class CSysApAppUi;
class CSysApSubscriber;

// CLASS DECLARATION
/**
*  CSysApBtController
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApBtController : 
              public CBase, 
              public MSysApBtController
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApBtController* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    ~CSysApBtController();

private:

    /**
    * Constructors.
    */         
    CSysApBtController( CSysApAppUi& aSysApAppUi );
    void ConstructL();

protected: // From MSysApBtController

    TInt SetPowerState( TBool aBtState );
 
private:
    
    CSysApAppUi&        iSysApAppUi;
    
    CBTEngSettings* iBTEngSettings;
};

#endif // SYSAPBTCONTROLLERIMPL_H

// End of File
