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
* Description:  CBSClient.h
*
*/



#ifndef __CBSCLIENT_H__
#define __CBSCLIENT_H__
#include <e32std.h>
#include <f32file.h>
#include <bamdesca.h>
#include <s32mem.h>
#include "mbsupdater.h"
#include "bsserverdefs.h"
#include "mbsbrandobserverregistrar.h"

class MBSElement;
class MBSBrandChangeObserver;
class MBSBackupRestoreStateObserver;
class CBSBrandObserver;

/**
 * Simple ClientServer for
 * testing and example purposes.
 */
class RBSClient : public RSessionBase,
				  public MBSBrandObserverRegistrar
    {
    
    public:     // Constructor and destructor
        RBSClient();
        ~RBSClient();


    public:     // New methods

        TInt Connect();
        void Close();
        
		void StartTransactionL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  const TDesC8& aDefaultBrandId,
        				  TLanguage aLanguage,
        				  TTransactionType aType,
						  TInt aReserved = 0 );
        
        TInt StopTransactionL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  TLanguage aLanguage,
        				  TInt aReserved = 0 );
        
        void CancelTransactionL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  TLanguage aLanguage,
        				  TInt aReserved = 0);
        
        void InsertL( MBSElement* aElement );
        void ReplaceL( MBSElement* aElement );
        void AppendL( MBSElement* aElement );
        
        void GetTextL( const TDesC8& aId, HBufC*& aValue );
        void GetBufferL( const TDesC8& aId, HBufC8*& aValue );
        void GetIntL( const TDesC8& aId, TInt& aValue );

        MBSElement* GetSeveralL( MDesC8Array& aIds );
		MBSElement* GetStructureL( const TDesC8& aId );
		void GetFileL( const TDesC8& aId, RFile& aFile );
		
		void RemoveBrandL( const TDesC8& aApplicationId,
								   const TDesC8& aBrandId );

		void RemoveBrandsL( const TDesC8& aApplicationId );

		void isBrandUpdateRequiredL (TInt& aUpdateRequired);

    	/*
    	* Register to observer brand data updates
    	* @param aObserver the observer interface
    	*/
        void RegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver* aBackupStateObserver);

    	/*
    	* Unregister a brand data update observer
    	* @param aObserver the observer interface
    	*/
        void UnRegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver* aBackupStateObserver );


	private:

        void InitClientL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  const TDesC8& aDefaultBrandId,
        				  TLanguage aLanguage,
        				  TTransactionType aType,
        				  TInt aReserved );
        				  
		TIpcArgs CreateArgumentsL( const TDesC8& aApplicationId,
        				  const TDesC8& aBrandId,
        				  TLanguage aLanguage,
        				  TInt aReserved );

        MBSElement* InternalizeElementL( RReadStream& aStream );
        
        void ExternalizeIdArrayL( RWriteStream& aStream, MDesC8Array& aArray );
	
		void SendUpdateL( MBSElement* aElement, TBSMessages aMessage );
		
	private: // from MBSBrandObserverRegistrar
		/*
    	* Registeres the brand observer to server
    	*/
        void RegisterObserverToServerL( TRequestStatus& aStatus );
        
        TInt GetNewVersionL();
        TInt GetValueL(TInt msg) ;
        TInt GetBackupStateL() ;
        TInt GetBackupRestoreL() ;
        
        
	private:
		TTransactionType iType;
		CBSBrandObserver* iObserver;
		TInt isUpdateRequired;
    };

#endif      //  __CBSCLIENT_H__


//  END OF FILE

