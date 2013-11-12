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

#include "presenceapidataobjfactoryaccessor.h"
#include "presenceeventcodec.h"
#include "presenceapidataobjfactory.h"

//DATA TYPES


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceApiDataObjFactoryAccessor::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceApiDataObjFactoryAccessor* CPresenceApiDataObjFactoryAccessor::NewL()
    {
    CPresenceApiDataObjFactoryAccessor* self = new( ELeave ) CPresenceApiDataObjFactoryAccessor() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }


// ---------------------------------------------------------------------------
// CPresenceApiDataObjFactoryAccessor::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceApiDataObjFactoryAccessor::ConstructL()
    {
    // FIXME: would be probably better to have the Event Codec and other factories as singleton
    iPresenceEventCodec = CPresenceEventCodec::NewL();
    
    iPresenceApiDataObjFactory = CPresenceApiDataObjFactory::NewL();
    }


// ---------------------------------------------------------------------------
// CPresenceApiDataObjFactoryAccessor::CPresenceApiDataObjFactoryAccessor()
// ---------------------------------------------------------------------------
//
CPresenceApiDataObjFactoryAccessor::CPresenceApiDataObjFactoryAccessor()
    {
    }

// ---------------------------------------------------------------------------
// CPresenceApiDataObjFactoryAccessor::~CPresenceApiDataObjFactoryAccessor()
// ---------------------------------------------------------------------------
//
CPresenceApiDataObjFactoryAccessor::~CPresenceApiDataObjFactoryAccessor()
    {
    delete iPresenceEventCodec;
    delete iPresenceApiDataObjFactory;
    }

    
// ---------------------------------------------------------------------------
// CPresenceApiDataObjFactoryAccessor::EventCodec()
// ---------------------------------------------------------------------------
//
MEventCodec& CPresenceApiDataObjFactoryAccessor::EventCodec()
    {
    return *iPresenceEventCodec;
    }

// ---------------------------------------------------------------------------
// CPresenceApiDataObjFactoryAccessor::ApiDataObjFactory()
// ---------------------------------------------------------------------------
//
MApiDataObjFactory& CPresenceApiDataObjFactoryAccessor::ApiDataObjFactory()
    {
    return *iPresenceApiDataObjFactory;
    }



// End of file







