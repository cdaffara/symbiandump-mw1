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
* Description:  Implementation of objectfactory
 *
*/



#include "searchobjectfactoryimp.h"

#include "searchelementimp.h"
#include "searchinfoimp.h"
#include "searchkeyinfoimp.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSearchObjectFactoryImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchObjectFactoryImp* CSearchObjectFactoryImp::NewL()
    {
    CSearchObjectFactoryImp* self = new( ELeave ) CSearchObjectFactoryImp;
    return self;
    
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchObjectFactoryImp, 
                                     MSearchObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchObjectFactoryImp, 
                                           MSearchObjectFactory )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CSearchObjectFactoryImp::~CSearchObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CSearchObjectFactoryImp::~CSearchObjectFactoryImp()
    {
    }


// ---------------------------------------------------------------------------
// CSearchObjectFactoryImp::CSearchObjectFactoryImp()
// ---------------------------------------------------------------------------
//
CSearchObjectFactoryImp::CSearchObjectFactoryImp()
    {
    }
    
    

 // ---------------------------------------------------------------------------
// CSearchObjectFactoryImp::NewSearchElementLC()
// ---------------------------------------------------------------------------
//
MSearchElement* CSearchObjectFactoryImp::NewSearchElementLC()
    {
    return CSearchElementImp::NewLC();
    }   
    
  // ---------------------------------------------------------------------------
// CSearchObjectFactoryImp::NewSearchInfoLC()
// ---------------------------------------------------------------------------
//
MSearchInfo* CSearchObjectFactoryImp::NewSearchInfoLC()
    {
    return CSearchInfoImp::NewLC();
    }    

// ---------------------------------------------------------------------------
// CSearchObjectFactoryImp::NewSupportedSearchKeyLC()
// ---------------------------------------------------------------------------
//
 MSearchKeyInfo* CSearchObjectFactoryImp::NewSearchKeyInfoLC()
   {
   return CSearchKeyInfoImp::NewLC();
   }    
    

