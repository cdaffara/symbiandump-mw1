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
* Description:  Implementation of MImFeatures
 *
*/


#include <ximpcontext.h>
#include "imapiobjbase.h"
//#include "ximpidentityimp.h"
#include "ximpcontextinternal.h"
#include "imfeaturesimp.h"
#include "imconversationimp.h"
#include "imobjectfactoryimp.h"




// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CImFeaturesImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CImFeaturesImp* CImFeaturesImp::NewL(MXIMPContext* aContext)
    {
    CImFeaturesImp* self = new( ELeave ) CImFeaturesImp();
    CleanupStack::PushL( self );
    self->ConstructL(aContext);
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//

XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CImFeaturesImp, 
                                     MImFeatures )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CImFeaturesImp, 
                                           MImFeatures )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CImFeaturesImp::~CImFeaturesImp()
// ---------------------------------------------------------------------------
//
CImFeaturesImp::~CImFeaturesImp()
    {
    delete iImConv;
    delete iImObjFactory;
    iCtxInternal = NULL;
    }


// ---------------------------------------------------------------------------
// CImFeaturesImp::CImFeaturesImp( CXIMPContextClientProxy& aCtxProxy )
// ---------------------------------------------------------------------------
//
CImFeaturesImp::CImFeaturesImp()
    {
    }

// ---------------------------------------------------------------------------
// CImFeaturesImp::SetCtxL(MXIMPContext* aContext)
// ---------------------------------------------------------------------------
//
void CImFeaturesImp::SetCtxL(MXIMPContext* aContext)
    {
    if(!aContext)
        User::Leave(KErrArgument);
    iCtxInternal = TXIMPGetInterface<MXIMPContextInternal>::From(*aContext,EReturnNullIfUnknown);
                       
    }

// ---------------------------------------------------------------------------
// CImFeaturesImp::ConstructL(MXIMPContext* aContext)
// ---------------------------------------------------------------------------
//
void CImFeaturesImp::ConstructL(MXIMPContext* aContext)
    {
    SetCtxL(aContext);
    iImObjFactory = CImObjectFactoryImp::NewL();
    iImConv = CImConversationImp::NewL( *iCtxInternal );

    }


// ---------------------------------------------------------------------------
// CImFeaturesImp::ImObjectFactory()
// ---------------------------------------------------------------------------
 MImObjectFactory& CImFeaturesImp::ImObjectFactory() const
    {
    return *iImObjFactory;
    }


// ---------------------------------------------------------------------------
// CImFeaturesImp::ImConversation()
// ---------------------------------------------------------------------------
//
 MImConversation& CImFeaturesImp::ImConversation() const
    {
    return *iImConv;
    }

// ---------------------------------------------------------------------------
// CImFeaturesImp::ImGroups() 
// ---------------------------------------------------------------------------
//
// MImGroups& CImFeaturesImp::ImGroups() const
//    {
    //return *iImConv;
//    }
    
 // ---------------------------------------------------------------------------
// CImFeaturesImp::ImInvtation() 
// ---------------------------------------------------------------------------
//
// MImInvitation& CImFeaturesImp::ImInvtation() const
//    {
    //return *iImConv;
//    }
  
   // ---------------------------------------------------------------------------
// CImFeaturesImp::ImSearch() 
// ---------------------------------------------------------------------------
//
// MImSearch& CImFeaturesImp::ImSearch() const
//    {
    //return *iImConv;
//    }  

