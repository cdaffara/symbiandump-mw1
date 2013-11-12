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

#ifndef TSIDLIST_H
#define TSIDLIST_H
#include <e32base.h>
#include <s32strm.h>
NONSHARABLE_CLASS(CTsIdList) : public CBase
    {
public:
    static CTsIdList* NewLC();
    static CTsIdList* NewL();
    static CTsIdList* NewLC(RReadStream& aStream);
    ~CTsIdList();
    TBool IsPresent( TInt aId ) const;
    void AppendL(const TInt aArray[], TInt aSize );
    void AppendL(const TArray<TInt>& aArray);
    void AppendL(TInt aId);
    void Reset();
    TInt Size() const;
    void ExternalizeL(RWriteStream &aStream) const;
    void InternalizeL(RReadStream &aStream);
    TBool operator ==( const CTsIdList& aList) const;
    TBool operator !=( const CTsIdList& aList) const;

private:
    CTsIdList();
    

private:
    RArray<TInt> iIds;

    };
#endif // TSIDLIST_H
