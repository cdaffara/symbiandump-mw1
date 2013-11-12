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
* Description:  This class handles columns.
*
*/




#ifndef C_XMPPSETTINGSCOLUMN_H
#define C_XMPPSETTINGSCOLUMN_H

#include <e32base.h>

class CRepository;

/**
 *  This class handles columns.
 *
 *  This class contains all methods needed to read and write columns table.
 *
 *
 *  @lib jabbersettings.lib
 *  @since S60 5.0
 */
class CXmppSettingsColumn: public CBase
    {

public:
	
		/**
     * Two-phased constructor.         
     * @return Instance of this class
     */ 
    static CXmppSettingsColumn* NewL( CRepository& aRepository );

    static CXmppSettingsColumn* NewLC( CRepository& aRepository );

	  /**
	   * Destructor
	   */
    virtual ~CXmppSettingsColumn();

    /**
     * This method checks that does this column exist. If it does not, it
     * creates it (if requested. Otherwise it leaves.
     *
     * @since S60 5.0
     * @param aAttrName Column name.
     * @param aCreateColumnIfNotExist Should new column be created if it is
     *                         not found.
     * @param aColKey onn return contains the key of the column.
     */
    void GetColumnKeyL( const TDesC& aAttrName,
                        TBool aCreateColumnIfNotExist,
                        TUint32& aColKey );

    /**
     * Tries to find columns key by it is name (value).
     *
     * @since S60 5.0
     * @param aColumnName Name of the column.
     * @param aColumnKey on return contains the columns key.
     */
    void FindByNameL( const TDesC& aColumnName,
                      TUint32& aColumnKey );

private:
		
		/**
		 * Default Contructor
		 */
    CXmppSettingsColumn( CRepository& aRepository );
		
    void ConstructL();

    /**
     * This method resolves next free key to be used as column key.
     *
     * @since S60 5.0
     * @param aColKey On return contains next free column key.
     */
    void NextFreeSlotL( TUint32& aColKey );

private: // data

    /**
     * Handle to the Central Repository. Doesnt Own.
     */
    CRepository& iRepository;

    };


#endif // C_XMPPSETTINGSCOLUMN_H
