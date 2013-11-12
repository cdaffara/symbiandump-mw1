/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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



#if (!defined __DSTSERVER_H__)
#define __DSTSERVER_H__

#include <test/testexecuteserverbase.h>

class CDstServer : public CTestServer
	{
public:
	RFs& Fs(){return iFs;};

protected:
	RFs iFs;
	};

#endif
