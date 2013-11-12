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
* Description:  Base class for API implementations.
*
*/

#include "searchapidataobjfactoryaccessor.h"
#include "searcheventcodec.h"
#include "searchapidataobjfactory.h"

//DATA TYPES


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSearchApiDataObjFactoryAccessor::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchApiDataObjFactoryAccessor* CSearchApiDataObjFactoryAccessor::NewL()
    {
    CSearchApiDataObjFactoryAccessor* self = new( ELeave ) CSearchApiDataObjFactoryAccessor() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CSearchApiDataObjFactoryAccessor::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchApiDataObjFactoryAccessor::ConstructL()
    {
    iEventCodec = CSearchEventCodec::NewL();    
    iSearchApiDataObjFactory = CSearchApiDataObjFactory::NewL();
    }


// ---------------------------------------------------------------------------
// CSearchApiDataObjFactoryAccessor::CSearchApiDataObjFactoryAccessor()
// ---------------------------------------------------------------------------
//
CSearchApiDataObjFactoryAccessor::CSearchApiDataObjFactoryAccessor()
    {
    }

// ---------------------------------------------------------------------------
// CSearchApiDataObjFactoryAccessor::~CSearchApiDataObjFactoryAccessor()
// ---------------------------------------------------------------------------
//
CSearchApiDataObjFactoryAccessor::~CSearchApiDataObjFactoryAccessor()
    {
    delete iEventCodec;
    delete iSearchApiDataObjFactory;
    }

    
// ---------------------------------------------------------------------------
// CSearchApiDataObjFactoryAccessor::EventCodec()
// ---------------------------------------------------------------------------
//
MEventCodec& CSearchApiDataObjFactoryAccessor::EventCodec()
    {
    return *iEventCodec;
    }

// ---------------------------------------------------------------------------
// CSearchApiDataObjFactoryAccessor::ApiDataObjFactory()
// ---------------------------------------------------------------------------
//
MApiDataObjFactory& CSearchApiDataObjFactoryAccessor::ApiDataObjFactory()
    {
    return *iSearchApiDataObjFactory;
    }



// End of file







