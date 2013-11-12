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






#include "saflash.h"
#include <flash_ui.h>

////////////////////////////////////////////////////////////////////////////////
// App initialization functions
////////////////////////////////////////////////////////////////////////////////

#include <eikstart.h>
#include <aknnotewrappers.h>

LOCAL_C CApaApplication* NewApplication( )
{
	return new(ELeave) CFlashStubApplication;
}

GLDEF_C TInt E32Main()
    {
    return EikStart::RunApplication(NewApplication);
    }

CFlashStubApplication::~CFlashStubApplication()
{
	if (iFlashLibrary.Handle())
	{
		iFlashLibrary.Close();
	}
}

const TUid KUidFlashStubApp = { 0x10282355};


TUid CFlashStubApplication::AppDllUid() const
{
	return KUidFlashStubApp;
}

_LIT(KStubFlashContent, "C:\\Data\\Others\\trans.swf");

CApaDocument* CFlashStubApplication::CreateDocumentL()
{
	FlashUIConfig config;
	config.iIsMMI = EFalse;
	config.iIsStubApp = ETrue;
	config.iContentFileName.Copy(KStubFlashContent);
	config.iContentChecksum = 0;
	config.iExtensions = NULL;
	config.iReserved1 = 0;
	config.iReserved2 = 0;
	if(!FlashStubsSupported())
	{
		User::Leave(KErrNotSupported);
	}
	return CreateFlashDocumentL(iFlashLibrary, this, config);
}



