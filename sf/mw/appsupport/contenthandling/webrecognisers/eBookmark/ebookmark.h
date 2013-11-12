// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __EBookmark_H__
#define __EBookmark_H__

#if !defined(__APMREC_H__)
#include <apmrec.h>
#endif

const TInt KMaxQualiferLength=12;

class CEBookmarkRecognizer : public CApaDataRecognizerType
// Recognizes eBookmark from files with eBm extension and BEGIN: and URL: fields
	{
public: // from CApaDataRecognizerType
	CEBookmarkRecognizer();
	TUint PreferredBufSize();
	TDataType SupportedDataTypeL(TInt aIndex) const;
#ifdef __UI_FRAMEWORKS_V2__
	static CApaDataRecognizerType* NewL();
#endif //__UI_FRAMEWORKS_V2__
private:
	// from CApaDataRecognizerType
	void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
	};

#endif // __EBookmark_H__
