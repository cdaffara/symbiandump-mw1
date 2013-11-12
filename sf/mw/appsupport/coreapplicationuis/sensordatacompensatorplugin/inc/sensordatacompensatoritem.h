/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sensor data compensation item
*
*/


#ifndef SENSORDATACOMPENSATORITEM_H
#define SENSORDATACOMPENSATORITEM_H

// INCLUDES
#include <e32std.h>

// CONSTANTS

// MACROS

// DATA TYPES
/**
* Compensation item types
*/
enum TCompensationItemType
    {
    EUnknownCompensationItem = 0,
    EUICompensationItem,
    EDeviceCompensationItem
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS
class TCompensationTypeAxisData;
class TCompensationTypeAngleData;

// CLASS DECLARATION

/**
*  Sensor data compensation item
*
*  sensordatacompensatorplugin.lib
*  @since S60 5.1
*/
NONSHARABLE_CLASS( CSensorDataCompensatorItem ) : public CBase
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSensorDataCompensatorItem* NewLC( const TDesC& aType );

        /**
        * Destructor.
        */
        virtual ~CSensorDataCompensatorItem();

        /**
        * Reset all values to zero in iValues array
        *
        * @param None
        * @return None
        */
        void ResetValues();

        /**
        * Checks if item is empty for requested data type.
        *
        * @param aDataType: data type to check
        * @return ETrue if empty. Otherwise EFalse.
        */
        TBool IsEmpty( TSensrvChannelDataTypeId aDataType ) const;

        /**
        * Finds item from array
        *
        * @param aType:  Type to find
        * @param aArray: Array to find
        * @return: Not owned pointer to found item or NULL.
        */
        static CSensorDataCompensatorItem* Find( const TDesC& aType,
                                                 const RPointerArray<CSensorDataCompensatorItem>& aArray );
        /**
        * Inserts a new item into array
        *
        * @param aType:  Item type to insert.
        * @param aArray: Array to insert
        * @return: Not owned pointer to newly created inserted item or NULL.
        */
        static CSensorDataCompensatorItem* InsertL( const TDesC& aType,
                                                   RPointerArray<CSensorDataCompensatorItem>& aArray );

        /**
        * Parses item values
        *
        * @param aValueType: Type of value
        * @param aValue:     Value to be parsed
        * @return: ETrue if succesful, EFalse otherwise
        */
        TBool Parse( const TDesC& aValueType, const TDesC& aValue );

        /**
        * Get compensation values for type axis data
        *
        * @param aData: Target
        * @return:      None
        */
        void GetCompensationTypeAxisData( TCompensationTypeAxisData& aData ) const;

        /**
        * Get compensation values for type angle data
        *
        * @param aData: Target
        * @return:      None
        */
        void GetCompensationTypeAngleData( TCompensationTypeAngleData& aData ) const;

        /**
        * Get compensation values for type direction data
        *
        * @param aData: Target
        * @return:      None
        */
        void GetCompensationTypeDirectionData( TCompensationTypeDirectionData& aData ) const;

       /**
        * Gets compensation type enum value from text
        *
        * @param aType: Type as text
        * @return:      See TCompensationItemType
        */
        static TInt GetCompensationType( const TDesC& aType );
        
#ifdef _DEBUG
        void DebugPrint();
#endif // _DEBUG

	private:

        /**
        * C++ default constructor.
        */
        CSensorDataCompensatorItem();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL( const TDesC& aType );

        static TInt DesToValueType( const TDesC& aValueType );

        /**
        * Compensation item value types
        */
        enum TCompensationValueType
            {
            EAxisDataX = 0,
            EAxisDataY,
            EAxisDataZ,
            ETappingAxisX,
            ETappingAxisY,
            ETappingAxisZ,
            EMagneticNorthAngle,
            ENumValues
            };

        /**
        * Class to handle multiple type instances in correct order.
        * For example "Device1.Flip", "Device2.Grip" etc.
        */
        NONSHARABLE_CLASS(TItemType)
            {
            public:
                TItemType();
                TInt Set( const TDesC& aType );
                static TInt Compare( const TItemType& aType1, const TItemType& aType2 );
            public:
                TInt iType; // See TItemType
                TInt iNumber;
            };

        /**
        * Class to handle item search by type string.
        */
        NONSHARABLE_CLASS(TFindItem)
            {
            public:
                TFindItem( const TDesC& aType );
            public:
                TItemType iItemType;
                TPtrC iType;
            };

        static TInt Compare( const CSensorDataCompensatorItem& aItem1,
                             const CSensorDataCompensatorItem& aItem2 );

        static TInt Compare( const TFindItem* aFindItem,
                             const CSensorDataCompensatorItem& aItem );

    private:
        /**
        * Full compensation item type name as text
        */
        HBufC* iType;

        /**
        * Compensation item type information
        */
        TItemType iItemType;

        /**
        * Array for holding compensation values
        */
        TFixedArray<TInt,ENumValues> iValues;

    };

#endif // SENSORDATACOMPENSATORITEM_H

// End of File
