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
* Description:  Config manager for Media recorder
*
*/


#include <e32base.h>
#include "CCMRConfigManager.h"
#include "CCMRVideoHWParams.h"
#include "CCMRSupportedCodecs.h"

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CCMRConfigManager::CCMRConfigManager()
// Default constructor.
// ---------------------------------------------------------------------------
//
CCMRConfigManager::CCMRConfigManager()
    {
    }

// ---------------------------------------------------------------------------
// CCMRConfigManager::ConstructL()
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
void CCMRConfigManager::ConstructL()
    {
	PRINT(_L("CCMRConfigManager::ConstructL in"));   
	iICMDataAvailable = EFalse;
	 
	TInt icmError = KErrNone;
	TRAP(icmError, iImagingConfigManager = CImagingConfigManager::NewL());
	if ( icmError )
		{
		// if ICM values cannot be read from central reposity or ICM is not available.
		PRINT((_L("CCMRConfigManager::ConstructL ICM initialization failed - using old default values. %d"), icmError));		
		}
	else
		{
		PRINT((_L("CCMRConfigManager::ConstructL ICM initialized")));
		}
	
	if ( icmError == KErrNone )
		{
		PRINT(_L("CCMRConfigManager::ConstructL reading plugin settings."));    		
		iImagingConfigManager->GetCamcorderMMFPluginSettings(iPluginSettings); 
		PRINT(_L("CCMRConfigManager::ConstructL plugin settings read."));    	
		}
	else
		{
		PRINT(_L("CCMRConfigManager::ConstructL ICM init failed - using old defaults instead."));
		iPluginSettings.iVideoNightFrameRate = KCMRNightModeFrameRate;
		iPluginSettings.iCMRAvgVideoBitRateScaler = KCMRAvgVideoBitRateScaler;
		iPluginSettings.iVideoComplexitySetting = KCMRVideoComplexityARMSubQCIF;
		iPluginSettings.iCMRLatencyQualityTradeoff = KCMRLatencyQyalityTradeoff;
		iPluginSettings.iCMRPictureQuality = KCMRPictureQuality;
		iPluginSettings.iCMRQualityTemporalTradeoff = KCMRQualityTemporalTradeoff;
		iPluginSettings.iCMRNumCameraBuffers = KCMRNumCameraBuffers;
		iPluginSettings.iCMRNumCameraBuffersARM	= KCMRNumCameraBuffersARM;	
		iPluginSettings.iCMRMinNumOutputBuffers = KCMRMinNumOutputBuffers;
		}
		
	// set some reasonable defaults for Video Quality set.
	iVideoFrameSize = TSize(KCMRQCIFWidth, KCMRQCIFHeight);
    iVideoCodec = KCMRMimeTypeH263;
    iAudioCodec = KCMRFourCCIdAMRNB;
    iVideoPixelAspectRatioNum = KCMRAspectRatioNum;
    iVideoPixelAspectRatioDenom = KCMRAspectRatioDenom;
		
	ResetVideoQualitySet();
		
	PRINT(_L("CCMRConfigManager::ConstructL out"));   		
    }

// ---------------------------------------------------------------------------
// CCMRConfigManager::NewL()
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCMRConfigManager* CCMRConfigManager::NewL()
    {
	PRINT(_L("CCMRConfigManager::NewL in"));    
    CCMRConfigManager* self = CCMRConfigManager::NewLC();
    CleanupStack::Pop( self );
    PRINT(_L("CCMRConfigManager::NewL out"));    
    return self;
    }

// ---------------------------------------------------------------------------
// CCMRConfigManager::NewLC()
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CCMRConfigManager* CCMRConfigManager::NewLC()
    {
	PRINT(_L("CCMRConfigManager::NewLC in"));    
    CCMRConfigManager* self = new( ELeave ) CCMRConfigManager;
    CleanupStack::PushL( self );
    self->ConstructL();
	PRINT(_L("CCMRConfigManager::NewLC out"));        
    return self;
    }

// ---------------------------------------------------------------------------
// CCMRConfigManager::~CCMRConfigManager
// Destructor
// ---------------------------------------------------------------------------
//
CCMRConfigManager::~CCMRConfigManager()
    {
	PRINT(_L("CCMRConfigManager::~CCMRConfigManager in"));     
   	delete iImagingConfigManager;
	PRINT(_L("CCMRConfigManager::~CCMRConfigManager out"));        	
    }

// ---------------------------------------------------------------------------
// Set video codec mime type.
// ---------------------------------------------------------------------------
//    
void CCMRConfigManager::SetVideoCodec(TDesC8& aVideoCodec)
	{
	PRINT((_L("CCMRConfigManager::SetVideoCodec() in")));	
	if (iVideoCodec != aVideoCodec)
		{
		iVideoCodec = aVideoCodec;
		ResetVideoQualitySet();		
		}
	else
		{
		PRINT((_L("CCMRConfigManager::SetVideoCodec() already using given codec")));		
		}
	PRINT((_L("CCMRConfigManager::SetVideoCodec() out")));				
	}

// ---------------------------------------------------------------------------
// Set audio codec mime type (TFourCC).
// ---------------------------------------------------------------------------
//
void CCMRConfigManager::SetAudioCodec(TFourCC& aAudioCodec)
	{
	PRINT((_L("CCMRConfigManager::SetAudioCodec() in")));	
	if ( iAudioCodec.FourCC() != aAudioCodec.FourCC() )
		{
		PRINT((_L("CCMRConfigManager::SetAudioCodec() 1")));		
		iAudioCodec.Set(aAudioCodec);
		PRINT((_L("CCMRConfigManager::SetAudioCodec() 2")));		
		ResetVideoQualitySet();		
		}
	else
		{
		PRINT((_L("CCMRConfigManager::SetAudioCodec() already using given codec")));		
		}
	PRINT((_L("CCMRConfigManager::SetAudioCodec() out")));				
	}

// ---------------------------------------------------------------------------
// Set video frame size.
// ---------------------------------------------------------------------------
// 
void CCMRConfigManager::SetVideoFrameSize(TSize aVideoFrameSize)
	{
	PRINT((_L("CCMRConfigManager::SetVideoFrameSize() in, width: %d, height: %d"), aVideoFrameSize.iWidth, aVideoFrameSize.iHeight));			
	if (iVideoFrameSize != aVideoFrameSize)
		{
		iVideoFrameSize = aVideoFrameSize;
		ResetVideoQualitySet();		
		}
	else
		{
		PRINT((_L("CCMRConfigManager::SetVideoFrameSize() already using given size")));		
		}
	PRINT((_L("CCMRConfigManager::SetVideoFrameSize() out")));		
	}
 
// ---------------------------------------------------------------------------
// Set video pixel aspect ratio.
// ---------------------------------------------------------------------------
//
void CCMRConfigManager::SetVideoPixelAspectRatio(TUint aNumerator, TUint aDenominator)
	{
	PRINT((_L("CCMRConfigManager::SetVideoPixelAspectRatio() in, num: %d, denom: %d"), aNumerator, aDenominator));	
	if ((iVideoPixelAspectRatioNum != aNumerator) ||
		(iVideoPixelAspectRatioDenom != aDenominator) )
		{
		iVideoPixelAspectRatioNum = aNumerator;
		iVideoPixelAspectRatioDenom = aDenominator;
		ResetVideoQualitySet();		
		}
	else
		{
		PRINT((_L("CCMRConfigManager::SetVideoPixelAspectRatio() already using given pixel aspect ratio")));		
		}
	PRINT((_L("CCMRConfigManager::SetVideoPixelAspectRatio() out")));		
	}

// ---------------------------------------------------------------------------
// Query whether Imaging Configuration data is available
// ---------------------------------------------------------------------------
//
TBool CCMRConfigManager::IsICMConfigDataAvailable() const
	{
	PRINT((_L("CCMRConfigManager::IsICMConfigDataAvailable(): %d"), iICMDataAvailable));		
	return iICMDataAvailable;
	}

// ---------------------------------------------------------------------------
// Get CamcorderMMFPlugin specific settings.
// ---------------------------------------------------------------------------
//
const TCamcorderMMFPluginSettings CCMRConfigManager::PluginSettings() const
    {
    PRINT((_L("CCMRConfigManager::PluginSettings() being used.")));
    return iPluginSettings;
    }
    
// ---------------------------------------------------------------------------
//  Get Video Quality settings.
// ---------------------------------------------------------------------------
//
const TVideoQualitySet CCMRConfigManager::VideoQualitySettings() const
    {
	PRINT((_L("CCMRConfigManager::VideoQualitySettings() being used.")));    
    return iActiveVideoQualitySet;
    }    

// ---------------------------------------------------------------------------
// Resets config manager to new Video Quality set.
// ---------------------------------------------------------------------------
//
void CCMRConfigManager::ResetVideoQualitySet()
	{
	PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() in")));					
    if ( iImagingConfigManager )
	    {
	    TVideoQualitySet newQualitySet;
	    PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() getting Video Quality Set from ICM.")));
	    TInt error = iImagingConfigManager->GetVideoQualitySet( newQualitySet,
	                 			                  			   	iVideoFrameSize,
	                             			     			   	iVideoCodec,
	                                  						   	iVideoPixelAspectRatioNum,
	                                  			   				iVideoPixelAspectRatioDenom,
	                                  			   				iAudioCodec,
	                                  			   				0 );	// aCameraDisplayID, 0 = ignore and use default.
	    PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() ICM error: %d"), error));	                                  			   				
	                                  			
		// Test if we get valid data   
		if ( error == KErrNone)
			{
			iActiveVideoQualitySet = newQualitySet;
			iICMDataAvailable = ETrue;
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() Video quality set reset, new settings:")));			
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iVideoQualitySetLevel: %d "), iActiveVideoQualitySet.iVideoQualitySetLevel ));
#ifdef _DEBUG
			RDebug::Printf("CCMRConfigManager::ResetVideoQualitySet() iVideoFileMimeType: %s ", iActiveVideoQualitySet.iVideoFileMimeType );
			RDebug::Printf("CCMRConfigManager::ResetVideoQualitySet() iVideoCodecMimeType: %s ", iActiveVideoQualitySet.iVideoCodecMimeType );
#endif
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iVideoWidth: %d, iVideoHeight: %d "), iActiveVideoQualitySet.iVideoWidth, iActiveVideoQualitySet.iVideoHeight ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iVideoFrameRate: %f "), iActiveVideoQualitySet.iVideoFrameRate ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iVideoBitRate: %d "), iActiveVideoQualitySet.iVideoBitRate ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iVideoEncoderUID: %d "), iActiveVideoQualitySet.iVideoEncoderUID.iUid ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iRandomAccessRate: %f "), iActiveVideoQualitySet.iRandomAccessRate ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iVideoPixelAspectRatio Num: %d, Denom: %d "), iActiveVideoQualitySet.iVideoPixelAspectRatioNum, iActiveVideoQualitySet.iVideoPixelAspectRatioDenom ));
#ifdef _DEBUG			
			RDebug::Printf("CCMRConfigManager::ResetVideoQualitySet() iPreferredSupplier: %s ", iActiveVideoQualitySet.iPreferredSupplier);
#endif			
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAudioFourCCType: %d "), iActiveVideoQualitySet.iAudioFourCCType.FourCC() ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAudioBitRate: %d "), iActiveVideoQualitySet.iAudioBitRate ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAudioSamplingRate: %d "), iActiveVideoQualitySet.iAudioSamplingRate ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAudioChannels: %d "), iActiveVideoQualitySet.iAudioChannels ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAudioEncoderUID: %d "), iActiveVideoQualitySet.iAudioEncoderUID.iUid ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAVSyncStartDelay: %d "), iActiveVideoQualitySet.iAVSyncStartDelay ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iAVSyncResumeDelay: %d "), iActiveVideoQualitySet.iAVSyncResumeDelay ));
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iCameraDisplayId: %d "), iActiveVideoQualitySet.iCameraDisplayId ));																														
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() iCamcorderVisible: %d "), iActiveVideoQualitySet.iCamcorderVisible ));				
			}
		else
			{
			PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() matching video quality set not found from ICM or ICM error")));
			iICMDataAvailable = EFalse;
			}
	    }
	else
		{
		PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() ICM not available")));
		iICMDataAvailable = EFalse;
		}	 
	PRINT((_L("CCMRConfigManager::ResetVideoQualitySet() out")));							   
	}
