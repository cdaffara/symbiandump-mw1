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
#include "protocolimdatahostimp.h"
#include "imdatacacheimp.h"
#include "ximphost.h"
#include "imlogutils.h"
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
    
    XImLogger::Log(_L("COperationLaunchCleaner::ProcessL Started"));
	CXIMPOperationBase::ProcessL();
    
    // modified by pankaj
    //iMyHost->ImDataAccess().ImDataCache().CleanExpired();
    MProtocolImDataHost* imProtocolHost = 
					static_cast <MProtocolImDataHost*> (iMyHost->GetProtocolHost(MProtocolImDataHost::KInterfaceId) );
	imProtocolHost->ImDataCache().CleanExpired() ;
    
    iMyHost->HandleInternalRequestCompleted( iReqId, KErrNone );
    XImLogger::Log(_L("COperationLaunchCleaner::ProcessL Completed"));
    }

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::RequestCompleted()
// ---------------------------------------------------------------------------
//
void COperationLaunchCleaner::RequestCompletedL()
    {
    XImLogger::Log(_L("COperationLaunchCleaner::RequestCompletedL Started"));
    CXIMPOperationBase::RequestCompletedL();
    XImLogger::Log(_L("COperationLaunchCleaner::RequestCompletedL Completed"));
    }
// ---------------------------------------------------------------------------
// COperationLaunchCleaner::Type()
// ---------------------------------------------------------------------------
//
TInt COperationLaunchCleaner::Type() const
    {
    return NImOps::EIMCleaner;
    }

// ---------------------------------------------------------------------------
// COperationLaunchCleaner::EndOperation()
// ---------------------------------------------------------------------------
//
void COperationLaunchCleaner::EndOperation()
    {
    XImLogger::Log(_L("COperationLaunchCleaner::EndOperation Started"));
    CXIMPOperationBase::EndOperation();
    iMyHost->StartCleanerTimer();
    XImLogger::Log(_L("COperationLaunchCleaner::EndOperation Completed"));
    }

// End of file
