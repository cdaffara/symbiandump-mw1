/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CBSSERVER.H
*
*/



#ifndef __CBSSERVER_H__
#define __CBSSERVER_H__
#include <e32base.h>

#include "rbsobjowningptrarray.h"
#include "mbsbackupobserver.h"

// FORWARD DECLARATIONS
class CBSSession;
class CBSStorageManager;
class CBSInstallHandler;
class CBSBackupObserver;

/**
 * Service server.
 */
NONSHARABLE_CLASS( CBSServer ): public CPolicyServer,
                                public MBSBackupObserver
    {
    public:
    	NONSHARABLE_CLASS( CBSFileMapping ): public CBase
    		{
    		public:
	    		static CBSFileMapping* NewL( CBSSession* aSession,
	    									 const TDesC& aFile,
	    									 TBool aVersioned );
	    		
	    		~CBSFileMapping();
    		private: // constructors 
	    		void ConstructL( CBSSession* aSession,
								 const TDesC& aFile );
	    		CBSFileMapping( TBool aVersioned );
			
			public: // new methods
				CBSSession* Session();
				const TDesC& File();
				TBool Versioned();
				
			private: // data
				// doesn't own: session
				CBSSession* iSession;
				HBufC* 		iFile;
			  	TBool 		iVersioned;
    		};
    	
    public:     // Execution and destructors
        static void ExecuteL();
        virtual ~CBSServer();

    private:    // C++ constructor
        CBSServer();

    protected:     // Methods from CPolicyServer

        CSession2* NewSessionL( const TVersion& aVersion,
                                const RMessage2& aMessage ) const;


        CPolicyServer::TCustomResult CustomFailureActionL(
                                const RMessage2& aMsg,
                                TInt aAction,
                                const TSecurityInfo& aMissing);

    public: // From MBSBackupObserver
    
        void HandleBackupStateL( TBackupState aState );
        
    public:     //New methods
	    void SessionCreatedL( CBSSession* aSession );
	    void SessionDied( CBSSession* aSession );
    
		TBool MatchSessionL( const TDesC& aApplicationId,
							 const TDesC& aBrandId,
							 TLanguage aLanguageId,
							 CBSSession* aSession,
							 TInt aReserved = 0 );
							 
		TBool MatchSessionUninstallL( const TDesC& aApplicationId,
								 const TDesC& aBrandId, CBSSession* aSession);
								 							 
	    void RegisterFileForSessionL( CBSSession* aSession, 
	    							  const TDesC& aFile,
	    							  TBool aVersioned );

		TArray<CBSServer::CBSFileMapping*> RegisteredFiles();
		
		TBool FileStillInUse( CBSSession* aSession,
							  const TDesC& aFile );

		void BrandUpdatedL( const TDesC& aApplicationId,
							const TDesC& aBrandId,
							TLanguage aLanguageId,
							CBSSession* aSession,
							TInt aReserved );
        /**
         * Displays the info for each open session - except for the one that 
         * called this method.
         *
         * @param aSession the session that called this method
         * @param aErrorCode a system-wide errorcode which is simply displayed
         *        by this method
         */
        void DisplaySessionInfoL( CBSSession* aSession, TInt aErrorCode );

	private: // New methods
		void UnRegisterSessionL( CBSSession* aSession );

		
		void CleanupFileL( const TDesC& aFile );
	    
	    void InitializeL();
	    
    private:    //Data
        TInt iSessionCount;
        RPointerArray<CBSSession> iSessions;
        RBSObjOwningPtrArray<CBSFileMapping> iFileMapping;
        CBSStorageManager*		iStorageManager;
        CBSInstallHandler*      iInstallHandler;
        
        // Own. Observer for backup and restore events
        CBSBackupObserver*      iBackupObserver;
    };

#endif      //  __CBSSERVER_H__


//  END OF FILE


