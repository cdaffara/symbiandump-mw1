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
* Description:  Implementation of MsearchFeature
 *
*/


#include <ximpcontext.h>
#include "searchapiobjbase.h"

#include "ximpcontextinternal.h"
#include "searchfeatureimp.h"

#include "searchimp.h"
#include "searchobjectfactoryimp.h"



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSearchFeatureImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchFeatureImp* CSearchFeatureImp::NewL(MXIMPContext* aContext)
    {
    CSearchFeatureImp* self = new( ELeave ) CSearchFeatureImp();
    CleanupStack::PushL( self );
    self->ConstructL(aContext);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//

XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchFeatureImp, 
                                     MSearchFeature )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchFeatureImp, 
                                           MSearchFeature)
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CSearchFeatureImp::~CSearchFeatureImp()
// ---------------------------------------------------------------------------
//
CSearchFeatureImp::~CSearchFeatureImp()
    {
    delete iSearch;
    delete iSearchObjFactory;
    iCtxInternal = NULL;
    }


// ---------------------------------------------------------------------------
// CSearchFeatureImp::CSearchFeatureImp( CXIMPContextClientProxy& aCtxProxy )
// ---------------------------------------------------------------------------
//
CSearchFeatureImp::CSearchFeatureImp()
    {
    }

// ---------------------------------------------------------------------------
// CSearchFeatureImp::SetCtxL(MXIMPContext* aContext)
// ---------------------------------------------------------------------------
//
void CSearchFeatureImp::SetCtxL(MXIMPContext* aContext)
    {
    if(!aContext)
        User::Leave(KErrArgument);
    iCtxInternal = TXIMPGetInterface<MXIMPContextInternal>::From(*aContext,EReturnNullIfUnknown);
                       
    }

// ---------------------------------------------------------------------------
// CSearchFeatureImp::ConstructL(MXIMPContext* aContext)
// ---------------------------------------------------------------------------
//
void CSearchFeatureImp::ConstructL(MXIMPContext* aContext)
    {
    SetCtxL(aContext);
    iSearchObjFactory = CSearchObjectFactoryImp::NewL();
    iSearch = CSearchImp::NewL( *iCtxInternal );

    }


// ---------------------------------------------------------------------------
// CSearchFeatureImp::SearchObjectFactory()
// ---------------------------------------------------------------------------
 MSearchObjectFactory& CSearchFeatureImp::SearchObjectFactory() const
    {
    return *iSearchObjFactory;
    }



   // ---------------------------------------------------------------------------
// CSearchFeatureImp::Search() 
// ---------------------------------------------------------------------------
//
 MSearch& CSearchFeatureImp::Search() const
    {
    return *iSearch;
    }  

