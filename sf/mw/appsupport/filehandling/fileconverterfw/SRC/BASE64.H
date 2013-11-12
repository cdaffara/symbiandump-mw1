// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#if !defined(__e32base_h__)
#include <e32base.h>
#endif

const TInt KErrInvalidSource=(-1);
const TUint8 KErrInvalidCharacter=65;

enum TBase64Panic
	{
	EBase64Overflow
	};

class CBase64CodecBase : public CBase
	{
public:
	void Reset();
	TInt Encode(const TDesC8& aSource, TDes8& aResult) const;
	TInt Decode(const TDesC8& aSource, TDes8& aResult);
	TUint8 Base64Char(TText8 aChar) const;
private:
	TUint8 iQuart[4];
	TInt iQuartCount;
	};

