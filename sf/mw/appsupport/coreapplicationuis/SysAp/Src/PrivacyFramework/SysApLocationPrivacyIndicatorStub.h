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
* Description:  The stub implementation of MSysApLocationPrivacyIndicator.
*
*/


#ifndef SYSAPLOCATIONPRIVACYINDICATORSTUB_H
#define SYSAPLOCATIONPRIVACYINDICATORSTUB_H

// INCLUDES
#include <e32base.h>
#include "MSysApLocationPrivacyIndicator.h"

// FORWARD DECLARATIONS

class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApLocationPrivacyIndicator
*  
*  @lib   sysap
*  @since 2.X 
*/

class CSysApLocationPrivacyIndicator : public CBase, public MSysApLocationPrivacyIndicator
{
public:

    /**
    * Two-phased constructor.
    */         
    static CSysApLocationPrivacyIndicator* NewL( CSysApAppUi& aSysApAppUi );

    /**
    * Destructor.
    */
    ~CSysApLocationPrivacyIndicator();

private:

    /**
    * Constructor.
    */         
    CSysApLocationPrivacyIndicator();

protected: // From MSysApLocationPrivacyIndicator

    /**
    * Initializes Location Privacy indicator in startup.
    * @param None
    * @return TBool
    */
    void InitL();
};

#endif // SYSAPLOCATIONPRIVACYINDICATORSTUB_H

// End of File
