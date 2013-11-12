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
* Description:  MSearchEvent API object implementation.
 *
*/

#include "searcheventimp.h"
#include "ximpapiobjbase.h"
#include "ximpapieventbase.h"
#include "searchinfoimp.h"
#include <ximpbase.h>
#include "ximpobjecthelpers.h"
#include "ximpidentityimp.h"
// ---------------------------------------------------------------------------
// CSearchEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchEventImp* CSearchEventImp::NewLC(CXIMPIdentityImp& aGroupId,
            									RSearchInfoImpArray* aSearchResults)
    {
    CSearchEventImp* self = new( ELeave ) CSearchEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aGroupId,	aSearchResults );
            
    return self;
    }

// ---------------------------------------------------------------------------
// CSearchEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CSearchEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CSearchEventImp* self = new( ELeave ) CSearchEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }
// ---------------------------------------------------------------------------
// CSearchEventImp::EqualsContent
// ---------------------------------------------------------------------------
//
TBool CSearchEventImp::EqualsContent( 
    const CXIMPApiEventBase& /*aOtherInstance*/ ) const 
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase
// CSearchEventImp::ExternalizeL
// ---------------------------------------------------------------------------
//
void CSearchEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    iSearchId->ExternalizeL( aStream );
    WriteArrayToStreamL( *iSearchResults, aStream );
    }


// ---------------------------------------------------------------------------
// CSearchEventImp::~CSearchEventImp()
// ---------------------------------------------------------------------------
//
CSearchEventImp::~CSearchEventImp()
    {
    delete iSearchId;
    delete iSearchResults;
    iSearchResults=NULL;
    }


// ---------------------------------------------------------------------------
// CSearchEventImp::CSearchEventImp()
// ---------------------------------------------------------------------------
//
CSearchEventImp::CSearchEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CSearchEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchEventImp::ConstructL(CXIMPIdentityImp& aSearchId,
            						RSearchInfoImpArray* aSearchResults)
    {
    iSearchId=&aSearchId;   // ownership is transfered here, need to delete 
    iSearchResults=aSearchResults; // ownership is transfered here, need to delete 
    }


// ---------------------------------------------------------------------------
// CSearchEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchEventImp::ConstructL( RReadStream& aStream )
    {
    iConstructedFromStream = ETrue;
    iSearchId = CXIMPIdentityImp::NewLC();
    CleanupStack::Pop( 1 ); // iSearchId
    iSearchResults = new ( ELeave ) RSearchInfoImpArray;
    InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchEventImp,
                                     MSearchEvent )
    XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchEventImp,
                                           MSearchEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this ) 
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CSearchEventImp,
                                      MSearchEvent )



// ---------------------------------------------------------------------------
// CSearchEventImp::SearchCount()
// ---------------------------------------------------------------------------
//	
 TInt CSearchEventImp::SearchResultCount() const 
	 {
	 return iSearchResults->Count();	
	 }
// ---------------------------------------------------------------------------
// CSearchEventImp::Search()
// ---------------------------------------------------------------------------
//	 
 const MSearchInfo& CSearchEventImp::SearchResult( TInt aIndex ) const 
	 {
	 return *(*iSearchResults)[ aIndex ]; 	
	 
	 }

// ---------------------------------------------------------------------------
// CSearchEventImp::Search()
// ---------------------------------------------------------------------------
//
    
  const MXIMPIdentity& CSearchEventImp::SearchId() const 
	  {
	  return *iSearchId;	
	  }
    // ---------------------------------------------------------------------------
// CSearchEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CSearchEventImp::WriteArrayToStreamL(
        RSearchInfoImpArray& aArray, RWriteStream& aStream ) const
    {
    TInt count = aArray.Count();
    aStream.WriteInt32L( count );
        {
        for( TInt a = 0; a < count; ++a )
            {
            aArray[ a ]->ExternalizeL( aStream );
            }
        }
    }

// ---------------------------------------------------------------------------
// CSearchEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//
void CSearchEventImp::FillArrayFromStreamL(
        RSearchInfoImpArray& aArray,
        RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CSearchInfoImp* newInfo = CSearchInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aArray.AppendL( newInfo ); // trap here if there is  any error in the append
        CleanupStack::Pop(); // newInfo
        }
    }

// ---------------------------------------------------------------------------
// CSearchEventImp::InternalizeL( )
// ---------------------------------------------------------------------------
//

void CSearchEventImp::InternalizeL(  RReadStream& aStream ) 
    { 
    iSearchId->InternalizeL( aStream ); 
    FillArrayFromStreamL( *iSearchResults, aStream );  
    }
