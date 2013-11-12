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

#include "imapidataobjfactoryaccessor.h"
#include "imeventcodec.h"
#include "imapidataobjfactory.h"

//DATA TYPES


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CImApiDataObjFactoryAccessor::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CImApiDataObjFactoryAccessor* CImApiDataObjFactoryAccessor::NewL()
    {
    CImApiDataObjFactoryAccessor* self = new( ELeave ) CImApiDataObjFactoryAccessor() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CImApiDataObjFactoryAccessor::ConstructL()
// ---------------------------------------------------------------------------
//
void CImApiDataObjFactoryAccessor::ConstructL()
    {
    // FIXME: would be probably better to have the Event Codec and other factories as singleton
    iImEventCodec = CImEventCodec::NewL();    
    iImApiDataObjFactory = CImApiDataObjFactory::NewL();
    }


// ---------------------------------------------------------------------------
// CImApiDataObjFactoryAccessor::CImApiDataObjFactoryAccessor()
// ---------------------------------------------------------------------------
//
CImApiDataObjFactoryAccessor::CImApiDataObjFactoryAccessor()
    {
    }

// ---------------------------------------------------------------------------
// CImApiDataObjFactoryAccessor::~CImApiDataObjFactoryAccessor()
// ---------------------------------------------------------------------------
//
CImApiDataObjFactoryAccessor::~CImApiDataObjFactoryAccessor()
    {
    delete iImEventCodec;
    delete iImApiDataObjFactory;
    }

    
// ---------------------------------------------------------------------------
// CImApiDataObjFactoryAccessor::EventCodec()
// ---------------------------------------------------------------------------
//
MEventCodec& CImApiDataObjFactoryAccessor::EventCodec()
    {
    return *iImEventCodec;
    }

// ---------------------------------------------------------------------------
// CImApiDataObjFactoryAccessor::ApiDataObjFactory()
// ---------------------------------------------------------------------------
//
MApiDataObjFactory& CImApiDataObjFactoryAccessor::ApiDataObjFactory()
    {
    return *iImApiDataObjFactory;
    }



// End of file







