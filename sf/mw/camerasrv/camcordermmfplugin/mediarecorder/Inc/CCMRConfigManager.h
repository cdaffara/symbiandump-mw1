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


#ifndef CCMRCONFIGMANAGER_H
#define CCMRCONFIGMANAGER_H

#include <imagingconfigmanager.h>
#include <mmf/common/mmffourcc.h>

/**
 *  CamcorderMMFPlugin config manager.
 *  Manager for config/setup parameters used in Media Recorder.
 *  Wraps Imaging Config Manager for CamcorderMMFPlugin usage.
 *
 *  @lib CAMCMEDIARECORDER.lib
 *  @since S60 v3.2
 */
class CCMRConfigManager : public CBase
    {
public:

    /**
     * Two-phased constructor.
     */
    static CCMRConfigManager* NewL();
    
    /**
     * Two-phased constructor.
     */
    static CCMRConfigManager* NewLC();    
    
    /**
    * Destructor.
    */
    ~CCMRConfigManager();
    
    /**
     * Set video codec mime type.
     *
     * @since S60 v3.2
     * @param aVideoCodec Video codec.         
     */    
	void SetVideoCodec(TDesC8& aVideoCodec);
    
    /**
     * Set audio codec mime type (TFourCC).
     *
     * @since S60 v3.2
     * @param aAudioCodec Audio codec.     
     */    
    void SetAudioCodec(TFourCC& aAudioCodec);

    /**
     * Set video frame size.
     *
     * @since S60 v3.2
     * @param aFrameSize Video frame size.    
     */    
    void SetVideoFrameSize(TSize aVideoFrameSize);

    /**
     * Set video pixel aspect ratio.
     *
     * @since S60 v3.2
     * @param aNumerator Numerator.    
     * @param aDenominator Denominator.         
     */    
    void SetVideoPixelAspectRatio(TUint aNumerator, TUint aDenominator);

    /**
     * Query whether Imaging Configuration data is available
     *
     * @since S60 v3.2
     * @return EFalse/ETrue whether ICM data is available.
     */    
    TBool IsICMConfigDataAvailable() const;

    /**
     * Get CamcorderMMFPlugin specific settings.
     *
     * @since S60 v3.2
     * @return CamcorderMMFPlugin specific settings.
     */
	const TCamcorderMMFPluginSettings PluginSettings() const;

    /**
     * Get Video Quality settings.
     *
     * @since S60 v3.2
     * @return Video quality settings.
     */
	const TVideoQualitySet VideoQualitySettings() const;	
	

private:

    CCMRConfigManager();

    void ConstructL();
    
    /**
     * Resets config manager to new Video Quality (TVideoQualitySet iActiveVideoQualitySet) 
     * setting using Imaging Config Manager (ICM)
     *
     * @since S60 v3.2
     */
	void ResetVideoQualitySet();

private: // data

    /**
     * Set video frame size.
     */
	TSize iVideoFrameSize;
     
     /**
     * Set video codec mime type.
     */
    TBuf8<256> iVideoCodec;

    /**
     * Set audio codec type (TFourCC)
     */     
    TFourCC iAudioCodec;
     
     /**
     * Set video pixel aspect ration numerator.
     */
    TUint iVideoPixelAspectRatioNum;
     	
    /**
     * Set video pixel aspect ration denomirator.
     */
    TUint iVideoPixelAspectRatioDenom;
     
    /**
     * Current active Video Quality settings set.
     */
	TBool iICMDataAvailable;

    /**
     * Current active Video Quality settings set.
     */
    TVideoQualitySet iActiveVideoQualitySet;

    /**
     * Current active CamcorderMMFPlugin settings set.
     */
    TCamcorderMMFPluginSettings iPluginSettings;
	
    /**
     * Imaging Config Manager
     * Own.
     */
	CImagingConfigManager* iImagingConfigManager;
    };


#endif // CCMRCONFIGMANAGER_H
