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


#ifndef CFPENDINGPERSISTENCYTASK_H
#define CFPENDINGPERSISTENCYTASK_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION
class CCFOperationNode;

/**
* CCFPendingPersistencyTask is used to store information
* about store and restore operations during system backup and restore.
*/
NONSHARABLE_CLASS( CCFPendingPersistencyTask ) : public CBase
	{
    public: // Types

	    /**
	    * Store or restore operation.
	    */
	    enum TCFPersistencyMode
	        {
	        EStore = 0, 
	        ERestore, 
	        };

	public: // Constructors and destructor
	
		/**
		* Destructor.
		*/
		~CCFPendingPersistencyTask();
	
		/**
		* Two-phased constructor.
		*/
		static CCFPendingPersistencyTask* NewL( TCFPersistencyMode aMode,
				const TDesC& aFileName, CCFOperationNode& aNode );
	
		/**
		* Two-phased constructor.
		*/
		static CCFPendingPersistencyTask* NewLC( TCFPersistencyMode aMode,
				const TDesC& aFileName, CCFOperationNode& aNode );
	
	public: // New functions
	
		TCFPersistencyMode Mode();
		const TDesC& FileName();
		CCFOperationNode& Operation();
		
		
	private:
	
		/**
		* Constructor for performing 1st stage construction
		*/
		CCFPendingPersistencyTask( TCFPersistencyMode aMode,
				CCFOperationNode& aNode );
	
		/**
		* EPOC default constructor for performing 2nd stage construction
		*/
		void ConstructL( const TDesC& aFileName );
		
		
	private:

		// Own: persistency operation type: Store/Restore
		TCFPersistencyMode iPersistencyMode;
		
		// Own: file name for the persistency operation
		HBufC* iFileName;
		
		// Ref: reference to the operation node requested the persistency op.
		CCFOperationNode* iOperationNode;
	};

#endif // CFPENDINGPERSISTENCYTASK_H

