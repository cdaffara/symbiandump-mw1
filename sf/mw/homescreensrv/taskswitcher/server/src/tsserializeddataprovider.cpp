/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include <s32mem.h>
#include "tsserializeddataprovider.h"
#include "tsmodel.h"
// -----------------------------------------------------------------------------
/**
 * Leaving constructor. 
 * @param observer - data observer
 */
CTsSerializedDataProvider* CTsSerializedDataProvider::NewL(MTsDataObserver &observer)
{
    return new (ELeave) CTsSerializedDataProvider(observer);
    
}

// -----------------------------------------------------------------------------
/**
 * Constructor
 * @param observer - data observer
 */
CTsSerializedDataProvider::CTsSerializedDataProvider(MTsDataObserver& observer)
:
    iObserver(observer)
{}

// -----------------------------------------------------------------------------
/**
 * Destructor
 */
CTsSerializedDataProvider::~CTsSerializedDataProvider()
{
    iData.Close();
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation.
 * @see MTsDataProvider::Data()
 */
const TDesC8& CTsSerializedDataProvider::Data() const
{
    return iData;
}

// -----------------------------------------------------------------------------
/**
 * Interface implementation.
 * @see MTsModelObserver::dataChanged(MTsModel &)
 */
void CTsSerializedDataProvider::DataChanged(MTsModel &model)
{
    serializeModel(model);
}

// -----------------------------------------------------------------------------
/**
 * Serialize source model into internal data storage
 * @param src - model
 */
void CTsSerializedDataProvider::serializeModel(const MTsModel &src)
{
    TRAPD(errNo, serializeModelL(src));
    if(KErrNone == errNo ) {
        iObserver.DataChanged();
    }
}

// -----------------------------------------------------------------------------
/**
 * Serialize source model into internal data storage. Function can leave.
 * @param src - source model
 */
void CTsSerializedDataProvider::serializeModelL(const MTsModel &src)
{
    const TInt KExpandSize = 256;
    CBufFlat* buf = CBufFlat::NewL(KExpandSize);
    CleanupStack::PushL(buf);
    RBufWriteStream stream(*buf); 
    CleanupClosePushL(stream);
    
    serializeModelL(stream, src);
    
    CleanupStack::PopAndDestroy(&stream);
    iData.Close();
    iData.CreateL(buf->Size());
    buf->Read(0, iData, buf->Size());
    CleanupStack::PopAndDestroy(buf);
}

// -----------------------------------------------------------------------------
/**
 * Serialize source model into destination stream. Function can leave.
 * @param dst - destination stream
 * @param src - source model
 */
void CTsSerializedDataProvider::serializeModelL(RWriteStream& dst, const MTsModel &src)
{
    dst.WriteInt32L( src.Count() );
    for (TInt iter(0); iter < src.Count(); ++iter) {
        dst << src.ItemL( iter );
    }
}
