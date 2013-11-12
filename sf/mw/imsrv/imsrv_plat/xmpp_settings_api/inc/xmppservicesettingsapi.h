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
* Description:  This class handles dynamic name-value pair storing.
*
*/


#ifndef C_XMPPSETTINGSAPI_H
#define C_XMPPSETTINGSAPI_H



#include <e32base.h>

class CRepository;
class CXmppSettingsRecord;
class CXmppSettingsColumn;


/**
 *  This class handles dynamic name-value pair storing.
 *  Be warned, all actions are final.
 *
 *
 *  This is how this component works:
 *  It has two tables. First one, the column table, contains only single row.
 *  This table is actually a header table for the record table. When you set
 *  an new attribute, we check that does it already exist in column table. If
 *  it does not, it is added as last entry. After that data of the attribute 
 *  is set to the record table into same column as it is attribute name is in
 *  the column table. Row of the settings record is determined by
 *  given settingsrecordid.
 *
 *  The column table can hold 0xFFFE (65534) of different user attributes
 *  and you can add 0xFF (255) different settingsrecords.
 *
 *  Below is an picture of the tables. Only the ID column is mandatory. All 
 *  other columns are defined by user.
 *
 *   ------------------------------------------------------------
 *   | COLUMNS TABLE                                             |
 *   |-----------------------------------------------------------
 *   | 1  |     2       |      3      |    4     |  5... |      |
 *   ------------------------------------------------------------
 *   | ID | SERVER-ADDR | SERVER-PORT | USERNAME | etc...|      |
 *   ------------------------------------------------------------
 *   
 *   ------------------------------------------------------------
 *   | RECORDS TABLE                                            |
 *   |-----------------------------------------------------------
 *   |  1 |          2          |  3   |    4    |  5... |      |
 *   ------------------------------------------------------------
 *   |  1 |   www.gizmo.com     | 5223 | Myerdin | etc...|      |
 *   ------------------------------------------------------------
 *   |  2 |                     | 5222 | Ballmer | etc...|      |
 *   ------------------------------------------------------------
 *   |  3 |   login.com         |      | Apple   | etc...|      |
 *   ------------------------------------------------------------
 *   |  4 |   aaa.bbb.com       | 1234 | user    | etc...|      |
 *   ------------------------------------------------------------
 * 
 *  Usage examples: 
 *  @code
 *  ---- Example 1 ----------------------------------------------
 *   Usage example:
 *   First Create settings record:
 *
 *   TUint32 settingId(0);
 *   CXmppSettingsApi* sapi = CXmppSettingsApi::NewLC();
 *   sapi->CreateSettingsRecordL( settingId );
 *   CleanupStack::PopAndDestroy( sapi );
 *
 *   The settingId contains now new id.
 *
 * ---- Example 2 ----------------------------------------------
 *   You can request default settings record (currently returns the first):
 *
 *   TUint32 settingId(0);
 *   sapi->DefaultSettingsRecordL( settingId );
 *
 *   The settingId contains now the id.
 *
 *  ---- Example 3 ----------------------------------------------
 *   When you want to add new parameter to the settings record, do following:
 *
 *   _LIT( KOurParam, "USERNAME");
 *
 *   sapi->SetParamL( settingId,
 *                    KOurParam(),
 *                    aUsername );
 *
 *   If this is the first time the parameter is added to the settings record,
 *   it creates new room for it.
 *
 *  ---- Example 4 ----------------------------------------------
 *   When you want to get parameter from the settings record, do following:
 *
 *   _LIT( KOurParam, "USERNAME");
 *   RBuf value;
 *   value.CreateL( KMaxParamLength );
 *
 *   sapi->SetParamL( settingId,
 *                    KOurParam(),
 *                    value );
 *
 *   Value now contains the value recordd in the settings record. If no value
 *   has been set for this settins records column, then it cannot be
 *   found ->leaves KErrNotFound.
 *
 *  @endcode
 *
 *  @lib jabbersettings.lib
 *  @since S60 5.0
 */
class CXmppSettingsApi: public CBase
    {

public:
		
		/**
     * Two-phased constructor.         
     * @return Instance of this class
     */
    IMPORT_C static CXmppSettingsApi* NewL();

    IMPORT_C static CXmppSettingsApi* NewLC();
		
		/**
	   * Destructor
	   */
    IMPORT_C virtual ~CXmppSettingsApi();


    /**
     * Creates new settings record.
     *
     * @since S60 5.0
     * @param aSettingsRecordId On return contains the new setting id.
     */
    IMPORT_C void CreateSettingsRecordL( TUint32& aSettingsRecordId );

    /**
     * Removes Settings record using given settings record id.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id to be removed.
     */
    IMPORT_C void RemoveSettingsRecordL( TUint32 aSettingsRecordId );

    /**
     * Returns default settings record id.
     *
     * @since S60 5.0
     * @param aSettingsRecordId On return contains the setting id.
     */
    IMPORT_C void DefaultSettingsRecordL( TUint32& aSettingsRecordId );

    /**
     * Fills the array with settings ids. If there is none, array
     * is zero length.
     *
     * @since S60 5.0
     * @param aArray Array is filled with setting ids.
     */
    IMPORT_C void GetSettingsRecordIdArrayL( RArray<TUint32>& aArray );

    /**
     * Sets value of the param. If ParamName is not found, it creates new.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue Parameters value.
     */
    IMPORT_C void SetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             const TDesC& aAttrValue );

    /**
     * Sets value of the param. If ParamName is not found, it creates new.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue Parameters value.
     */
    IMPORT_C void SetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             TInt aAttrValue );

    /**
     * Gets value of the param.
     * - KErrNotFound if setting collection or Param is not found.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue On return, contains the param value.
     */
    IMPORT_C void GetParamL( TUint32 aSettingsRecordId,
                             const TDesC& aAttrName,
                             TDes& aAttrValue );

    /**
     * Gets value of the param.
     * - KErrNotFound if setting collection or Param is not found.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id of interest.
     * @param aAttrName Parameters name.
     * @param aAttrValue On return, contains the param value.
     */
    IMPORT_C void GetParamL( TUint32 aSettingsRecordId,
                                const TDesC& aAttrName,
                                TInt& aAttrValue );

    /**
     * Removes this param from this Settings record.
     * - KErrNotFound If Settings record or param is not found.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id.
     * @param aAttrName Parameters name.
     */
    IMPORT_C void RemoveParamL( TUint32 aSettingsRecordId,
                                const TDesC& aAttrName );

	
	 /**
     * This method does checks before setting or getting values.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id to be changed.
     * @param aAttrName Parameters name.
     * @param aCreateColumnIfNotExist Should new column be created if it is not
     *                         founded (by aAttrName).
     * @param aTheKey On return contains key to be created/changed.
     */
    IMPORT_C void GetRepositoryKeyL( TUint32 aSettingsRecordId,
                       const TDesC& aAttrName,
                       TBool aCreateColumnIfNotExist,
                       TUint32& aTheKey );
                       
private:
		
		/**
		 * Default Contructor
		 */
    CXmppSettingsApi();

    void ConstructL();

   	 /**
     * This method does checks before setting or getting values.
     *
     * @since S60 5.0
     * @param aSettingsRecordId Setting id to be changed.
     * @param aAttrName Parameters name.
     * @param aCreateColumnIfNotExist Should new column be created if it is not
     *                         founded (by aAttrName).
     * @param aTheKey On return contains key to be created/changed.
     */
    void GetColumnKeyL( TUint32 aSettingsRecordId,
                       const TDesC& aAttrName,
                       TBool aCreateColumnIfNotExist,
                       TUint32& aTheKey );

private: // data

    /**
     * Central Repository.
     * Own.
     */
    CRepository* iRepository;

    /**
     * Class for settings records handling.
     * Own.
     */
    CXmppSettingsRecord* iRecords;

    /**
     * Class for column table handling.
     * Own.
     */
    CXmppSettingsColumn* iColumns;

    };

#endif // C_XMPPSETTINGSAPI_H
