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
* Description:  Msearchelement  implementation.
*
*/


#include <ximpobjecthelpers.h>
#include "searchelementimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSearchElementImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchElementImp* CSearchElementImp::NewLC()
    {
    CSearchElementImp* self = new( ELeave ) CSearchElementImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CSearchElementImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchElementImp* CSearchElementImp::NewL()
    {
    CSearchElementImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSearchElementImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CSearchElementImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchElementImp, MSearchElement )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchElementImp, MSearchElement )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CSearchElementImp::~CSearchElementImp()
// ---------------------------------------------------------------------------
//
CSearchElementImp::~CSearchElementImp()
    {
    delete iIdentity;
    iIdentity=NULL;
    delete iLabelIdentity;
    iLabelIdentity=NULL;
    }

// ---------------------------------------------------------------------------
// CSearchElementImp::CSearchElementImp()
// ---------------------------------------------------------------------------
//
CSearchElementImp::CSearchElementImp()
    {
    }


// ---------------------------------------------------------------------------
// CSearchElementImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchElementImp::ConstructL()
    {
    iIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iIdentity );
    iLabelIdentity = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( iLabelIdentity );
    }

// ---------------------------------------------------------------------------
// CSearchElementImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CSearchElementImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iIdentity->ExternalizeL(aStream);
    iLabelIdentity->ExternalizeL(aStream);
    aStream.WriteInt32L(iSearchKey);
    }

// ---------------------------------------------------------------------------
// CSearchElementImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchElementImp::InternalizeL( RReadStream& aStream )
    {
    iIdentity->InternalizeL(aStream);
    iLabelIdentity->InternalizeL(aStream);
   	iSearchKey=aStream.ReadInt32L();
    }
  
    


// ---------------------------------------------------------------------------
// CSearchElementImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CSearchElementImp::EqualsContent( 
    const CXIMPApiDataObjBase& /*aOtherInstance*/ ) const
    {
    return ETrue;
    }
 // ---------------------------------------------------------------------------
// CSearchElementImp::SetRequestL
// ---------------------------------------------------------------------------
//
void CSearchElementImp::SetRequestL( MXIMPIdentity& aSearchStringId,
        										  TSearchKey aKey )
                                   				  
    {
    if ( EReadLabel == aKey ) // if client trys to set EReadLabel then leave
        {
        User::Leave(  KErrArgument );
        }
    const CXIMPIdentityImp* identityImp =
    			TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( aSearchStringId );
    						
    delete iIdentity;
    iIdentity=NULL;
    iIdentity =TXIMPObjectCloner< CXIMPIdentityImp>::CloneL(*identityImp );
    						
    iSearchKey=aKey;
   
    }

// ---------------------------------------------------------------------------
// CSearchElementImp::SetRequestL
// ---------------------------------------------------------------------------
//
void CSearchElementImp::SetRequestL( MXIMPIdentity& aSearchStringId,
                                     MXIMPIdentity& aLabelId )
                                                  
    {
    const CXIMPIdentityImp* identityImp =
                TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( aSearchStringId );
                            
    delete iIdentity;
    iIdentity=NULL;
    iIdentity =TXIMPObjectCloner< CXIMPIdentityImp>::CloneL(*identityImp );
    
    const CXIMPIdentityImp* labelIdentity = TXIMPGetImpClassOrPanic< CXIMPIdentityImp>::From( aLabelId );                      
    delete iLabelIdentity;
    iLabelIdentity=NULL;
    iLabelIdentity =TXIMPObjectCloner< CXIMPIdentityImp>::CloneL(*labelIdentity );
    iSearchKey = EReadLabel ; // make sure that if label is there then key is EReadLabel
   
    }
// ---------------------------------------------------------------------------
// CSearchElementImp::GetSearchId
// ---------------------------------------------------------------------------
//    
MXIMPIdentity& CSearchElementImp::GetSearchId() const
    {
    return *iIdentity;	
    }
// ---------------------------------------------------------------------------
// CSearchElementImp::GetSearchLabel
// ---------------------------------------------------------------------------
//    
MXIMPIdentity& CSearchElementImp::GetSearchLabel() const
    {
    return *iLabelIdentity;  
    }
// ---------------------------------------------------------------------------
// CSearchElementImp ::GetSearchKey
// ---------------------------------------------------------------------------
//        
TSearchKey CSearchElementImp::GetSearchKey() const
   {
   return (TSearchKey)iSearchKey;
   }

// End of file
