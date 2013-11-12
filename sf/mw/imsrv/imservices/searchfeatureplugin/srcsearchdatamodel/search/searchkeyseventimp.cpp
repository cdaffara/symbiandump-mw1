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

#include "searchkeyseventimp.h"
#include "ximpapiobjbase.h"
#include "ximpapieventbase.h"

#include <ximpbase.h>
#include "ximpobjecthelpers.h"
#include "searchkeyinfoimp.h"

// ---------------------------------------------------------------------------
// CSearchKeysEventImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CSearchKeysEventImp* CSearchKeysEventImp::NewLC( RSearchKeyInfoImpArray* aKeyInfoArray  )
    {
    CSearchKeysEventImp* self = new( ELeave ) CSearchKeysEventImp();
    CleanupStack::PushL( self );
    self->ConstructL( aKeyInfoArray );           
    return self;
    }

// ---------------------------------------------------------------------------
// CSearchKeysEventImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
CXIMPApiEventBase* CSearchKeysEventImp::NewFromStreamLC( RReadStream& aStream )
    {
    CSearchKeysEventImp* self = new( ELeave ) CSearchKeysEventImp;
    CleanupStack::PushL( self );
    self->ConstructL( aStream );
    return self;
    }
// ---------------------------------------------------------------------------
// CSearchKeysEventImp::EqualsContent
// ---------------------------------------------------------------------------
//
TBool CSearchKeysEventImp::EqualsContent( 
    const CXIMPApiEventBase& /*aOtherInstance*/ ) const 
    {
    return ETrue;
    }

// ---------------------------------------------------------------------------
// From class CXIMPApiEventBase
// CSearchKeysEventImp::ExternalizeL
// ---------------------------------------------------------------------------
//
void CSearchKeysEventImp::ExternalizeL( RWriteStream& aStream ) const
    {
    WriteArrayToStreamL( *iInfoArray, aStream );
    }


// ---------------------------------------------------------------------------
// CSearchKeysEventImp::~CSearchKeysEventImp()
// ---------------------------------------------------------------------------
//
CSearchKeysEventImp::~CSearchKeysEventImp()
    {
    delete iInfoArray;
    iInfoArray = NULL;
    }


// ---------------------------------------------------------------------------
// CSearchKeysEventImp::CSearchKeysEventImp()
// ---------------------------------------------------------------------------
//
CSearchKeysEventImp::CSearchKeysEventImp()
    {
    }


// ---------------------------------------------------------------------------
// CSearchKeysEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchKeysEventImp::ConstructL( RSearchKeyInfoImpArray* aKeyInfoArray  )
    {
    iInfoArray = aKeyInfoArray ; // ownership is transfered here, need to delete 
    }


// ---------------------------------------------------------------------------
// CSearchKeysEventImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CSearchKeysEventImp::ConstructL( RReadStream& aStream )
    {
    iInfoArray = new ( ELeave) RSearchKeyInfoImpArray;
    InternalizeL( aStream );
    }

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CSearchKeysEventImp, 
                                            MSearchKeysEvent )
                                     
XIMPIMP_IF_BASE_GET_CLASS( CXIMPApiEventBase, this )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CSearchKeysEventImp,
        MSearchKeysEvent )
    XIMPIMP_IF_BASE_GET_CONST_CLASS( CXIMPApiEventBase, this ) 
XIMPIMP_IF_BASE_GET_INTERFACE_END()



// ---------------------------------------------------------------------------
// Implement methods from base event interface and base event class
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_EVENT_BASE_METHODS( CSearchKeysEventImp,
                                      MSearchKeysEvent )



// ---------------------------------------------------------------------------
// CSearchKeysEventImp::SupportedKeysCount()
// ---------------------------------------------------------------------------
//	
 TInt CSearchKeysEventImp::InfoCount() const 
	 {
	 return iInfoArray->Count();	
	 }
// ---------------------------------------------------------------------------
// CSearchKeysEventImp::SupportedKey()
// ---------------------------------------------------------------------------
//	 
 const MSearchKeyInfo& CSearchKeysEventImp::SearchKeyInfo( TInt aIndex ) const 
	 {
	 return *(*iInfoArray)[ aIndex ]; 	
	 
	 }


// ---------------------------------------------------------------------------
// CSearchEventImp::FillArrayFromStreamL
// ---------------------------------------------------------------------------
//	
void CSearchKeysEventImp::FillArrayFromStreamL( RSearchKeyInfoImpArray& aKeyInfoArray, 
        						   				RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for( TInt a = 0; a < count; ++a )
        {
        // the stream MUST be in order (since it was externalized by this class)
        CSearchKeyInfoImp* newInfo = CSearchKeyInfoImp::NewLC();
        newInfo->InternalizeL( aStream );
        aKeyInfoArray.AppendL( newInfo ); // trap here if there is  any error in the append
        CleanupStack::Pop(); // newInfo
        }
    }
// ---------------------------------------------------------------------------
// CSearchEventImp::WriteArrayToStreamL()
// ---------------------------------------------------------------------------
//
void CSearchKeysEventImp::WriteArrayToStreamL( RSearchKeyInfoImpArray& aKeyInfoArray, 
        						   			   RWriteStream& aStream  ) const
    {
    TInt count = aKeyInfoArray.Count();
    aStream.WriteInt32L( count );
     
    for( TInt a = 0; a < count; ++a )
        {
        aKeyInfoArray[ a ]->ExternalizeL( aStream );
        }
    }


// ---------------------------------------------------------------------------
// CSearchKeysEventImp::InternalizeL( )
// ---------------------------------------------------------------------------
//

void CSearchKeysEventImp::InternalizeL(  RReadStream& aStream ) 
    { 
    FillArrayFromStreamL( *iInfoArray, aStream );  
    }
