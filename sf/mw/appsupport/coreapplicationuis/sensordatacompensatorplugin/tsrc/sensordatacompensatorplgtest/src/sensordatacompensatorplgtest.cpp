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
* Description:  Module test for CSensorDataCompensator API.
*
*/


// INCLUDE FILES
#include <e32debug.h>
#include <e32property.h>
#include <StifParser.h>
#include <stiftestinterface.h>
#include <sensordatacompensatorplugintypes.h>
#include "sensordatacompensatorplgtest.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS
_LIT_SECURITY_POLICY_C1( KReadDeviceDataPolicy, ECapabilityReadDeviceData );
_LIT_SECURITY_POLICY_C1( KWriteDeviceDataPolicy, ECapabilityWriteDeviceData );

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

static void DefineTestPubSubL( TInt aKey, TInt aType )
    {
    RDebug::Printf("[SensDataCompTest] DefineTestPubSubL key %d", aKey );
    TInt err( RProperty::Define(
        KTestPSUid, aKey, aType, KReadDeviceDataPolicy, KWriteDeviceDataPolicy ) );
    if ( err != KErrNone && err != KErrAlreadyExists )
        {
        User::Leave( err );
        }
    RDebug::Printf("[SensDataCompTest] DefineTestPubSubL return");
    }

static void SetTestPubSubL( TInt aKey, TDesC8& aValue )
    {
    RDebug::Printf("[SensDataCompTest] SetTestPubSubL key %d value %S", aKey, &aValue );
    User::LeaveIfError( RProperty::Set( KTestPSUid, aKey, aValue ) );
    RDebug::Printf("[SensDataCompTest] SetTestPubSubL return");
    }

static void SetTestPubSubL( TInt aKey, TInt aValue )
    {
    RDebug::Printf("[SensDataCompTest] SetTestPubSubL key %d value %d", aKey, aValue );
    User::LeaveIfError( RProperty::Set( KTestPSUid, aKey, aValue ) );
    RDebug::Printf("[SensDataCompTest] SetTestPubSubL return");
    }

// FORWARD DECLARATIONS

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::CSensorDataCompensatorPlgTest
// Returns: KErrNone
// ---------------------------------------------------------
CSensorDataCompensatorPlgTest::CSensorDataCompensatorPlgTest( CTestModuleIf& aTestModuleIf ) :
    CScriptBase( aTestModuleIf )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CSensorDataCompensatorPlgTest()");
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CSensorDataCompensatorPlgTest return");
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::ConstructL
// ---------------------------------------------------------
void CSensorDataCompensatorPlgTest::ConstructL()
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::ConstructL()");
    TInt i( 0 );
    for( ; i < ETestIntKeyCount; ++i )
        {
        DefineTestPubSubL( i, RProperty::EInt );
        }
    for( ; i < ETestDes8KeyCount; ++i )
        {
        DefineTestPubSubL( i, RProperty::EByteArray );
        }
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::ConstructL return");
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::NewL
// ---------------------------------------------------------
CSensorDataCompensatorPlgTest* CSensorDataCompensatorPlgTest::NewL( CTestModuleIf& aTestModuleIf )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::NewL()");
    CSensorDataCompensatorPlgTest* self = new (ELeave) CSensorDataCompensatorPlgTest( aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::NewL self %p", self );
    return self;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::~CSensorDataCompensatorPlgTest
// ---------------------------------------------------------
CSensorDataCompensatorPlgTest::~CSensorDataCompensatorPlgTest()
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::~CSensorDataCompensatorPlgTest()");
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::~CSensorDataCompensatorPlgTest return");
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::LibEntryL
// Returns: Poiter to CSensorDataCompensatorPlgTest class
// ---------------------------------------------------------
EXPORT_C CSensorDataCompensatorPlgTest* LibEntryL( CTestModuleIf& aTestModuleIf )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::LibEntryL()");
    CSensorDataCompensatorPlgTest* libEntry( CSensorDataCompensatorPlgTest::NewL(aTestModuleIf) );
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::LibEntryL libEntry %p", libEntry );
    return libEntry;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::RunMethodL
// ---------------------------------------------------------
TInt CSensorDataCompensatorPlgTest::RunMethodL( CStifItemParser& aItem )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::RunMethodL()");
    const TStifFunctionInfo KFunctions[] =
        {
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.
        ENTRY( "CreateCompensatorPlugin", CSensorDataCompensatorPlgTest::CreateCompensatorPluginL ),
        ENTRY( "DestroyCompensatorPlugin", CSensorDataCompensatorPlgTest::DestroyCompensatorPluginL ),
        ENTRY( "SetState", CSensorDataCompensatorPlgTest::SetStateL ),
        ENTRY( "SetExpectedData", CSensorDataCompensatorPlgTest::SetExpectedDataL ),
        ENTRY( "VerifyTestResult", CSensorDataCompensatorPlgTest::VerifyTestResultL ),
        ENTRY( "CheckCount", CSensorDataCompensatorPlgTest::CheckCountL )
        };
    const TInt count( sizeof( KFunctions ) / sizeof( TStifFunctionInfo ) );
    TInt ret( RunInternalL( KFunctions, count, aItem ) );
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::RunMethodL return - ret %d", ret );
    return ret;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::CreateCompensatorPluginL
// ---------------------------------------------------------

TInt CSensorDataCompensatorPlgTest::CreateCompensatorPluginL( CStifItemParser& aItem )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CreateCompensatorPluginL()");
    
    // Initialize pubsub-key. Otherwise context framework does not give notification
    // if the same test case is run several time in sequence.
    SetTestPubSubL( ETestKeyState, KTestValueUndefined );
    TInt dataType( 0 );
    User::LeaveIfError( aItem.GetNextInt( dataType ) );
    iDataType = static_cast<TSensrvChannelDataTypeId>( dataType );
    TInt compensationType( 0 );
    User::LeaveIfError(aItem.GetNextInt( compensationType ) );
    iCompensationType = static_cast<TSensorCompensationType>( compensationType );

    // Load plug-in library
    _LIT( KCompensationPluginName, "sensordatacompensatorplugin.dll" );
    TUid CompensationPluginUid = { 0x2001FE5D };
    TUidType uid( KNullUid, KNullUid, CompensationPluginUid );

    User::LeaveIfError( iLibrary.Load( KCompensationPluginName, uid ) );

    // Function at ordinal 1 creates CSensorDataCompensatorPlugin instance
    TLibraryFunction entry = iLibrary.Lookup( 1 );

    if ( !entry )
        {
        User::Leave( KErrCorrupt );
        }

    CreateSensorDataCompensatorFuncL createL = reinterpret_cast<CreateSensorDataCompensatorFuncL>( entry );

    iPlugin = ( *createL )( iDataType, iCompensationType, *this );
    // Wait a moment to get cf listener thread initialized in compensator.
    // Otherwise indications of context changes do not get detected and test cases fail.
    User::After( 1000000 );

    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CreateCompensatorL return");
    return KErrNone;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::DestroyCompensatorPluginL
// ---------------------------------------------------------
TInt CSensorDataCompensatorPlgTest::DestroyCompensatorPluginL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::DestroyCompensatorPluginL()");
    delete iPlugin;
    iPlugin = NULL;
    iLibrary.Close();
    ResetDataItems();
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::DestroyCompensatorPluginL return");
    return KErrNone;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::SetStateL
// ---------------------------------------------------------
TInt CSensorDataCompensatorPlgTest::SetStateL( CStifItemParser& aItem )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::SetStateL()");
    TInt key( 0 );
    User::LeaveIfError( aItem.GetNextInt( key ) );
    TInt value( 0 );
    User::LeaveIfError( aItem.GetNextInt( value ) );
    SetTestPubSubL( key, value );
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::SetStateL return" );
    return KErrNone;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::SetExpectedDataL
// ---------------------------------------------------------
TInt CSensorDataCompensatorPlgTest::SetExpectedDataL( CStifItemParser& aItem )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::SetExpectedDataL()");

    User::LeaveIfError( aItem.GetNextInt( iExpectedItemCount ) );

    switch( iDataType )
        {
        case TSensrvAccelerometerAxisData::KDataTypeId:
        case TSensrvMagnetometerAxisData::KDataTypeId:
            {
            if ( ( iExpectedItemCount < 0 ) || ( iExpectedItemCount > KCompTestMaxTypeCount ) )
                {
                User::Leave( KErrArgument );
                }

            for ( TInt i( 0 ) ; i < iExpectedItemCount; ++i )
                {
                User::LeaveIfError( aItem.GetNextInt( iExpectedXAxisValue.At( i ) ) );
                User::LeaveIfError( aItem.GetNextInt( iExpectedYAxisValue.At( i ) ) );
                User::LeaveIfError( aItem.GetNextInt( iExpedtedZAxisValue.At( i ) ) );
                }
            break;
            }
        case TSensrvMagneticNorthData::KDataTypeId:
            {
            if ( iExpectedItemCount > KCompTestMaxTypeCount )
                {
                User::Leave( KErrArgument );
                }
            for ( TInt i( 0 ) ; i < iExpectedItemCount; ++i )
                {
                User::LeaveIfError( aItem.GetNextInt( iExpectedMagneticNorthValue.At( i ) ) );
                }
            break;
            }
        case TSensrvTappingData::KDataTypeId:
            {
            if ( iExpectedItemCount > KCompTestSinleTypeCount )
                {
                User::Leave( KErrArgument );
                }
            for ( TInt i( 0 ) ; i < iExpectedItemCount; ++i )
                {
                User::LeaveIfError( aItem.GetNextInt( iExpectedTappingXAxisValue.At( i ) ) );
                User::LeaveIfError( aItem.GetNextInt( iExpectedTappingYAxisValue.At( i ) ) );
                User::LeaveIfError( aItem.GetNextInt( iExpectedTappingZAxisValue.At( i ) ) );
                }
            break;
            }
        default:
            {
            User::Leave( KErrArgument );
            }
        }
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::SetExpectedDataL return" );
    return KErrNone;
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::ResetDataItems
// ---------------------------------------------------------
void CSensorDataCompensatorPlgTest::ResetDataItems( )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::ResetDataItems()");

    for ( TInt i( 0 ) ; i <  KCompTestMaxTypeCount; ++i )
        {
        iExpectedXAxisValue.At( i )         = 0xBADF00D;
        iExpectedYAxisValue.At( i )         = 0xBADF00D;
        iExpedtedZAxisValue.At( i )         = 0xBADF00D;
        iExpectedMagneticNorthValue.At( i ) = 0xBADF00D;
        iExpectedTappingXAxisValue.At( i )  = 0xBADF00D;
        iExpectedTappingYAxisValue.At( i )  = 0xBADF00D;
        iExpectedTappingZAxisValue.At( i )  = 0xBADF00D;
    }

    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::ResetDataItems return" );
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::VerifyTestResult
// ---------------------------------------------------------
TInt CSensorDataCompensatorPlgTest::VerifyTestResultL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::VerifyTestResult()");
    if ( !iTestResult )
        {
        User::Leave( KErrGeneral );
        }
    return KErrNone;
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::VerifyTestResult return" );
    }


// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::VerifyTestResult
// ---------------------------------------------------------
TInt CSensorDataCompensatorPlgTest::CheckCountL( CStifItemParser& /*aItem*/ )
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CheckCountL()");

    TInt itemCount( iPlugin->GetCompensationItemCount() );

    if ( itemCount == iExpectedItemCount )
    	{
		iTestResult = ETrue;
		}

    return KErrNone;
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CheckCountL return" );
    }

// ---------------------------------------------------------
// CSensorDataCompensatorPlgTest::CompensationValuesUpdated
// ---------------------------------------------------------
void CSensorDataCompensatorPlgTest::CompensationValuesUpdated()
    {
    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CompensationValuesUpdated()");

	// If match is found, set iTestResult to ETrue
	iTestResult = EFalse;
    // Temporary result
    TBool TempResult( ETrue );

	TInt itemCount( iPlugin->GetCompensationItemCount() );

	// Check expected values
	switch( iDataType )
		{
		case TSensrvAccelerometerAxisData::KDataTypeId:
		case TSensrvMagnetometerAxisData::KDataTypeId:
			{
            if ( itemCount == 0 && itemCount == iExpectedItemCount )
                {
                RDebug::Printf("[SensDataCompTest] TCompensationTypeAxisData verification:, itemcount: %d", itemCount );
                iTestResult = ETrue;
                }
            else
                {
                TPckgBuf<TCompensationTypeAxisData> pckg;

                TInt readXAxisValue( 0 );
                TInt readYAxisValue( 0 );
                TInt readZAxisValue( 0 );

                for ( TInt i( 0 ) ; i < itemCount && ( TempResult ) ; ++i )
                    {
                    pckg.Copy( iPlugin->GetCompensationItem( i ) );

                    readXAxisValue = pckg().iAxisX;
                    readYAxisValue = pckg().iAxisY;
                    readZAxisValue = pckg().iAxisZ;

                    if (   itemCount != iExpectedItemCount
                        || iExpectedXAxisValue.At( i ) != readXAxisValue
                        || iExpectedYAxisValue.At( i ) != readYAxisValue
                        || iExpedtedZAxisValue.At( i ) != readZAxisValue )
                        {
                        RDebug::Printf("[SensDataCompTest] TCompensationTypeAxisData verification failed, index: %d, count:%d<->%d, X:%d<->%d, Y:%d<->%d, Z:%d<->%d",
                                i,
                                iExpectedItemCount,
                                itemCount,
                                iExpectedXAxisValue.At( i ),
                                readXAxisValue,
                                iExpectedYAxisValue.At( i ),
                                readYAxisValue,
                                iExpedtedZAxisValue.At( i ),
                                readZAxisValue );

                        TempResult = EFalse;
                        }
                    else
                        {
                        RDebug::Printf("[SensDataCompTest] TCompensationTypeAxisData verified, index: %d, count:%d<->%d, X:%d<->%d, Y:%d<->%d, Z:%d<->%d",
                                i,
                                iExpectedItemCount,
                                itemCount,
                                iExpectedXAxisValue.At( i ),
                                readXAxisValue,
                                iExpectedYAxisValue.At( i ),
                                readYAxisValue,
                                iExpedtedZAxisValue.At( i ),
                                readZAxisValue );
                        }
                    }
                iTestResult = TempResult;
                }
			break;
			}
		case TSensrvMagneticNorthData::KDataTypeId:
			{
            if ( itemCount == 0 && itemCount == iExpectedItemCount )
                {
                RDebug::Printf("[SensDataCompTest] TCompensationTypeAngleData verification:, itemcount: %d", itemCount );
                iTestResult = ETrue;
                }
            else
                {
                TPckgBuf<TCompensationTypeAngleData> pckg;

                TInt readAngle(0);

                for ( TInt i( 0 ) ; i < itemCount && ( TempResult ); ++i )
                    {
                    pckg.Copy( iPlugin->GetCompensationItem( i ) );

                    readAngle = pckg().iAngle;

                    if (    itemCount != iExpectedItemCount
                         || iExpectedMagneticNorthValue.At( i ) != readAngle )
                        {
                        RDebug::Printf("[SensDataCompTest] TCompensationTypeAngleData verification failed, index: %d, count:%d<->%d, angle:%d<->%d",
                                i,
                                iExpectedItemCount,
                                itemCount,
                                iExpectedMagneticNorthValue.At( i ),
                                readAngle );

                        TempResult = EFalse;
                        }
                    else
                        {
                        RDebug::Printf("[SensDataCompTest] TCompensationTypeAngleData verified, index: %d, count:%d<->%d, angle:%d<->%d",
                                i,
                                iExpectedItemCount,
                                itemCount,
                                iExpectedMagneticNorthValue.At( i ),
                                readAngle );
                        }
                    }
                iTestResult = TempResult;
                }

			break;
			}
		case TSensrvTappingData::KDataTypeId:
			{
            if ( itemCount == 0 && itemCount == iExpectedItemCount )
                {
                RDebug::Printf("[SensDataCompTest] TCompensationTypeDirectionData verification:, itemcount: %d", itemCount );
                iTestResult = ETrue;
                }
            else
                {
                TPckgBuf<TCompensationTypeDirectionData> pckg;

                TInt readXAxisValue( 0 );
                TInt readYAxisValue( 0 );
                TInt readZAxisValue( 0 );



                for ( TInt i( 0 ) ; i < itemCount; ++i )
                    {
                    pckg.Copy( iPlugin->GetCompensationItem( i ) );

                    readXAxisValue = pckg().iTappingAxisX;
                    readYAxisValue = pckg().iTappingAxisY;
                    readZAxisValue = pckg().iTappingAxisZ;

                    if (    itemCount != iExpectedItemCount
                         || itemCount != KCompTestSinleTypeCount
                         || iExpectedTappingXAxisValue.At( i ) != readXAxisValue
                         || iExpectedTappingYAxisValue.At( i ) != readYAxisValue
                         || iExpectedTappingZAxisValue.At( i ) != readZAxisValue )
                        {
                        RDebug::Printf("[SensDataCompTest] TCompensationTypeDirectionData verification failed, index: %d, count:%d<->%d, X:%d<->%d, Y:%d<->%d, Z:%d<->%d",
                                i,
                                iExpectedItemCount,
                                itemCount,
                                iExpectedTappingXAxisValue.At( i ),
                                readXAxisValue,
                                iExpectedTappingYAxisValue.At( i ),
                                readYAxisValue,
                                iExpectedTappingZAxisValue.At( i ),
                                readZAxisValue );

                        TempResult = EFalse;
                        }
                    else
                        {
                        RDebug::Printf("[SensDataCompTest] TCompensationTypeDirectionData verified, index: %d, count:%d<->%d, X:%d<->%d, Y:%d<->%d, Z:%d<->%d",
                                i,
                                iExpectedItemCount,
                                itemCount,
                                iExpectedTappingXAxisValue.At( i ),
                                readXAxisValue,
                                iExpectedTappingYAxisValue.At( i ),
                                readYAxisValue,
                                iExpectedTappingZAxisValue.At( i ),
                                readZAxisValue );
                        }
                    }
                iTestResult = TempResult;
                }

            break;
			}
		default:
			{
			break;
			}
		}

    RDebug::Printf("[SensDataCompTest] CSensorDataCompensatorPlgTest::CompensationValuesUpdated()");
    }
