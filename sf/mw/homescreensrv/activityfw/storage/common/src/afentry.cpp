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
#include "afentry.h"
#include <s32mem.h>

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create and initialize instance
 * @return entry instance
 */
CAfEntry* CAfEntry::NewL()
{
    CAfEntry *self = CAfEntry::NewLC();
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create, initialize and push instance into cleanup stack
 * @return entry instance
 */
CAfEntry* CAfEntry::NewLC()
{
    CAfEntry *self = new (ELeave)CAfEntry();
    CleanupStack::PushL(self);
    return self;
}

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create and initialize instance
 * @param flags - entry flags
 * @param applicationId - application unique identifier
 * @param activityId - activity unique identifier
 * @param imgSrc - thumbanail source
 * @param privateData - privated application data
 * @param publicData - public activity data
 * @return entry instance
 */
CAfEntry* CAfEntry::NewL(TInt flags,
                         TInt applicationId,
                         const TDesC &activityId,
                         const TDesC &customActivityName,
                         const TDesC &imgSrc,
                         const TDesC8 &privateData,
                         const TDesC8 &publicData,
                         const TTime &timestamp)
{
    CAfEntry* self = CAfEntry::NewLC(flags, 
                                     applicationId, 
                                     activityId, 
                                     customActivityName,
                                     imgSrc, 
                                     privateData, 
                                     publicData,
                                     timestamp);
    CleanupStack::Pop(self);
    return self;
}

// -----------------------------------------------------------------------------
/**
 * Two-phase constructor. Create, initialize and push instance into cleanup stack
 * @param flags - entry flags
 * @param applicationId - application unique identifier
 * @param activityId - activity unique identifier
 * @param imgSrc - thumbanail source
 * @param privateData - privated application data
 * @param publicData - public activity data
 * @return entry instance
 */
CAfEntry* CAfEntry::NewLC(TInt flags,
                          TInt applicationId,
                          const TDesC &activityId,
                          const TDesC &customActivityName,
                          const TDesC &imgSrc,
                          const TDesC8 &privateData,
                          const TDesC8 &publicData,
                          const TTime &timestamp)
{
    CAfEntry *self = CAfEntry::NewLC();
    self->ConstructL(flags, 
                     applicationId, 
                     activityId, 
                     customActivityName,
                     imgSrc, 
                     privateData, 
                     publicData,
                     timestamp);
    return self;
}

// -----------------------------------------------------------------------------
CAfEntry* CAfEntry::NewLC(const RMessage2& msg, int offset)
{
    CAfEntry* self = CAfEntry::NewLC();
    RBuf8 serializedEntry;
    CleanupClosePushL(serializedEntry);
    ReallocL(serializedEntry, msg.GetDesMaxLength(offset));
    msg.ReadL(offset, serializedEntry);
    RDesReadStream entryReader(serializedEntry);
    CleanupClosePushL(entryReader);
    entryReader >> (*self);
    CleanupStack::PopAndDestroy(&entryReader);
    CleanupStack::PopAndDestroy(&serializedEntry);
    return self;
}
// -----------------------------------------------------------------------------
/**
 * First phase constructor
 */
CAfEntry::CAfEntry()
{
}

// -----------------------------------------------------------------------------
/**
 * Second phase constructor. Initialize instance
 * @param flags - entry flags
 * @param applicationId - application unique identifier
 * @param activityId - activity unique identifier
 * @param imgSrc - thumbanail source
 * @param privateData - privated application data
 * @param publicData - public activity data 
 */
void CAfEntry::ConstructL(TInt flags,
                          TInt applicationId,
                          const TDesC &activityId,
                          const TDesC &customActivityName,
                          const TDesC &imgSrc,
                          const TDesC8 &privateData,
                          const TDesC8 &publicData,
                          const TTime &timestamp)
{
    mFlags = flags;
    mAppId = applicationId;
    CopyL(mActivityId, activityId);
    CopyL(mCustomActivityName, customActivityName);
    CopyL(mImgSrc, imgSrc);
    CopyL(mPrivateData, privateData);
    CopyL(mPublicData, publicData);
    mTimestamp = timestamp;
}

// -----------------------------------------------------------------------------
/**
 * Destructor. Release allocated resources 
 */
CAfEntry::~CAfEntry()
{
    mActivityId.Close();
    mCustomActivityName.Close();
    mPrivateData.Close();
    mPublicData.Close();
    mImgSrc.Close();
}

// -----------------------------------------------------------------------------
/**
 * Provide size of serialized entry
 * @return size of serialized entry instance
 */
TInt CAfEntry::Size() const
{
    return (sizeof(TInt) * 4) + //flags + appId + actId size info + customActivityName size info
           mActivityId.Size() + //actId content size
           mCustomActivityName.Size() + // customActivityName content size
           TPckgBuf<TTime>().Size() + // timestamp size
           DataSize(); //data size
           
}

// -----------------------------------------------------------------------------
/**
 * Provide size of serialized entry
 * @return size of serialized entry instance
 */
TInt CAfEntry::DataSize() const
{
    return (sizeof(TInt) * 3) + //privData size info + pubData size info + imgSrc size info
            mImgSrc.Size() + //imgSize content size
            mPrivateData.Size() + //privData content size
            mPublicData.Size(); //pubData content size 
}
// -----------------------------------------------------------------------------
/**
 * Serialize entry content into output stream.
 * @param stream - output stream
 */
void CAfEntry::ExternalizeL(RWriteStream &stream) const
{
    stream.WriteInt32L(mFlags);
    stream.WriteInt32L(mAppId);
    ExternalizeL(stream, mActivityId);
    ExternalizeL(stream, mCustomActivityName);
    stream.WriteL(TPckgBuf<TTime>(mTimestamp));
    ExternalizeDataOnlyL(stream);
}

// -----------------------------------------------------------------------------
/**
 * Deserialize entry content from input stream
 * @param stream - input stream
 */
void CAfEntry::InternalizeL(RReadStream &stream)
{
    mFlags = stream.ReadInt32L();
    mAppId = stream.ReadInt32L();
    InternalizeL(mActivityId, stream);
    InternalizeL(mCustomActivityName, stream);
    TPckgBuf<TTime> timestampBuf;
    stream.ReadL(timestampBuf);
    mTimestamp = timestampBuf();
    InternalizeDataOnlyL(stream);
}

// -----------------------------------------------------------------------------
/**
 * Serialize entry content into output stream.
 * @param stream - output stream
 */
void CAfEntry::ExternalizeDataOnlyL(RWriteStream &stream) const
{
    ExternalizeL(stream, mImgSrc);
    ExternalizeL(stream, mPrivateData);
    ExternalizeL(stream, mPublicData);
}

// -----------------------------------------------------------------------------
/**
 * Deserialize entry content from input stream
 * @param stream - input stream
 */
void CAfEntry::InternalizeDataOnlyL(RReadStream &stream)
{
    
    InternalizeL(mImgSrc, stream);
    InternalizeL(mPrivateData, stream);
    InternalizeL(mPublicData, stream);
    
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity flags
 * @return activity flags 
 */
TInt CAfEntry::Flags() const
{
    return mFlags;
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity identifier.
 * @return activity identifier
 */
TInt CAfEntry::ApplicationId() const
{
    return mAppId;
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity identifier.
 * @return activity identifier
 */
const TDesC& CAfEntry::ActivityId() const
{
    return mActivityId;
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity custom name.
 * @return activity custom name
 */
const TDesC& CAfEntry::CustomActivityName() const
{
    return mCustomActivityName;
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity data.
 * @param rights - type of requested data
 * @return activity data
 */
const TDesC8& CAfEntry::Data(CAfEntry::AccessRights rights) const
{
    return Private == rights ? mPrivateData : mPublicData;
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity data.
 * @param rights - type of requested data
 * @return activity data
 */
void CAfEntry::SetDataL(const TDesC8& src, CAfEntry::AccessRights rights)
{
    CopyL(Private == rights ? mPrivateData : mPublicData, src);
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity timestamp
 * @return activity timestamp 
 */
TTime CAfEntry::Timestamp() const
{
    return mTimestamp;
}

// -----------------------------------------------------------------------------
/**
 * Provide access to activity thumbail path
 * @return path to activity thumbnail 
 */
const TDesC& CAfEntry::ImageSrc() const
{
    return mImgSrc;
}

// -----------------------------------------------------------------------------
/**
 * Set new value of image source
 */
void CAfEntry::SetImageSrcL(const TDesC& src)
{
    CopyL(mImgSrc, src);
}

// -----------------------------------------------------------------------------
/**
 * Reallocate blob buffer to requested size
 * @param dst - destination buffer
 * @param length - requested length
 */
void CAfEntry::ReallocL(RBuf8 &dst,TInt length)
{
    if (0 < length) {
        if (dst.MaxLength() < length) {
            dst.ReAllocL(length);
        }
    } else {
        dst.Close();
    }
}

// -----------------------------------------------------------------------------
/**
 * Reallocate text buffer to requested size
 * @param dst - destination buffer
 * @param length - requested length
 */
void CAfEntry::ReallocL(RBuf &dst,TInt length)
{
    if (0 < length) {
        if (dst.MaxLength() < length) {
            dst.ReAllocL(length);
        }
    } else {
        dst.Close();
    }
}
// -----------------------------------------------------------------------------
/**
 * Copy blob content from input stream
 * @param dst - destination buffer
 * @param src - source buffer
 */
void CAfEntry::CopyL(RBuf8 &dst,const TDesC8 &src)
{
    ReallocL(dst, src.Length());
    if(0 < src.Length()) {
        dst.Copy(src);
    }
}

// -----------------------------------------------------------------------------
/**
 * Copy text content from input buffer
 * @param dst - destination buffer
 * @param src - source buffer
 */
void CAfEntry::CopyL(RBuf &dst,const TDesC &src)
{
    ReallocL(dst, src.Length());
    if(0 < src.Length()) {
        dst.Copy(src);
    }
}

// -----------------------------------------------------------------------------
/**
 * Deserialize blob content from input stream
 * @param dst - destination buffer
 * @param src - input stream
 */
void CAfEntry::InternalizeL(RBuf8 &dst, RReadStream &src)
{
    const TInt length(src.ReadInt32L());
    ReallocL(dst, length);
    if (0 < length) {
        src.ReadL(dst, length);
    }
}

// -----------------------------------------------------------------------------
/**
 * Deserialize text content from input stream
 * @param dst - destination buffer
 * @param src - input stream
 */
void CAfEntry::InternalizeL(RBuf &dst, RReadStream &src)
{
    const TInt length(src.ReadInt32L());
    ReallocL(dst, length);
    if (0 < length) {
        src.ReadL(dst, length);
    }
}

// -----------------------------------------------------------------------------
/**
 * Serialize blob content into output stream
 * @param dst - destination stream
 * @param src - input buffer
 */
void CAfEntry::ExternalizeL(RWriteStream &dst,const TDesC8 & src)
{
    dst.WriteInt32L(src.Length());
    if (src.Length()) {
        dst.WriteL(src, src.Length());
    }
}

// -----------------------------------------------------------------------------
/**
 * Serialize text content into output stream
 * @param dst - destination stream
 * @param src - input buffer
 */
void CAfEntry::ExternalizeL(RWriteStream &dst,const TDesC& src)
{
    dst.WriteInt32L(src.Length());
    if (src.Length()) {
        dst.WriteL(src, src.Length());
    }
}

// -----------------------------------------------------------------------------
RPointerArray<CAfEntry>& operator <<(RPointerArray<CAfEntry>& dst, const TDesC8 &src)
{
    dst.ResetAndDestroy();
    RDesReadStream srcStream(src);
    CleanupClosePushL(srcStream);
    int numOfItems(srcStream.ReadInt32L());
    for (int i(0); i < numOfItems; ++i) {
        CAfEntry *entry = CAfEntry::NewLC();
        srcStream >> (*entry);
        dst.AppendL(entry);
        CleanupStack::Pop(entry);
    }
    CleanupStack::PopAndDestroy(&srcStream);
    return dst;
}

// -----------------------------------------------------------------------------
RBuf8& operator <<(RBuf8 &dst, const RPointerArray<CAfEntry>& src)
{
    int iter(0), 
        requiredSize(sizeof(int));
    for(iter =0; iter< src.Count(); ++iter) {
        requiredSize += src[iter]->Size();
    }
    CAfEntry::ReallocL(dst, requiredSize);
    RDesWriteStream dstStream(dst);
    CleanupClosePushL(dstStream);
    dstStream.WriteInt32L(src.Count());
    for (iter =0; iter < src.Count(); ++iter) {
        dstStream << *(src[iter]);
    }
    CleanupStack::PopAndDestroy(&dstStream);
    return dst;
}
