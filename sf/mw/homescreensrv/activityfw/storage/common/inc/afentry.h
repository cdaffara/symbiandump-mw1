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
#ifndef AFENTRYDATA_H
#define AFENTRYDATA_H

#include <e32base.h>
#include <s32strm.h>
class RDbColWriteStream;
class RDbColReadStream;
const TInt KAfMask(0x0001);

#ifdef AF_ADD_MASK
#undef AF_ADD_MASK
#endif
#define AF_ADD_MASK(n) (KAfMask << n)

class CAfEntry: public CBase
{
public:
    enum AccessRights {
    Private =0,
    Public
    };

    enum Flags {
    Invisible = KAfMask,
    Persistent = 0x40000000,
    };

public:
    static CAfEntry* NewL();

    static CAfEntry* NewLC();

    static CAfEntry* NewL(TInt flags,
                          TInt applicationId,
                          const TDesC &activityId,
                          const TDesC &customActivityName,
                          const TDesC &imgSrc,
                          const TDesC8 &privateData,
                          const TDesC8 &publicData,
                          const TTime &timestamp);

    static CAfEntry* NewLC(TInt flags,
                           TInt applicationId,
                           const TDesC &activityId,
                           const TDesC &customActivityName,
                           const TDesC &imgSrc,
                           const TDesC8 &privateData,
                           const TDesC8 &publicData,
                           const TTime &timestamp);
    
    static CAfEntry* NewLC(const RMessage2& msg,
                           int offset =0);

    ~CAfEntry();

    TInt Size() const;

    TInt DataSize() const;

    void ExternalizeL(RWriteStream &stream) const;

    void InternalizeL(RReadStream &stream);

    void ExternalizeDataOnlyL(RWriteStream &stream) const;

    void InternalizeDataOnlyL(RReadStream &stream);

    TInt Flags() const;

    TInt ApplicationId() const;

    const TDesC& ActivityId() const;
    
    const TDesC& CustomActivityName() const;
    
    TTime Timestamp() const;

    const TDesC& ImageSrc() const;

    void SetImageSrcL(const TDesC& src);

    const TDesC8& Data(CAfEntry::AccessRights rights) const;

    void SetDataL(const TDesC8& src, CAfEntry::AccessRights rights);

    static void ReallocL(RBuf8 &dst,TInt length);

    static void ReallocL(RBuf &dst,TInt length);

    static void CopyL(RBuf8 &dst,const TDesC8 &src);

    static void CopyL(RBuf &dst,const TDesC &src);

private:
    CAfEntry();

    void ConstructL(TInt flags,
                    TInt applicationId,
                    const TDesC &activityId,
                    const TDesC &customActivityName,
                    const TDesC &imgSrc,
                    const TDesC8 &privateData,
                    const TDesC8 &publicData,
                    const TTime &timestamp);

    static void InternalizeL(RBuf8 &dst, RReadStream &src);

    static void InternalizeL(RBuf &dst, RReadStream &src);

    static void ExternalizeL(RWriteStream &dst,const TDesC8 & src);

    static void ExternalizeL(RWriteStream &dst,const TDesC& src);
private:
    TInt mFlags;
    TInt mAppId;
    RBuf mActivityId;
    RBuf mCustomActivityName;
    RBuf mImgSrc;
    RBuf8 mPrivateData;
    RBuf8 mPublicData;
    TTime mTimestamp;

};

RPointerArray<CAfEntry>& operator <<(RPointerArray<CAfEntry>& dst, const TDesC8 &src);

RBuf8& operator << (RBuf8 &dst, const RPointerArray<CAfEntry>& src);

#endif
