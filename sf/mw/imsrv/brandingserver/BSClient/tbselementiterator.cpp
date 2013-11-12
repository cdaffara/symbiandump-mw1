/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  tbselementiterator.cpp
*
*/




// INCLUDE FILES
#include    "tbselementiterator.h"
#include    <E32STD.H>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TBSElementIterator::TBSElementIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
TBSElementIterator::TBSElementIterator( const TDesC& aElementId )
	: iElementId( aElementId )
    {
    }

MBSElement& TBSElementIterator::Next()
    {
    }


//  End of File
