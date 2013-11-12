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
* Description:  Compensation item handling
*
*/


// INCLUDE FILES
#include <sensordatacompensatorplugintypes.h>
#include <sensrvaccelerometersensor.h>
#include <sensrvmagnetometersensor.h>
#include <sensrvmagneticnorthsensor.h>
#include <sensrvtappingsensor.h>
#include "sensordatacompensatoritem.h"
#include "trace.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// MACROS

// LOCAL CONSTANTS AND MACROS
_LIT( KAxisDataX, "AxisDataX" );
_LIT( KAxisDataY, "AxisDataY" );
_LIT( KAxisDataZ, "AxisDataZ" );
_LIT( KTappingAxisX, "TappingAxisX" );
_LIT( KTappingAxisY, "TappingAxisY" );
_LIT( KTappingAxisZ, "TappingAxisZ" );
_LIT( KMagneticNorthAngle, "MagneticNorthAngle" );
_LIT( KDeg0,   "CW0");
_LIT( KDeg90,  "CW90");
_LIT( KDeg180, "CW180");
_LIT( KDeg270, "CW270");
_LIT( KUiText, "UI" );
_LIT( KDeviceText, "Device" );
const TText KDot = '.';
const TInt KUndefinedCompensationItemNum = -1;
_LIT( KUiOrientationTypeText, "UI*.*" );
_LIT( KDeviceOrientationTypeText, "Device*.*" );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ParseInt
// -----------------------------------------------------------------------------
//
static TInt ParseInt( TInt& aIntValue, const TDesC& aValue )
    {
    FUNC_LOG;
    TLex lex( aValue );
    TInt ret( lex.Val( aIntValue ) );
    if ( ret != KErrNone )
        {
        ERROR_1( ret, "ParseInt() - Unknown int value: [%S]", &aValue );
        aIntValue = 0;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CompareInt
// -----------------------------------------------------------------------------
//
static TInt CompareInt( TInt aValue1, TInt aValue2 )
    {
    if ( aValue1 < aValue2 )
        {
        return -1;
        }
    if ( aValue1 > aValue2 )
        {
        return 1;
        }
    return 0;
    }

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CSensorDataCompensatorTspController::CSensorDataCompensatorTspController
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//

CSensorDataCompensatorItem::CSensorDataCompensatorItem()
	{
	}

// -----------------------------------------------------------------------------
// CSensorDataCompensatorTspController::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorItem* CSensorDataCompensatorItem::NewLC( const TDesC& aType )
    {
    FUNC_LOG;

    CSensorDataCompensatorItem* self = new( ELeave ) CSensorDataCompensatorItem();
    CleanupStack::PushL( self );
    self->ConstructL( aType );
    return self;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::ConstructL
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorItem::ConstructL( const TDesC& aType )
    {
    FUNC_LOG;

    iType = aType.AllocL();
    User::LeaveIfError( iItemType.Set( aType ) );
    ResetValues();
	}

// Destructor
CSensorDataCompensatorItem::~CSensorDataCompensatorItem()
    {
    FUNC_LOG;

    delete iType;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::ResetValues
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorItem::ResetValues()
    {
    FUNC_LOG;

    iValues.Reset();
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::IsEmpty
// -----------------------------------------------------------------------------
//
TBool CSensorDataCompensatorItem::IsEmpty( TSensrvChannelDataTypeId aDataType ) const
    {
    FUNC_LOG;

    TBool ret( EFalse );

        switch( aDataType )
            {
            case TSensrvAccelerometerAxisData::KDataTypeId:
            case TSensrvMagnetometerAxisData::KDataTypeId: // Fall through
                {
				if (   !iValues[ EAxisDataX ]
					&& !iValues[ EAxisDataY ]
					&& !iValues[ EAxisDataZ ] )
                    {
                    ret = ETrue;
                    }
                break;
                }
            case TSensrvMagneticNorthData::KDataTypeId:
                {

				if ( !iValues[ EMagneticNorthAngle ] )
                    {
                    ret = ETrue;
                    }
                break;
                }

            case TSensrvTappingData::KDataTypeId:
                {
				if (   !iValues[ ETappingAxisX ]
					&& !iValues[ ETappingAxisY ]
					&& !iValues[ ETappingAxisZ ] )
				    {
				    ret = ETrue;
				    }
				break;
            }
            default:
                {
                break;
                }
            }
        return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::Find
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorItem* CSensorDataCompensatorItem::Find(
        const TDesC& aType,
        const RPointerArray<CSensorDataCompensatorItem>& aArray )
    {
    FUNC_LOG;
    CSensorDataCompensatorItem* ret = NULL;
    TFindItem findItem( aType );
    TInt i( aArray.FindInOrder( findItem, Compare ) );
    if (i >= 0 && i < aArray.Count() )
        {
        INFO_2( "CSensorDataCompensatorItem::Find() - Found %S at %d", &aType, i );
        ret = aArray[ i ];
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::Find
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorItem* CSensorDataCompensatorItem::InsertL(
        const TDesC& aType,
        RPointerArray<CSensorDataCompensatorItem>& aArray )
    {
    FUNC_LOG;
    CSensorDataCompensatorItem* ret = NewLC( aType );
    aArray.InsertInOrderL( ret, Compare );
    CleanupStack::Pop( ret );
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::DesToValueType
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorItem::DesToValueType( const TDesC& aValueType )
    {
    FUNC_LOG;

    TInt ret( KErrNotFound );
    if ( !aValueType.Compare( KAxisDataX ) )
        {
        ret = EAxisDataX;
        }
    else if ( !aValueType.Compare( KAxisDataY ) )
        {
        ret = EAxisDataY;
        }
    else if ( !aValueType.Compare( KAxisDataZ ) )
        {
        ret = EAxisDataZ;
        }
    else if ( !aValueType.Compare( KTappingAxisX ) )
        {
        ret = ETappingAxisX;
        }
    else if ( !aValueType.Compare( KTappingAxisY ) )
        {
        ret = ETappingAxisY;
        }
    else if ( !aValueType.Compare( KTappingAxisZ ) )
        {
        ret = ETappingAxisZ;
        }
    else if ( !aValueType.Compare( KMagneticNorthAngle ) )
        {
        ret = EMagneticNorthAngle;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::GetCompensationTypeAxisData
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorItem::GetCompensationTypeAxisData( TCompensationTypeAxisData& aData ) const
    {
    FUNC_LOG;

    aData.iAxisX = iValues[ EAxisDataX ];
    aData.iAxisY = iValues[ EAxisDataY ];
    aData.iAxisZ = iValues[ EAxisDataZ ];
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::GetCompensationTypeAngleData
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorItem::GetCompensationTypeAngleData( TCompensationTypeAngleData& aData ) const
    {
    FUNC_LOG;

    aData.iAngle = iValues[ EMagneticNorthAngle ];
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::GetCompensationTypeDirectionData
// -----------------------------------------------------------------------------
//
void CSensorDataCompensatorItem::GetCompensationTypeDirectionData( TCompensationTypeDirectionData& aData ) const
    {
    FUNC_LOG;

    aData.iTappingAxisX = iValues[ ETappingAxisX ];
    aData.iTappingAxisY = iValues[ ETappingAxisY ];
    aData.iTappingAxisZ = iValues[ ETappingAxisZ ];
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::GetCompensationType
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorItem::GetCompensationType( const TDesC& aType )
    {
    FUNC_LOG;
    TInt ret( EUnknownCompensationItem );
    if ( aType.MatchF(KUiOrientationTypeText) != KErrNotFound )
        {
        ret = EUICompensationItem;
        }
    else if ( aType.MatchF(KDeviceOrientationTypeText) != KErrNotFound )
        {
        ret = EDeviceCompensationItem;
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::Parse
// -----------------------------------------------------------------------------
//
TBool CSensorDataCompensatorItem::Parse( const TDesC& aValueType, const TDesC& aValue )
    {
    FUNC_LOG;

    TBool ret( EFalse );
    TInt valueType( DesToValueType( aValueType ) );
    if ( valueType >= 0 )
        {
        TInt value = 0;
        if (    valueType == ETappingAxisX
             || valueType == ETappingAxisY
             || valueType == ETappingAxisZ )
            {
            // Parse enum value
            if( !aValue.CompareF( KDeg0 ) )
                {
                value = TCompensationTypeDirectionData::ECw0;
                }
            else if( !aValue.CompareF( KDeg90 ) )
                {
                value = TCompensationTypeDirectionData::ECw90;
                }
            else if( !aValue.CompareF( KDeg180 ) )
                {
                value = TCompensationTypeDirectionData::ECw180;
                }
            else if( !aValue.CompareF( KDeg270 ) )
                {
                value = TCompensationTypeDirectionData::ECw270;
                }
            else
                {
                INFO_1( "CSensorDataCompensatorItem::ParseItem() - Unknown text value: [%S]", &aValue );

                value = TCompensationTypeDirectionData::ECw0;
                }
            }
        else
            {
            ParseInt( value, aValue );
            }

        // Update value if it has changed

        TInt previousValue = iValues[ valueType ];

        if ( previousValue != value )
            {
            INFO_3( "CSensorDataCompensatorItem::ParseItem() - ValueType %S changed [%D, %D]", &aValueType, previousValue, value );

            iValues[ valueType ] = value;
            ret = ETrue;
            }
        else
            {
            INFO_2( "CSensorDataCompensatorItem::ParseItem() - ValueType %S not changed [%D]", &aValueType, previousValue );
            }
        }
    else
        {
        INFO_1( "CSensorDataCompensatorItem::ParseItem() - Unknown value type: %S", &aValueType );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::TItemType::TItemType
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorItem::TItemType::TItemType() :
        iType(EUnknownCompensationItem),
        iNumber(KUndefinedCompensationItemNum)
    {
    FUNC_LOG;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::TItemType::Set
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorItem::TItemType::Set( const TDesC& aType )
    {
    FUNC_LOG;

    iType = EUnknownCompensationItem; // Reset
    iNumber = KUndefinedCompensationItemNum;

    TInt ret( KErrNotFound );
    TInt len( aType.Length() );
    TInt splitEndPos( aType.Locate( KDot ) );
    if ( splitEndPos >= 0 && splitEndPos < len )
        {
        TInt splitStartPos( KErrNotFound );
        iType = GetCompensationType( aType );
        if ( iType == EUICompensationItem )
            {
            splitStartPos = KUiText().Length();
            ret = KErrNone;
            }
        else if ( iType == EDeviceCompensationItem )
            {
            splitStartPos = KDeviceText().Length();
            ret = KErrNone;
            }
        if ( ret == KErrNone && splitStartPos >= 0 && splitStartPos < splitEndPos )
            {
            TPtrC num( aType.Mid( splitStartPos, splitEndPos - splitStartPos ) );
            ret = ParseInt( iNumber, num );
            }
        }
    if ( ret != KErrNone )
        {
        iType = EUnknownCompensationItem;
        iNumber = KUndefinedCompensationItemNum;
        }
    ERROR( ret, "CSensorDataCompensatorItem::TItemType::Set() - Fail");
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::TItemType::Compare
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorItem::TItemType::Compare(
        const CSensorDataCompensatorItem::TItemType& aType1,
        const CSensorDataCompensatorItem::TItemType& aType2 )
    {
    TInt ret( CompareInt( aType1.iType, aType2.iType ) );
    if ( !ret )
        {
        ret = CompareInt( aType1.iNumber, aType2.iNumber ); 
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::TFindItem::TFindItem
// -----------------------------------------------------------------------------
//
CSensorDataCompensatorItem::TFindItem::TFindItem( const TDesC& aType )
    {
    FUNC_LOG;
    iItemType.Set( aType );
    iType.Set( aType );
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::Compare
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorItem::Compare( const CSensorDataCompensatorItem& aItem1,
                                          const CSensorDataCompensatorItem& aItem2 )
    {
    TInt ret( TItemType::Compare( aItem1.iItemType, aItem2.iItemType ) );
    if ( !ret )
        {
        ret = aItem1.iType->Compare( *( aItem2.iType ) );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::Compare
// -----------------------------------------------------------------------------
//
TInt CSensorDataCompensatorItem::Compare( const CSensorDataCompensatorItem::TFindItem* aFindItem,
                                          const CSensorDataCompensatorItem& aItem )
    {
    TInt ret( TItemType::Compare( aFindItem->iItemType, aItem.iItemType ) );
    if ( !ret )
        {
        ret = aFindItem->iType.Compare( *( aItem.iType ) );
        }
    return ret;
    }

// -----------------------------------------------------------------------------
// CSensorDataCompensatorItem::DebugPrint
// -----------------------------------------------------------------------------
//
#ifdef _DEBUG
void CSensorDataCompensatorItem::DebugPrint()
    {
    INFO(   "------------------------------" );
    INFO_3( "Item: %S %d %d", iType, iItemType.iType, iItemType.iNumber );
    INFO_3( "AxisData: %d %d %d", iValues[ EAxisDataX ], iValues[ EAxisDataY ], iValues[ EAxisDataZ ] );
    INFO_3( "TappingAxis: %d %d %d", iValues[ ETappingAxisX ], iValues[ ETappingAxisY ], iValues[ ETappingAxisZ ] );
    INFO_1( "MagneticNorthAngle: %d", iValues[ EMagneticNorthAngle ] );
    INFO(   "------------------------------" );
    }
#endif // _DEBUG

// End of file
