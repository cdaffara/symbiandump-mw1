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
* Description:  MBSAccess
*
*/




#ifndef __MBSACCESS_H
#define __MBSACCESS_H

#include <e32std.h>
#include <mbselement.h>
#include <bamdesca.h>
#include <f32file.h>


// forward declarations
class MBSBrandChangeObserver;
class MBSBackupRestoreStateObserver;
class CFbsBitmap;

/**
 * An interface for getting branding information
 *
 * @since
 */
class MBSAccess
    {
    public:

    	/*
    	* Get text type item
    	* @param aId ID of the item
    	* @return the text item
    	*/
        virtual HBufC* GetTextL( const TDesC8& aId ) = 0;

    	/*
    	* Get text type item
    	* @param aId ID of the item
    	* @return the text item
    	*/
        virtual HBufC* GetTextLC( const TDesC8& aId ) = 0;

    	/*
    	* Get 8-bit data type item
    	* @param aId ID of the item
    	* @return the data buffer item
    	*/
        virtual HBufC8* GetBufferL( const TDesC8& aId ) = 0;

    	/*
    	* Get 8-bit data type item
    	* @param aId ID of the item
    	* @return the data buffer item
    	*/
        virtual HBufC8* GetBufferLC( const TDesC8& aId ) = 0;

    	/*
    	* Get integer type item
    	* @param aId ID of the item
    	* @return the integer item
    	*/
        virtual TInt GetIntL( const TDesC8& aId ) = 0;


    	/*
    	* Get an array of items
    	* @param aIds ID's of the items
    	* @return the items
    	*/
        virtual MBSElement* GetSeveralL( MDesC8Array& aIds ) = 0;


    	/*
    	* Get a file item
    	* @param aId ID of the item
    	* @param aFile on return will contain an open file handle
    	*        to the file
    	*/
        virtual void GetFileL( const TDesC8& aId,
        					   RFile& aFile ) = 0;

    	/*
    	* Get a structure type item
    	* @param aIds ID of the item
    	* @return structure branding element
    	*/
        virtual MBSElement* GetStructureL( const TDesC8& aId ) = 0;

    	/*
    	* Register to observer brand data updates
    	* @param aObserver the observer interface
    	*/
        virtual void RegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver* aBackupObserver ) = 0;

    	/*
    	* Unregister a brand data update observer
    	* @param aObserver the observer interface
    	*/
        virtual void UnRegisterObserverL( MBSBrandChangeObserver* aObserver, MBSBackupRestoreStateObserver *aBackupObserver ) = 0;

    	/*
    	* Get a bitmap type item.
    	* Note that this method supports skins, but as skin support
    	* requires CONE, it will only try to load skin bitmaps if
    	* CONE is available. Else it will only load normal bitmaps.
    	* @param aBitmapId the ID of the bitmap item itself
    	* @param aBitmap on return will contain the bitmap
    	* @param aMask on return will contain the mask
    	*/
		virtual void GetBitmapL( const TDesC8& aBitmapId,
								 CFbsBitmap*& aBitmap,
								 CFbsBitmap*& aMask ) = 0;


	virtual TInt isBrandUpdateRequiredL () = 0;
        /*
        * Closes the handle to server and deletes the object.
        */
		virtual void Close() = 0;

	protected:

    	/*
    	* Destructor
    	*/
    	virtual ~MBSAccess() {};
    };

#endif //__MBSACCESS_H
