/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Configuration manager using Central Repository for retrieving
*                settings for clients such as CamcorderMMFPlugin
*
*/


#ifndef IMAGINGCONFIGMANAGER_H
#define IMAGINGCONFIGMANAGER_H

#include <mmf/common/mmfutilities.h>

const TUint KMaxStringLength = 256; //Used for all text fields in setting sets
const TUint KBasicQualityGranularity = 100; // the "multiplier" used for
                                              // getting each level. e.g.
                                              // level 1=100, level 2=200, ...
const TUint KNumberOfNominalLevels = 3; // How many basic levels of quality
                                        // there is.

/*
 * Video quality set structure
 * 
 * @since S60 v3.2
 */
class TVideoQualitySet
    {
    public:
        // Enum level for the Quality set, used as ID
        TUint iVideoQualitySetLevel;
        // Video file format mime type, e.g. "video/3gpp"
        TText8 iVideoFileMimeType[ KMaxStringLength ];
        // Video codec mime type, e.g. "video/mp4v-es"
        TText8 iVideoCodecMimeType[ KMaxStringLength ];
        // Video picture width in pixels (luminance), resolution width
        TInt iVideoWidth;
        // Video picture height in pixels (luminance), resolution height
        TInt iVideoHeight;
        // Video framerate in fps
        TReal iVideoFrameRate;
        // Video bitrate in bps
        TInt iVideoBitRate;
        // Video encoder UID
        TUid iVideoEncoderUID;
        // Random access point rate, in pictures per second. For example, to
        // request a random access point every ten seconds, set the value to
        // 0.1. Random access means in video case usually that an INTRA frame
        // is forced by the encoder to make the video accessible at that time
        // without depending on the previous frames
        TReal iRandomAccessRate;
        // Video's pixel aspect ratio numerator, e.g. in PAL 16:11 pixel
        // aspect ratio means that this value is set to 16
        TUint iVideoPixelAspectRatioNum; 
        // Video's pixel aspect ratio denominator, e.g. in PAL 16:11 pixel
        // aspect ratio means that this value is set to 11
        TUint iVideoPixelAspectRatioDenom;
        // Preferred supplier for the MMF camcorder plug-in (e.g. "Nokia")
        TText8 iPreferredSupplier[ KMaxStringLength ];

        // Audio codec FourCC, e.g. " AMR"
        TFourCC iAudioFourCCType;
        // Audio bitrate in bps
        TInt iAudioBitRate;
        // Audio sampling rate in Hz
        TInt iAudioSamplingRate;
        // Number of audio channels; in practice mono(1) vs stereo(2)
        TInt iAudioChannels;
        // Audio encoder UID
        TUid iAudioEncoderUID;
        // Video recording A/V sync start delay (ms), only used by
        // CamcorderMMFPlugIn, but depends on the qualityset
        TInt iAVSyncStartDelay;
        // Video recording A/V sync resume delay (ms), only used by
        // CamcorderMMFPlugIn, but depends on the qualityset
        TInt iAVSyncResumeDelay;
        // A non-zero ID telling unique combination of camera (primary/
        // secondary) and/or display (cover UI/main display) and/or rotation
        // setting combined into a number for separating each setting set
        // type from others
        TUint iCameraDisplayId;
        // Camcorder specific field that can be either 1(true) or 0(false)
        // meaning if the current quality settings set should be shown on
        // Camcorder UI or not
        TUint iCamcorderVisible;
	};

/*
 * Camcorder MMF Plug-in Specific settings structure
 * 
 * @since S60 v3.2
 */
class TCamcorderMMFPluginSettings
	{
	public:
		// Video framerate in nightmode (fps)
		TReal iVideoNightFrameRate;
		// Video bitrate scaler for remaining time calculation during
		// first 3seconds.
		TReal iCMRAvgVideoBitRateScaler;
		// Video Complexity Setting
		TInt iVideoComplexitySetting;
		// Quality is more important in camcorder than delay
		TReal iCMRLatencyQualityTradeoff;
		// range is [0...100]
		TUint iCMRPictureQuality;
		// Try to maintain the frame-rate,
		// 1.0 means the picture quality is sacrificed 
		// "all the way" to match the target fps
		TReal iCMRQualityTemporalTradeoff;
		// number of camera buffers to use when using HW accelerated encoder
		TUint iCMRNumCameraBuffers;
		// number of camera buffers to use when using SW encoder
		TUint iCMRNumCameraBuffersARM;		
		// min number of output (bitstream) buffers
		TUint iCMRMinNumOutputBuffers;
	};


/*
 * Image quality set structure
 * 
 * @since S60 v3.2
 */
class TImageQualitySet
    {
    public:
        // Enum level for the Quality set, used as ID
        TUint iImageQualitySetLevel;
        // Image file format mime type, e.g. "image/jpeg"
        TText8 iImageFileMimeType[ KMaxStringLength ];
        // Image picture width in pixels, resolution width
        TInt iImageWidth;
        // Image picture height in pixels, resolution height
        TInt iImageHeight;
        // Image extension, e.g. ".jpg"
        TText8 iImageFileExtension[ KMaxStringLength ];
        // Image encoder UID
        TUid iImageEncoderUID;
        // Compression quality, determines wether file size or image quality
        // is preferred over the other
        TInt iCompressionQuality;
        // Estimated image size, in bytes
        TInt iEstimatedSize;
        // A non-Zero ID telling unique combination of camera (primary/
        // secondary) and/or display (cover UI/main display) and/or rotation
        // setting combined into a number for separating each setting set type
        // from others
        TUint iCameraDisplayId;
        // Camcorder specific field that can be either 1(true) or 0(false)
        // meaning if the current quality settings set should be shown on
        // Camcorder UI or not
        TUint iCamcorderVisible;
    };

/**
 *  API class for getting imaging configurations and settings
 *
 *  This class can be used to query supported Video and Image quality settings
 *  (defined above) and CamcorderMMFPlugin settings.
 *
 *  @lib imagingconfigmanager.lib
 *  @since S60 v3.2
 */
class CImagingConfigManager : public CBase
    {
    public:
        /*
         * Enumeration for nominal quality levels. Client must use these
         * enumerations to get quality sets. However, if
         * NumberOfQualityLevels() below indicates there are more than
         * KNumberOfNominalLevels, client can use values in-between the
         * nominal values to get set for some other quality level. It is
         * however up to the implementation of the API to map such
         * intermediate values to actual levels.
         */
        enum TQualityLevels
            {
            // client can try asking values in between these but the class may
            // round the values depending on the availability
            EQualityMin = 0,      // EQualityLow might not be the lowest.
                                  // Level cannot go below this value
            EQualityLow = 100,    // use this e.g. when MMS compatible
                                  // settings are needed for video
            EQualityNormal = 200, // use this when normal quality settings
                                  // are needed, i.e. typically higher quality
                                  // than MMS in Video, but possibly not the
                                  // highest still
            EQualityHigh = 300    // use this when highest possible quality
                                  // settings are needed
            };

        /**
         * Create new CImagingConfigManager object
         *
         * Possible leave codes: KErrGeneral & KErrNoMemory + possibly other
         *                       CenRep Error codes
         *
         * @since S60 v3.2
         * @return Allocated and initialized CImagingConfigManager object
         */
        IMPORT_C static CImagingConfigManager* NewL();

        /**
         * Create new CImagingConfigManager object
         *
         * Possible leave codes: KErrGeneral & KErrNoMemory + possibly other
         *                       CenRep Error codes
         *
         * @since S60 v3.2
         * @return Allocated and initialized CImagingConfigManager object
         */
        IMPORT_C static CImagingConfigManager* NewLC();

        
        /**
         * Destructor
         *
         * @since S60 v3.2
         */
        ~CImagingConfigManager();

        /**
         * Get number of defined video quality levels. This is always at least
         * KNumberOfNominalLevels but can be higher
         *
         * @since S60 v3.2
         * @return Number of video quality levels
         */
        IMPORT_C TInt NumberOfVideoQualityLevels() const;

        /**
         * Get an array of quality levels that are in use with the current
         * product (for the given CameraDisplayId or for all levels)
         *
         * Possible leave codes: KErrGeneral & KErrNoMemory
         *
         * @since S60 v3.2
         * @param aLevels Returns an array of video quality levels
         * @param aCameraDisplayID Unique ID for some camera-display
         *        combination. If 0, then all levels are returned.
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetVideoQualityLevelsL( CArrayFixFlat<TUint>& aLevels,
                                              TUint aCameraDisplayID = 0 );

        /**
         * Get quality set associated with the given level. The level should
         * be between EQualityHigh and EQualityMin, but these are just for
         * guidance in general. These are specified in their own documentation
         * more specifically. If there is no set associated with given
         * intermediate level, then set from a nearest level is returned.
         *
         * @since S60 v3.2
         * @param aSet Returned video quality set
         * @param aLevel Level for which a quality set if requested
         * @param aCameraDisplayID Unique ID for some camera-display
         *        combination. If 0, then ignored and the smallest ID is
         *        returned with given aLevel
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetVideoQualitySet( TVideoQualitySet& aSet,
                                          TInt aLevel,
                                          TUint aCameraDisplayID = 0 );

        /**
         * Get quality set associated with the given video resolution, its
         * MIME-type and audio fourCC. Both of these are needed to be uniquely
         * able to select a single quality setting set. If certain pixel
         * aspect ratio is wanted for the video, it can be requested with
         * aVideoPixelAspectRatioNum and aVideoPixelAspectRatioDenom
         * parameters. Otherwise they should be set to zero.
         *
         * @since S60 v3.2
         * @param aSet Returned video quality set
         * @param aVideoResolution Size of the video resolution that is
         *        requested
         * @param aVideoCodecMimeType MIME type of the video codec
         * @param aVideoPixelAspectRatioNum Numerator of video's pixel aspect
         *        ratio
         * @param aVideoPixelAspectRatioDenom Denominator of video's pixel
         *        aspect ratio
         * @param aAudioFourCCType Four Character Code of the audio type
         * @param aCameraDisplayID Unique ID for some camera-display
         *        combination. If 0, then ignored and the smallest ID is
         *        returned for the given parameters
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetVideoQualitySet( TVideoQualitySet& aSet,
                                  const TSize& aVideoResolution,
                                  const TDesC8& aVideoCodecMimeType,
                                  TUint aVideoPixelAspectRatioNum,
                                  TUint aVideoPixelAspectRatioDenom,
                                  TFourCC aAudioFourCCType,
                                  TUint aCameraDisplayID = 0 );

        /**
         * Get all settings for Camcorder
         *
         * @since S60 v3.2
         * @param aSet Returned setting set for CamcorderMMFPlugin
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetCamcorderMMFPluginSettings(
                                TCamcorderMMFPluginSettings& aSet) const;

        /**
         * Get number of defined image quality levels. This is always at least
         * KNumberOfNominalLevels but can be higher
         *
         * @since S60 v3.2
         * @return Number of image quality levels
         */
        IMPORT_C TInt NumberOfImageQualityLevels() const;

        /**
         * Get an array of quality levels that are in use with the current
         * product (for the given CameraDisplayId or for all levels)
         *
         * Possible leave codes: KErrGeneral & KErrNoMemory
         *
         * @since S60 v3.2
         * @param aLevels Returns an array of image quality levels
         * @param aCameraDisplayID Unique ID for some camera-display
         *        combination. If 0, then all levels are returned.
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetImageQualityLevelsL( CArrayFixFlat<TUint>& aLevels,
                                              TUint aCameraDisplayID = 0 );

        /**
         * Get image quality set associated with the given level. The level
         * should be between EQualityHigh and EQualityMin. One the defined
         * nominal levels should be used especially if NumberOfQualityLevels()
         * == KNumberOfNominalLevels. If there is no set associated with given
         * intermediate level, then set from a nearest level is returned.
         *
         * @since S60 v3.2
         * @param aSet Returned image quality set
         * @param aLevel Level for which a quality set if requested
         * @param aCameraDisplayID Unique ID for some camera-display
         *        combination. If 0, then ignored and the smallest ID is
         *        returned with given aLevel
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetImageQualitySet( TImageQualitySet& aSet,
                                          TInt aLevel,
                                          TUint aCameraDisplayID = 0 );

        /**
         * Get quality set associated with the given image resolution. Leaves
         * if there is no defined set for the resolution.
         *
         * @since S60 v3.2
         * @param aSet Returned image quality set
         * @param aImageResolution Size of the image resolution that is
         *        requested
         * @param aCameraDisplayID Unique ID for some camera-display
         *        combination. If 0, then ignored and the smallest ID is
         *        returned with given aLevel
         * @return KErrNone if no error, otherwise error message ID
         */
        IMPORT_C TInt GetImageQualitySet( TImageQualitySet& aSet,
                                          const TSize& aImageResolution,
                                          TUint aCameraDisplayID = 0 );
        
    private:
        /**
         * Default constructor, doing nothing
         */
        CImagingConfigManager();

        /**
         * Actual constructor where the reading from the Central Repository
         * happens
         */
        void ConstructL();
        
        /**
         * Helper function to check if the given value is negative
         */
        TBool CheckIfNegative( const TInt aValue ) const;
        
        /**
         * Helper function to get the right part of a string starting after
         * the given key word
         */
        TPtrC GetRightPartL( const TPtr& aPtr, const TDesC& aSearchable ) const;
        
        /**
         * Helper function to get the left part of a string before the colon
         */
        TPtrC GetLeftPart( const TPtrC& aPtrc ) const;
        
        /**
         * Helper function to copy TPtrC to TText8*
         */
        void SetTextStringL( const TPtrC& aPtr, TText8* aStringToFill ) const;
        
        /**
         * Method for sorting level numbers in ascending order
         */
        TInt SortLevels(CArrayFixFlat<TUint>& aLevels);

    private: // data
        /**
         * Video quality setting sets are stored internally to this pointer
         * Own
         */
        CArrayFixFlat< TVideoQualitySet >* iVideoQualitySets;
        
        /**
         * Camcorder MMF Plug-in's settings are stored here
         * Own
         */
        TCamcorderMMFPluginSettings iCamcorderMMFPluginSettings;
        
        /**
         * Image quality setting sets are stored internally to this pointer
         * Own
         */
        CArrayFixFlat< TImageQualitySet >* iImageQualitySets;
    };

#endif //IMAGINGCONFIGMANAGER_H