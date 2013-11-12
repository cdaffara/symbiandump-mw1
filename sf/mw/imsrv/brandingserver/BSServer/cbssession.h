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
* Description:  cbssession.h
*
*/



#ifndef __CBSSESSION_H__
#define __CBSSESSION_H__

#include <e32base.h>
#include <s32mem.h>
#include "rbsobjowningptrarray.h"
#include "bsserverdefs.h"
#include "mbsbackupobserver.h"

class CBSServer;
class CBSBrandHandler;
class MBSElement;
class CBSStorageManager;

/**
 * Service side session.
 *
 * @lib
 * @since
 */
 class CBSSession : public CSession2,
                    public MBSBackupObserver
    {
    
    
    public:
    	
    	/**
    	 * Class for encapsulating the session information.
    	 */
    	NONSHARABLE_CLASS( CBSSessionInfo ): public CBase
    		{
    		public:
	    		
	    		/**
	    		 * Creates a new CBSSessionInfo
	    		 *
	    		 * @param aFileName the filename of the process, that created 
	    		 *        this session
	    		 * @param aCaption a caption of the process, that created this 
	    		 *        session
	    		 * @param aThreadId thread id where this session was created
	    		 * @param aProcessId processId of the process, that created 
	    		 *        this session
	    		 * @return a new CBSSessionInfo instance
	    		 */
	    		static CBSSessionInfo* NewL( const TDesC& aFileName, 
	    		                             const TDesC& aCaption,
	    		                             TThreadId aThreadId,
	    		                             TProcessId aProcessId );
        	    /**
                 * Destructor.
                 */  		
	    		~CBSSessionInfo();
	    		
    		private: 

                /**
                 * Symbian second-phase constructor
                 *
	    		 * @param aFileName the filename of the process, that created 
	    		 *        this session
	    		 * @param aCaption a caption of the process, that created this 
	    		 *        session
                 */                    
	    		void ConstructL( const TDesC& aFileName, 
	    		                 const TDesC& aCaption );

                /**
                 * Constructor.
	    		 *
	    		 * @param aThreadId thread id where this session was created
	    		 * @param aProcessId processId of the process, that created 
	    		 *        this session
                 */
	    		CBSSessionInfo( TThreadId aThreadId, TProcessId aProcessId );
			
			public: 

			    /**
                 * Returns the filename of the process, that created this 
                 * session.
                 *
                 * @return a filename
                 */ 
				const TDesC& FileName();

			    /**
                 * Returns the caption of the process, that created this 
	    		 * session.
                 *
                 * @return a caption
                 */ 
				const TDesC& Caption();

			    /**
                 * Returns the thread id where this session was created.
                 *
                 * @return a thread id
                 */ 
				TThreadId ThreadId();
				
			    /**
                 * Returns the processId of the process, that created 
	    		 * this session.
                 *
                 * @return a process id
                 */ 
				TProcessId ProcessId();
				
			private: // data
				
                /**
                 * Filename
                 * Own.
                 */
				HBufC* 		iFileName;

                /**
                 * Caption
                 * Own.
                 */
			  	HBufC*      iCaption;
			  	
                /**
                 * Thread Id.
                 */
			  	TThreadId  	iThreadId;

                /**
                 * Process Id.
                 */
			  	TProcessId  iProcessId;			  	
    		};      
    
    
    public:     // Constructors and destructors
        static CBSSession* NewL();
        virtual ~CBSSession();


    protected:  // C++ constructors
    	/**
        * C++ default constructor.
        */
        CBSSession();

        /**
        * Symbian OS constructor
        */
		void ConstructL();

    public:     // Methods derived from CSession2
    	/**
    	* From CSession2
    	*/
        void CreateL();
    	/**
    	* From CSession2
    	*/
        void ServiceL( const RMessage2 &aMessage );
    	/**
    	* From CSession2
    	*/
        void ServiceError( const RMessage2& aMessage,
                           TInt aError );
		
    public:     // From MBSBackupObserver
    
    	/*
    	* @see MBSBackupObserver
    	*/
		void HandleBackupStateL( TBackupState aState );

	public: // New methods
		
        /**
        *
        */
        CBSServer* Server() { return (CBSServer*) CSession2::Server(); }


		TBool MatchSessionL( const TDesC& aApplicationId,
							 const TDesC& aBrandId,
							 TLanguage aLanguageId,
							 TInt aReserved );
							 
		TBool MatchSessionUninstallL( const TDesC& aApplicationId,
								 const TDesC& aBrandId);
								 
		void BrandUpdatedL( const TDesC& aApplicationId,
							 const TDesC& aBrandId,
							 TLanguage aLanguageId,
							 TInt aReserved );



	    /**
         * Returns the filename of the process, that created this 
         * session.
         *
         * @return a filename
         */ 
		const TDesC& FileName();

	    /**
         * Returns the caption of the process, that created this 
		 * session.
         *
         * @return a caption
         */ 
		const TDesC& Caption();

	    /**
         * Returns the thread id where this session was created.
         *
         * @return a thread id
         */ 
		TInt ThreadId( TThreadId& aThreadId );
		
	    /**
         * Returns the processId of the process, that created 
		 * this session.
         *
         * @return a process id
         */
        TInt ProcessId( TProcessId& aProcessId );

        /**
         * Returns, if the session info for this object is available.
         *
         * @return ETrue, if information about this object is available;
         *         EFalse otherwise
         */
        TBool InfoAvailable();

        
    private:    // New methods

        /**
		*  Dispatch clients message
		*
		*  @since
		*  @param aMessage client's message
        */
        TBool DispatchMessageL( const RMessage2 &aMessage );


		/**
		* Initializes the server
		* @param aMessage client's message
		*/
        void InitUpdateL( const RMessage2 &aMessage, TTransactionType aType );

		/**
		* Initializes the server
		* @param aMessage client's message
		*/
        void InitAccessL( const RMessage2 &aMessage );


		/**
		* Get text type branding item
		* @param aMessage client's message
		*/
        void GetTextL( const RMessage2 &aMessage );

		/**
		* Get text type branding item
		* @param aMessage client's message
		*/
        void GetBufferL( const RMessage2 &aMessage );

		/**
		* Get integer type branding item
		* @param aMessage client's message
		*/
        void GetIntL( const RMessage2 &aMessage );

		/**
		* Get file type branding item
		* @param aMessage client's message
		*/
        void GetFileL( const RMessage2 &aMessage );

		/**
		* Prepares several textual type branding items.
		* These have to be fetched with GetSeveralText.
		* @param aMessage client's message
		*/
        void PrepareSeveralL( const RMessage2 &aMessage );

		/**
		* Gets the several textual branding items
		* previously prepared.
		* @param aMessage client's message
		*/
        void GetSeveralL( const RMessage2 &aMessage );


		/**
		* Prepares several structure type branding items.
		* These have to be fetched with GetStructure.
		* @param aMessage client's message
		*/
        void PrepareStructureL( const RMessage2 &aMessage );

		/**
		* Gets the several structure type branding items
		* previously prepared.
		* @param aMessage client's message
		*/
        void GetStructureL( const RMessage2 &aMessage );

		void isBrandUpdateRequiredL (const RMessage2 &aMessage);



		/**
		* 
		* 
		* @param aMessage client's message
		*/
        void InstallL( const RMessage2 &aMessage );

		/**
		* 
		* 
		* @param aMessage client's message
		*/
        void ReplaceL( const RMessage2 &aMessage );


		/**
		* 
		* 
		* @param aMessage client's message
		*/
        void AppendL( const RMessage2 &aMessage );
		
		/**
		* 
		* 
		* @param aMessage client's message
		*/
		MBSElement* InternalizeElementL( RReadStream& aStream, TBool aAppending = EFalse );


		/**
		* Cancels the transaction
		*/
        void StartTransactionL( const RMessage2 &aMessage );


		/**
		* Cancels the transaction
		*/
        void CancelTransactionL( const RMessage2 &aMessage );

		/**
		* Finishes the transaction
		*/
        void StopTransactionL( const RMessage2 &aMessage );


		/**
		* Internalize element ids
		*/
        void InternalizeElementIdsL( RReadStream& aStream, RBSObjOwningPtrArray<HBufC8>& aArray );

		/**
		* 
		* 
		* @param aMessage client's message
		*/
        void RemoveBrandL( const RMessage2 &aMessage );


		/**
		* 
		* 
		* @param aMessage client's message
		*/
        void RemoveApplicationL( const RMessage2 &aMessage );

		void PrepareAppendL();
		
		void PrepareReplaceL();
		
		TInt GetNewVersionL();


        /**
         * Extracts some information from the specified RMessage2
         * and saves it.
         *
         * @param aMessage a message from a client
         */
        void ExtractInfoL( const RMessage2& aMessage );

    private:    // Data
    	TBool iInitialized;
    	TBool iAppending;
    	TBool iAccessInit;
    	TBool iWriteNeeded;
    	
    	// is backup process currently active
    	TBool iBackupActive;
    	
    	HBufC* iBrandId;
    	HBufC* iDefaultBrandId;
    	HBufC* iApplicationId;
    	TLanguage iLanguageId;
    
        TInt iValue;
        HBufC* iText;
        CBSBrandHandler* iBranding;
        CBufFlat* iSeveralData;
        HBufC8*		iBuffer;
        TBool iMessageCompleted;
        
        CBSStorageManager* iStorageManager;
        
        TBool iObserverActive;
        RMessage2 iObserverMessage;
        
        // To send backup event to client
        TBool iIsBackup ;
		TInt iReserved;
        
        /**
         * Contains information about this object.
         * Own.
         */
        CBSSessionInfo* iSessionInfo;
		
    };

#endif      //  __CBSSESSION_H__


//  END OF FILE

