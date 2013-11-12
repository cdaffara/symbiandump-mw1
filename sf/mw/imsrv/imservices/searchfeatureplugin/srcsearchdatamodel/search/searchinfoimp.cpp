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
* Description:  MsearchInfo API object implementation.
*
*/

#include "searchinfoimp.h"
#include "ximpidentityimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSearchInfoImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchInfoImp* CSearchInfoImp::NewLC()
    {
    CSearchInfoImp* self = new( ELeave ) CSearchInfoImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// CSearchInfoImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchInfoImp* CSearchInfoImp::NewL()
    {
    CSearchInfoImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CSearchInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CSearchInfoImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchInfoImp, MSearchInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()
XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchInfoImp, MSearchInfo )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CSearchInfoImp::~CSearchInfoImp()
// ---------------------------------------------------------------------------
//
CSearchInfoImp::~CSearchInfoImp()
    {
    iFirstname.Close();
    iLastname.Close();
    iEmailId.Close();
    }

// ---------------------------------------------------------------------------
// CSearchInfoImp::CSearchInfoImp()
// ---------------------------------------------------------------------------
//
CSearchInfoImp::CSearchInfoImp()
    {
    }


// ---------------------------------------------------------------------------
// CSearchInfoImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchInfoImp::ConstructL()
    {
    iFirstname.CreateL( 0 );
    iLastname.CreateL( 0 );
    iEmailId.CreateL( 0 );
    }

// ---------------------------------------------------------------------------
// CSearchInfoImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
void CSearchInfoImp::ExternalizeL( RWriteStream& aStream ) const
    {
  
    XIMPRBuf16Helper::ExternalizeL( iFirstname, aStream );
    XIMPRBuf16Helper::ExternalizeL( iLastname, aStream );
    XIMPRBuf16Helper::ExternalizeL( iEmailId, aStream ); 
    }

// ---------------------------------------------------------------------------
// CSearchInfoImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CSearchInfoImp::InternalizeL( RReadStream& aStream )
    {
    XIMPRBuf16Helper::InternalizeL( iFirstname, aStream );
    XIMPRBuf16Helper::InternalizeL( iLastname, aStream );
    XIMPRBuf16Helper::InternalizeL( iEmailId, aStream );
     
    }
  


// ---------------------------------------------------------------------------
// From MSearchInfo class.
// CSearchInfoImp::GetFieldInfo()
// ---------------------------------------------------------------------------
//
const TDesC16& CSearchInfoImp::GetFieldInfo(TInfoType aType) const
    {
     switch(aType)
	     {
	   	 case EFirstName:
		   	 {
		   	 return iFirstname;	
		   	 }
	   	 case ELastName:
		   	 {
		   	 return iLastname;	
		   	 }
	   	 case EEmailAddress:
		   	 {
		   	 return iEmailId;	
		   	 }
	     default :
	         {
	         return KNullDesC();
	         }
	     }
    }
    


// ---------------------------------------------------------------------------
// From MsearchInfo class.
// CSearchInfoImp::SetFieldInfoL()
// ---------------------------------------------------------------------------
//
void CSearchInfoImp::SetFieldInfoL(const TDesC16& aFieldInfo ,TInfoType aType) 
    
    {
    switch(aType)
	     {
	   	 case EFirstName:
		   	 {
		   	 HBufC16* buf = aFieldInfo.AllocL();
    		 iFirstname.Close();
    		 iFirstname.Assign( buf );
		   	 break;
		   	 }
	   	 case ELastName:
		   	 {
		   	 HBufC16* buf= aFieldInfo.AllocL();
    		 iLastname.Close();
             iLastname.Assign( buf );
		   	 break;	
		   	 }
	   	 case EEmailAddress:
		   	 {
		   	 HBufC16* buf = aFieldInfo.AllocL();
    		 iEmailId.Close();
    		 iEmailId.Assign( buf );
		   	 break;	
		   	 }
	   
	     }
   
    }
 

// ---------------------------------------------------------------------------
// CSearchInfoImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CSearchInfoImp::EqualsContent( 
    const CXIMPApiDataObjBase& /*aOtherInstance*/ ) const
    {
    return ETrue; 
    }
// ---------------------------------------------------------------------------
// CSearchInfoImp:: InfoLinearOrder
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CSearchInfoImp::InfoLinearOrder(
        const CSearchInfoImp& /*aA*/,
        const CSearchInfoImp& /*aB*/ )
    {
    // Just returns 1 so that the results are appended as they come from server.
    return 1;
    }
    

// End of file
