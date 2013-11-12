/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Im Service Connection operation factory
 *
*/


#include "imoperationfactoryimp.h"
#include "imoperationdefs.h"
#include "operationlaunchcleaner.h"
//#include "ximpoperationbase.h"
//#include "ximppanics.h"
#include "imlogutils.h"
#include "operationsendmessage.h"
#include "operationreceivemessage.h"
using namespace NImOps;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::NewL()
// Singleton access method.
// -----------------------------------------------------------------------------
//
EXPORT_C CImOperationFactory* CImOperationFactory::NewL()
    {
    XImLogger::Log(_L("CImOperationFactory::NewL Started"));
    CImOperationFactory* self = new ( ELeave ) CImOperationFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    XImLogger::Log(_L("CImOperationFactory::NewL Completed"));
    return self;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CImOperationFactory::CImOperationFactory()
    {
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::~CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CImOperationFactory::~CImOperationFactory()
    {
    XImLogger::Log(_L("CImOperationFactory::~CImOperationFactory Started"));
    XImLogger::Log(_L("CImOperationFactory::~CImOperationFactory Completed"));
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::ConstructL()
// -----------------------------------------------------------------------------
//
void CImOperationFactory::ConstructL()
    {
    XImLogger::Log(_L("CImOperationFactory::ConstructL Started"));
    XImLogger::Log(_L("CImOperationFactory::ConstructL Completed"));
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CreateOperationL()
// -----------------------------------------------------------------------------
//
CXIMPOperationBase* CImOperationFactory::CreateOperationL( TInt aOperation )
{
	XImLogger::Log(_L("CImOperationFactory::CreateOperationL Started"));
	CXIMPOperationBase* op = NULL;
    NImOps::TImOpTypes operation = ( NImOps::TImOpTypes ) aOperation;
    switch( operation )
        {
        case ESendImMessage:
            {
            XImLogger::Log(_L("CImOperationFactory::CreateOperationL  = ESendImMessage"));
            op = new ( ELeave ) COperationSendMessage();
            break;
            }
        case EIMCleaner:
            {
            XImLogger::Log(_L("CImOperationFactory::CreateOperationL  = EIMCleaner"));
            op = new ( ELeave ) COperationLaunchCleaner();
            break;
            }
        case EHandleReceiveMessage: 
            {
            XImLogger::Log(_L("CImOperationFactory::CreateOperationL  = EHandleReceiveMessage"));
            op = new ( ELeave ) COperationReceiveMessage();
            break;	
            }
        default:
        	{
            break;
            }
        }
    XImLogger::Log(_L("CImOperationFactory::CreateOperationL Completed"));
   return op;
}
// End of file


