/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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

#include "imapidataobjfactory.h"
#include "ximpapidataobjbase.h"

#include <s32strm.h>


// ============================ MEMBER FUNCTIONS =============================


// ---------------------------------------------------------------------------
// CImEventCodec::NewL()
// ---------------------------------------------------------------------------
//
CImApiDataObjFactory* CImApiDataObjFactory::NewL()
    {
    CImApiDataObjFactory* self = new( ELeave ) CImApiDataObjFactory() ;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CImEventCodec::ConstructL()
// ---------------------------------------------------------------------------
//
void CImApiDataObjFactory::ConstructL()
    {
    }


/**
 * Method to instantiate new data object object
 * of requested type and construct it from the stream.
 *
 * @param aDataObjInterfaceId
 * @return Data object constructor function.
 */
CXIMPApiDataObjBase* CImApiDataObjFactory::NewFromStreamLC( 
            TInt32 aDataObjInterfaceId,
            RReadStream& aStream )
    {
    return NewDataObjectFromStreamLC( aDataObjInterfaceId, aStream );
    }

// ---------------------------------------------------------------------------
// CImApiDataObjFactory::CImApiDataObjFactory()
// ---------------------------------------------------------------------------
//
CImApiDataObjFactory::CImApiDataObjFactory()
    {
    }

// ---------------------------------------------------------------------------
// CImApiDataObjFactory::~CImApiDataObjFactory()
// ---------------------------------------------------------------------------
//
CImApiDataObjFactory::~CImApiDataObjFactory()
    {
    }


// ---------------------------------------------------------------------------
// ImApiDataObjFactory::InternalizeL
// ---------------------------------------------------------------------------
//
void CImApiDataObjFactory::InternalizeL( 
        RReadStream& aStream,
        RImReqParamArray& aArray )
    {
    TInt32 arrLen = aStream.ReadInt32L();

    for ( TInt i = 0; i < arrLen; i++ )
        {
        SImReqParam param;
        param.iParamType = aStream.ReadInt32L();
        CXIMPApiDataObjBase* dataObject = NewDataObjectFromStreamLC( param.iParamType, aStream );
        param.iParam = dataObject;
        aArray.AppendL( param );
        CleanupStack::Pop( dataObject );
        }
    }

// ---------------------------------------------------------------------------
// ImApiDataObjFactory::ExternalizeL
// ---------------------------------------------------------------------------
//
void CImApiDataObjFactory::ExternalizeL( 
            RWriteStream& aWs,
            const RImReqParamArray& aArray )
    {
    // write array length
    aWs.WriteInt32L( aArray.Count() );

    for ( TInt i = 0; i < aArray.Count(); i++ )
        {
        // trust the type within the param struct
        aWs.WriteInt32L( aArray[ i ].iParamType );

        // write the actual object
        CXIMPApiDataObjBase* theBase = aArray[ i ].iParam;
        theBase->ExternalizeL( aWs );
        }

    aWs.CommitL();
    }

// ---------------------------------------------------------------------------
// ImApiDataObjFactory::InternalizeLC
// ---------------------------------------------------------------------------
//
CXIMPApiDataObjBase* CImApiDataObjFactory::InternalizeLC(
        RReadStream& aStream )
    {
    TInt32 objType = aStream.ReadInt32L();
    CXIMPApiDataObjBase* dataObject = NewDataObjectFromStreamLC( objType, aStream );
    return dataObject;
    }

// ---------------------------------------------------------------------------
// ImApiDataObjFactory::ExternalizeL
// ---------------------------------------------------------------------------
//
void CImApiDataObjFactory::ExternalizeL(
        RWriteStream& aWs,
        const CXIMPApiDataObjBase& aDataObj )
    {
    aWs.WriteInt32L( aDataObj.Base().GetInterfaceId() );
    aDataObj.ExternalizeL( aWs );
    aWs.CommitL();
    }


// End of file

