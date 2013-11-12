/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Configuration manager using Central Repositary for retrieving
*                settings for clients such as CamcorderMMFPlugin
*
*/


#include <centralrepository.h>
#include "imagingconfigmanager.h"
#include "icmcenrepkeys.h"

/*
* These constant literals are used for parsing the Central Repository strings
*
*/
_LIT( KPeriod, "." );

_LIT( KComma, "," );
_LIT( KQualitySetLevel, "QualitySetLevel" );
_LIT( KVideoFileMimeType, "VideoFileMimeType" );
_LIT( KVideoCodecMimeType, "VideoCodecMimeType" );
_LIT( KVideoWidth, "VideoWidth" );
_LIT( KVideoHeight, "VideoHeight" );
_LIT( KVideoFrameRate, "VideoFrameRate" );
_LIT( KVideoBitRate, "VideoBitRate" );
_LIT( KVideoEncoderUID, "VideoEncoderUID=0" ); // "=0" there is a way to
                                             // ignore the "0x" from the
                                             // beginning of hex-number
_LIT( KRandomAccessRate, "RandomAccessRate" );
_LIT( KVideoPixelAspectRatioNum, "VideoPixelAspectRatioNum" );
_LIT( KVideoPixelAspectRatioDenom, "VideoPixelAspectRatioDenom" );
_LIT( KPreferredSupplier, "PreferredSupplier" );
_LIT( KAudioFourCCType, "AudioFourCCType" );
_LIT( KAudioBitrate, "AudioBitrate" );
_LIT( KAudioSamplingRate, "AudioSamplingRate" );
_LIT( KAudioChannels, "AudioChannels" );
_LIT( KAudioEncoderUID, "AudioEncoderUID=0" ); // "=0" there is a way to
                                             // ignore the "0x" from the
                                             // beginning of hex-number
_LIT( KAVSyncStartDelay, "AVSyncStartDelay" );
_LIT( KAVSyncResumeDelay, "AVSyncResumeDelay" );
_LIT( KCameraDisplayId, "CameraDisplayId" );
_LIT( KCamcorderVisible, "CamcorderVisible" );
_LIT( KVideoNightFrameRate, "VideoNightFrameRate" );
_LIT( KCMRAvgVideoBitRateScaler, "CMRAvgVideoBitRateScaler" );
_LIT( KVideoComplexitySetting, "VideoComplexitySetting" );
_LIT( KCMRLatencyQualityTradeoff, "CMRLatencyQualityTradeoff" );
_LIT( KCMRPictureQuality, "CMRPictureQuality" );
_LIT( KCMRQualityTemporalTradeoff, "CMRQualityTemporalTradeoff" );
_LIT( KCMRNumCameraBuffers, "CMRNumCameraBuffers" );
_LIT( KCMRNumCameraBuffersARM, "CMRNumCameraBuffersARM" );
_LIT( KCMRMinNumOutputBuffers, "CMRMinNumOutputBuffers" );
_LIT( KImageFileMimeType, "ImageFileMimeType" );
_LIT( KImageWidth, "ImageWidth" );
_LIT( KImageHeight, "ImageHeight" );
_LIT( KImageFileExtension, "ImageFileExtension" );
_LIT( KImageEncoderUID, "ImageEncoderUID=0" ); // "=0" there is a way to
                                             // ignore the "0x" from the
                                             // beginning of hex-number
_LIT( KCompressionQuality, "CompressionQuality" );
_LIT( KEstimatedSize, "EstimatedSize" );

// Constants used in Central Repository parsing and granularity
const TInt KGranularity = 1;
const TUint32 KRowMask = 0xFFFF0000;
const TUint32 KRowVideo = 1 << 16;  // VideoQualitySets' offset
const TUint32 KRowCCor = 2 << 16;   // CamcorderMMFPluginSettings' offset
const TUint32 KRowImage = 3 << 16;  // ImageQualitySets' offset
const TText8 KZero = '\0';          // null-termination mark of strings
const TInt KInitBufSize = 1;        // Initial size of reserved buffer
const TInt KUndefined = -1;         // used as initial undefined value

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Constructor of CImagingConfigManager
// ---------------------------------------------------------------------------
//
CImagingConfigManager::CImagingConfigManager()
    {
    }

// ---------------------------------------------------------------------------
// Destructor of CImagingConfigManager
// ---------------------------------------------------------------------------
//
CImagingConfigManager::~CImagingConfigManager()
    {
    if( iVideoQualitySets )
        {
        iVideoQualitySets->Reset();
        delete iVideoQualitySets;
        }
    if( iImageQualitySets )
        {
        iImageQualitySets->Reset();
        delete iImageQualitySets;
        }
    }

// ---------------------------------------------------------------------------
// ConstructL() of CImagingConfigManager
// ---------------------------------------------------------------------------
//
void CImagingConfigManager::ConstructL()
    {
    RArray< TUint32 > foundCols; // This is used for fetching the columns of
                                 // data from Central Repository
    TInt buffersize = KInitBufSize; // buffer variable's size
    HBufC* buffer = NULL;        // Common buffer to be used for temporary
                                 // storage for reads from Central Repository
    TInt keyCount;               // Number of keys found from CenRep
    TUint uidtemp = 0;           // Temp TUint storage for UIDs

    // Create the repository
    CRepository *repository = CRepository::NewLC( KCRUidImagingConfigManager );

    // Alloc quality sets
    iVideoQualitySets =
            new ( ELeave ) CArrayFixFlat< TVideoQualitySet >( KGranularity );
    iImageQualitySets =
            new ( ELeave ) CArrayFixFlat< TImageQualitySet >( KGranularity );
    
    // Find video quality setting sets by using the KRowMask
    User::LeaveIfError( repository->FindL( KRowVideo, KRowMask, foundCols ) );

    buffer = HBufC::NewLC( buffersize );
    keyCount = foundCols.Count();
    
    // For each key, parse the settings into the TVideoQualitySet
    for ( TInt i = 0 ; i < keyCount ; i++ )
        {
        TVideoQualitySet tmpVideoQualitySet;
        TPtr ptr = buffer->Des();
        buffersize = 0;
        // String with settings is fetched from CenRep
        TInt ret = repository->Get( foundCols[ i ], ptr, buffersize );
        // KErrOverFlow is returned if and when 1 is not big enough size
        // for buffer
        if ( ret == KErrOverflow )
            {
            // More space is reserved
            CleanupStack::PopAndDestroy(); // buffer
            buffer = HBufC::NewLC( buffersize );
            ptr.Set( buffer->Des() );
            User::LeaveIfError( repository->Get( foundCols[ i ], ptr ) );
            }
        else
            {
            User::LeaveIfError( ret );
            }
        // Fetched string is parsed right and left and the value is stored
        // to suitable form into the TVideoQualitySet
        TPtrC temp = GetRightPartL( ptr, KQualitySetLevel() );
        TLex lextemp = GetLeftPart( temp );
        TBuf<1024> treal;
        User::LeaveIfError( lextemp.Val(
                    tmpVideoQualitySet.iVideoQualitySetLevel, EDecimal ) );
        
        temp.Set( GetRightPartL( ptr, KVideoFileMimeType() ) );
        temp.Set( GetLeftPart( temp ) );
        SetTextStringL( temp, tmpVideoQualitySet.iVideoFileMimeType );
        
        temp.Set( GetRightPartL( ptr, KVideoCodecMimeType() ) );
        temp.Set( GetLeftPart( temp ) );
        SetTextStringL( temp, tmpVideoQualitySet.iVideoCodecMimeType );
        
        temp.Set( GetRightPartL( ptr, KVideoWidth() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iVideoWidth ) );
        
        temp.Set( GetRightPartL( ptr, KVideoHeight() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iVideoHeight ) );
        
        temp.Set( GetRightPartL( ptr, KVideoFrameRate() ) );
        temp.Set( GetLeftPart( temp ) );
        treal = temp;
        TLocale tlocale;
        TChar ch = tlocale.DecimalSeparator();
        if( temp.Find( KPeriod ) >= 0 )
            {
            treal[temp.Find( KPeriod )] = ch;
            }
        lextemp = treal;
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iVideoFrameRate ) );
        
        temp.Set( GetRightPartL( ptr, KVideoBitRate() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iVideoBitRate ) );
        
        temp.Set( GetRightPartL( ptr, KVideoEncoderUID() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( uidtemp, EHex ) );
        tmpVideoQualitySet.iVideoEncoderUID.iUid = uidtemp;
        
        temp.Set( GetRightPartL( ptr, KRandomAccessRate() ) );
        temp.Set( GetLeftPart( temp ) );
        treal = temp;
        if( temp.Find( KPeriod ) >= 0 )
            {
            treal[temp.Find( KPeriod )] = ch;
            }
        lextemp = treal;
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iRandomAccessRate ) );
        
        temp.Set( GetRightPartL( ptr, KVideoPixelAspectRatioNum() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val(
                    tmpVideoQualitySet.iVideoPixelAspectRatioNum ) );
        
        temp.Set( GetRightPartL( ptr, KVideoPixelAspectRatioDenom() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val(
                    tmpVideoQualitySet.iVideoPixelAspectRatioDenom ) );
        
        temp.Set( GetRightPartL( ptr, KPreferredSupplier() ) );
        temp.Set( GetLeftPart( temp ) );
        SetTextStringL( temp, tmpVideoQualitySet.iPreferredSupplier );
        
        temp.Set( GetRightPartL( ptr, KAudioFourCCType() ) );
        temp.Set( GetLeftPart( temp ) );
        TBuf8<4> fourccbuf8;
        fourccbuf8.Copy(temp);
        tmpVideoQualitySet.iAudioFourCCType.Set(fourccbuf8);
        
        TUint32 intti = tmpVideoQualitySet.iAudioFourCCType.FourCC();
        
        temp.Set( GetRightPartL( ptr, KAudioBitrate() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iAudioBitRate ) );
        
        temp.Set( GetRightPartL( ptr, KAudioSamplingRate() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val(
                    tmpVideoQualitySet.iAudioSamplingRate ) );
        
        temp.Set( GetRightPartL( ptr, KAudioChannels() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iAudioChannels ) );
        
        temp.Set( GetRightPartL( ptr, KAudioEncoderUID() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( uidtemp, EHex ) );
        tmpVideoQualitySet.iAudioEncoderUID.iUid = uidtemp;
        
        temp.Set( GetRightPartL( ptr, KAVSyncStartDelay() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iAVSyncStartDelay ) );
        
        temp.Set( GetRightPartL( ptr, KAVSyncResumeDelay() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val(
                    tmpVideoQualitySet.iAVSyncResumeDelay ) );
        
        temp.Set( GetRightPartL( ptr, KCameraDisplayId() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iCameraDisplayId ) );
        
        temp.Set( GetRightPartL( ptr, KCamcorderVisible() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpVideoQualitySet.iCamcorderVisible ) );
        
        iVideoQualitySets->AppendL( tmpVideoQualitySet );
        }

    for( TInt i = foundCols.Count() - 1 ; i >= 0 ; i-- )
        {
        foundCols.Remove( i );
        }
    User::LeaveIfError( repository->FindL( KRowCCor, KRowMask, foundCols ) );
    keyCount = foundCols.Count();
    for ( TInt i = 0 ; i < keyCount ; i++ )
        {
        TPtr ptr = buffer->Des();
        buffersize = 0;
        TInt ret = repository->Get( foundCols[ i ], ptr, buffersize );
        if ( ret == KErrOverflow )
            {
            CleanupStack::PopAndDestroy(); // buffer
            buffer = HBufC::NewLC( buffersize );
            ptr.Set( buffer->Des() );
            User::LeaveIfError( repository->Get( foundCols[ i ], ptr ) );
            }
        else if( ret != KErrNone )
            {
            User::LeaveIfError( ret );
            }
        else
            {
            }
        
        TBuf<1024> treal;    
        TPtrC temp = GetRightPartL( ptr, KVideoNightFrameRate() );
        temp.Set( GetLeftPart( temp ) );
        treal = temp;
        TLocale tlocale;
        TChar ch = tlocale.DecimalSeparator();
        if( temp.Find( KPeriod ) >= 0 )
            {
            treal[temp.Find( KPeriod )] = ch;
            }
        TLex lextemp(treal);
        User::LeaveIfError( lextemp.Val(
                    iCamcorderMMFPluginSettings.iVideoNightFrameRate ) );
        
        temp.Set( GetRightPartL( ptr, KCMRAvgVideoBitRateScaler() ) );
        temp.Set( GetLeftPart( temp ) );
        treal = temp;
        if( temp.Find( KPeriod ) >= 0 )
            {
            treal[temp.Find( KPeriod )] = ch;
            }
        lextemp = treal;
        User::LeaveIfError( lextemp.Val(
                    iCamcorderMMFPluginSettings.iCMRAvgVideoBitRateScaler ) );
        
        temp.Set( GetRightPartL(ptr, KVideoComplexitySetting() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val(
                    iCamcorderMMFPluginSettings.iVideoComplexitySetting ) );
        
        temp.Set( GetRightPartL( ptr, KCMRLatencyQualityTradeoff() ) );
        temp.Set( GetLeftPart( temp ) );
        treal = temp;
        if( temp.Find( KPeriod ) >= 0 )
            {
            treal[temp.Find( KPeriod )] = ch;
            }
        lextemp = treal;
        User::LeaveIfError( lextemp.Val(
                    iCamcorderMMFPluginSettings.iCMRLatencyQualityTradeoff ) );
        
        temp.Set( GetRightPartL( ptr, KCMRPictureQuality() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val(
                    iCamcorderMMFPluginSettings.iCMRPictureQuality ) );
        
        temp.Set( GetRightPartL( ptr, KCMRQualityTemporalTradeoff() ) );
        temp.Set( GetLeftPart( temp ) );
        treal = temp;
        if( temp.Find( KPeriod ) >= 0 )
            {
            treal[temp.Find( KPeriod )] = ch;
            }
        lextemp = treal;
        User::LeaveIfError( lextemp.Val(
                    iCamcorderMMFPluginSettings.iCMRQualityTemporalTradeoff ) );
        
        if(ptr.Find(KCMRNumCameraBuffers()) >= 0)
            {
            temp.Set( GetRightPartL( ptr, KCMRNumCameraBuffers() ) );
            lextemp = GetLeftPart( temp );
            User::LeaveIfError( lextemp.Val(
                        iCamcorderMMFPluginSettings.iCMRNumCameraBuffers ) );
            
            temp.Set( GetRightPartL( ptr, KCMRNumCameraBuffersARM() ) );
            lextemp = GetLeftPart( temp );
            User::LeaveIfError( lextemp.Val(
                        iCamcorderMMFPluginSettings.iCMRNumCameraBuffersARM ) );
            
            temp.Set( GetRightPartL( ptr, KCMRMinNumOutputBuffers() ) );
            lextemp = GetLeftPart( temp );
            User::LeaveIfError( lextemp.Val(
                        iCamcorderMMFPluginSettings.iCMRMinNumOutputBuffers ) );
            }
        else
            {
            // To ensure backwards compatibility we give these three new
            // settings default values of 2, 1 and 4 respectively according
            // to old implementation in MMFPlug-in
            iCamcorderMMFPluginSettings.iCMRNumCameraBuffers = 2;
            iCamcorderMMFPluginSettings.iCMRNumCameraBuffersARM = 1;
            iCamcorderMMFPluginSettings.iCMRMinNumOutputBuffers = 4;
            }
        }

    for( TInt i = foundCols.Count() - 1 ; i >= 0 ; i-- )
        {
        foundCols.Remove( i );
        }
    User::LeaveIfError( repository->FindL( KRowImage, KRowMask, foundCols ) );
    keyCount = foundCols.Count();
    for (TInt i = 0 ; i < keyCount ; i++)
        {
        TPtr ptr = buffer->Des();
        buffersize = 0;
        TInt ret = repository->Get( foundCols[ i ], ptr, buffersize );
        if ( ret == KErrOverflow )
            {
            CleanupStack::PopAndDestroy(); // buffer
            buffer = HBufC::NewLC( buffersize );
            ptr.Set( buffer->Des() );
            User::LeaveIfError( repository->Get( foundCols[ i ], ptr ) );
            }
        else if( ret != KErrNone )
            {
            User::LeaveIfError( ret );
            }
        else
            {
            }

        TImageQualitySet tmpImageQualitySet;

        TPtrC temp = GetRightPartL( ptr, KQualitySetLevel() );
        TLex lextemp( GetLeftPart( temp ) );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iImageQualitySetLevel,
                                       EDecimal ) );

        temp.Set( GetRightPartL( ptr, KImageFileMimeType() ) );
        temp.Set( GetLeftPart( temp ) );
        SetTextStringL( temp, tmpImageQualitySet.iImageFileMimeType );

        temp.Set( GetRightPartL( ptr, KImageWidth() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iImageWidth ) );

        temp.Set( GetRightPartL( ptr, KImageHeight() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iImageHeight ) );

        temp.Set( GetRightPartL( ptr, KImageFileExtension() ) );
        temp.Set( GetLeftPart( temp ) );
        SetTextStringL( temp, tmpImageQualitySet.iImageFileExtension );

        temp.Set( GetRightPartL( ptr, KImageEncoderUID() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( uidtemp, EHex ) );
        tmpImageQualitySet.iImageEncoderUID.iUid = uidtemp;
        
        temp.Set( GetRightPartL( ptr, KCompressionQuality() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iCompressionQuality ) );

        temp.Set( GetRightPartL( ptr, KEstimatedSize() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iEstimatedSize ) );
        
        temp.Set( GetRightPartL( ptr, KCameraDisplayId() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iCameraDisplayId ) );
        
        temp.Set( GetRightPartL( ptr, KCamcorderVisible() ) );
        lextemp = GetLeftPart( temp );
        User::LeaveIfError( lextemp.Val( tmpImageQualitySet.iCamcorderVisible ) );
        
        iImageQualitySets->AppendL( tmpImageQualitySet );
        }

    foundCols.Close();
    CleanupStack::PopAndDestroy(); // buffer
    CleanupStack::PopAndDestroy( repository );
    }

// ---------------------------------------------------------------------------
// CheckIfNegative( TInt aValue ) of CImagingConfigManager, private method
// ---------------------------------------------------------------------------
//
TBool CImagingConfigManager::CheckIfNegative( const TInt aValue ) const
    {
    TBool retval = EFalse;
    if( aValue < 0 )
        {
        retval = ETrue;
        }
    return retval;
    }

// ---------------------------------------------------------------------------
// GetRightPartL( TPtr aPtr, const TDesC& aSearchable ) of 
// CImagingConfigManager, private method
// ---------------------------------------------------------------------------
//
TPtrC CImagingConfigManager::GetRightPartL( const TPtr& aPtr,
                                            const TDesC& aSearchable ) const
    {
    // First verify the values so that we know if they aren't legal
    if( CheckIfNegative( aPtr.Find( aSearchable ) ) ||
        CheckIfNegative( ( ( aPtr.Length() -
                             aPtr.Find( aSearchable ) ) -
                           aSearchable.Length() ) - 1 ) )
        {
        User::Leave( KErrGeneral );
        }
    return aPtr.Right( ( ( aPtr.Length() -
                           aPtr.Find( aSearchable ) ) -
                         aSearchable.Length() ) - 1 );
    }

// ---------------------------------------------------------------------------
// GetLeftPartL( TPtrC aPtrc ) of CImagingConfigManager, private method
// ---------------------------------------------------------------------------
//
TPtrC CImagingConfigManager::GetLeftPart( const TPtrC& aPtrc ) const
    {
    if( CheckIfNegative( aPtrc.Find( KComma ) ) )
        {
        return aPtrc;
        }
    return aPtrc.Left( aPtrc.Find( KComma ) );
    }

// ---------------------------------------------------------------------------
// SetTextStringL( TPtrC& aPtr, TText8& aStringToFill ) of
// CImagingConfigManager,private method
// ---------------------------------------------------------------------------
//
void CImagingConfigManager::SetTextStringL( const TPtrC& aPtr,
                                            TText8* aStringToFill ) const
    {
    if( static_cast< TUint >( aPtr.Length() ) > KMaxStringLength )
        {
        User::Leave( KErrOverflow );
        }
    if( aStringToFill == NULL )
        {
        User::Leave( KErrGeneral );
        }
    for( TInt j = 0 ; j < aPtr.Length() ; j++ )
        {
        aStringToFill[ j ] = ( aPtr[ j ] & 0xff);
        }
    for( TUint j = aPtr.Length() ; j < KMaxStringLength ; j++ )
        {
        aStringToFill[ j ] = KZero;
        }
    }

// ---------------------------------------------------------------------------
// NewL() of CImagingConfigManager
// ---------------------------------------------------------------------------
//
EXPORT_C CImagingConfigManager* CImagingConfigManager::NewL()
    {
    CImagingConfigManager* self = CImagingConfigManager::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// NelLC() of CImagingConfigManager
// ---------------------------------------------------------------------------
//
EXPORT_C CImagingConfigManager* CImagingConfigManager::NewLC()
    {
    CImagingConfigManager* self = new( ELeave ) CImagingConfigManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// Get number of defined video quality levels. This is always at least
// KNumberOfNominalLevels but can be higher
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::NumberOfVideoQualityLevels() const
    {
    TInt retval = 0;
    if( iVideoQualitySets )
        {
        retval = iVideoQualitySets->Count();
        }
    return retval;
    }

// ---------------------------------------------------------------------------
// Get an array of video quality levels that are in use with the current
// product with given Camera/Display ID. If the ID is zero, then all levels
// dispite of the ID value are returned.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetVideoQualityLevelsL(
                                        CArrayFixFlat<TUint>& aLevels,
                                        TUint aCameraDisplayID )
    {
    TKeyArrayFix key(0,ECmpTUint);
    aLevels.Reset();
    // We know the total size beforehand so reserve all the space at once
    aLevels.SetReserveL( iVideoQualitySets->Count() );
    for( TInt i = 0 ; i < iVideoQualitySets->Count() ; i++ )
        {
        if( aCameraDisplayID == 0 ||
            iVideoQualitySets->At( i ).iCameraDisplayId == aCameraDisplayID )
            {
            aLevels.AppendL(
                iVideoQualitySets->At( i ).iVideoQualitySetLevel );
            }
        }
    User::LeaveIfError(aLevels.Sort(key));
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get video quality set associated with the given level and Camera/Display ID
// if it is nonzero. If the ID is zero, smallest matching ID set is returned.
// If there is no set associated with given intermediate
// level, then set from a nearest nominal level is returned (dividable by
// KBasicQualityGranularity).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetVideoQualitySet(
                                        TVideoQualitySet& aSet,
                                        TInt aLevel,
                                        TUint aCameraDisplayID )
    {
    for( TUint i = 0 ; i < KMaxStringLength ; i++ )
        {
        aSet.iVideoFileMimeType[ i ] = NULL;
        aSet.iVideoCodecMimeType[ i ] = NULL;
        aSet.iPreferredSupplier[ i ] = NULL;
        }
    aSet.iVideoQualitySetLevel = 0;
    aSet.iVideoWidth = 0;
    aSet.iVideoHeight = 0;
    aSet.iVideoFrameRate = 0.0;
    aSet.iVideoBitRate = 0;
    aSet.iVideoEncoderUID.iUid = 0;
    aSet.iRandomAccessRate = 0;
    aSet.iVideoPixelAspectRatioNum = 0;
    aSet.iVideoPixelAspectRatioDenom = 0;
    aSet.iAudioFourCCType = 0;
    aSet.iAudioBitRate = 0;
    aSet.iAudioSamplingRate = 0;
    aSet.iAudioChannels = 0;
    aSet.iAudioEncoderUID.iUid = 0;
    aSet.iAVSyncStartDelay = 0;
    aSet.iAVSyncResumeDelay = 0;
    aSet.iCameraDisplayId = 0;
    aSet.iCamcorderVisible = 0;
    
    TInt diff; // This is used to get the nearest nominal level if the exact
               // level isn't available
    TInt min = KMaxTInt; // set to as big number as possible initially
    TInt minIndex = KUndefined;
    TBool paramSet = EFalse;
    TUint smallestCameraDisplayID = KMaxTUint;
    
    for( TInt i = 0 ;
         i < iVideoQualitySets->Count() && aCameraDisplayID == 0 ;
         i++ )
        {
        if(iVideoQualitySets->At( i ).iCameraDisplayId < smallestCameraDisplayID)
            {
            smallestCameraDisplayID =
               iVideoQualitySets->At( i ).iCameraDisplayId;
            }
        }

    for( TInt i = 0 ; i < iVideoQualitySets->Count() && paramSet == EFalse ; i++ )
        {
        if( ( aCameraDisplayID == 0 &&
              iVideoQualitySets->At( i ).iCameraDisplayId ==
                   smallestCameraDisplayID ) ||
            iVideoQualitySets->At( i ).iCameraDisplayId == aCameraDisplayID )
            {
            if( iVideoQualitySets->At( i ).iVideoQualitySetLevel == aLevel )
                {
                aSet = iVideoQualitySets->At( i );
                paramSet = ETrue;
                }
            else
                {
                diff = iVideoQualitySets->At( i ).iVideoQualitySetLevel - aLevel;
                if( static_cast< TInt >(
                       iVideoQualitySets->At( i ).iVideoQualitySetLevel )
                       < aLevel )
                    {
                    diff = - diff;
                    }
                if( min > diff )
                    {
                    min = diff;
                    minIndex = i;
                    }
                else if( min == diff && i > minIndex )
                    {
                    //If the difference is the same between two sets from the
                    //asked set, let's take the bigger one. Eg. 101 over 99
                    //if 100 was asked, but isn't present
                    minIndex = i;
                    }
                }
            }
        }
    if( minIndex != KUndefined && paramSet == EFalse )
        {
        aSet = iVideoQualitySets->At( minIndex );
        }
    else
        {
        return KErrGeneral; // No Quality sets were read from CenRep
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get quality set associated with the given video resolution, its MIME-type,
// audio fourCC and Camera/Display ID (which can be zero for all possible ID
// values). All of these are needed to be uniquely able to select
// a single quality setting set. If certain pixel aspect ratio is wanted for
// the video, it can be requested with aVideoPixelAspectRatioNum and
// aVideoPixelAspectRatioDenom parameters. Otherwise they should be set to
// zero.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetVideoQualitySet(
                                    TVideoQualitySet& aSet,
                                    const TSize& aVideoResolution,
                                    const TDesC8& aVideoCodecMimeType,
                                    TUint aVideoPixelAspectRatioNum,
                                    TUint aVideoPixelAspectRatioDenom,
                                    TFourCC aAudioFourCCType,
                                    TUint aCameraDisplayID )
    {
    for( TUint i = 0 ; i < KMaxStringLength ; i++ )
        {
        aSet.iVideoFileMimeType[ i ] = NULL;
        aSet.iVideoCodecMimeType[ i ] = NULL;
        aSet.iPreferredSupplier[ i ] = NULL;
        }
    aSet.iVideoQualitySetLevel = 0;
    aSet.iVideoWidth = 0;
    aSet.iVideoHeight = 0;
    aSet.iVideoFrameRate = 0.0;
    aSet.iVideoBitRate = 0;
    aSet.iVideoEncoderUID.iUid = 0;
    aSet.iRandomAccessRate = 0;
    aSet.iVideoPixelAspectRatioNum = 0;
    aSet.iVideoPixelAspectRatioDenom = 0;
    aSet.iAudioFourCCType = 0;
    aSet.iAudioBitRate = 0;
    aSet.iAudioSamplingRate = 0;
    aSet.iAudioChannels = 0;
    aSet.iAudioEncoderUID.iUid = 0;
    aSet.iAVSyncStartDelay = 0;
    aSet.iAVSyncResumeDelay = 0;
    aSet.iCameraDisplayId = 0;
    aSet.iCamcorderVisible = 0;
    
    TBool paramSet = EFalse;
    TUint smallestCameraDisplayID = KMaxTUint;
    
    for( TInt i = 0 ;
         i < iVideoQualitySets->Count() && aCameraDisplayID == 0 ;
         i++ )
        {
        if(iVideoQualitySets->At( i ).iCameraDisplayId < smallestCameraDisplayID)
            {
            smallestCameraDisplayID =
               iVideoQualitySets->At( i ).iCameraDisplayId;
            }
        }
    
    for( TInt i = 0 ;
         i < iVideoQualitySets->Count() && paramSet == EFalse ;
         i++ )
        {
        // videoCodecMimeTypeLength & audioFourCCTypeLength are used with
        // memcompare to compare strings
        TUint videoCodecMimeTypeLength = KMaxStringLength;
        for( TUint j = 0 ;
             j < KMaxStringLength &&
             videoCodecMimeTypeLength == KMaxStringLength ;
             j++ )
            {
            if( iVideoQualitySets->At( i ).iVideoCodecMimeType[ j ] == KZero )
                {
                videoCodecMimeTypeLength = j;
                }
            }

        // Check that all given parameters match with the current set
        if( iVideoQualitySets->At( i ).iVideoWidth ==
                aVideoResolution.iWidth &&
            iVideoQualitySets->At( i ).iVideoHeight ==
                aVideoResolution.iHeight &&
           memcompare( iVideoQualitySets->At( i ).iVideoCodecMimeType,
                videoCodecMimeTypeLength,
                aVideoCodecMimeType.Ptr(),
                aVideoCodecMimeType.Length() ) == 0 &&
           (iVideoQualitySets->At( i ).iAudioFourCCType.FourCC() == aAudioFourCCType.FourCC() ) &&
           ( ( aCameraDisplayID == 0 &&
               iVideoQualitySets->At( i ).iCameraDisplayId ==
                   smallestCameraDisplayID ) ||
             iVideoQualitySets->At( i ).iCameraDisplayId == aCameraDisplayID ) )
            {
            if( aVideoPixelAspectRatioNum == 0 &&
                aVideoPixelAspectRatioDenom == 0 )
                {
                aSet = iVideoQualitySets->At( i );
                paramSet = ETrue;
                }
            else if( iVideoQualitySets->At( i ).iVideoPixelAspectRatioNum ==
                        aVideoPixelAspectRatioNum &&
                    iVideoQualitySets->At( i ).iVideoPixelAspectRatioDenom ==
                        aVideoPixelAspectRatioDenom )
                {
                aSet = iVideoQualitySets->At( i );
                paramSet = ETrue;
                }
            else
                {
                }
            }
        }
    if(paramSet == EFalse)
        {
        return KErrGeneral;
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get all settings for Camcorder
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetCamcorderMMFPluginSettings(
                                TCamcorderMMFPluginSettings& aSet ) const
    {
    aSet = iCamcorderMMFPluginSettings;
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get number of defined image quality levels. This is always at least
// KNumberOfNominalLevels but can be higher
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::NumberOfImageQualityLevels() const
    {
    TInt retval = 0;
    if( iImageQualitySets )
    {
        retval = iImageQualitySets->Count();
    }
    return retval;
    }

// ---------------------------------------------------------------------------
// Get an array of image quality levels that are in use with the current
// product with given Camera/Display ID. If the ID is zero, then all levels
// dispite of the ID value are returned.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetImageQualityLevelsL(
                                        CArrayFixFlat<TUint>& aLevels,
                                        TUint aCameraDisplayID )
    {
    TKeyArrayFix key(0,ECmpTUint);
    aLevels.Reset();
    for( TInt i = 0 ; i < iImageQualitySets->Count() ; i++ )
        {
        if( aCameraDisplayID == 0 ||
            iImageQualitySets->At( i ).iCameraDisplayId == aCameraDisplayID )
            {
            aLevels.AppendL(
                iImageQualitySets->At( i ).iImageQualitySetLevel );
            }
        }
    User::LeaveIfError(aLevels.Sort(key));
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get image quality set associated with the given level and Camera/Display ID
// if it is nonzero. If the ID is zero, smallest matching ID set is returned.
// If there is no set associated with given intermediate
// level, then set from a nearest level is returned (dividable by
// KBasicQualityGranularity).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetImageQualitySet(
                                        TImageQualitySet& aSet,
                                        TInt aLevel,
                                        TUint aCameraDisplayID )
    {
    for( TUint i = 0 ; i < KMaxStringLength ; i++ )
        {
        aSet.iImageFileMimeType[ i ] = NULL;
        aSet.iImageFileExtension[ i ] = NULL;
        }
    aSet.iImageQualitySetLevel = 0;
    aSet.iImageWidth = 0;
    aSet.iImageHeight = 0;
    aSet.iImageEncoderUID.iUid = 0;
    aSet.iCompressionQuality = 0;
    aSet.iEstimatedSize = 0;
    aSet.iCameraDisplayId = 0;
    aSet.iCamcorderVisible = 0;

    TInt diff;
    TInt min = KMaxTInt; // set to as big number as possible initially
    TInt minIndex = KUndefined;
    TBool paramSet = EFalse;
    TUint smallestCameraDisplayID = KMaxTUint;
    
    for( TInt i = 0 ;
         i < iImageQualitySets->Count() && aCameraDisplayID == 0 ;
         i++ )
        {
        if(iImageQualitySets->At( i ).iCameraDisplayId < smallestCameraDisplayID)
            {
            smallestCameraDisplayID =
               iImageQualitySets->At( i ).iCameraDisplayId;
            }
        }
    
    for( TInt i = 0 ; i < iImageQualitySets->Count() && paramSet == EFalse ; i++ )
        {
        if( ( aCameraDisplayID == 0 &&
              iImageQualitySets->At( i ).iCameraDisplayId ==
                   smallestCameraDisplayID ) ||
            iImageQualitySets->At( i ).iCameraDisplayId == aCameraDisplayID )
            {
            if( iImageQualitySets->At( i ).iImageQualitySetLevel == aLevel )
                {
                aSet = iImageQualitySets->At( i );
                paramSet = ETrue;
                }
            else
                {
                diff = iImageQualitySets->At( i ).iImageQualitySetLevel - aLevel;
                if( static_cast< TInt >( iImageQualitySets->At( i ).iImageQualitySetLevel ) < aLevel )
                    {
                    diff = -diff;
                    }
                if( min > diff )
                    {
                    min = diff;
                    minIndex = i;
                    }
                else if( min == diff && i > minIndex )
                    {
                    //If the difference is the same between two sets from the
                    //asked set, let's take the bigger one. Eg. 101 over 99
                    //if 100 was asked, but isn't present
                    minIndex = i;
                    }
                }
            }
        }
    if( minIndex != KUndefined && paramSet == EFalse )
        {
        aSet = iImageQualitySets->At( minIndex );
        }
    else
        {
        return KErrGeneral; // No Quality sets were read from CenRep
        }
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Get quality set associated with the given image resolution and its
// Camera/Display ID (which can be zero for all possible ID
// values). All of these are needed to be uniquely able to select
// a single quality setting set.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CImagingConfigManager::GetImageQualitySet(
                    TImageQualitySet& aSet,
                    const TSize& aImageResolution,
                    TUint aCameraDisplayID )
    {
    for( TUint i = 0 ; i < KMaxStringLength ; i++ )
        {
        aSet.iImageFileMimeType[ i ] = NULL;
        aSet.iImageFileExtension[ i ] = NULL;
        }
    aSet.iImageQualitySetLevel = 0;
    aSet.iImageWidth = 0;
    aSet.iImageHeight = 0;
    aSet.iImageEncoderUID.iUid = 0;
    aSet.iCompressionQuality = 0;
    aSet.iEstimatedSize = 0;
    aSet.iCameraDisplayId = 0;
    aSet.iCamcorderVisible = 0;

    TBool paramSet = EFalse;
    TUint smallestCameraDisplayID = KMaxTUint;
    
    for( TInt i = 0 ;
         i < iImageQualitySets->Count() && aCameraDisplayID == 0 ;
         i++ )
        {
        if(iImageQualitySets->At( i ).iCameraDisplayId < smallestCameraDisplayID)
            {
            smallestCameraDisplayID =
               iImageQualitySets->At( i ).iCameraDisplayId;
            }
        }
    
    for( TInt i = 0 ; i < iImageQualitySets->Count() && paramSet == EFalse; i++ )
        {
        if( iImageQualitySets->At( i ).iImageWidth ==
            aImageResolution.iWidth &&
            iImageQualitySets->At( i ).iImageHeight ==
            aImageResolution.iHeight &&
            ( ( aCameraDisplayID == 0 &&
               iImageQualitySets->At( i ).iCameraDisplayId ==
                   smallestCameraDisplayID ) ||
              iImageQualitySets->At( i ).iCameraDisplayId == aCameraDisplayID ) )
            {
            aSet = iImageQualitySets->At( i );
            paramSet = ETrue;
            }
        }
    if(paramSet == EFalse)
        {
        return KErrGeneral;
        }
    return KErrNone;
    }
