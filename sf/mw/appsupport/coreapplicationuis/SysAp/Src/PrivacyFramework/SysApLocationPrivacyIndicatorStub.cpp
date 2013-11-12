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
* Description:  CSysApLocationPrivacyIndicator stub implementation
 *
*/


// INCLUDES

#include "SysApLocationPrivacyIndicatorStub.h"
#include "SysApAppUi.h"
#include "SysAp.hrh"

MSysApLocationPrivacyIndicator* CreateSysApLocationPrivacyIndicatorL( CSysApAppUi& aSysApAppUi )
    {
    return CSysApLocationPrivacyIndicator::NewL( aSysApAppUi );
    }

// ============================ MEMBER FUNCTIONS ==============================

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::NewL() 
// ----------------------------------------------------------------------------
CSysApLocationPrivacyIndicator* CSysApLocationPrivacyIndicator::NewL( CSysApAppUi& /* aSysApAppUi */ )
    {
    TRACES( RDebug::Print( _L("CSysApLocationPrivacyIndicator::NewL") ) ); 
    CSysApLocationPrivacyIndicator* self = new(ELeave) CSysApLocationPrivacyIndicator();
    return self;
    }


// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::CSysApLocationPrivacyIndicator() 
// ----------------------------------------------------------------------------
CSysApLocationPrivacyIndicator::CSysApLocationPrivacyIndicator()
    {
    }

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::~CSysApLocationPrivacyIndicator()
// ----------------------------------------------------------------------------

CSysApLocationPrivacyIndicator::~CSysApLocationPrivacyIndicator()
    {
    }

// ----------------------------------------------------------------------------
// CSysApLocationPrivacyIndicator::InitL()
// ----------------------------------------------------------------------------

void CSysApLocationPrivacyIndicator::InitL()
    {
    }

// End of File



