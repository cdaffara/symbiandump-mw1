/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPendingPersistencyTask class declaration.
*
*/

#include "cfpendingpersistencytask.h"

CCFPendingPersistencyTask::CCFPendingPersistencyTask( TCFPersistencyMode aMode,
		CCFOperationNode& aNode ) :
		iPersistencyMode( aMode ),
		iOperationNode( &aNode )
	{
	}


CCFPendingPersistencyTask::~CCFPendingPersistencyTask()
	{
	delete iFileName;
	}

CCFPendingPersistencyTask* CCFPendingPersistencyTask::NewLC( 
		TCFPersistencyMode aMode,
		const TDesC& aFileName, 
		CCFOperationNode& aNode )
    {
	CCFPendingPersistencyTask* self = new (ELeave)CCFPendingPersistencyTask(
			aMode, aNode );
	CleanupStack::PushL(self);
	self->ConstructL( aFileName );
	return self;
    }

CCFPendingPersistencyTask* CCFPendingPersistencyTask::NewL( 
		TCFPersistencyMode aMode,
		const TDesC& aFileName, 
		CCFOperationNode& aNode )
    {
	CCFPendingPersistencyTask* self=CCFPendingPersistencyTask::NewLC(
			aMode, aFileName, aNode );
	CleanupStack::Pop(); // self;
	return self;
    }

void CCFPendingPersistencyTask::ConstructL( const TDesC& aFileName )
	{
	iFileName = aFileName.AllocL();
	}


CCFPendingPersistencyTask::TCFPersistencyMode CCFPendingPersistencyTask::Mode()
	{
	return iPersistencyMode;
	}

const TDesC& CCFPendingPersistencyTask::FileName()
	{
	if( iFileName )
		{
		return *iFileName;
		}
	else
		{
		return KNullDesC;
		}
	}

CCFOperationNode& CCFPendingPersistencyTask::Operation()
	{
	return *iOperationNode;
	}
