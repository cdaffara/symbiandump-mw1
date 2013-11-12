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
* Description:  Implementation of class CConnectionInfoArray.
*
*/


// INCLUDE FILES
#include "ConnectionInfoArray.h"
#include "ConnectionInfo.h"

// CONSTANTS

LOCAL_D const TInt KArrayGranularity = 3;


// ================= MEMBER FUNCTIONS =======================
// ---------------------------------------------------------
// CConnectionInfoArray::CConnectionInfoArray
// ---------------------------------------------------------
//
CConnectionInfoArray::CConnectionInfoArray() 
: CArrayPtrFlat<CConnectionInfo>( KArrayGranularity )
    {
    }

// ---------------------------------------------------------
// CConnectionInfoArray::MdcaCount
// ---------------------------------------------------------
//
TInt  CConnectionInfoArray::MdcaCount() const
    {
    return Count();
    }


// ---------------------------------------------------------
// CConnectionInfoArray::MdcaPoint
// ---------------------------------------------------------
//
TPtrC CConnectionInfoArray::MdcaPoint( TInt aIndex ) const
    {
    return this->At( aIndex )->ListItem();    
    }


// End of File
