/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:        Defines the stub application class
*
*/







#ifndef __FLASH_STUB__INCLUDE__
#define __FLASH_STUB__INCLUDE__

#include <aknapp.h>
#include <akndoc.h>

class CFlashStubApplication : public CAknApplication
{
public:
	~CFlashStubApplication();
	TUid AppDllUid() const;

protected:
	CApaDocument* CreateDocumentL();
	RLibrary iFlashLibrary;
};

#endif // __FLASH_STUB_INCLUDE__
