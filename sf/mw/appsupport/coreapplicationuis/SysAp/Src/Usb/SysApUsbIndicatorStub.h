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
* Description:  The stub implementation of MSysApUsbIndicator.
*
*/


#ifndef SYSAPUSBINDICATORSTUB_H
#define SYSAPUSBINDICATORSTUB_H

// INCLUDES
#include <e32base.h>
#include "MSysApUsbIndicator.h"

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApUsbIndicator
*  
*  @lib   sysap
*  @since 2.1 
*/

class CSysApUsbIndicator : public CBase, public MSysApUsbIndicator
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
};

#endif // SYSAPUSBINDICATORSTUB_H

// End of File
