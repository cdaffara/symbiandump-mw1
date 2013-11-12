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
* Description:  Search Service  operation factory
 *
*/


#include "searchoperationfactoryimp.h"

#include "searchlogutils.h"
#include "searchoperationdefs.h"
#include "operationsearch.h"
#include "operationhandlesearchresult.h"
#include "operationgetsearchkeys.h"
#include "operationhandlesearchkeys.h"

using namespace NSearchOps;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::NewL()
// 
// -----------------------------------------------------------------------------
//
EXPORT_C CSearchOperationFactory* CSearchOperationFactory::NewL()
    {
    XSearchLogger::Log(_L("CSearchOperationFactory::NewL Started"));
    CSearchOperationFactory* self = new ( ELeave ) CSearchOperationFactory();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    XSearchLogger::Log(_L("CSearchOperationFactory::NewL Completed"));
    return self;
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CSearchOperationFactory::CSearchOperationFactory()
    {
    }

// -----------------------------------------------------------------------------
// CXIMPOperationFactory::~CXIMPOperationFactory()
// -----------------------------------------------------------------------------
//
CSearchOperationFactory::~CSearchOperationFactory()
    {
    XSearchLogger::Log(_L("CSearchOperationFactory::~CSearchOperationFactory Started"));
    XSearchLogger::Log(_L("CSearchOperationFactory::~CSearchOperationFactory Completed"));
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::ConstructL()
// -----------------------------------------------------------------------------
//
void CSearchOperationFactory::ConstructL()
    {
    XSearchLogger::Log(_L("CSearchOperationFactory::ConstructL Started"));
    XSearchLogger::Log(_L("CSearchOperationFactory::ConstructL Completed"));
    }


// -----------------------------------------------------------------------------
// CXIMPOperationFactory::CreateOperationL()
// -----------------------------------------------------------------------------
//
CXIMPOperationBase* CSearchOperationFactory::CreateOperationL( TInt aOperation )
	{
	XSearchLogger::Log(_L("CSearchOperationFactory::CreateOperationL Started"));
	CXIMPOperationBase* op = NULL;
    NSearchOps::TSearchOpTypes operation = ( NSearchOps::TSearchOpTypes ) aOperation;
    switch( operation )
        {
        case ESearch:
            {
            XSearchLogger::Log(_L("CSearchOperationFactory::CreateOperationL  = ESearch"));
            op = new ( ELeave ) COperationSearch();
            break;
            }
        case EHandleSearchResults:
            {
            XSearchLogger::Log(_L("CSearchOperationFactory::CreateOperationL  = EHandleSearchResults"));
            op = new ( ELeave ) COperationHandleSearchResult();
            break;
            }
      case EGetSearchKeys:
            {
            XSearchLogger::Log(_L("CSearchOperationFactory::CreateOperationL  = EGetSearchKeys"));
            op = new ( ELeave ) COperationGetSearchKeys();
            break;
            }
      case EHandleSearchKeys:
            {
            XSearchLogger::Log(_L("CSearchOperationFactory::CreateOperationL  = EHandleSearchKeys"));
            op = new ( ELeave ) COperationHandleSearchKeys();
            break;
            }      
        default:
        	{
            break;
            }
        }
    XSearchLogger::Log(_L("CSearchOperationFactory::CreateOperationL Completed"));
   	return op;
	}
// End of file


