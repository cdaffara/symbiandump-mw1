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
// RECSCHEME.H
// 
//

#ifndef __WEBURLREC_H__
#define __WEBURLREC_H__

#if !defined(__APMREC_H__)
#include <apmrec.h>
#endif

const TInt KMaxQualiferLength=12;

class CApaSchemeRecognizer : public CApaDataRecognizerType
// Recognises schemes which form the front of urls
	{
public: // from CApaDataRecognizerType
	CApaSchemeRecognizer();
	TUint PreferredBufSize();
	TDataType SupportedDataTypeL(TInt aIndex) const;
#ifdef __UI_FRAMEWORKS_V2__
	static CApaDataRecognizerType* NewL();
#endif //__UI_FRAMEWORKS_V2__
private:
	// from CApaDataRecognizerType
	void DoRecognizeL(const TDesC& aName, const TDesC8& aBuffer);
	TBool FindScheme(const TDesC& aScheme, const TDesC& aUrl);
	};

#endif // __WEBURLREC_H__
