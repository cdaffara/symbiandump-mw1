// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined(__RECWAP_H__)
#define __RECWAP_H__


// base includes
#include <apmrec.h> // CApaDataRecognizerType


// constants
const TInt KMimeWapRecognizerValue=0x1000515E;
const TUid KUidMimeWapRecognizer={KMimeWapRecognizerValue};


//
// CApaWapRecognizer
//
//##ModelId=3B666F81033C
class CApaWapRecognizer : public CApaDataRecognizerType
	{
public: // from CApaDataRecognizerType
	//##ModelId=3B666F81035B
	CApaWapRecognizer();
	//##ModelId=3B666F81035A
	virtual TUint PreferredBufSize();
	//##ModelId=3B666F810358
	virtual TDataType SupportedDataTypeL(TInt aIndex) const;
#ifdef __UI_FRAMEWORKS_V2__
	static CApaDataRecognizerType* NewL();
#endif //__UI_FRAMEWORKS_V2__
private: // from CApaDataRecognizerType
	//##ModelId=3B666F81034F
	virtual void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
	};

#endif // __RECWAP_H__
