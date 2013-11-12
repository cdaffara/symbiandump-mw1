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
* Description:   One value adapter for MDesCArray.
*
*/


//  INCLUDES
#include "tbsmdescarrayadapter.h"
#include <e32std.h>
#include <badesca.h>


//LOCAL constants
namespace
    {
    //Panic
    _LIT( KBSMDesArrayAdapterPanic, "BSDesArrAdp" );

    //Panic reasons
    enum TBSMDesArrayAdapterReasons
        {
        EMdcaPointOutOfBounds
        };

    void BSMDesAdapPanic( TBSMDesArrayAdapterReasons aPanicReason )
        {
        User::Panic( KBSMDesArrayAdapterPanic, aPanicReason );
        }
    }




// ================= MEMBER FUNCTIONS =======================
// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C TBSMDesCArrayAdapter::TBSMDesCArrayAdapter( const TDesC& aDesc )
    : iDesc( aDesc )
    {
    }

// -----------------------------------------------------------------------------
// TBSMDesCArrayAdapter::MdcaCount()
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TBSMDesCArrayAdapter::MdcaCount() const
    {
    //there is just one adapted descriptor
    return 1;
    }

// -----------------------------------------------------------------------------
// TBSMDesCArrayAdapter::MdcaPoint()
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC TBSMDesCArrayAdapter::MdcaPoint( TInt aIndex ) const
    {
    //there is just one adapted descriptor
    __ASSERT_ALWAYS( aIndex == 0, BSMDesAdapPanic( EMdcaPointOutOfBounds ) );
    return iDesc;
    }



//  End of File

