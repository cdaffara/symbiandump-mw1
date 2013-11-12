/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is used for data table manipulations.
*
*/




#ifndef C_XMPPSETTINGSRECORD_H
#define C_XMPPSETTINGSRECORD_H

#include <e32base.h>

class CRepository;

/**
 *  This class is used for data table manipulations.
 *
 *  This class contains all methods needed to read and write to settings id
 *  table.
 *
 *
 *  @lib jabbersettings.lib
 *  @since S60 5.0
 */
class CXmppSettingsRecord: public CBase
    {

public:
		
		/**
     * Two-phased constructor.         
     * @return Instance of this class
     */
    static CXmppSettingsRecord* NewL( CRepository& aRepository );

    static CXmppSettingsRecord* NewLC( CRepository& aRepository );
		
		/**
	   * Destructor
	   */
    virtual ~CXmppSettingsRecord();

    /**
     * This method checks does this setting id exist. Leaves with
     * KErrNotFound if it does not.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id.
     * @param aSettingsRecordKey If setting id is found, this contains the setting
     *                    key on return.
     */
    void FindSettingsRecordKeyL( TUint32 aSettingsRecordId,
                                 TUint32& aSettingsRecordKey );

    /**
     * This method fids all setting keys.
     *
     * @since S60 5.0
     * @@param aSettingsRecordKeys On return contains all setting keys.
     */
    void GetSettingsRecordkeysL( RArray<TUint32>& aResults );

    /**
     * This method creates unique setting id.
     *
     * @since S60 5.0
     * @param aNewSettingId On return contains new setting id.
     */
    void CreateSettingsRecordIdL( TUint32& aNewSettingId );

private:

		/**
		 * Default Contructor
		 */
    CXmppSettingsRecord( CRepository& aRepository );

    void ConstructL();

    /**
     * This method returns next available slot for settings record. It can
     * be new index or it can use deleted slot.
     *
     * @since S60 5.0
     * @param aFreeKey On return contains the next free key.
     */
    void NextFreeSlotL( TUint32& aFreeKey );

    /**
     * This method fids all setting keys. It fills the internal array with
     * unique ids.
     *
     * @since S60 5.0
     */
    void GetSettingsRecordkeysL();

private: // Data

    /**
     * Handle to the Central Repository.
     */
    CRepository& iRepository;

    /**
     * Contains setting id keys.
     */
    RArray<TUint32> iSettingsRecordKeyArray;

    };


#endif // C_XMPPSETTINGSRECORD_H
