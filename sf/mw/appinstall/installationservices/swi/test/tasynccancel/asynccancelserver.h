/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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

/**
 @test
 @internalComponent
*/

#ifndef ASYNCCANCELSERVER_H
#define ASYNCCANCELSERVER_H



#include <test/testexecuteserverbase.h>
class CAsyncCancelServer : public CTestServer
	{
public:
	static CAsyncCancelServer* NewL();
	virtual CTestStep* CreateTestStep(const TDesC& aStepName);
private:
	};


	
#endif
// End of file
