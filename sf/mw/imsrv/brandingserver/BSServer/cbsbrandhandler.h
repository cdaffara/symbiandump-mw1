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
* Description:  CBSBrandHandler.h
*
*/



#ifndef __CBSBRANDHANDLER_H__
#define __CBSBRANDHANDLER_H__

#include "rbsobjowningptrarray.h"

#include <e32std.h>
#include <e32base.h>
#include <barsc.h>
#include <bamdesca.h>
#include <f32file.h>
#include <s32file.h>

// FORWARD DECLARATIONS
class MBSElement;
class CBSStorageManager;
class CBSSession;

/**
 *
 *
 */
NONSHARABLE_CLASS( CBSBrandHandler ): public CBase
	{
	public:
		/**
		* Two-phased constructor.
		*/
		static CBSBrandHandler* NewL( const TDesC& aApplicationId,
									  const TDesC& aBrandId, 
									  const TDesC& aDefaultBrandId, 
									  TLanguage aLanguage,
									  CBSSession* aSession,
									  TInt aReserved = 0);



    	virtual ~CBSBrandHandler();

	private: // constructors
	
		void ConstructL( const TDesC& aApplicationId,
						 const TDesC& aBrandId,
						 const TDesC& aDefaultBrandId,
						 CBSSession* aSession );
						 
		CBSBrandHandler( TLanguage aLanguage,
						 TInt aReserved );

	public:
		HBufC* GetTextL( const TDesC8& aId );
		HBufC8* GetBufferL( const TDesC8& aId );
		TInt GetIntL( const TDesC8& aId );
		MBSElement* GetSeveralL( RBSObjOwningPtrArray<HBufC8>& aIds );
		void GetFileL( const TDesC8& aId, RFile& aFile );
		MBSElement* GetStructureL( TDesC8& aId );
    	void SetDefaultBrandIdL( const TDesC8& aBrandId );


		TInt isBrandUpdateRequiredL ();

    private:     // New methods

		MBSElement* ReadElementLC( const TDesC8& aId, TBool aForceDefault = EFalse );
		
		MBSElement* ReadStreamL( const TDesC8& aId, 
								 RFileReadStream& aStream, 
								 TBool aAllowEmptyId = EFalse );

		void VerifyVersionL();
		void VerifyVersionL( RFileReadStream& aStream );

	private: // data
		HBufC* iDefaultBrand;
        ///<File server session, owned
        RFs                     iFs;

		HBufC* iApplicationId;
		HBufC* iBrandId;
		HBufC* iDefaultBrandId;
	    TLanguage iLanguage;
	    
	    TBool isDefaultBrandUsed;
		
	    RFile* iHandle;
	    
	    CBSStorageManager* iStorageManager;
	    
	    // doesn't own: session
	    CBSSession* iSession;
	    TInt iReserved ;

    };

#endif      //  __CBSBRANDHANDLER_H__


//  END OF FILE

