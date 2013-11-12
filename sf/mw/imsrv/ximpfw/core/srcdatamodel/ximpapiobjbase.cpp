/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base class for XIMP API implementations.
*
*/

#include "ximpapiobjbase.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CXIMPApiObjBase::CXIMPApiObjBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPApiObjBase::CXIMPApiObjBase()
    {
    }


// ---------------------------------------------------------------------------
// CXIMPApiObjBase::~CXIMPApiObjBase()
// ---------------------------------------------------------------------------
//
EXPORT_C CXIMPApiObjBase::~CXIMPApiObjBase()
    {
    HandleUnknownGetInterface(0,MXIMPBase::EReturnNullIfUnknown);
    }


// ---------------------------------------------------------------------------
// CXIMPApiObjBase::HandleUnknownGetInterface()
// ---------------------------------------------------------------------------
//
EXPORT_C TAny* CXIMPApiObjBase::HandleUnknownGetInterface( 
    TInt32 aInterfaceId, 
    MXIMPBase::TIfGetOps aOps ) const
    {
    XIMP_UNUSED_FORMAL_PARAM( aInterfaceId );

    if( aOps == MXIMPBase::EPanicIfUnknown )
        {
        #ifdef __WINS__
        User::SetJustInTime(ETrue);
        #endif // __WINS__
        
        User::Panic( _L("CXIMPApiObjBase"), KErrExtensionNotSupported );
        }
    
    return NULL;
    }


// End of file
