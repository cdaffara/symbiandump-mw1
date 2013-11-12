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
* Description:  Implementation of ImManagement
 *
*/


//#include "ximpstatusimp.h"
//#include "ximprestrictedobjectcollectionimp.h"
//#include "ximpobjectcollectionimp.h"
//#include "ximpidentityimp.h"
//#include "ximpdatasubscriptionstateimp.h"
#include "imobjectfactoryimp.h"
#include "imconversationinfoimp.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CImObjectFactoryImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CImObjectFactoryImp* CImObjectFactoryImp::NewL()
    {
    CImObjectFactoryImp* self = new( ELeave ) CImObjectFactoryImp;
    return self;
    
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CImObjectFactoryImp, 
                                     MImObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CImObjectFactoryImp, 
                                           MImObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CImObjectFactoryImp::~CImObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CImObjectFactoryImp::~CImObjectFactoryImp()
    {
    }


// ---------------------------------------------------------------------------
// CImObjectFactoryImp::CImObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CImObjectFactoryImp::CImObjectFactoryImp()
    {
    }
    
    
// ---------------------------------------------------------------------------
// CImObjectFactoryImp::NewImConversationInfoLC()
// ---------------------------------------------------------------------------
//
MImConversationInfo* CImObjectFactoryImp::NewImConversationInfoLC()
    {
    return CImConversationInfoImp::NewLC();
    }
 // ---------------------------------------------------------------------------
// CImObjectFactoryImp::NewImGroupInfoLC()
// ---------------------------------------------------------------------------
//
//MImGroupInfo* CImObjectFactoryImp::NewImGroupInfoLC()
    //{
    //return CImConversationInfoImp::NewLC();
    //}
    
    
// ---------------------------------------------------------------------------
// CImObjectFactoryImp::NewImInvitationInfoLC()
// ---------------------------------------------------------------------------
//
//MImInvitationInfo* CImObjectFactoryImp::NewImInvitationInfoLC()
    //{
    //return CImConversationInfoImp::NewLC();
    //}
    
 // ---------------------------------------------------------------------------
// CImObjectFactoryImp::NewImSearchInfoLC()
// ---------------------------------------------------------------------------
//
//MImSearchInfo* CImObjectFactoryImp::NewImSearchInfoLC()
    //{
    //return CImConversationInfoImp::NewLC();
    //}   
    
    
    
 
