// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/**
 @file
 @internalComponent - Internal Symbian test code  
*/

#if (!defined __TCNF_STEP_H__)
#define __TCNF_STEP_H__
#include "TConArcTestServer.h"
#include <concnf.h>

class CTCnfStep : public CTestStep
	{
public:
	CTCnfStep();
	~CTCnfStep();
	virtual TVerdict doTestStepL();
private:
	void testConverterCNFWriterL();
	void CreateConv1L();
	void ChangeLocaleL(TLanguage aLanguage);
private:
	TDataType mimeWord_Tcnf;
	TTranslation engword_Tcnf;
	TTranslation frword_Tcnf;
	};

_LIT(KTCnfStep,"TCnf");
#endif

