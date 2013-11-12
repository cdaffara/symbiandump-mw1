/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ICMSTIF class member functions
*
*/



// INCLUDE FILES
#include <e32cmn.h>
#include <StifTestInterface.h>
#include "ICMSTIF.h"

#include <e32debug.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CICMSTIF::CICMSTIF
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CICMSTIF::CICMSTIF()
    {

    }

// -----------------------------------------------------------------------------
// CICMSTIF::ConstructL
// Symbian 2nd phase constructor can leave.
// Note: If OOM test case uses STIF Logger, then STIF Logger must be created
// with static buffer size parameter (aStaticBufferSize). Otherwise Logger 
// allocates memory from heap and therefore causes error situations with OOM 
// testing. For more information about STIF Logger construction, see STIF Users 
// Guide.
// -----------------------------------------------------------------------------
//
void CICMSTIF::ConstructL()
    {
    iLog = CStifLogger::NewL( KICMSTIFLogPath, 
                          KICMSTIFLogFile);
/*    // SetAllocFailureSimulation( RHeap::EDeterministic, 0 ); // Enable OOM test loop
    SetAllocFailureSimulation( RHeap::ENone, 0 ); // Disable OOM test loop
    iMemTestEnabled = ETrue;*/
    }

// -----------------------------------------------------------------------------
// CICMSTIF::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CICMSTIF* CICMSTIF::NewL()
    {
    CICMSTIF* self = new (ELeave) CICMSTIF;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CICMSTIF::~CICMSTIF()
    { 
    delete iLog;
    }

// -----------------------------------------------------------------------------
// CICMSTIF::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
TInt CICMSTIF::InitL( 
    TFileName& /*aIniFile*/, 
    TBool /*aFirstTime*/ )
    {
    return KErrNone;

    }



// -----------------------------------------------------------------------------
// CICMSTIF::Case
// Returns a test case by number.
//
// This function contains an array of all available test cases 
// i.e pair of case name and test function. If case specified by parameter
// aCaseNumber is found from array, then that item is returned.
// 
// The reason for this rather complicated function is to specify all the
// test cases only in one place. It is not necessary to understand how
// function pointers to class member functions works when adding new test
// cases. See function body for instructions how to add new test case.
// -----------------------------------------------------------------------------
//
const TCaseInfo CICMSTIF::Case ( 
    const TInt aCaseNumber ) const 
     {
    /**
    * To add new test cases, implement new test case function and add new 
    * line to KCases array specify the name of the case and the function 
    * doing the test case
    * In practice, do following
    * 1) Make copy of existing test case function and change its name
    *    and functionality. Note that the function must be added to 
    *    ICMSTIF.cpp file and to ICMSTIF.h 
    *    header file.
    * 2) Add entry to following KCases array either by using FUNCENTRY or 
    *    ENTRY macro. 
    * ENTRY macro takes two parameters: test case name and test case 
    * function name.
    * FUNCENTRY macro takes only test case function name as a parameter and uses
    * that as a test case name and test case function name.
    */
    
    static TCaseInfoInternal const KCases[] =
        {
        // To add new test cases, add new items to this array
		ENTRY( "TestCase001", CICMSTIF::TestCase_001L ),
		ENTRY( "TestCase002", CICMSTIF::TestCase_002L ),
        ENTRY( "TestCase003", CICMSTIF::TestCase_003L ),
        ENTRY( "TestCase004", CICMSTIF::TestCase_004L ),
        ENTRY( "TestCase005", CICMSTIF::TestCase_005L ),
        ENTRY( "TestCase006", CICMSTIF::TestCase_006L ),
        ENTRY( "TestCase007", CICMSTIF::TestCase_007L ),
        ENTRY( "TestCase008", CICMSTIF::TestCase_008L ),
        ENTRY( "TestCase009", CICMSTIF::TestCase_009L ),
        ENTRY( "TestCase010", CICMSTIF::TestCase_010L ),
        ENTRY( "TestOOM",     CICMSTIF::TestCase_OOML )
        };

    // Verify that case number is valid
    if( (TUint) aCaseNumber >= sizeof( KCases ) / 
                               sizeof( TCaseInfoInternal ) )
        {
        // Invalid case, construct empty object
        TCaseInfo null( (const TText*) L"" );
        null.iMethod = NULL;
        return null;
        }

    // Construct TCaseInfo object and return it
    TCaseInfo tmp ( KCases[ aCaseNumber ].iCaseName );
    tmp.iMethod = KCases[ aCaseNumber ].iMethod;
    return tmp;
     
    }
    
// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_001L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//     
TInt CICMSTIF::TestCase_001L( TTestResult& aResult )
	{
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
	delete icm;
	CImagingConfigManager* icm2 = CImagingConfigManager::NewLC();
	CleanupStack::Pop( icm2 );
    aResult.iResult = KErrNone;
    aResult.iResultDes = _L("CImagingConfigManager creations with NewL and NewLC succeeded both.");

    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_002L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_002L( TTestResult& aResult )
	{
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    if(icm->NumberOfVideoQualityLevels() < 1 || icm->NumberOfVideoQualityLevels() > 100)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("NumberOfVideoQualityLevels() didn't return value between 1-100.");
        }
    else
        {
        aResult.iResult = KErrNone;
		aResult.iResultDes = _L("NumberOfVideoQualityLevels() OK.");
        }
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_003L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_003L( TTestResult& aResult )
	{
	_LIT8(KFileMime, "video/3gpp");
	_LIT8(KVideoCodecMimeType, "video/H263-2000");
	_LIT8(KPreferredSupplier, "Nokia");
	const TUint32 KAudioFourCCType = 0x524D4120;
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
	CleanupStack::PushL( icm );
    CArrayFixFlat<TUint>* levels = new( ELeave ) CArrayFixFlat< TUint >( 1 );
    TVideoQualitySet videoset;
    icm->GetVideoQualityLevelsL(*levels);
    
    if(levels->Count() != 10 || levels->At(0) != 98
                             || levels->At(1) != 99 || levels->At(2) != 200
                             || levels->At(3) != 201 || levels->At(4) != 202 || levels->At(5) != 203
                             || levels->At(6) != 301 || levels->At(7) != 302 || levels->At(8) != 303
                             || levels->At(9) != 304 )
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("GetVideoQualityLevelsL()->At didn't return the expected values.");
        }
    else
        {
        icm->GetVideoQualitySet( videoset, /*100*/levels->At(1) );
        TInt length1 = KMaxStringLength;
        TInt length2 = KMaxStringLength;
        TInt length3 = KMaxStringLength;
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iVideoFileMimeType[i] == '\0')
                {
                length1 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iVideoCodecMimeType[i] == '\0')
                {
                length2 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iPreferredSupplier[i] == '\0')
                {
                length3 = i;
                break;
                }
            }
        if( videoset.iVideoQualitySetLevel != 99 ||
            memcompare(videoset.iVideoFileMimeType, length1, KFileMime().Ptr(), KFileMime().Length()) ||
            memcompare(videoset.iVideoCodecMimeType, length2, KVideoCodecMimeType().Ptr(), KVideoCodecMimeType().Length()) ||
            videoset.iVideoWidth != 176 ||
            videoset.iVideoHeight != 144 ||
            videoset.iVideoFrameRate != 15.0 ||
            videoset.iVideoBitRate != 60000 ||
            videoset.iVideoEncoderUID.iUid != 0 ||
            videoset.iRandomAccessRate != 0.2 ||
            videoset.iVideoPixelAspectRatioNum != 12 ||
            videoset.iVideoPixelAspectRatioDenom != 11 ||
            memcompare(videoset.iPreferredSupplier, length3, KPreferredSupplier().Ptr(), KPreferredSupplier().Length()) ||
            videoset.iAudioFourCCType.FourCC() != KAudioFourCCType ||
            videoset.iAudioBitRate != 12200 ||
            videoset.iAudioSamplingRate != 8000 ||
            videoset.iAudioChannels != 1 ||
            videoset.iAudioEncoderUID.iUid != 0 ||
            videoset.iAVSyncStartDelay != -200 ||
            videoset.iAVSyncResumeDelay != 0 ||
            videoset.iCameraDisplayId != 2 ||
            videoset.iCamcorderVisible != 100)
            {
            aResult.iResult = KErrGeneral;
		    aResult.iResultDes = _L("GetVideoQualitySetL() didn't return expected values.");
            }
        else
            {
            aResult.iResult = KErrNone;
    		aResult.iResultDes = _L("GetVideoQualitySetL() OK.");
            }
        }
    CleanupStack::Pop( icm );
    delete levels;
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_004L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_004L( TTestResult& aResult )
	{
	_LIT8(KFileMime, "video/3gpp");
	_LIT8(KVideoCodecMimeType, "video/H263-2000");
	_LIT8(KPreferredSupplier, "Nokia");
	const TUint32 KAudioFourCCType = 0x524D4120;
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    TVideoQualitySet videoset;
    TSize resolution(176,144);
    icm->GetVideoQualitySet(videoset, resolution, KVideoCodecMimeType(), 0, 0, TFourCC(KAudioFourCCType));
    
    if(videoset.iVideoHeight != resolution.iHeight ||
       videoset.iVideoWidth != resolution.iWidth ||
       videoset.iVideoQualitySetLevel != 99)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("GetVideoQualitySetL() didn't return expected value.");
        }
    else
        {
        TInt length1 = KMaxStringLength;
        TInt length2 = KMaxStringLength;
        TInt length3 = KMaxStringLength;
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iVideoFileMimeType[i] == '\0')
                {
                length1 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iVideoCodecMimeType[i] == '\0')
                {
                length2 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iPreferredSupplier[i] == '\0')
                {
                length3 = i;
                break;
                }
            }
        if( memcompare(videoset.iVideoFileMimeType, length1, KFileMime().Ptr(), KFileMime().Length()) ||
            memcompare(videoset.iVideoCodecMimeType, length2, KVideoCodecMimeType().Ptr(), KVideoCodecMimeType().Length()) ||
            videoset.iVideoWidth != 176 ||
            videoset.iVideoHeight != 144 ||
            videoset.iVideoFrameRate != 15.0 ||
            videoset.iVideoBitRate != 60000 ||
            videoset.iVideoEncoderUID.iUid != 0 ||
            videoset.iRandomAccessRate != 0.2 ||
            videoset.iVideoPixelAspectRatioNum != 12 ||
            videoset.iVideoPixelAspectRatioDenom != 11 ||
            memcompare(videoset.iPreferredSupplier, length3, KPreferredSupplier().Ptr(), KPreferredSupplier().Length()) ||
            videoset.iAudioFourCCType.FourCC() != KAudioFourCCType ||
            videoset.iAudioBitRate != 12200 ||
            videoset.iAudioSamplingRate != 8000 ||
            videoset.iAudioChannels != 1 ||
            videoset.iAudioEncoderUID.iUid != 0 ||
            videoset.iAVSyncStartDelay != -200 ||
            videoset.iAVSyncResumeDelay != 0 ||
            videoset.iCameraDisplayId != 2 ||
            videoset.iCamcorderVisible != 100)
            {
            aResult.iResult = KErrGeneral;
		    aResult.iResultDes = _L("GetVideoQualitySetL() didn't return expected values.");
            }
        else
            {
            aResult.iResult = KErrNone;
    		aResult.iResultDes = _L("GetVideoQualitySetL() OK.");
            }
        }
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_005L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_005L( TTestResult& aResult )
	{
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    TCamcorderMMFPluginSettings ccorSettings;
    icm->GetCamcorderMMFPluginSettings(ccorSettings);
    
    if(ccorSettings.iVideoNightFrameRate != 5.00 ||
       ccorSettings.iCMRAvgVideoBitRateScaler != 0.9 ||
       ccorSettings.iVideoComplexitySetting != 1 ||
       ccorSettings.iCMRLatencyQualityTradeoff != 1.0 ||
       ccorSettings.iCMRPictureQuality != 50 ||
       ccorSettings.iCMRQualityTemporalTradeoff != 0.8 ||
       ccorSettings.iCMRNumCameraBuffers != 2 ||
       ccorSettings.iCMRNumCameraBuffersARM != 1 ||
       ccorSettings.iCMRMinNumOutputBuffers != 4)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("GetCamcorderMMFPluginSettingsL() didn't return expected value.");
        }
    else
        {
        aResult.iResult = KErrNone;
    	aResult.iResultDes = _L("GetCamcorderMMFPluginSettingsL() OK.");
        }
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_006L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_006L( TTestResult& aResult )
	{
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    if(icm->NumberOfImageQualityLevels() != 27)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("NumberOfImageQualityLevels() didn't return 27 as expected.");
        }
    else
        {
        aResult.iResult = KErrNone;
		aResult.iResultDes = _L("NumberOfImageQualityLevels() OK.");
        }
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_007L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_007L( TTestResult& aResult )
	{
	_LIT8(KImageFileMimeType, "image/jpeg");
	_LIT8(KImageFileExtension, ".jpg");
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
	CleanupStack::PushL( icm );
    CArrayFixFlat<TUint>* levels = new( ELeave ) CArrayFixFlat< TUint >( 1 );
    TImageQualitySet imageset;
    icm->GetImageQualityLevelsL(*levels, 2);
    
    if(levels->Count() != 24 || levels->At(0) != 99 || levels->At(1) != 100 || levels->At(2) != 101
                           || levels->At(3) != 102 || levels->At(4) != 103 || levels->At(5) != 104
                           || levels->At(6) != 105 || levels->At(7) != 106 || levels->At(8) != 199
                           || levels->At(9) != 200 || levels->At(10) != 201 || levels->At(11) != 202
                           || levels->At(12) != 203 || levels->At(13) != 204 || levels->At(14) != 205
                           || levels->At(15) != 206 || levels->At(16) != 299 || levels->At(17) != 300
                           || levels->At(18) != 301 || levels->At(19) != 302 || levels->At(20) != 303
                           || levels->At(21) != 304 || levels->At(22) != 305 || levels->At(23) != 306)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("GetImageQualityLevelsL()->At didn't return expected value.");
        }
    else
        {
        icm->GetImageQualitySet( imageset, levels->At(1) );
        TInt length1 = KMaxStringLength;
        TInt length2 = KMaxStringLength;
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(imageset.iImageFileMimeType[i] == '\0')
                {
                length1 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(imageset.iImageFileExtension[i] == '\0')
                {
                length2 = i;
                break;
                }
            }
        if( memcompare(imageset.iImageFileMimeType, length1, KImageFileMimeType().Ptr(), KImageFileMimeType().Length()) ||
            memcompare(imageset.iImageFileExtension, length2, KImageFileExtension().Ptr(), KImageFileExtension().Length()) ||
            imageset.iImageWidth < 160 ||
            imageset.iImageWidth > 13333 ||
            imageset.iImageHeight < 120 ||
            imageset.iImageHeight > 10000 ||
            imageset.iCompressionQuality < 10 ||
            imageset.iCompressionQuality > 100 ||
            imageset.iEstimatedSize < 1024 ||
            imageset.iEstimatedSize > 10240000 ||
            imageset.iCameraDisplayId > 10000 ||
            imageset.iCamcorderVisible > 10000)
            {
            aResult.iResult = KErrGeneral;
		    aResult.iResultDes = _L("GetImageQualityLevelsL() didn't return expected values.");
            }
        else
            {
            aResult.iResult = KErrNone;
    		aResult.iResultDes = _L("GetImageQualityLevelsL() OK.");
            }
        }
    CleanupStack::Pop( icm );
    delete levels;
	delete icm;
    return KErrNone;
	}

// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_008L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_008L( TTestResult& aResult )
	{
	_LIT8(KImageFileMimeType, "image/jpeg");
	_LIT8(KImageFileExtension, ".jpg");
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    TImageQualitySet imageset;
    TSize resolution(1280,960);
    icm->GetImageQualitySet(imageset, resolution, 2);
    
    if(imageset.iImageHeight != resolution.iHeight ||
       imageset.iImageWidth != resolution.iWidth ||
       imageset.iImageQualitySetLevel > 10000)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("GetImageQualitySetL() resolution didn't return expected value.");
        }
    else
        {
        TInt length1 = KMaxStringLength;
        TInt length2 = KMaxStringLength;
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(imageset.iImageFileMimeType[i] == '\0')
                {
                length1 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(imageset.iImageFileExtension[i] == '\0')
                {
                length2 = i;
                break;
                }
            }
        if( memcompare(imageset.iImageFileMimeType, length1, KImageFileMimeType().Ptr(), KImageFileMimeType().Length()) ||
            memcompare(imageset.iImageFileExtension, length2, KImageFileExtension().Ptr(), KImageFileExtension().Length()) ||
            imageset.iCompressionQuality < 10 ||
            imageset.iCompressionQuality > 100 ||
            imageset.iEstimatedSize < 1024 ||
            imageset.iEstimatedSize > 10240000 ||
            imageset.iCameraDisplayId > 10000 ||
            imageset.iCamcorderVisible > 10000)
            {
            aResult.iResult = KErrGeneral;
		    aResult.iResultDes = _L("GetImageQualitySetL() didn't return expected values.");
            }
        else
            {
            aResult.iResult = KErrNone;
    		aResult.iResultDes = _L("GetImageQualitySetL() OK.");
            }
        }
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_009L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_009L( TTestResult& aResult )
	{
	_LIT8(KFileMime, "video/3gpp");
	_LIT8(KVideoCodecMimeType, "video/H263-2000");
	_LIT8(KPreferredSupplier, "Nokia");
	const TUint32 KAudioFourCCType = 0x524D4120;
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    TVideoQualitySet videoset;
    TSize resolution(176,144);
    icm->GetVideoQualitySet( videoset, 125 );// This should return video set num 99
    
    if(videoset.iVideoHeight != resolution.iHeight ||
       videoset.iVideoWidth != resolution.iWidth ||
       videoset.iVideoQualitySetLevel != 99)
        {
        aResult.iResult = KErrGeneral;
		aResult.iResultDes = _L("GetVideoQualitySetL() didn't return expected value.");
        }
    else
        {
        TInt length1 = KMaxStringLength;
        TInt length2 = KMaxStringLength;
        TInt length3 = KMaxStringLength;
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iVideoFileMimeType[i] == '\0')
                {
                length1 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iVideoCodecMimeType[i] == '\0')
                {
                length2 = i;
                break;
                }
            }
        for( TInt i=0; i<KMaxStringLength; i++)
            {
            if(videoset.iPreferredSupplier[i] == '\0')
                {
                length3 = i;
                break;
                }
            }
        if( memcompare(videoset.iVideoFileMimeType, length1, KFileMime().Ptr(), KFileMime().Length()) ||
            memcompare(videoset.iVideoCodecMimeType, length2, KVideoCodecMimeType().Ptr(), KVideoCodecMimeType().Length()) ||
            videoset.iVideoWidth != 176 ||
            videoset.iVideoHeight != 144 ||
            videoset.iVideoFrameRate != 15.0 ||
            videoset.iVideoBitRate != 60000 ||
            videoset.iVideoEncoderUID.iUid != 0 ||
            videoset.iRandomAccessRate != 0.2 ||
            videoset.iVideoPixelAspectRatioNum != 12 ||
            videoset.iVideoPixelAspectRatioDenom != 11 ||
            memcompare(videoset.iPreferredSupplier, length3, KPreferredSupplier().Ptr(), KPreferredSupplier().Length()) ||
            videoset.iAudioFourCCType.FourCC() != KAudioFourCCType ||
            videoset.iAudioBitRate != 12200 ||
            videoset.iAudioSamplingRate != 8000 ||
            videoset.iAudioChannels != 1 ||
            videoset.iAudioEncoderUID.iUid != 0 ||
            videoset.iAVSyncStartDelay != -200 ||
            videoset.iAVSyncResumeDelay != 0 ||
            videoset.iCameraDisplayId != 2 ||
            videoset.iCamcorderVisible != 100)
            {
            aResult.iResult = KErrGeneral;
		    aResult.iResultDes = _L("GetVideoQualitySetL() didn't return expected values.");
            }
        else
            {
            aResult.iResult = KErrNone;
    		aResult.iResultDes = _L("GetVideoQualitySetL() OK.");
            }
        }
	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_010L
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_010L( TTestResult& aResult )
	{
	_LIT8(KImageFileMimeType, "image/jpeg");
	_LIT8(KImageFileExtension, ".jpg");
	CImagingConfigManager* icm = CImagingConfigManager::NewL();
    TImageQualitySet imageset;
    
    icm->GetImageQualitySet( imageset, 305, 2 );
    TInt length1 = KMaxStringLength;
    TInt length2 = KMaxStringLength;
    for( TInt i=0; i<KMaxStringLength; i++)
        {
        if(imageset.iImageFileMimeType[i] == '\0')
            {
            length1 = i;
            break;
            }
        }
    for( TInt i=0; i<KMaxStringLength; i++)
        {
        if(imageset.iImageFileExtension[i] == '\0')
            {
            length2 = i;
            break;
            }
        }
    if( memcompare(imageset.iImageFileMimeType, length1, KImageFileMimeType().Ptr(), KImageFileMimeType().Length()) ||
        memcompare(imageset.iImageFileExtension, length2, KImageFileExtension().Ptr(), KImageFileExtension().Length()) ||
        imageset.iImageWidth < 160 ||
        imageset.iImageWidth > 13000 ||
        imageset.iImageHeight < 120 ||
        imageset.iImageHeight > 10000 ||
        imageset.iCompressionQuality < 10 ||
        imageset.iCompressionQuality > 100 ||
        imageset.iEstimatedSize < 1024 ||
        imageset.iEstimatedSize > 10240000 ||
        imageset.iCameraDisplayId > 10000 ||
        imageset.iCamcorderVisible > 10000)
        {
        aResult.iResult = KErrGeneral;
	    aResult.iResultDes = _L("GetImageQualityLevelsL() didn't return expected values.");
        }
    else
        {
        aResult.iResult = KErrNone;
		aResult.iResultDes = _L("GetImageQualityLevelsL() OK.");
        }

	delete icm;
    return KErrNone;
	}


// -----------------------------------------------------------------------------
// CICMSTIF::TestCase_OOML
//
// <INSERT DESCRIPTION HERE>
//
// Pre-Condition: <INSERT PRE-CONDITIONS HERE>
// -----------------------------------------------------------------------------
//    
TInt CICMSTIF::TestCase_OOML( TTestResult& aResult )
    {
    TInt error = KErrNone;
    _LIT(KTestCase, "TestCase0??");
    
    for( TInt i = 0; Case(i).iMethod != NULL && !error; i++ )
        {
        // Get the pointer to test case function
        TCaseInfo tmp = Case(i);
        if(KErrNotFound == tmp.iCaseName.Match(KTestCase()))
            {
            break;
            }
        SetAllocFailureSimulation( RHeap::EDeterministic, 1 ); // Enable OOM test loop
        aResult.SetResult( KErrNoMemory, _L("") ); // Initial value
        for (; ( aResult.iResult == KErrNoMemory ) && !error;)
            {      
            // Switches on alloc failure simulation if the parameters are set for it
            AllocFailureSimulation( ETrue ); 
            RDebug::Print(_L("*** OOM loop : %d ***\n"), iCurrentAllocFailureRate );

            iMethod = tmp.iMethod;        
            
            TRAP( error, ( this->*iMethod )( aResult ) );
            if ( error == KErrNoMemory )
                {
                error = KErrNone;
                }
            }
        AllocFailureSimulation( EFalse );
        }

    SetAllocFailureSimulation( RHeap::ENone, 1 ); // Disable OOM test loop
    User::LeaveIfError( error );
    return KErrNone;
    }


/*
*/    

void CICMSTIF::AllocFailureSimulation (TBool aSwitchedOn)
	{
	if (aSwitchedOn)
		{
		__UHEAP_SETFAIL ( iAllocFailureType, iCurrentAllocFailureRate );
        iCurrentAllocFailureRate++;
		}
	else
		{
		__UHEAP_RESET;
        iCurrentAllocFailureRate = iAllocFailureRate;
		}
	}


/*
*/    

void CICMSTIF::SetAllocFailureSimulation( RHeap::TAllocFail aAllocFailureType, TInt aAllocFailureRate )
    {
    iAllocFailureType = aAllocFailureType;
    iAllocFailureRate = aAllocFailureRate;
    iCurrentAllocFailureRate = aAllocFailureRate;
    }

// -----------------------------------------------------------------------------
// CICMSTIF::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
TInt CICMSTIF::GetTestCasesL( 
    const TFileName& /*aConfig*/, 
    RPointerArray<TTestCaseInfo>& aTestCases )
    {

    // Loop through all test cases and create new
    // TTestCaseInfo items and append items to aTestCase array    
    for( TInt i = 0; Case(i).iMethod != NULL; i++ )
        {
 
	    // Allocate new TTestCaseInfo from heap for a testcase definition.
        TTestCaseInfo* newCase = new( ELeave ) TTestCaseInfo();
    
	    // PushL TTestCaseInfo to CleanupStack.    
        CleanupStack::PushL( newCase );

	    // Set number for the testcase.
	    // When the testcase is run, this comes as a parameter to RunTestCaseL.
        newCase->iCaseNumber = i;

	    // Set title for the test case. This is shown in UI to user.
        newCase->iTitle.Copy( Case(i).iCaseName );

	    // Append TTestCaseInfo to the testcase array. After appended 
	    // successfully the TTestCaseInfo object is owned (and freed) 
	    // by the TestServer. 
        User::LeaveIfError(aTestCases.Append ( newCase ) );
        
	    // Pop TTestCaseInfo from the CleanupStack.
        CleanupStack::Pop( newCase );

        }

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CICMSTIF::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CICMSTIF::RunTestCaseL(
    const TInt aCaseNumber ,
    const TFileName& /*aConfig*/,
    TTestResult& aResult )
    {
    // Return value
    TInt execStatus = KErrNone;

    // Get the pointer to test case function
    TCaseInfo tmp = Case ( aCaseNumber );

    // Check that case number was valid
    if ( tmp.iMethod != NULL )
        {
        // Valid case was found, call it via function pointer
        iMethod = tmp.iMethod;        
        execStatus  = ( this->*iMethod )( aResult );                  
        }
    else
        {        
        // Valid case was not found, return error.
        execStatus = KErrNotFound;
        }

    // Return case execution status (not the result of the case execution)
    return execStatus;
    }

// -----------------------------------------------------------------------------
// CICMSTIF::OOMTestQueryL
// Used to check if a particular test case should be run in OOM conditions and 
// which memory allocations should fail.    
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
TBool CICMSTIF::OOMTestQueryL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */, 
                                TOOMFailureType& /* aFailureType */, 
                                TInt& /* aFirstMemFailure */, 
                                TInt& /* aLastMemFailure */ ) 
    {
    _LIT( KOOMTestQueryL, "CICMSTIF::OOMTestQueryL" );
    iLog->Log( KOOMTestQueryL ); 

    return EFalse;

    }

// -----------------------------------------------------------------------------
// CICMSTIF::OOMTestInitializeL
// Used to perform the test environment setup for a particular OOM test case. 
// Test Modules may use the initialization file to read parameters for Test 
// Module initialization but they can also have their own configure file or 
// some other routine to initialize themselves.  
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CICMSTIF::OOMTestInitializeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

// -----------------------------------------------------------------------------
// CICMSTIF::OOMHandleWarningL
// In some cases the heap memory allocation should be skipped, either due to
// problems in the OS code or components used by the code being tested, or even 
// inside the tested components which are implemented this way on purpose (by 
// design), so it is important to give the tester a way to bypass allocation 
// failures.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CICMSTIF::OOMHandleWarningL( 
                                const TFileName& /* aTestCaseFile */,
                                const TInt /* aCaseNumber */, 
                                TInt& /* aFailNextValue */ )
    {
    }

// -----------------------------------------------------------------------------
// CICMSTIF::OOMTestFinalizeL
// Used to perform the test environment cleanup for a particular OOM test case.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//                  
void CICMSTIF::OOMTestFinalizeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CTestModuleBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CTestModuleBase* LibEntryL()
    {
    return CICMSTIF::NewL();

    }

// -----------------------------------------------------------------------------
// SetRequirements handles test module parameters(implements evolution
// version 1 for test module's heap and stack sizes configuring).
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt SetRequirements( CTestModuleParam*& aTestModuleParam, 
                                TUint32& aParameterValid )
    {

    /* --------------------------------- NOTE ---------------------------------
    USER PANICS occurs in test thread creation when:
    1) "The panic occurs when the value of the stack size is negative."
    2) "The panic occurs if the minimum heap size specified is less
       than KMinHeapSize".
       KMinHeapSize: "Functions that require a new heap to be allocated will
       either panic, or will reset the required heap size to this value if
       a smaller heap size is specified".
    3) "The panic occurs if the minimum heap size specified is greater than
       the maximum size to which the heap can grow".
    Other:
    1) Make sure that your hardware or Symbian OS is supporting given sizes.
       e.g. Hardware might support only sizes that are divisible by four.
    ------------------------------- NOTE end ------------------------------- */

    // Normally STIF uses default heap and stack sizes for test thread, see:
    // KTestThreadMinHeap, KTestThreadMinHeap and KStackSize.
    // If needed heap and stack sizes can be configured here by user. Remove
    // comments and define sizes.


    aParameterValid = KStifTestModuleParameterChanged;

    CTestModuleParamVer01* param = CTestModuleParamVer01::NewL();
    // Stack size
    param->iTestThreadStackSize= 2*16384; // 32K stack
    // Heap sizes
    param->iTestThreadMinHeap = 4096;   // 4K heap min
    param->iTestThreadMaxHeap = 8*1048576;// 8M heap max

    aTestModuleParam = param;

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// E32Dll is a DLL entry point function
// Returns: KErrNone: No error
// -----------------------------------------------------------------------------
//
#ifndef EKA2 // Hide Dll entry point to EKA2
GLDEF_C TInt E32Dll(
    TDllReason /*aReason*/) // Reason
    {
    return(KErrNone);

    }
#endif // EKA2

//  End of File
