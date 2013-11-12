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
* Description:  Internal operation for launching cleaner
 *
*/


#include "operationlaunchcleaner.h"
#include "protocolpresencedatahostimp.h"
#include "presencedatacacheimp.h"
#include "ximphost.h"

#include "ximptrace.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::COperationLaunchCleaner()
// ---------------------------------------------------------------------------
//
EXPORT_C COperationLaunchCleaner::COperationLaunchCleaner()
    {
    }



// ---------------------------------------------------------------------------
// COperationLaunchCleaner::~COperationLaunchCleaner()
// ---------------------------------------------------------------------------
//
COperationLaunchCleaner::~COperationLaunchCleaner()
    {
    }

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::ConstructL()
// ---------------------------------------------------------------------------
//
void COperationLaunchCleaner::ConstructL( const TDesC8& /*aParamPck*/ )
    {
    }

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::ProcessL()
// ---------------------------------------------------------------------------
//
void COperationLaunchCleaner::ProcessL()
    {
    TRACE(_L("COperationLaunchCleaner::ProcessL()" ) );
    CXIMPOperationBase::ProcessL();

    iMyHost->PresenceDataAccess().PresenceDataCache().CleanExpired();

    iMyHost->HandleInternalRequestCompleted( iReqId, KErrNone );
    }

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationLaunchCleaner::RequestCompletedL()
    {
    TRACE(_L("COperationLaunchCleaner::RequestCompletedL()" ) );
    CXIMPOperationBase::RequestCompletedL();
    }


// ---------------------------------------------------------------------------
// COperationLaunchCleaner::Type()
// ---------------------------------------------------------------------------
//
TInt
    COperationLaunchCleaner::Type() const
    {
    return NPresenceOps::EXIMPCleaner;
    }

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::EndOperation()
// ---------------------------------------------------------------------------
//
void COperationLaunchCleaner::EndOperation()
    {
    CXIMPOperationBase::EndOperation();
    iMyHost->StartCleanerTimer();
    }


// End of file
