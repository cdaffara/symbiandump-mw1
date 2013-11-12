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
* Description:  The stub implementation of MSysApBtController.
*
*/


#ifndef SYSAPBTCONTROLLERSTUB_H
#define SYSAPBTCONTROLLERSTUB_H

// INCLUDES
#include <e32base.h>
#include "MSysApBtController.h"

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApBtController
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApBtController : public CBase, public MSysApBtController
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
    * Constructor.
    */         
    CSysApBtController();

protected: // From MSysApBtController

    TInt SetPowerState( TBool /* aBtState */ );

};

#endif // SYSAPBTCONTROLLERSTUB_H

// End of File
