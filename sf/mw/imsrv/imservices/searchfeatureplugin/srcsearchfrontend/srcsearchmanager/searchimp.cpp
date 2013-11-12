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
* Description:  Implementation of MSearch
*
*/



#include <ximpobjecthelpers.h>
#include <ximpidentityimp.h>
#include <ximpcontextinternal.h>

#include "searchoperationdefs.h"
#include "searchimp.h"
#include "searchelementimp.h"



// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSearchImp::NewL()
// ---------------------------------------------------------------------------
//

 CSearchImp* CSearchImp::NewL( MXIMPContextInternal& aContext )
	{
	XSearchLogger::Log(_L("CSearchImp::NewL Started"));
	CSearchImp* self = new( ELeave ) CSearchImp( aContext );    
	XSearchLogger::Log(_L("CSearchImp::NewL Completed"));
	return self;
	}


// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
	XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchImp, 
	                                     MSearch )
	XIMPIMP_IF_BASE_GET_INTERFACE_END()


	XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchImp, 
	                                           MSearch )
	XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// CSearchImp::~CSearchImp()
// ---------------------------------------------------------------------------
//	
CSearchImp::~CSearchImp()
    {
	XSearchLogger::Log(_L("CSearchImp::~CSearchImp Started - Completed"));
	}
// ---------------------------------------------------------------------------
// CSearchImp::CSearchImp()
// ---------------------------------------------------------------------------
//    
CSearchImp::CSearchImp(MXIMPContextInternal& aContext )
    : iContext( aContext )
    {
    }


 // ---------------------------------------------------------------------------
// CSearchImp::SearchL
// ---------------------------------------------------------------------------
//
 TXIMPRequestId CSearchImp::SearchL( const MXIMPIdentity& aSearchId,
    								 const RPointerArray< MSearchElement >& aObjs,
                                    		   TInt aSearchLimit )
    {
     
	CBufFlat* buffer = CBufFlat::NewL( 10 );
    CleanupStack::PushL( buffer );

    RBufWriteStream ws;
    CleanupClosePushL( ws );
    ws.Open( *buffer ); // CSI: 65 # Does not return a value


  // identity
    const CXIMPIdentityImp* identityImp =
        TXIMPGetImpClassOrPanic< const CXIMPIdentityImp >::From( aSearchId );
    identityImp->ExternalizeL( ws );
    
  
    
    // Get count of objects
    TInt objCount( aObjs.Count() );
    // write the count
    ws.WriteInt32L( objCount );
    // objects
    for ( TInt count(0); count < objCount; count++ )
        {
        const CSearchElementImp* eleImp = 
	    TXIMPGetImpClassOrPanic< const CSearchElementImp >::From( *aObjs[count] );
        eleImp->ExternalizeL( ws );
        }

    // write the Search Limit
    ws.WriteInt32L( aSearchLimit );
    
    ws.CommitL();
    CleanupStack::PopAndDestroy();  //ws

    HBufC8* packedBuf = buffer->Ptr( 0 ).AllocL();
    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PushL( packedBuf );
    

    // Queue the operation
    TXIMPRequestId reqId =
        iContext.QueueOperationL( NSearchOps::ESearch, *packedBuf );
    CleanupStack::PopAndDestroy( packedBuf );

                                 
    return reqId ;
    } 
 
 // ---------------------------------------------------------------------------
// CSearchImp::GetSearchKeysL
// ---------------------------------------------------------------------------
//
 TXIMPRequestId CSearchImp::GetSearchKeysL()
    {  
    return iContext.QueueOperationL( NSearchOps::EGetSearchKeys , KNullDesC8);
    } 

// end of file


