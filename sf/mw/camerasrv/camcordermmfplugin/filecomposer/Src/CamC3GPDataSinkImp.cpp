/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  3GP file composer
*
*/

// INCLUDE FILES
#include <f32file.h>
#include <mmf/server/mmffile.h>
#include <sysutildomaincrkeys.h>	// for critical disk level CentralRepository keys
#include <3gplibrary/mp4lib.h>
#include "CamC3GPDataSink.h"
#include "CamC3GPDataSinkImp.h"
#include "CCMRMediaSink.h"
#include "CCMRSupportedCodecs.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>

// MACROS
// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

#include "OstTraceDefinitions.h"
#ifdef OST_TRACE_COMPILER_IN_USE
#include "CamC3GPDataSinkImpTraces.h"
#endif

// CONSTANTS
const TUint   KVideoAverageBitRate   = 48000;     // Default average bitrate for video
const TUint   KVideoMaxBitRate       = 64000;     // Default maximum bitrate for video
const TUint   KVideoXResolution      = 176;       // Default width of video
const TUint   KVideoYResolution      = 144;       // Default height of video
const TUint   KVideoBufferSize       = 8192;      // Maximum video frame size
const TUint   KAudioMaxFrameSize     = 32;        // Maximum AMR audio frame size
const TUint   KAudioBufferNumber     = 10;        // Number of audio frames to be buffered
const TUint   KVideoTimeScale        = 30000;     // Video timescale in the output file; use 30000 since it corresponds roughly to ~30 fps.
                                                  // 29970 would be exact for H.263, but it may not be exactly camera's baseframerate. And it may be harder for some players than 30000
                                                  // The most important is now that this value is in the same scale as used in MediaRecorder for Camera API framerate.
                                                  // If that is 15, then this should be 30000. But some other, e.g. 14.985 fps is used there, then it is better to change this one too.
const TUint   KAudioTimeScale        = 8000;      // Audio timescale in the output file, safer to use the same value as sampling rate although makes the times more difficult to understand
const TUint   KAudioFrameDuration    = 8*20;      // AMR frame is 20 ms, but timescale is 8000 => duration is 160 / 8000 s
const TUint8  KAMRAudioFramesPerSample  = 10;     // Number of AMR audio frames per sample in the output file
const TUint8  KAACAudioFramesPerSample  = 1;      // Number of AAC audio frames per sample in the output file
const TUint   KAACDefaultSampleRate  = 16000;     // Default samplerate for AAC that is used as audio track timescale in case we get no audiobuffers, but audio codec is selected.
const TUint16 KAudioModeSet          = 0x81ff;    // AMR modeset: all modes possible
const TUint   KDiskSafetyLimit       = 400000;    // Amount of free disk space to leave unused
const TReal   KMetaDataCoeff         = 1.03;      // Coefficient to estimate metadata amount
const TInt    KFreeDiskSpaceCounter  = 10;        // Interval when to find out real free disk space
const TUint   KFTYPSize              = 24;        // Size of FTYP box in bytes in 3GP files
const TUint   KCamCMaxClipDurationInSecs = 5400;  // Maximun video clip duration in seconds
const TInt    KLimitForLargeFileBuffers = 100000; // Bitrate limit to toggle to larger output file buffers in composer.
const TInt    KDelayUseBitrates      = 3000000;   // Delay used in GetRemainingTime until function uses real file sizes instead of avarage bitrates.
const TInt    KCamC3GPDeleteFileQueueGranularity  = 10; // Optimal value is the number of temporary files
const TUint32 KCamC3GPMaximumFileSize = 4294967295UL; // max size for RFile

_LIT(KTmpFileName, "\\system\\Temp\\CamcorderTMP");               // Temporary output file name

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCamC3GPDataSink::NewL
//
// Sink constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCamC3GPDataSink* CCamC3GPDataSink::NewL(M3GPDataSinkObserver *aObserver)
    {
	CCamC3GPDataSinkImp* self = new (ELeave) CCamC3GPDataSinkImp();
	CleanupStack::PushL(self);
	self->ConstructL(aObserver);
	CleanupStack::Pop();
	return self;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::~CCamC3GPDataSinkImp
//
// Sink destructor.
// -----------------------------------------------------------------------------
//
CCamC3GPDataSinkImp::~CCamC3GPDataSinkImp(void)
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMC3GPDATASINKIMP_CCAMC3GPDATASINKIMP, "e_CCamC3GPDataSinkImp::~CCamC3GPDataSinkImp 1" );
    PRINT(_L("CCamC3GPDataSinkImp::~CCamC3GPDataSinkImp enter"));

    if ( iFileName != KNullDesC )
        {
        // we need to try to stop sink
        TInt error = KErrNone;
        TRAP(error, SinkStopL());
        iFileName = KNullDesC;
        }

    if (iMP4Handle)
        {
        MP4ComposeClose(iMP4Handle);
        iMP4Handle = NULL;
        }

    if (iFS)
        {
        iFS->Delete(iTmpFileName);
        iFS->Close();
        delete iFS;
        iFS = NULL;
        }

    delete [] iVideoBuffer;
    delete [] iAudioBuffer;
    iObserver = NULL;
    iMMFFile = NULL; // not owned
    iFile = NULL; // not owned

    // Delete the CIdle async file remover object
    delete iIdleDelete;
    iIdleDelete = 0;

    // Do async temp file deletion for rest of files
    if ( iDeleteFileQueue )
        {
        // Reset and destroy the file name pointers from queue (if any left).
        iDeleteFileQueue->ResetAndDestroy();
        }
    delete iDeleteFileQueue;
    iDeleteFileQueue = 0;
    PRINT(_L("CCamC3GPDataSinkImp::~CCamC3GPDataSinkImp exit"));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMC3GPDATASINKIMP_CCAMC3GPDATASINKIMP, "e_CCamC3GPDataSinkImp::~CCamC3GPDataSinkImp 0" );
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::ConstructL
//
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::ConstructL(M3GPDataSinkObserver *aObserver)
	{
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMC3GPDATASINKIMP_CONSTRUCTL, "e_CCamC3GPDataSinkImp::ConstructL 1" );
    PRINT(_L("CCamC3GPDataSinkImp::ConstructL enter"));
    iObserver = aObserver;

    iVideoBuffer = new (ELeave) TUint8[KVideoBufferSize];
    iVideoBufferSize = KVideoBufferSize;

    iAudioBuffer = new (ELeave) TUint8[KAudioMaxFrameSize * KAudioBufferNumber];
    iAudioBufferSize = KAudioMaxFrameSize * KAudioBufferNumber;

    iMP4Handle = NULL;
    iMMFFile = NULL;

    iFileName = KNullDesC;

    iBytesReceived = 0;
    iBytesOfMetadata = 0;

    iVideoXResolution = 0;
    iVideoYResolution = 0;
    iVideoAverageBitRate = -1;
    iVideoMaxBitRate = -1;
    iAudioAverageBitRate = -1;

    iBufferSize = 0;
    iFileCodecType = MP4_TYPE_NONE;
    iVideoTimestamp = 0;
    iVideoBufferTimestamp = -1;
    iFirstVideoFrameTimestamp = 0;
    iVideoRandomAccessPoint = EFalse;
    iVideoBufferRandomAccessPoint = EFalse;
    iVideoFrameDuration = 0;
    iVideoBufferFrameSize = 0;
    iVideoFrameNumber = 0;
    iVideoIntraFrameNumber = 0;
    iVideoDecSpecInfoSize = 0;
    iAudioDecSpecInfoSize = 0;
    iAudioBufferFrameSize = 0;
    iAudioFrameNumber = 0;
    iAudioFramesInBuffer = 0;
    iFreeDiskSpace = 0;
    iFreeDiskSpaceCounter = 0;
    iAvailableSpaceAtStart = 0;
    iAvarageEndTime = -1;
    iAudioAACFrameDuration = 0;
    iAudioAACSamplerate = 0;
    iAVCOutputLevel = 10;
    iCriticalDiskVal = 0;

    iSizeLimit = KCamC3GPMaximumFileSize; // max size for RFile
    iFileSizeLimitReached = EFalse;
    iDiskFull = EFalse;
    iFS = NULL;

    iDeleteFileQueue = new( ELeave ) RPointerArray<MP4FileName>( KCamC3GPDeleteFileQueueGranularity );

    // Idle priority Active object for async video temp file deletion
    iIdleDelete = CIdle::NewL( CActive::EPriorityIdle );

    PRINT(_L("CCamC3GPDataSinkImp::ConstructL exit"));
	OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMC3GPDATASINKIMP_CONSTRUCTL, "e_CCamC3GPDataSinkImp::ConstructL 0" );
	}

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::OpenFileL
//
// Opens a 3GP file for writing.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::OpenFileL(CMMFFile* aMMFFile, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat )
    {
    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL CMMFFile enter")));

    TFileName aFileName;
    iMMFFile = aMMFFile;
    aFileName = iMMFFile->FullName();

    OpenFileL(aFileName, aAudioCodecType, aVideoCodecType, aFileFormat);
    }

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::OpenFileL
//
// Opens a 3GP file for writing.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::OpenFileL(TFileName aFileName, TFourCC aAudioCodecType, const TDesC8& aVideoCodecType, TCamCSinkFileFormat aFileFormat)
    {
    MP4Err    error;

    OstTrace0( CAMERASRV_PERFORMANCE, CCAMC3GPDATASINKIMP_OPENFILEL, "e_CCamC3GPDataSinkImp::OpenFileL 1" );
    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL TFileName enter")));

    if (iMP4Handle)
        {
        User::Leave(KErrGeneral);
        }

    iVideoBufferTimestamp = -1;     // reset duration information for new file
    iFirstVideoFrameTimestamp = 0;
    iAvailableSpaceAtStart = 0;
    iAvarageEndTime = -1;
    iVideoFrameNumber = 0;
    iVideoIntraFrameNumber = 0;
    iBytesReceived = 0;
    iAudioAACFrameDuration = 0;
    iAudioAACSamplerate = 0;
    iFileHandleExists = EFalse;

    iDiskFull = EFalse;
    iTmpFileName = KTmpFileName;
    iFileName = aFileName;
    TInt errorcode;

    OstTrace0( CAMERASRV_PERFORMANCE, DUP2_CCAMC3GPDATASINKIMP_OPENFILEL, "e_CCamC3GPDataSinkImp::OpenFileL_create_temp_files 1" );
    if (!iFS) // Don't allocate new file server, if there is one already
        {
        iFS = new (ELeave) RFs;

        errorcode = iFS->Connect();
        if ( errorcode != KErrNone)
            {
            delete(iFS);
            iFS = NULL;
            User::Leave( errorcode );
            }
        }

    TParse fp;
    User::LeaveIfError(iFS->Parse(iFileName, fp));
    TPtrC driveletter = fp.Drive();
    TChar drl = driveletter[0];
    User::LeaveIfError(iFS->CharToDrive(drl, iDriveNumber));

    // Get critical level for this drive type
    TDriveInfo driveInfo;
    iFS->Drive(driveInfo, iDriveNumber);

    iCriticalDiskVal = 0;
	if ( driveInfo.iType == EMediaRam ) // RAM drives have different critical levent than others
		{
		PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Asking disk critical level, memtype: EMediaRam")));
		CRepository* repository = CRepository::NewLC( KCRUidDiskLevel );
		User::LeaveIfError( repository->Get( KRamDiskCriticalLevel, iCriticalDiskVal ) );
		PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Got disk critical level: %d"),iCriticalDiskVal ));
		CleanupStack::PopAndDestroy( repository );
		}
	else // Some other media type
		{
		PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Asking disk critical level, memtype: other")));
		CRepository* repository = CRepository::NewLC( KCRUidDiskLevel );
		User::LeaveIfError( repository->Get( KDiskCriticalThreshold, iCriticalDiskVal ) );
		PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Got disk critical level: %d"),iCriticalDiskVal ));
		CleanupStack::PopAndDestroy( repository );
		}

    errorcode = iFS->MkDirAll(fp.DriveAndPath());
    if ( (errorcode != KErrAlreadyExists ) && ( errorcode != KErrNone ) )
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Error creating output path: '%s', error: %d"), fp.DriveAndPath().Ptr(), errorcode ));
        User::Leave(errorcode);
        }

    errorcode = iFS->SetAtt( iFileName, KEntryAttNormal, KEntryAttReadOnly );
    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Deleting File: '%s'"), iFileName.Ptr()));
    errorcode = iFS->Delete(iFileName);
    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Deleting File Error: %d   if error is -1 its ok - means same name file doesn't exists."), errorcode));

    if ( errorcode == KErrInUse && iMMFFile )
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL using rfiles")));
        iMMFFile->SinkPrimeL();
        iFile = &(iMMFFile->FileL());
        iFileHandleExists = ETrue;
        }
    else if ( ( errorcode != KErrNone ) && ( errorcode != KErrNotFound ) && ( errorcode != KErrPathNotFound ) )
        {
        User::Leave(errorcode);
        }

    iTmpFileName.Insert(0, fp.Drive());
    errorcode = iFS->SetAtt( iTmpFileName, KEntryAttNormal, KEntryAttReadOnly );
    errorcode = iFS->Delete(iTmpFileName);
    if ( ( errorcode != KErrNone ) && ( errorcode != KErrNotFound ) && ( errorcode != KErrPathNotFound ) )
    	{
        if ( errorcode == KErrInUse )
        	{
        	// use actual output filename incase other instance still running with temporary file.
            PRINT((_L("CCamC3GPDataSinkImp::OpenFileL temporary output file in use (-14). Using actual output filename instead.")));
        	iTmpFileName = aFileName;
            errorcode = iFS->SetAtt( iTmpFileName, KEntryAttNormal, KEntryAttReadOnly );
            errorcode = iFS->Delete(iTmpFileName);
            if ( ( errorcode != KErrNone ) && ( errorcode != KErrNotFound ) && ( errorcode != KErrPathNotFound ) )
            	{
            	User::Leave(errorcode);
            	}
        	}
        else
        	{
        	User::Leave(errorcode);
        	}
        }

    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Temp files cleared")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP3_CCAMC3GPDATASINKIMP_OPENFILEL, "e_CCamC3GPDataSinkImp::OpenFileL_create_temp_files 0" );

    // Find used audio codec
    if ( ( aAudioCodecType == TFourCC(KCMRFourCCIdAMRNB) ) ) // AMR-NB
        {
        iFileCodecType |= MP4_TYPE_AMR_NB;
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Audio codec: AMR-NB")));
        }
    else if ( aAudioCodecType == TFourCC(KCMRFourCCIdMPEG4AAC) ) // AAC
        {
        iFileCodecType |= MP4_TYPE_MPEG4_AUDIO;
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Audio codec: AAC")));
        }
    else
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Audio codec: none")));
        // if audio codec is null we don't generate audiotrack to composed file.
        }

    // Find used video codec
    mp4_u8 videoLevel = 10;

    if ( aVideoCodecType == KNullDesC8 ) // No video codec set.
        {
        // filecomposer will always create videotrack, whether we get video frames or not.
        iFileCodecType |= MP4_TYPE_H263_PROFILE_0;
        }
    else
        {
        TBuf8<256> matchstring;
        matchstring = KCMRMimeTypeH263;
        matchstring += _L8( "*" );

        if ( ( aVideoCodecType.MatchF( matchstring ) != KErrNotFound ) ) // H.263
            {
            matchstring = KCMRMimeTypeH263Profile3;
            matchstring += _L8( "*" );

            if ( aVideoCodecType.MatchF( matchstring ) != KErrNotFound )
                {
                iFileCodecType |= MP4_TYPE_H263_PROFILE_3;   // H.263 profile 3
                PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 profile 3")));
                }
            else
                {
                iFileCodecType |= MP4_TYPE_H263_PROFILE_0;   // H.263 profile 0
                PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 profile 0")));
                }
            // check if level is indicated too
            matchstring = _L8("*level=*");
            if (aVideoCodecType.MatchF( matchstring ) != KErrNotFound )
                {
                // yes, there is, check what it is
                if ( aVideoCodecType.MatchF( _L8("*level=10*") ) != KErrNotFound )
                    {
                    videoLevel = 10;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 10")));
                    }
                else if ( aVideoCodecType.MatchF( _L8("*level=20*") ) != KErrNotFound )
                    {
                    videoLevel = 20;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 20")));
                    }
                else if ( aVideoCodecType.MatchF( _L8("*level=30*") ) != KErrNotFound )
                    {
                    videoLevel = 30;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 30")));
                    }
                else if ( aVideoCodecType.MatchF( _L8("*level=40*") ) != KErrNotFound )
                    {
                    videoLevel = 40;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 40")));
                    }
                else if ( aVideoCodecType.MatchF( _L8("*level=45*") ) != KErrNotFound )
                    {
                    videoLevel = 45;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 45")));
                    }
                else if ( aVideoCodecType.MatchF( _L8("*level=50*") ) != KErrNotFound )
                    {
                    videoLevel = 50;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 50")));
                    }
                else
                    {
                    // assume 10
                    videoLevel = 10;
                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Video codec: H.263 level 10")));
                    }
                }
            }
        else // MPEG-4
            {
            matchstring = KCMRMimeTypeMPEG4V;
            matchstring += _L8( "*" );

            if ( aVideoCodecType.MatchF(matchstring) != KErrNotFound )
                {
                iFileCodecType |= MP4_TYPE_MPEG4_VIDEO;     // MPEG-4
                }
            else // H.264 AVC
	            {
	            matchstring = KCMRMimeTypeH264AVC;
	            matchstring += _L8( "*" );

	            if ( aVideoCodecType.MatchF(matchstring) != KErrNotFound )
	                {
                    iAVCOutputLevel = 10;
		            // check if profile & level is indicated too
		            matchstring = _L8("*profile-level-id=*");
		            if (aVideoCodecType.MatchF( matchstring ) != KErrNotFound )
		                {
		                // yes, there is, check what it is
	                    // Determine if other AVC profile is used:
                        matchstring = _L8( "*profile-level-id=42*" ); // Main Profile 
                        if (aVideoCodecType.MatchF( matchstring ) != KErrNotFound)
                            {
                            iFileCodecType |= MP4_TYPE_AVC_PROFILE_BASELINE;     // H.264 AVC Baseline profile found 
                            }
	                    matchstring = _L8( "*profile-level-id=4D*" ); // Main Profile 
	                    if (aVideoCodecType.MatchF( matchstring ) != KErrNotFound)
	                        {
	                        iFileCodecType |= MP4_TYPE_AVC_PROFILE_MAIN;     // H.264 AVC Main profile found 
	                        }
	                    matchstring = _L8( "*profile-level-id=64*" ); // High Profile 
	                    if (aVideoCodecType.MatchF( matchstring ) != KErrNotFound)
	                        {
	                        iFileCodecType |= MP4_TYPE_AVC_PROFILE_HIGH;     // H.264 AVC Baseline profile found 
	                        }
		                
	                    // Determine if other AVC level is used:
		                if ( aVideoCodecType.MatchF( _L8("*00A*") ) != KErrNotFound )
		                    {
		                    iAVCOutputLevel = 10; 	// Level 1
		                    }
		                else if ( (aVideoCodecType.MatchF( _L8("*profile-level-id=42900B*") ) != KErrNotFound) ||
                                  (aVideoCodecType.MatchF( _L8("*profile-level-id=4D500B*") ) != KErrNotFound) || 
                                  (aVideoCodecType.MatchF( _L8("*profile-level-id=644009*") ) != KErrNotFound) )
		                    {
		                    iAVCOutputLevel = 101; 	// Level 1b
		                    }
                        else if ( (aVideoCodecType.MatchF( _L8("*profile-level-id=42800B*") ) != KErrNotFound) ||
                                  (aVideoCodecType.MatchF( _L8("*profile-level-id=4D400B*") ) != KErrNotFound) || 
                                  (aVideoCodecType.MatchF( _L8("*profile-level-id=64400B*") ) != KErrNotFound) )
		                    {
		                    iAVCOutputLevel = 11;	// Level 1.1
		                    }
		                else if ( aVideoCodecType.MatchF( _L8("*00C*") ) != KErrNotFound )
		                    {
		                    iAVCOutputLevel = 12;	// Level 1.2
		                    }
		                else if ( aVideoCodecType.MatchF( _L8("*00D*") ) != KErrNotFound )
		                    {
		                    iAVCOutputLevel = 13;	// Level 1.3
		                    }
		                else if ( aVideoCodecType.MatchF( _L8("*014*") ) != KErrNotFound )
		                    {
		                    iAVCOutputLevel = 20;	// Level 2
		                    }
                        else if ( aVideoCodecType.MatchF( _L8("*015*") ) != KErrNotFound )
                            {
                            iAVCOutputLevel = 21;   // Level 2.1
                            }
                        else if ( aVideoCodecType.MatchF( _L8("*016*") ) != KErrNotFound )
                            {
                            iAVCOutputLevel = 22;   // Level 2.2
                            }
                        else if ( aVideoCodecType.MatchF( _L8("*01E*") ) != KErrNotFound )
                            {
                            iAVCOutputLevel = 30;   // Level 3
                            }
                        else if ( aVideoCodecType.MatchF( _L8("*01F*") ) != KErrNotFound )
                            {
                            iAVCOutputLevel = 31;   // Level 3.1
                            }
                        else if ( aVideoCodecType.MatchF( _L8("*020*") ) != KErrNotFound )
                            {
                            iAVCOutputLevel = 32;   // Level 3.2
                            }
                        else if ( aVideoCodecType.MatchF( _L8("*028*") ) != KErrNotFound )
                            {
                            iAVCOutputLevel = 4;   // Level 4
                            }
		                else
		                    {
		                    // assume level 1
		                    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Warning unknown video codec type - defaulting level 1.0")));
		                    iAVCOutputLevel = 10;
		                    }
		                }
	                }
	            else
		            {
    				PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Unsupported video codec type")));
		            User::Leave(KErrArgument);
		            }
	            }
            }
        }

    if ( iFileHandleExists )
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL using rfile to open mp4handle")));
        error = MP4ComposeOpenFileHandle(&iMP4Handle, iFile, (TDriveNumber)iDriveNumber, iFileCodecType);
        }
    else
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL using descriptors to open mp4handle")));
        error = MP4ComposeOpen(&iMP4Handle, (MP4FileName)iTmpFileName.Ptr(), iFileCodecType);
        }

    if ( error == MP4_OUT_OF_MEMORY )
    	{
    	PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeOpen, error=%d"), error));
    	User::Leave(KErrNoMemory);
    	}
    else if (error != MP4_OK)
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeOpen, error=%d"), error));
        TInt64 currentdiskspace = DriveFreeSpaceL();
        iFileName = KNullDesC;

        if ( currentdiskspace < ((TInt64)KDiskSafetyLimit+iCriticalDiskVal+CurrentMetadataSize()) )
            {
            PRINT((_L("CCamC3GPDataSinkImp::OpenFileL disk full, available: %d"), I64INT(currentdiskspace)));
            iDiskFull = ETrue;
            User::Leave(KErrDiskFull);
            }
        else
            {
            PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Compose and Open failed, error=%d"), error));
            iFileName = KNullDesC;
            User::Leave(KErrGeneral);
            }
        }
    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Compose and Open done")));

    mp4_u32 composeFlags = 0;
    if ( aFileFormat == E3GPP2 )
        {
        composeFlags |= ( MP4_FLAG_METADATALAST | MP4_FLAG_LONGCLIP | MP4_FLAG_GENERATE_3G2);
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Fileformat: 3G2")));
        }
    else if ( aFileFormat == EMPEG4 )
        {
        composeFlags |= ( MP4_FLAG_METADATALAST | MP4_FLAG_LONGCLIP | MP4_FLAG_GENERATE_MP4);
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Fileformat: MP4")));
        }
    else // E3GPP
        {
        composeFlags |= ( MP4_FLAG_METADATALAST | MP4_FLAG_LONGCLIP );
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Fileformat: 3GP")));
        }

    if ( ( iVideoMaxBitRate + iAudioAverageBitRate ) >= KLimitForLargeFileBuffers )
        {
        composeFlags |= MP4_FLAG_LARGEFILEBUFFER;
        }

    error = MP4ComposeSetFlags(iMP4Handle, composeFlags );
    if ( error == MP4_OUT_OF_MEMORY )
    	{
    	PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeSetFlags, error=%d"), error));
    	User::Leave(KErrNoMemory);
    	}
    else if (error != MP4_OK)
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeSetFlags, error=%d"), error));
        TInt64 currentdiskspace2 = DriveFreeSpaceL();

        if ( currentdiskspace2 < ((TInt64)KDiskSafetyLimit+iCriticalDiskVal+CurrentMetadataSize()) )
            {
            PRINT((_L("CCamC3GPDataSinkImp::OpenFileL disk full, available: %d"), I64INT(currentdiskspace2)));
            iDiskFull = ETrue;
            User::Leave(KErrDiskFull);
            }
        else
            {
            User::Leave(KErrGeneral);
            }
        }
    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Compose flags set")));

    if ((iFileCodecType & MP4_TYPE_H263_PROFILE_0) ||
        (iFileCodecType & MP4_TYPE_H263_PROFILE_3) ||
        (iFileCodecType & MP4_TYPE_MPEG4_VIDEO)    ||
        (iFileCodecType & MP4_TYPE_AVC_PROFILE_BASELINE) ||
        (iFileCodecType & MP4_TYPE_AVC_PROFILE_MAIN) ||
        (iFileCodecType & MP4_TYPE_AVC_PROFILE_HIGH))
        {
        // Set default values for video parameters if they are not set
        if (iVideoAverageBitRate < 0)
            {
            iVideoAverageBitRate = KVideoAverageBitRate;
            }
        if (iVideoMaxBitRate < 0)
            {
            iVideoMaxBitRate = KVideoMaxBitRate;
            }
        if (iVideoXResolution == 0)
            {
            iVideoXResolution = KVideoXResolution;
            }
        if (iVideoYResolution == 0)
            {
            iVideoYResolution = KVideoYResolution;
            }

        error = MP4ComposeAddVideoDescription(iMP4Handle,
                                              (mp4_u32)KVideoTimeScale,
                                              (mp4_u16)iVideoXResolution,
                                              (mp4_u16)iVideoYResolution,
                                              (mp4_u32)iVideoMaxBitRate,
                                              (mp4_u32)iVideoAverageBitRate);

        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeAddVideoDescription, error=%d"), error));
        if (error != MP4_OK)
            {
            User::Leave(KErrGeneral);
            }
        if ( (iFileCodecType & (MP4_TYPE_H263_PROFILE_0 | MP4_TYPE_H263_PROFILE_3))
            && ( videoLevel != 10) )
            {
            // H.263 level should be given to 3gp library like this
            error = MP4ComposeWriteVideoDecoderSpecificInfo(iMP4Handle,
                                                            (mp4_u8*)&videoLevel,
                                                            (mp4_u32)1);
            if ( error != MP4_OK )
                {
                PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeWriteVideoDecoderSpecificInfo, error=%d"), error));
                User::Leave(KErrGeneral);
                }
            }
        }

    if (iFileCodecType & MP4_TYPE_AMR_NB)
        {
        error = MP4ComposeAddAudioDescription(iMP4Handle,
                                              (mp4_u32)KAudioTimeScale,
                                              (mp4_u8)KAMRAudioFramesPerSample,
                                              (mp4_u16)KAudioModeSet);
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeAddAudioDescription, AMR-NB error=%d"), error));
        if (error != MP4_OK)
            {
            User::Leave(KErrGeneral);
            }
        }

    if ( iFileCodecType & MP4_TYPE_MPEG4_AUDIO)
        {
        // To make sure we always write proper timescale to output file (even in case when we get no audio
        // buffers) the default samplerate is passed to 3GP library here.
        // When we get audio decoder specific information in WriteBuffer() the MP4ComposeAddAudioDescription()
        // will be called again with correct samplerate.
        error = MP4ComposeAddAudioDescription(iMP4Handle,
                                              (mp4_u32)KAACDefaultSampleRate,
                                              (mp4_u8)KAACAudioFramesPerSample,
                                              (mp4_u16)KAudioModeSet);
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL MP4ComposeAddAudioDescription, AAC error=%d"), error));
        if (error != MP4_OK)
            {
            User::Leave(KErrGeneral);
            }
        }

    if ( iAvailableSpaceAtStart == 0 )
        {
        TVolumeInfo volumeinfo;
        User::LeaveIfError(iFS->Volume(volumeinfo, iDriveNumber));
        iAvailableSpaceAtStart = volumeinfo.iFree - (TInt64)(KDiskSafetyLimit+iCriticalDiskVal);
        }

    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Setting async file remover handler")));
    error = MP4ComposeSetTempFileRemoverObserver(&iMP4Handle, this);
    if (error != MP4_OK)
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Setting async file remover handler FAILED")));
        }

    if ((iFileCodecType & MP4_TYPE_MPEG4_VIDEO)    ||
        (iFileCodecType & MP4_TYPE_AVC_PROFILE_BASELINE) ||
        (iFileCodecType & MP4_TYPE_AVC_PROFILE_MAIN) ||
        (iFileCodecType & MP4_TYPE_AVC_PROFILE_HIGH))
        {
        PRINT((_L("CCamC3GPDataSinkImp::OpenFileL Composing UDTA atom")));
        ComposeUDTAL();
        }

    PRINT((_L("CCamC3GPDataSinkImp::OpenFileL exit")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMC3GPDATASINKIMP_OPENFILEL, "e_CCamC3GPDataSinkImp::OpenFileL 0" );
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::SetSizeLimit
//
// Set size limit of the 3GP file to be recorded in bytes. The limit must be
// set before the recording starts.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::SetSizeLimit(TUint aSize)
    {
    PRINT((_L("CCamC3GPDataSinkImp::SetSizeLimit enter, requested limit: %u"), aSize));
    if (iBytesReceived)
        {
        PRINT(_L("CCamC3GPDataSinkImp::SetSizeLimit NOT set, recording"));
        return;
        }

    TInt64 rfileMaxSize = KCamC3GPMaximumFileSize; 
    if ( aSize == 0 || aSize > rfileMaxSize )
        {
        PRINT((_L("CCamC3GPDataSinkImp::SetSizeLimit 0 or over RFile max size, using internal max instead.")));
        iSizeLimit = rfileMaxSize; //max size for RFile
        }
    else
        {
        iSizeLimit = aSize;
        }
    PRINT((_L("CCamC3GPDataSinkImp::SetSizeLimit set to: high:%u low:%u"), I64HIGH(iSizeLimit), I64LOW(iSizeLimit)));
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::GetElapsedTime
//
// Return the amount of time recording has been on in microseconds.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCamC3GPDataSinkImp::GetElapsedTime()
    {
    PRINT(_L("CCamC3GPDataSinkImp::GetElapsedTime in"));
    TTimeIntervalMicroSeconds elapsed;
    if (iVideoBufferTimestamp < TTimeIntervalMicroSeconds(0))
        {
        elapsed = 0;
        }
    else
        {
        elapsed = iVideoBufferTimestamp.Int64() - iFirstVideoFrameTimestamp.Int64();
        }
    PRINT((_L("CCamC3GPDataSinkImp::GetElapsedTime out, elapsed=%d"), I64INT(elapsed.Int64()) ));
    return elapsed;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::GetRemainingTime
//
// Return the estimated remaining time for the recording in microseconds.
// This method takes into account the file size and disk full restrictions.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CCamC3GPDataSinkImp::GetRemainingTimeL()
    {
    TTimeIntervalMicroSeconds  elapsed;
    TTimeIntervalMicroSeconds  remaining;
    TTimeIntervalMicroSeconds  endtime;
    TInt64 availableSpace;
    TInt64 usedSpace;
    TInt64 metaDataSize;
    TBool remainingFromSizeLimit = EFalse;

    PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL in")));

    if ( iAvailableSpaceAtStart == 0 )
        {
        TVolumeInfo volumeinfo;
        User::LeaveIfError(iFS->Volume(volumeinfo, iDriveNumber));
        iAvailableSpaceAtStart = volumeinfo.iFree - (TInt64)(KDiskSafetyLimit+iCriticalDiskVal);
        }

    if (iSizeLimit && ( iSizeLimit < iAvailableSpaceAtStart ) )
        {
        // use sizelimit as available space.
        PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL Limiting remainingtime by sizelimit: %d."), I64INT(iSizeLimit)));
        remainingFromSizeLimit = ETrue;
        }
    else
        {
        PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL Limiting remainingtime by available space at start: %d "), I64INT(iAvailableSpaceAtStart) ));
        }

    if ( iVideoBufferTimestamp < TTimeIntervalMicroSeconds(0) )
        {
        elapsed = 0;
        }
    else
        {
        elapsed = iVideoBufferTimestamp.Int64() - iFirstVideoFrameTimestamp.Int64();
        PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL elapsed with first=%d current=%d elapsed=%d"), I64INT(iFirstVideoFrameTimestamp.Int64()), I64INT(iVideoBufferTimestamp.Int64()), I64INT(elapsed.Int64()) ));
        }

    if (elapsed < (TTimeIntervalMicroSeconds)((TInt64)KDelayUseBitrates) )
        {
        // Use average audio/video bitrates to estimate remaining time
        TUint  averageBitRate;
        TUint  averageByteRate;

        PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL: BitRates Video: %d , Audio: %d"), iVideoAverageBitRate, iAudioAverageBitRate ));

        averageBitRate = (TUint)((iVideoAverageBitRate + iAudioAverageBitRate) * KMetaDataCoeff);
        averageByteRate = averageBitRate / 8;

        PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL: avaragebitrate: %d , avaragebyterate: %d"), averageBitRate, averageByteRate ));

        usedSpace = elapsed.Int64() * averageByteRate / 1000000; // 1000000 is for conversion between microseconds and seconds

        PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL: elapsed: %d, usedspace: %d"), I64INT(elapsed.Int64()), usedSpace ));

        metaDataSize = 2*CurrentMetadataSize();
        if (remainingFromSizeLimit)
            {
            availableSpace = iSizeLimit - usedSpace - metaDataSize;
            }
        else
            {
            availableSpace = iAvailableSpaceAtStart - usedSpace - metaDataSize;
            }

        if (availableSpace <= 0 || averageByteRate == 0)
            {
            remaining = 0;
            }
        else
            {
            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL: availablespace: %d, atStart: %d "), I64INT(elapsed.Int64()), I64INT(iAvailableSpaceAtStart) ));

            remaining = availableSpace * 1000000 / averageByteRate; // 1000000 is for conversion between microseconds and seconds

            if ( (remaining.Int64() + elapsed.Int64()) > (TInt64(KCamCMaxClipDurationInSecs)*1000000) )
                {
                remaining = (TInt64(KCamCMaxClipDurationInSecs)*1000000) - elapsed.Int64();
                }

            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL: remaining: %d "), I64INT(remaining.Int64()) ));
            }

        // update also iAvarageEndTime to smooth jump at KDelayUseBitrates sec point.
        iAvarageEndTime = elapsed.Int64()+ remaining.Int64();
        }
    else // use real filesize estimates.
        {
        // used space is mediadata + 2x metadata (metadata in temp-files written and additionaö reserved space for stop copying it to output file.
        usedSpace = CurrentFileSize()+ 2*CurrentMetadataSize();
        if (remainingFromSizeLimit)
            {
            availableSpace = iSizeLimit - usedSpace;
            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL availableSpace from sizelimit: %d "), I64INT(iAvailableSpaceAtStart) ));
            }
        else
            {
            availableSpace = iAvailableSpaceAtStart - usedSpace;
            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL availableSpace from availablespaceAtStart: %d "), I64INT(iAvailableSpaceAtStart) ));
            }

        if (availableSpace <= 0)
            {
            remaining = 0;
            }
        else
            {
            // preserve integer precision by scaling the first dividend up in calculation
            if (remainingFromSizeLimit)
                {
                // divide the greater of iSizeLimit and elapsed with usedSpace first to prevent overflow
                if ( iSizeLimit  > elapsed.Int64() )
                    {
                    PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL iSizeLimit > elapsed: %d vs. %d"), I64INT(iSizeLimit), I64INT(elapsed.Int64()) ));
                    endtime = (((iSizeLimit * 1000) / usedSpace ) * elapsed.Int64() ) / 1000;
                    }
                else
                    {
                    PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL iSizeLimit < elapsed: %d vs. %d"), I64INT(iSizeLimit), I64INT(elapsed.Int64()) ));
                    endtime = ( iSizeLimit  * ( (elapsed.Int64() * 1000) / usedSpace )) / 1000;
                    }
                }
            else
                {
                // divide the greater of iAvailableSpaceAtStart and elapsed with usedSpace first to prevent overflow
                if (iAvailableSpaceAtStart > elapsed.Int64() )
                    {
                    PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL iAvailableSpaceAtStart > elapsed: %d vs. %d"), I64INT(iAvailableSpaceAtStart), I64INT(elapsed.Int64()) ));
                    endtime = (( (iAvailableSpaceAtStart * 1000) / usedSpace ) * elapsed.Int64() ) / 1000;
                    }
                else
                    {
                    PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL iAvailableSpaceAtStart < elapsed: %d vs. %d"), I64INT(iAvailableSpaceAtStart), I64INT(elapsed.Int64()) ));
                    endtime = ( iAvailableSpaceAtStart * ( (elapsed.Int64() * 1000) / usedSpace )) / 1000;
                    }
                }

            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL Endtime: %d"), I64INT(endtime.Int64()) ));
            if ( iAvarageEndTime.Int64() == -1 )
                {
                iAvarageEndTime = endtime;
                }
            else
                {
                iAvarageEndTime = (( iAvarageEndTime.Int64() * 7 ) + endtime.Int64() ) / 8;
                }

            if ( iAvarageEndTime.Int64() > (TInt64(KCamCMaxClipDurationInSecs)*1000000) )
                {
                iAvarageEndTime = (TInt64(KCamCMaxClipDurationInSecs)*1000000);
                }

            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL elapsed: %d , usedspace: %d"), I64INT(elapsed.Int64()), I64INT(usedSpace) ));
            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL available: %d, atStart: %d"), I64INT(availableSpace), iAvailableSpaceAtStart));
            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL AvgEndtime: %d"), I64INT(iAvarageEndTime.Int64()) ));
            remaining = iAvarageEndTime.Int64() - elapsed.Int64();
            PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL remaining: %d"), I64INT(remaining.Int64()) ));
            }
        }

    // Check if remaining time has reached 0 and we need to stop right away
    if ( remaining <= TInt64(0) )
        {
        if ( elapsed >= TInt64(0) ) // we are recording.
            {
            if ( remainingFromSizeLimit || (elapsed >= (TInt64(KCamCMaxClipDurationInSecs)*1000000)) )
                {
                // Size limit has been set and we reach wanted size -> remaining time is 0
                iFileSizeLimitReached = ETrue;
                iObserver->MfcoSizeLimitReachedL();
                }
            else
                {
                // Diskfull
                iDiskFull = ETrue;
                iObserver->MfcoDiskFullL();
                }
            }
        remaining = 0;
        }
    PRINT((_L("CCamC3GPDataSinkImp::GetRemainingTimeL out")));
    return remaining;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::SinkStopL
//
// Order the sink to finalize and close the current 3GP file.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::SinkStopL()
    {
    MP4Err  error;

    OstTrace0( CAMERASRV_PERFORMANCE, CCAMC3GPDATASINKIMP_SINKSTOPL, "e_CCamC3GPDataSinkImp::SinkStopL 1" );
    PRINT((_L("CCamC3GPDataSinkImp::SinkStopL enter")));

    if (!iMP4Handle)
        {
        OstTrace0( CAMERASRV_PERFORMANCE, DUP2_CCAMC3GPDATASINKIMP_SINKSTOPL, "e_CCamC3GPDataSinkImp::SinkStopL 0" );
        return;
        }

    if (iVideoFrameDuration) // Write remaining video frame to disk
        {
        error = MP4ComposeWriteVideoFrame(iMP4Handle,
                                          (mp4_u8 *)iVideoBuffer,
                                          (mp4_u32)iVideoBufferFrameSize,
                                          (mp4_u32)iVideoFrameDuration,
                                          (mp4_bool)iVideoBufferRandomAccessPoint);
        if (error != MP4_OK)
            {
            PRINT((_L("CCamC3GPDataSinkImp::SinkStopL MP4ComposeWriteVideoFrame, error=%d"), error));
            User::Leave(KErrWrite);
            }
        }


    PRINT((_L("CCamC3GPDataSinkImp::SinkStopL video frames written")));


    if (iAudioFramesInBuffer) // Write remaining audio frames to disk
        {
        error = MP4ComposeWriteAudioFrames(iMP4Handle,
                                           (mp4_u8 *)iAudioBuffer,
                                           (mp4_u32)iAudioBufferFrameSize,
                                           (mp4_u32)iAudioFramesInBuffer,
                                           (mp4_u32)iAudioFramesInBuffer * KAudioFrameDuration);
        if (error != MP4_OK)
            {
            PRINT((_L("CCamC3GPDataSinkImp::SinkStopL MP4ComposeWriteAudioFrames, error=%d"), error));
            User::Leave(KErrWrite);
            }
        }

    PRINT((_L("CCamC3GPDataSinkImp::SinkStopL audio frames written")));


    error = MP4ComposeClose(iMP4Handle);
    iMP4Handle = NULL;
    if (error != MP4_OK)
        {
        PRINT((_L("CCamC3GPDataSinkImp::SinkStopL MP4ComposeClose, error=%d"), error));
        TInt64 currentdiskspace = 0;
        TInt freespaceError = KErrNone;
        TRAP(freespaceError, currentdiskspace = DriveFreeSpaceL());

        if ( (freespaceError == KErrNone) &&
             (currentdiskspace < ((TInt64)KDiskSafetyLimit+iCriticalDiskVal+CurrentMetadataSize())) )
            {
            PRINT((_L("CCamC3GPDataSinkImp::SinkStopL disk full, available: %d"), I64INT(currentdiskspace)));
            iDiskFull = ETrue;
            User::Leave(KErrDiskFull);
            }
        else if ( freespaceError != KErrNone )
            {
            User::Leave(KErrWrite); // There was error reading free disk space - probably memory card read/write error.
            }
        else    
            {
            User::Leave(KErrGeneral);
            }
        }

    PRINT((_L("CCamC3GPDataSinkImp::SinkStopL file composed and closed")));

    if ( iFileHandleExists )
        {
        iMMFFile->SinkStopL();
        iMMFFile = NULL; // not owned
        }
    else
        {
        if (!iBytesReceived)
            {
            PRINT((_L("CCamC3GPDataSinkImp::SinkStopL No data written, iTmpFileName left: '%s'"), iTmpFileName.Ptr()));
            }
        else
            {
            iFS->SetEntry(iTmpFileName, TTime(0), NULL, KEntryAttHidden);
            PRINT((_L("CCamC3GPDataSinkImp::SinkStopL Renaming temp file to: '%s'"), iFileName.Ptr()));
            User::LeaveIfError(iFS->Rename(iTmpFileName, iFileName));
            }
        }

    iFileName = KNullDesC;

    iBytesOfMetadata = 0;
    iBufferSize = 0;
    iFileCodecType = MP4_TYPE_NONE;
    iVideoTimestamp = 0;
    iVideoBufferTimestamp = -1;
    iVideoRandomAccessPoint = EFalse;
    iVideoBufferRandomAccessPoint = EFalse;
    iVideoFrameDuration = 0;
    iVideoBufferFrameSize = 0;
    iVideoDecSpecInfoSize = 0;
    iAudioDecSpecInfoSize = 0;
    iAudioBufferFrameSize = 0;
    iAudioFrameNumber = 0;
    iAudioFramesInBuffer = 0;
    iFileSizeLimitReached = EFalse;
    iDiskFull = EFalse;
    iFreeDiskSpace = 0;
    iFreeDiskSpaceCounter = 0;
    iAudioAACFrameDuration = 0;
    iAudioAACSamplerate = 0;

    PRINT((_L("CCamC3GPDataSinkImp::SinkStopL() updating available diskspace - in")));
    TVolumeInfo volumeinfo;
    if ( iFS && iFS->Volume(volumeinfo, iDriveNumber) == KErrNone )
        {
        iAvailableSpaceAtStart = volumeinfo.iFree - (TInt64)(KDiskSafetyLimit+iCriticalDiskVal);
        PRINT((_L("CCamC3GPDataSinkImp::SinkStopL() updating available diskspace - done")));
        }

    PRINT((_L("CCamC3GPDataSinkImp::SinkStopL exit")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMC3GPDATASINKIMP_SINKSTOPL, "e_CCamC3GPDataSinkImp::SinkStopL 0" );
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::WriteBuffer
//
// Write an audio/video buffer to the sink. The sink copies the given buffer
// and writes it to the file.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::WriteBufferL(CCMRMediaBuffer* aBuffer)
    {
    PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL enter")));
    TInt64  videoduration = 0;
    TUint8* tmpaudiobuffer = 0;
    TInt64 currentfilesize;
    MP4Err  error = MP4_OK;


    if (!iMP4Handle)
        {
        PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL No MP4Handle, returning.")));
        return;
        }

    if (iFileSizeLimitReached || iDiskFull)
        {
        PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL sizelimit reached or disk full returning")));
        return;
        }

    iBufferType = aBuffer->Type();
    iBufferSize = aBuffer->BufferSize();

    TTimeIntervalMicroSeconds elapsed;
    if (iVideoBufferTimestamp < TTimeIntervalMicroSeconds(0))
        {
        elapsed = 0;
        }
    else
        {
        elapsed = iVideoBufferTimestamp.Int64() - iFirstVideoFrameTimestamp.Int64();
        }

    currentfilesize = CurrentFileSize() + CurrentMetadataSize();
    if ( ( iSizeLimit && ((currentfilesize + (TUint)iBufferSize) > iSizeLimit) ) ||
         (elapsed.Int64() >= (TInt64(KCamCMaxClipDurationInSecs)*1000000)) )
        {
        PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL sizelimit reached, filesize: %d"), I64INT(currentfilesize)));
        PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL sizelimit is set to: high:%u low:%u"), I64HIGH(iSizeLimit), I64LOW(iSizeLimit)));
        PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL elapsed time: high:%u low:%u"), I64HIGH(elapsed.Int64()), I64LOW(elapsed.Int64())));        
        iFileSizeLimitReached = ETrue;
        iObserver->MfcoSizeLimitReachedL();
        return;
        }

    PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL, filesize: %d, bufsize: %d")
            , I64INT(currentfilesize), iBufferSize));

    currentfilesize += CurrentMetadataSize(); // extra reserve for stop (copy metadata from temp files to end of output file.
    if ( currentfilesize >= iAvailableSpaceAtStart )
        {
        PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL disk full, calc size: %d"), I64INT(currentfilesize)));
        iDiskFull = ETrue;
        iObserver->MfcoDiskFullL();
        return;
        }

    PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL, available: %d")
            , I64INT(iAvailableSpaceAtStart-currentfilesize)));

    iBytesReceived += (TUint)iBufferSize;
    switch (iBufferType)
        {
        case CCMRMediaBuffer::EAudioAMRNB:
            {
            iAudioFrameNumber++;

            if ( ( (TUint)iBufferSize + iAudioBufferFrameSize ) > iAudioBufferSize) // Incoming buffer doesn't fit into allocated buffer
                {
                tmpaudiobuffer = new (ELeave) TUint8[(TUint)iBufferSize + iAudioBufferFrameSize];
                Mem::Copy(tmpaudiobuffer, iAudioBuffer, (TInt)iAudioBufferFrameSize);
                delete [] iAudioBuffer;
                iAudioBuffer = tmpaudiobuffer;
                iAudioBufferSize = (TUint)iBufferSize + iAudioBufferFrameSize;
                }

            Mem::Copy(iAudioBuffer + iAudioBufferFrameSize, aBuffer->Data().Ptr(), iBufferSize);
            iAudioBufferFrameSize += (TUint)iBufferSize;
            iAudioFramesInBuffer++;

            if (iAudioFramesInBuffer == KAMRAudioFramesPerSample) // Buffer several audio frames before writing to disk
                {
                error = MP4ComposeWriteAudioFrames(iMP4Handle,
                                                   (mp4_u8 *)iAudioBuffer,
                                                   (mp4_u32)iAudioBufferFrameSize,
                                                   (mp4_u32)iAudioFramesInBuffer,
                                                   (mp4_u32)iAudioFramesInBuffer * KAudioFrameDuration);
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL AMR-NB MP4ComposeWriteAudioFrames, error=%d"), error));
                if (error != MP4_OK)
                    {
                    User::Leave(KErrWrite);
                    }

                iAudioFramesInBuffer = 0;
                iAudioBufferFrameSize = 0;
                }

            break;
            }
        case CCMRMediaBuffer::EAudioMPEG4AAC:
            {
            if ( !iAudioAACFrameDuration )
                {
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL AAC Audio frameduration not set! Error= -18"), error));
                User::Leave ( KErrNotReady );
                }

            iAudioFrameNumber++;
            error = MP4ComposeWriteAudioFrames(iMP4Handle,
                                               (mp4_u8 *)aBuffer->Data().Ptr(),
                                               (mp4_u32)aBuffer->BufferSize(),
                                               (mp4_u32)KAACAudioFramesPerSample,
                                               (mp4_u32)KAACAudioFramesPerSample * iAudioAACFrameDuration);
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL AAC MP4ComposeWriteAudioFrames, error=%d"), error));
            if (error != MP4_OK)
                {
                User::Leave(KErrWrite);
                }
            break;
            }
        case CCMRMediaBuffer::EAudioAMRWB:
            {
            break;
            }
        case CCMRMediaBuffer::EVideoH263:
        case CCMRMediaBuffer::EVideoMPEG4:
            {
            iVideoTimestamp = aBuffer->TimeStamp();
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL EVideoH263/EVideoMPEG4 Timestamp of the buffer is %d"), I64INT(iVideoTimestamp.Int64())));
            iVideoRandomAccessPoint = aBuffer->RandomAccessPoint();
            iVideoFrameNumber++;
            if (iVideoRandomAccessPoint)
                {
                iVideoIntraFrameNumber++;
                }

            if (iVideoBufferTimestamp < TTimeIntervalMicroSeconds(0)) // First frame
                {
                iFirstVideoFrameTimestamp = iVideoTimestamp;
                iVideoBufferTimestamp = iVideoTimestamp;
                iVideoBufferRandomAccessPoint = iVideoRandomAccessPoint;

                if ((TUint)iBufferSize > iVideoBufferSize)
                    {
                    delete [] iVideoBuffer;
                    iVideoBuffer = new (ELeave) TUint8[(TUint)iBufferSize];
                    iVideoBufferSize = (TUint)iBufferSize;
                    }

                Mem::Copy(iVideoBuffer, aBuffer->Data().Ptr(), iBufferSize);
                iVideoBufferFrameSize = (TUint)iBufferSize;

                break;
                }

            videoduration = iVideoTimestamp.Int64() - iVideoBufferTimestamp.Int64(); // Duration in microseconds
            videoduration = TInt64((videoduration * KVideoTimeScale) / 1E6 + 0.5); // Duration scaled to KVideoTimeScale
            iVideoFrameDuration = (TUint)I64INT(videoduration);

            error = MP4ComposeWriteVideoFrame(iMP4Handle,
                                              (mp4_u8 *)iVideoBuffer,
                                              (mp4_u32)iVideoBufferFrameSize,
                                              (mp4_u32)iVideoFrameDuration,
                                              (mp4_bool)iVideoBufferRandomAccessPoint);
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteVideoFrame, error=%d"), error));
            if (error != MP4_OK)
                {
                User::Leave(KErrWrite);
                }

            if ((TUint)iBufferSize > iVideoBufferSize)
                {
                delete [] iVideoBuffer;
                iVideoBuffer = new (ELeave) TUint8[(TUint)iBufferSize];
                iVideoBufferSize = (TUint)iBufferSize;
                }

            Mem::Copy(iVideoBuffer, aBuffer->Data().Ptr(), iBufferSize);
            iVideoBufferFrameSize = (TUint)iBufferSize;
            iVideoBufferTimestamp = iVideoTimestamp;
            iVideoBufferRandomAccessPoint = iVideoRandomAccessPoint;

            break;
            }
        case CCMRMediaBuffer::EVideoMPEG4DecSpecInfo:
            {
            iVideoDecSpecInfoSize = iBufferSize;

            error = MP4ComposeWriteVideoDecoderSpecificInfo(iMP4Handle,
                                                            (mp4_u8 *)aBuffer->Data().Ptr(),
                                                            (mp4_u32)iBufferSize);
            if (error != MP4_OK)
                {
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteVideoDecoderSpecificInfo, error=%d"), error));
                User::Leave(KErrWrite);
                }

            break;
            }
        case CCMRMediaBuffer::EVideoH264NAL:
            {
            iVideoTimestamp = aBuffer->TimeStamp();
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL EVideoH264NAL Timestamp of the buffer is %d"), I64INT(iVideoTimestamp.Int64())));
            iVideoRandomAccessPoint = aBuffer->RandomAccessPoint();
            iVideoFrameNumber++;
            if (iVideoRandomAccessPoint)
                {
                iVideoIntraFrameNumber++;
                }

            if (iVideoBufferTimestamp < TTimeIntervalMicroSeconds(0)) // First frame
                {
                iFirstVideoFrameTimestamp = iVideoTimestamp;
                iVideoBufferTimestamp = iVideoTimestamp;
                iVideoBufferRandomAccessPoint = iVideoRandomAccessPoint;

                if ((TUint)(iBufferSize) > iVideoBufferSize)
                    {
                    delete [] iVideoBuffer;
                    iVideoBuffer = new (ELeave) TUint8[(TUint)iBufferSize];
                    iVideoBufferSize = (TUint)iBufferSize;
                    }

                ConvertNALEncapsulationToNALSizes( aBuffer );
                break;
                }

            videoduration = iVideoTimestamp.Int64() - iVideoBufferTimestamp.Int64(); // Duration in microseconds
            videoduration = TInt64((videoduration * KVideoTimeScale) / 1E6 + 0.5); // Duration scaled to KVideoTimeScale
            iVideoFrameDuration = (TUint)I64INT(videoduration);

            error = MP4ComposeWriteVideoFrame(iMP4Handle,
                                              (mp4_u8 *)iVideoBuffer,
                                              (mp4_u32)iVideoBufferFrameSize,
                                              (mp4_u32)iVideoFrameDuration,
                                              (mp4_bool)iVideoBufferRandomAccessPoint);
            if (error != MP4_OK)
                {
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteVideoFrame, error=%d"), error));
                User::Leave(KErrWrite);
                }

            if ((TUint)(iBufferSize) > iVideoBufferSize)
                {
                delete [] iVideoBuffer;
                iVideoBuffer = new (ELeave) TUint8[(TUint)iBufferSize];
                iVideoBufferSize = (TUint)iBufferSize;
                }

            ConvertNALEncapsulationToNALSizes( aBuffer );
            iVideoBufferTimestamp = iVideoTimestamp;
            iVideoBufferRandomAccessPoint = iVideoRandomAccessPoint;
            break;
            }
        case CCMRMediaBuffer::EVideoH264Bytestream:
            {
            // need to add NAL header to end on bytestream buffer
            iVideoTimestamp = aBuffer->TimeStamp();
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL EVideoH264Bytestream Timestamp of the buffer is %d"), I64INT(iVideoTimestamp.Int64())));
            iVideoRandomAccessPoint = aBuffer->RandomAccessPoint();
            iVideoFrameNumber++;
            if (iVideoRandomAccessPoint)
                {
                iVideoIntraFrameNumber++;
                }

            if (iVideoBufferTimestamp < TTimeIntervalMicroSeconds(0)) // First frame
                {
                iFirstVideoFrameTimestamp = iVideoTimestamp;
                iVideoBufferTimestamp = iVideoTimestamp;
                iVideoBufferRandomAccessPoint = iVideoRandomAccessPoint;

                if ((TUint)(iBufferSize) > iVideoBufferSize)
                    {
                    delete [] iVideoBuffer;
                    iVideoBuffer = new (ELeave) TUint8[(TUint)iBufferSize];
                    iVideoBufferSize = (TUint)iBufferSize;
                    }

                ConvertBytestreamHeadersToNALSizes(aBuffer);
                break;
                }

            videoduration = iVideoTimestamp.Int64() - iVideoBufferTimestamp.Int64(); // Duration in microseconds
            videoduration = TInt64((videoduration * KVideoTimeScale) / 1E6 + 0.5); // Duration scaled to KVideoTimeScale
            iVideoFrameDuration = (TUint)I64INT(videoduration);

            error = MP4ComposeWriteVideoFrame(iMP4Handle,
                                              (mp4_u8 *)iVideoBuffer,
                                              (mp4_u32)iVideoBufferFrameSize,
                                              (mp4_u32)iVideoFrameDuration,
                                              (mp4_bool)iVideoBufferRandomAccessPoint);
            if (error != MP4_OK)
                {
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteVideoFrame, error=%d"), error));
                User::Leave(KErrWrite);
                }

            if ((TUint)(iBufferSize) > iVideoBufferSize)
                {
                delete [] iVideoBuffer;
                iVideoBuffer = new (ELeave) TUint8[(TUint)iBufferSize];
                iVideoBufferSize = (TUint)iBufferSize;
                }

            ConvertBytestreamHeadersToNALSizes(aBuffer);
            iVideoBufferTimestamp = iVideoTimestamp;
            iVideoBufferRandomAccessPoint = iVideoRandomAccessPoint;
            break;
            }
        case CCMRMediaBuffer::EVideoH264NALDecSpecInfo:
            {
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL EVideoH264NALDecSpecInfo")));            
            HBufC8* outputAVCHeader = 0;
            outputAVCHeader = (HBufC8*) HBufC8::NewLC(16384);
            TPtr8 destptr = outputAVCHeader->Des();

	        // parse header & convert it to AVCDecoderConfigurationRecord -format
	        ConvertAVCHeaderNALL(aBuffer, destptr);
            iVideoDecSpecInfoSize = destptr.Length();

            error = MP4ComposeWriteVideoDecoderSpecificInfo(iMP4Handle,
                                                            (mp4_u8 *)destptr.Ptr(),
                                                            (mp4_u32)iVideoDecSpecInfoSize);

			CleanupStack::PopAndDestroy( outputAVCHeader );
            if (error != MP4_OK)
                {
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteVideoDecoderSpecificInfo, error=%d"), error));
                User::Leave(KErrWrite);
                }
            break;
            }
        case CCMRMediaBuffer::EVideoH264BytestreamDecSpecInfo:
            {
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL EVideoH264BytestreamDecSpecInfo")));            
            HBufC8* outputAVCHeader = 0;
            outputAVCHeader = (HBufC8*) HBufC8::NewLC(16384);
            TPtr8 destptr = outputAVCHeader->Des();

	        // parse header & convert it to AVCDecoderConfigurationRecord -format
	        ConvertAVCHeaderByteStreamL(aBuffer, destptr);
            iVideoDecSpecInfoSize = destptr.Length();

            error = MP4ComposeWriteVideoDecoderSpecificInfo(iMP4Handle,
                                                            (mp4_u8 *)destptr.Ptr(),
                                                            (mp4_u32)iVideoDecSpecInfoSize);

			CleanupStack::PopAndDestroy( outputAVCHeader );
            if (error != MP4_OK)
                {
                PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteVideoDecoderSpecificInfo, error=%d"), error));
                User::Leave(KErrWrite);
                }
            break;
            }
        case CCMRMediaBuffer::EAudioDecSpecInfo:
            {
			PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteAudioDecoderSpecificInfo in"), error));
            iAudioDecSpecInfoSize = iBufferSize;

            error = MP4ComposeWriteAudioDecoderSpecificInfo(iMP4Handle,
                                                            (mp4_u8 *)aBuffer->Data().Ptr(),
                                                            (mp4_u32)iBufferSize);
            PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteAudioDecoderSpecificInfo, error=%d"), error));
            if (error != MP4_OK)
                {
                User::Leave(KErrGeneral);
                }

			PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteAudioDecoderSpecificInfo - flags: iFileCodecType=%d Frameduration=%d"), iFileCodecType, iAudioAACFrameDuration));
            if ( (iFileCodecType & MP4_TYPE_MPEG4_AUDIO) && !iAudioAACFrameDuration )
                {
				PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteAudioDecoderSpecificInfo determining new AAC samplerate."), error));
                DetermineAACFrameDurationL( aBuffer );
                // done here because timescale is dependent of samplerate.
                error = MP4ComposeAddAudioDescription(iMP4Handle,
                                                     (mp4_u32)iAudioAACSamplerate,
                                                     (mp4_u8)KAACAudioFramesPerSample,
                                                     (mp4_u16)KAudioModeSet);
                if (error != MP4_OK)
                    {
                    PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeAddAudioDescription, error=%d"), error));
                    User::Leave(KErrWrite);
                    }
                }
			PRINT((_L("CCamC3GPDataSinkImp::WriteBufferL MP4ComposeWriteAudioDecoderSpecificInfo out"), error));
            break;
            }
        default:
            {
            break;
            }
        }
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::SetVideoFrameSize
//
// Give video frame size to sink.
// -----------------------------------------------------------------------------
//
TInt CCamC3GPDataSinkImp::SetVideoFrameSize(TSize aSize)
    {
    if (aSize.iWidth < 0)
        {
        return KErrArgument;
        }
    if (aSize.iHeight < 0)
        {
        return KErrArgument;
        }

    if (iBytesReceived)
        {
        return KErrNotReady;
        }

    iVideoXResolution = aSize.iWidth;
    iVideoYResolution = aSize.iHeight;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::SetAverageVideoBitRate
//
// Give average video bitrate to sink.
// -----------------------------------------------------------------------------
//
TInt CCamC3GPDataSinkImp::SetAverageVideoBitRate(TInt aBitRate)
    {
    if (aBitRate < 0)
        {
        return KErrArgument;
        }

    iVideoAverageBitRate = aBitRate;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::SetMaxVideoBitRate
//
// Give maximum video bitrate to sink.
// -----------------------------------------------------------------------------
//
TInt CCamC3GPDataSinkImp::SetMaxVideoBitRate(TInt aBitRate)
    {
    if (aBitRate < 0)
        {
        return KErrArgument;
        }

    iVideoMaxBitRate = aBitRate;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::SetAverageAudioBitRate
//
// Give average audio bitrate to sink.
// -----------------------------------------------------------------------------
//
TInt CCamC3GPDataSinkImp::SetAverageAudioBitRate(TInt aBitRate)
    {
    if (aBitRate < 0)
        {
        return KErrArgument;
        }

    iAudioAverageBitRate = aBitRate;
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::CurrentFileSize
//
// Estimate current output file size.
// Total file size = metadata size + media data size
// -----------------------------------------------------------------------------
//
TUint CCamC3GPDataSinkImp::CurrentFileSize() const
    {
    TUint filesize = 0;

    // Media data
    filesize += 8; // mdat box type and size
    filesize += iBytesReceived; // Data received from media recorder

    return filesize;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::CurrentMetadataSize
//
// Calculates current metadata size.
// -----------------------------------------------------------------------------
//
TUint CCamC3GPDataSinkImp::CurrentMetadataSize() const
    {
    TBool haveAudio;
    TBool haveVideo;
    TUint metadatasize = 0;

    haveAudio = EFalse;
    haveVideo = EFalse;


    // Metadata

    metadatasize += KFTYPSize; // FTYP

    if ((iFileCodecType & MP4_TYPE_H263_PROFILE_0) ||
        (iFileCodecType & MP4_TYPE_H263_PROFILE_3)) // H.263
        {
        haveVideo = ETrue;
        metadatasize += 574; // Constant size H.263 metadata
        metadatasize += (iVideoFrameNumber * 16 + iVideoIntraFrameNumber * 4); // Content dependent H.263 metadata
        }

    if (iFileCodecType & MP4_TYPE_MPEG4_VIDEO) // MPEG-4 video
        {
        haveVideo = ETrue;
        metadatasize += 596; // Constant size MPEG-4 video metadata
        metadatasize += (iVideoFrameNumber * 16 + iVideoIntraFrameNumber * 4 + (TUint)iVideoDecSpecInfoSize); // Content dependent MPEG-4 video metadata
        }

    if ( iFileCodecType & MP4_TYPE_AMR_NB ) // AMR-NB
        {
        haveAudio = ETrue;
        metadatasize += 514; // Constant size AMR metadata
        metadatasize += ((iAudioFrameNumber + KAMRAudioFramesPerSample - 1) / KAMRAudioFramesPerSample) * 16;
        }

    if ( iFileCodecType & MP4_TYPE_MPEG4_AUDIO ) // MPEG-4 AAC-LC
        {
        haveAudio = ETrue;
        metadatasize += 514; // Constant size metadata
        metadatasize += (iAudioFrameNumber * 16) + (TUint)iAudioDecSpecInfoSize;
        }

    if (haveAudio && haveVideo)
        metadatasize -= 116; // There is only one moov and mvhd in a file

    return metadatasize;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::DriveFreeSpaceL
//
// Calculate free space on a drive in bytes.
// -----------------------------------------------------------------------------
//
TInt64 CCamC3GPDataSinkImp::DriveFreeSpaceL()
    {
    TVolumeInfo volumeinfo;

    if (iFreeDiskSpaceCounter % KFreeDiskSpaceCounter == 0)
        {
        PRINT((_L("CCamC3GPDataSinkImp::DriveFreeSpaceL Asking Disk Free space")));
        User::LeaveIfError(iFS->Volume(volumeinfo, iDriveNumber));
        iFreeDiskSpace = volumeinfo.iFree;
        PRINT((_L("CCamC3GPDataSinkImp::DriveFreeSpaceL Received Disk Free space info")));
        }

    iFreeDiskSpaceCounter++;
    return iFreeDiskSpace;
    }


// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::DetermineAACFrameDuration
//
// Determines AAC audio frame duration.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::DetermineAACFrameDurationL( CCMRMediaBuffer* aBuffer )
    {
    TInt sampleRate = 0;
    TUint8 sampleRateIndex = *aBuffer->Data().Mid(0).Ptr();
    sampleRateIndex <<= 5;
    sampleRateIndex >>= 4;
    sampleRateIndex |= ((TUint8)*aBuffer->Data().Mid(1).Ptr())>>7;

    switch ( sampleRateIndex )
        {
        case 0x3:
            {
            PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL AAC Samplerate reset to 48000")));
            sampleRate = 48000;
            break;
            }
        case 0x5:
            {
			PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL AAC Samplerate reset to 32000")));
            sampleRate = 32000;
            break;
            }
        case 0x6:
            {
            PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL AAC Samplerate reset to 24000")));
            sampleRate = 24000;
            break;
            }
        case 0x8:
            {
            PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL AAC Samplerate reset to 16000")));
            sampleRate = 16000;
            break;
            }
        case 0xb:
            {
            PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL AAC Samplerate reset to 8000")));
            sampleRate = 8000;
            break;
            }
        default:
            {
            PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL unsupported AAC Samplerate - leaving -6")));
            User::Leave( KErrArgument );
            break;
            }
        }

    PRINT((_L("CCamC3GPDataSinkImp::DetermineAACFrameDurationL AAC Samplerate reset from %d to %d"), iAudioAACSamplerate, sampleRate));
    iAudioAACSamplerate = sampleRate;
    // formula to calculate frameduration from samplerate is: frameduration = (1024/samplerate)*1000
    // and in movie timescale: frameDurationInMovieTimescale = frameduration*samplerate/1000
    // thus with equal samplerate = timescale frameDurationInMovieTimescale for AAC is always 1024
    iAudioAACFrameDuration = 1024;
}

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::ConvertAVCHeaderL
//
// Convert AVC specific decoder config info to
// AVC Decoder Configuration Record -format
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::ConvertAVCHeaderNALL( CCMRMediaBuffer* aBuffer, TDes8& aDstBuf )
    {
    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL in Buffer length: %d"), aBuffer->Data().Length() ));    
	TUint8* inputPtr = (TUint8*)(aBuffer->Data().Ptr());
    TUint8* outputPtr = (TUint8*)(aDstBuf.Ptr());
    TUint8* spsPtr;
    TUint8* ppsPtr;

    TUint numSPS = 0;
    TUint numPPS = 0;

    TUint totalSPSLength = 0;
    TUint totalPPSLength = 0;

    TUint headerLength = aBuffer->Data().Length();
    TUint endIndex = headerLength;

   	TInt nalType = 0;
   	TUint nalLength;
   	TUint nalIndex;
    TUint nalOffset;

   	// Allocate memory for the temporary buffers
   	HBufC8* temp1 = (HBufC8*) HBufC8::NewLC(1000);
   	HBufC8* temp2 = (HBufC8*) HBufC8::NewLC(5000);

    spsPtr = const_cast<TUint8*>( temp1->Des().Ptr() );
    ppsPtr = const_cast<TUint8*>( temp2->Des().Ptr() );

    TUint numNalUnits = inputPtr[endIndex-4] + (inputPtr[endIndex-3]<<8) + (inputPtr[endIndex-2]<<16) + (inputPtr[endIndex-1]<<24);
    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL Nal Unit count: %d"), numNalUnits));    
    
	// Move endIndex to point to the first NAL unit's offset information
    endIndex = headerLength - numNalUnits*8 - 4;
	nalIndex = 0;

	while (nalIndex < numNalUnits)
        {
    	nalIndex++;

    	TInt tmp1 = inputPtr[endIndex++];
        TInt tmp2 = inputPtr[endIndex++]<<8;
        TInt tmp3 = inputPtr[endIndex++]<<16;
        TInt tmp4 = inputPtr[endIndex++]<<24;

    	nalOffset = tmp1 + tmp2 + tmp3 + tmp4;
        PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL Nal Unit start offset: %d"), nalOffset));    	

    	tmp1 = inputPtr[endIndex++];
        tmp2 = inputPtr[endIndex++]<<8;
        tmp3 = inputPtr[endIndex++]<<16;
        tmp4 = inputPtr[endIndex++]<<24;

    	nalLength = tmp1 + tmp2 + tmp3 + tmp4;
        PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL Nal Unit length: %d"), nalLength));    	

	   	nalType = inputPtr[nalOffset] & 0x1F;
        PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL Nal Unit type: %d"), nalType));
        
	   	if(nalType == 7)
		    {
	   		numSPS++;

		    // First store the SPS unit length with two bytes
   			spsPtr[totalSPSLength] = (nalLength >> 8) & 0xFF;
    		spsPtr[totalSPSLength+1] = nalLength & 0xFF;

			// Copy the SPS unit to the buffer
			Mem::Copy(&spsPtr[totalSPSLength+2], inputPtr+nalOffset , nalLength);
	        PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL stored SPS from offset: %d size: %d"), inputPtr+nalOffset, nalLength));			
			totalSPSLength += nalLength + 2;	// Two more for the size
		    }
  		else if(nalType == 8)
   		    {
			numPPS++;

		    // First store the SPS unit length with two bytes
   			ppsPtr[totalPPSLength] = (nalLength >> 8) & 0xFF;
    		ppsPtr[totalPPSLength+1] = nalLength & 0xFF;

			// Copy the SPS unit to the buffer
			Mem::Copy(&ppsPtr[totalPPSLength+2], inputPtr+nalOffset , nalLength);
            PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderNALL stored PPS from offset: %d size: %d"), inputPtr+nalOffset, nalLength));			
			totalPPSLength += nalLength + 2;	// Two more for the size
	   	    }
		else
   		    {
   		    }
        }

	// When the header has been parsed, form the AVC Decoder Configuration Record
	outputPtr[0] = 0x01;	// configurationVersion
	// AVCProfileIndication contains the profile code as defined in the AVC specification
    if (iFileCodecType & MP4_TYPE_AVC_PROFILE_BASELINE)
        {
        outputPtr[1] = 0x42;
        outputPtr[2] = 0x80;    // Bitstream obeys all Baseline profile constraints.
        // Profile compatibility, i.e. all 4 constrain set flags + reserved 4 zero bits
        if ( iAVCOutputLevel == 101 )
            {
            outputPtr[2] |= 0x10; // For level 1b, the 4th bit shall be == 1, otherwise it must be zero
            }
        // AVCLevelIndication contains the level code as defined in the AVC specification
        outputPtr[3] = (iAVCOutputLevel == 101) ? 0x0B : iAVCOutputLevel;
        }
    else if (iFileCodecType & MP4_TYPE_AVC_PROFILE_MAIN)
        {
        outputPtr[1] = 0x4D;
        outputPtr[2] = 0x40;    // Bitstream obeys all main profile constraints.
        if ( iAVCOutputLevel == 101 )
            {
            outputPtr[2] |= 0x10; // For level 1b, the 4th bit shall be == 1, otherwise it must be zero
            }
        // AVCLevelIndication contains the level code as defined in the AVC specification
        outputPtr[3] = (iAVCOutputLevel == 101) ? 0x0B : iAVCOutputLevel;
        }
    else if (iFileCodecType & MP4_TYPE_AVC_PROFILE_HIGH)
        {
        outputPtr[1] = 0x64;
        outputPtr[2] = 0x40;    // Bitstream obeys all Baseline profile constraints.
        outputPtr[3] = (iAVCOutputLevel == 101) ? 0x09 : iAVCOutputLevel;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }

	// lengthSizeMinusOne indicates the length in bytes of the NALUnitLength field minus one.
	outputPtr[4] = 0x03; // 4 bytes
	outputPtr[4] |= 0x0FC; // 6 reserved bits (all 1)
	// numOfSequenceParameterSets indicates the number of sequence parameter sets
	outputPtr[5] = numSPS;
	outputPtr[5] |= 0xE0;  // 3 reserved bits (all 1)

	TInt len = 6;
	// Copy the SPS unit(s) to the buffer
	Mem::Copy(&outputPtr[6], spsPtr , totalSPSLength);
	len += totalSPSLength;
	outputPtr[6+totalSPSLength] = numPPS;
	len += 1;

	// Copy the PPS unit(s) to the buffer
	Mem::Copy(&outputPtr[6+totalSPSLength+1], ppsPtr , totalPPSLength);
	len += totalPPSLength;
	aDstBuf.SetLength(len);

	CleanupStack::PopAndDestroy(temp2);
	CleanupStack::PopAndDestroy(temp1);
}

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL
//
// Convert AVC specific decoder config info  from Bytestream (ElementaryStream) encapsulation to
// AVC Decoder Configuration Record -format
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL( CCMRMediaBuffer* aBuffer, TDes8& aDstBuf )
    {
    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL in")));
	TUint8* inputPtr = (TUint8*)(aBuffer->Data().Ptr());
    TUint8* outputPtr = (TUint8*)(aDstBuf.Ptr());
    TUint8* spsPtr;
    TUint8* ppsPtr;

    TUint numSPS = 0;
    TUint numPPS = 0;

    TUint totalSPSLength = 0;
    TUint totalPPSLength = 0;

    TUint headerLength = aBuffer->Data().Length();
    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL buffer length: %d"), headerLength));

   	TInt nalType = 0;

   	// Allocate memory for the temporary buffers
   	HBufC8* temp1 = (HBufC8*) HBufC8::NewLC(1000);
   	HBufC8* temp2 = (HBufC8*) HBufC8::NewLC(5000);

    spsPtr = const_cast<TUint8*>( temp1->Des().Ptr() );
    ppsPtr = const_cast<TUint8*>( temp2->Des().Ptr() );

    // scan from beginning of buffer to end for SPS and PSP
    TInt i = 0;
    TInt j = 0;
    for (i=0; i<headerLength; i++)
    	{
    	if ( inputPtr[i] == 0 &&
    		 inputPtr[i+1] == 0 &&
    		 inputPtr[i+2] == 1 )
    		{ // found bytestream header [(00) 00 00 01]
    	    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL found header at: %d"), i));
    	   	nalType = inputPtr[i+3] & 0x1F;
            PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL NAL type: %d"), nalType));
    	   	if(nalType == 7) // SPS
    	   		{
    	   		numSPS++;
    	   		// find length of SPS
    	   		TInt j;
    	   		for (j=3; i+j+3<headerLength; j++)
    	   			{
    	   	    	if ( inputPtr[i+j] == 0 &&
    	   	    		 inputPtr[i+j+1] == 0 &&
    	   	    		 inputPtr[i+j+2] == 1 )
    	   	    		{
    	   	    		totalSPSLength = j-i-3;
    	   	            PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL SPS length: %d, count: %d"), totalSPSLength, numSPS));
    	   	    		break;
    	   	    		}
    	   			}
    	   		// if we didn't find next bytestream header then this is last buffer
    	   		if ( totalSPSLength == 0 )
    	   			{
    	   			totalSPSLength = headerLength - i - 3;
                    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL SPS length: %d (last), count: %d"), totalSPSLength, numSPS));
    	   			}

    		    // First store the SPS unit length with two bytes
       			spsPtr[0] = (totalSPSLength >> 8) & 0xFF;
        		spsPtr[1] = totalSPSLength & 0xFF;

        		// Copy the SPS unit to the buffer
    			Mem::Copy(&spsPtr[2], &inputPtr[i+3] , totalSPSLength);
    			totalSPSLength +=2;
    	   		}
    	   	else if ( nalType == 8 ) // PPS)
    	   		{
    	   		numPPS++;
    	   		// find length of PPS
    	   		for (j=3; i+j+3<headerLength; j++)
    	   			{
    	   	    	if ( inputPtr[i+j] == 0 &&
    	   	    		 inputPtr[i+j+1] == 0 &&
    	   	    		 inputPtr[i+j+2] == 1 )
    	   	    		{
    	   	    		totalPPSLength = j-i-3;
                        PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL PPS length: %d, count: %d"), totalPPSLength, numPPS));
    	   	    		break;
    	   	    		}
    	   			}
    	   		// if we didn't find next bytestream header then this is last buffer
    	   		if ( totalPPSLength == 0 )
    	   			{
    	   			totalPPSLength = headerLength - i - 3;
                    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL PPS length: %d (last), count: %d"), totalPPSLength, numPPS));
    	   			}

    		    // First store the SPS unit length with two bytes
    	   		ppsPtr[0] = (totalPPSLength >> 8) & 0xFF;
    	   		ppsPtr[1] = totalPPSLength & 0xFF;

    			// Copy the SPS unit to the buffer
    			Mem::Copy(&ppsPtr[2], &inputPtr[i+3], totalPPSLength);
    	   		totalPPSLength +=2;
    	   		}
    		}
    	}

	// When the header has been parsed, form the AVC Decoder Configuration Record
	outputPtr[0] = 0x01;	// configurationVersion
    // AVCProfileIndication contains the profile code as defined in the AVC specification
    if (iFileCodecType & MP4_TYPE_AVC_PROFILE_BASELINE)
        {
        outputPtr[1] = 0x42;
        outputPtr[2] = 0x80;    // Bitstream obeys all Baseline profile constraints.
        // Profile compatibility, i.e. all 4 constrain set flags + reserved 4 zero bits
        if ( iAVCOutputLevel == 101 )
            {
            outputPtr[2] |= 0x10; // For level 1b, the 4th bit shall be == 1, otherwise it must be zero
            }
        // AVCLevelIndication contains the level code as defined in the AVC specification
        outputPtr[3] = (iAVCOutputLevel == 101) ? 0x0B : iAVCOutputLevel;
        }
    else if (iFileCodecType & MP4_TYPE_AVC_PROFILE_MAIN)
        {
        outputPtr[1] = 0x4D;
        outputPtr[2] = 0x40;    // Bitstream obeys all main profile constraints.
        if ( iAVCOutputLevel == 101 )
            {
            outputPtr[2] |= 0x10; // For level 1b, the 4th bit shall be == 1, otherwise it must be zero
            }
        // AVCLevelIndication contains the level code as defined in the AVC specification
        outputPtr[3] = (iAVCOutputLevel == 101) ? 0x0B : iAVCOutputLevel;
        }
    else if (iFileCodecType & MP4_TYPE_AVC_PROFILE_HIGH)
        {
        outputPtr[1] = 0x64;
        outputPtr[2] = 0x40;    // Bitstream obeys all Baseline profile constraints.
        outputPtr[3] = (iAVCOutputLevel == 101) ? 0x09 : iAVCOutputLevel;
        }
    else
        {
        User::Leave(KErrNotSupported);
        }
    
	// lengthSizeMinusOne indicates the length in bytes of the NALUnitLength field minus one.
	outputPtr[4] = 0x03; // 4 bytes
	outputPtr[4] |= 0x0FC; // 6 reserved bits (all 1)
	// numOfSequenceParameterSets indicates the number of sequence parameter sets
	outputPtr[5] = numSPS;
	outputPtr[5] |= 0xE0;  // 3 reserved bits (all 1)

	TInt len = 6;

	// Copy the SPS unit(s) to the buffer
	Mem::Copy(&outputPtr[6], spsPtr , totalSPSLength);
	len += totalSPSLength;
	outputPtr[6+totalSPSLength] = numPPS;
	len += 1;

	// Copy the PPS unit(s) to the buffer
	Mem::Copy(&outputPtr[6+totalSPSLength+1], ppsPtr , totalPPSLength);
	len += totalPPSLength;
	aDstBuf.SetLength(len);

	CleanupStack::PopAndDestroy(temp2);
	CleanupStack::PopAndDestroy(temp1);
    PRINT((_L("CCamC3GPDataSinkImp::ConvertAVCHeaderByteStreamL out")));
}

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL
//
// Converts AVC frame from Bytestream (ElementaryStream) encapsulation to
// file format AVC sample structure by replacing bytestream headers with NAL unit sizes.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizes( CCMRMediaBuffer* aBuffer )
    {
    PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL in")));
    TUint8* inputPtr = (TUint8*)(aBuffer->Data().Ptr());
    TUint headerLength = aBuffer->Data().Length();
    PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL buffer length: %d"), headerLength));

    TInt nalLength = 0;
    TInt outputOffset = 0;
    TBool moreThanOneNAL = EFalse;
    TInt i = 0;
    TInt j = 0;
    for (i=0; i<headerLength-2; i++)
        {
        if ( inputPtr[i] == 0 &&
             inputPtr[i+1] == 0 &&
             inputPtr[i+2] == 1 )
            { // found bytestream header [(00) 00 00 01]
            PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL found header at: %d"), i));
            PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL NAL type: %d"), TInt(inputPtr[i+3] & 0x1F) ));
            if (moreThanOneNAL)
                {// we found start of next NAL unit in memory buffer so update previous size
                nalLength = i-j-3; // 3 is the bytestream header
                PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL NAL length: %d, toPos: %d"), nalLength, outputOffset));
                iVideoBuffer[outputOffset] = TUint8((nalLength >> 24) & 0xff);
                iVideoBuffer[outputOffset+1] = TUint8((nalLength >> 16) & 0xff);
                iVideoBuffer[outputOffset+2] = TUint8((nalLength >> 8) & 0xff);
                iVideoBuffer[outputOffset+3] = TUint8(nalLength & 0xff);
                PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL copy to:%d, from:%d, amount:%d"), outputOffset+4, j+3, nalLength ));
                Mem::Copy(iVideoBuffer+outputOffset+4, inputPtr+j+3, nalLength);
                outputOffset += 4 + nalLength; // 4 bytes for length information.   
                }
            moreThanOneNAL = ETrue;
            j=i;
            }
        }
    // and update last (or if only 1 NAL size:
    nalLength = headerLength-j-3; // 3 is the bytestream header
    PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL last NAL length: %d, toPos: %d"), nalLength, outputOffset));
    iVideoBuffer[outputOffset] = TUint8((nalLength >> 24) & 0xff);
    iVideoBuffer[outputOffset+1] = TUint8((nalLength >> 16) & 0xff);
    iVideoBuffer[outputOffset+2] = TUint8((nalLength >> 8) & 0xff);
    iVideoBuffer[outputOffset+3] = TUint8(nalLength & 0xff);
    PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL copy to:%d, from:%d, amount:%d"), outputOffset+4, j+3, nalLength ));    
    Mem::Copy(iVideoBuffer+outputOffset+4, inputPtr+j+3, nalLength);
    
    outputOffset += 4 + nalLength; // 4 bytes for length information.
    iVideoBufferFrameSize = outputOffset;
    PRINT((_L("CCamC3GPDataSinkImp::ConvertBytestreamHeadersToNALSizesL out")));
}

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes
//
// Converts AVC frame from NAL (EGenericPayload) encapsulation to
// file format AVC sample structure by replacing NAL encapsulation with NAL unit sizes.
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes( CCMRMediaBuffer* aBuffer )
    {
    PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes in")));
    TUint8* inputPtr = (TUint8*)(aBuffer->Data().Ptr());
    TUint bufferLength = aBuffer->Data().Length();
    PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes buffer length: %d"), bufferLength));

    // Offset to the end and get NAL unit count
    TInt offset = bufferLength-4; //last 4 bytes are the NAL unit count
    TInt nalCount = TInt(inputPtr[offset]) +
                   (TInt(inputPtr[offset + 1]) << 8) +
                   (TInt(inputPtr[offset + 2]) << 16) +
                   (TInt(inputPtr[offset + 3]) << 24);

    TInt frameStart = 0;
    TInt frameSize = 0;
    TInt outputOffset = 0;
    for(TInt i=0; i<nalCount; i++)
        {//go through all NAL units in buffer
        // Offset to the start of NAL Unit infos
        offset = bufferLength-4-(8*nalCount); // 4 is the NAL unit count at end of buffer, 8 bytes used per NAL Unit for FrameStartOffset and FrameSize.

        // Get frame start offset
        offset += 8*i;
        frameStart = TInt(inputPtr[offset]) +
                    (TInt(inputPtr[offset + 1]) << 8) +
                    (TInt(inputPtr[offset + 2]) << 16) +
                    (TInt(inputPtr[offset + 3]) << 24);

        PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes() NAL unit %d frame start: %d "), i, frameStart ));

        // Get frame size
        offset += 4;
        frameSize = TInt(inputPtr[offset]) +
                   (TInt(inputPtr[offset + 1]) << 8) +
                   (TInt(inputPtr[offset + 2]) << 16) +
                   (TInt(inputPtr[offset + 3]) << 24);

       PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes() NAL unit %d frame size: %d "), i, frameSize ));
       PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes() NAL unit %d type: %d "), i, TInt(inputPtr[frameStart] & 0x1F) ));

       iVideoBuffer[outputOffset] = TUint8((frameSize >> 24) & 0xff);
       iVideoBuffer[outputOffset+1] = TUint8((frameSize >> 16) & 0xff);
       iVideoBuffer[outputOffset+2] = TUint8((frameSize >> 8) & 0xff);
       iVideoBuffer[outputOffset+3] = TUint8(frameSize & 0xff);

       Mem::Copy(iVideoBuffer+outputOffset+4, inputPtr+frameStart, frameSize);
       outputOffset += 4 + frameSize; // 4 bytes for length information.
       }
    iVideoBufferFrameSize = outputOffset;
    PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes() new video buffer size: %d "), iVideoBufferFrameSize ));
    PRINT((_L("CCamC3GPDataSinkImp::ConvertNALEncapsulationToNALSizes out")));
    }

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::ComposeUDTA
//
// Compose UDTA (User Data) to video file
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::ComposeUDTAL()
    {
    mp4_u32 udtaBufferSize = 10240;
    HBufC8* udtaBuffer = HBufC8::NewL(udtaBufferSize);
    TUint8* buf = CONST_CAST(TUint8*, udtaBuffer->Ptr());
    CleanupStack::PushL(udtaBuffer);
    udtaBuffer->Des().FillZ(udtaBuffer->Des().MaxLength());

    mp4_u32  i = 0;
    
    // Compose free atom 
    // BoxHeader.Size      Unsigned int(32)  size of free atom
    LittleEndianPut32(buf+i, (mp4_u32)udtaBufferSize-i);
    i += 4;
    // BoxHeader.Type      Unsigned int(32)    'free'
    LittleEndianPut32(buf+i, (mp4_u32)0x66726565); //'free' indentifier for free atom
    
    
    MP4Err error;
    mp4_u8 udtalocation = MP4_UDTA_MOOV; // store UDTA to moov e.g. movie structure instead of individual track.
    error = MP4ComposeSetUserDataAtom(iMP4Handle, udtalocation, (mp4_u8 *)udtaBuffer->Des().Ptr(), udtaBufferSize);
    
    if ( error )
        {
        PRINT((_L("CCamC3GPDataSinkImp::MP4ComposeSetUserDataAtom error=%d"), error));
        User::Leave(KErrGeneral);    
        }

    CleanupStack::PopAndDestroy(udtaBuffer);
    }

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::LittleEndianPut32
//
// Convert 32bit unsigned value to little endian format into buffer
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::LittleEndianPut32(TUint8 *aPtr, TUint32 aVal)
    {
    mp4_u32 result;
    ((mp4_u8 *)&result)[0] = ((mp4_u8 *)&aVal)[3];
    ((mp4_u8 *)&result)[1] = ((mp4_u8 *)&aVal)[2];
    ((mp4_u8 *)&result)[2] = ((mp4_u8 *)&aVal)[1];
    ((mp4_u8 *)&result)[3] = ((mp4_u8 *)&aVal)[0];
    memcpy(aPtr, &result, 4);
    }

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::LittleEndianPut16
//
// Convert 16bit unsigned value to little endian format into buffer
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::LittleEndianPut16(TUint8 *aPtr, TUint16 aVal)
    {
    mp4_u16 result;
    ((mp4_u8 *)&result)[0] = ((mp4_u8 *)&aVal)[1];
    ((mp4_u8 *)&result)[1] = ((mp4_u8 *)&aVal)[0];
    memcpy(aPtr, &result, 2);
    }

// -----------------------------------------------------------------------------
// CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName
// -----------------------------------------------------------------------------
//
void CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName( MP4FileName tempFileName )
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMC3GPDATASINKIMP_M3GPMP4LIBDELETETEMPFILENAME, "e_CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName 1" );
    PRINT((_L("CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName entering")));
    MP4FileName* tempFileNamePtr = NULL;
    TInt result = KErrNoMemory;

    // Add image to the queue.
    tempFileNamePtr = new MP4FileName;
    PRINT((_L("CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName tempFileName=%x, tempFileNamePtr=%x"), tempFileName, tempFileNamePtr));
    if ( tempFileNamePtr && iDeleteFileQueue )
        {
        PRINT((_L("CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName file count=%d"), iDeleteFileQueue->Count()));
        
        *tempFileNamePtr = tempFileName;
        result = iDeleteFileQueue->Append( tempFileNamePtr );
        }
    if ( result != KErrNone ) // Append failed -> do sync remove
        {
        TInt err = wremove( tempFileName );
        PRINT((_L("CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName wremove sync err=%d, tempFileName=%x"), err, tempFileName));
        free(tempFileName);
        tempFileName = 0;
        if ( tempFileNamePtr )
            {
            delete tempFileNamePtr;
            tempFileNamePtr = 0;
            }
        }
    else  // Append OK, start async delete if not running already
        {
        if (iDeleteFileQueue->Count())
            {

            if ( !iIdleDelete->IsActive() )
                {
                PRINT((_L("CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName() Start IdleDelete, file count=%d"), iDeleteFileQueue->Count()));
                iIdleDelete->Start( TCallBack( IdleDelete, this ) );
                }
            }
        }

    PRINT((_L("CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName exiting")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMC3GPDATASINKIMP_M3GPMP4LIBDELETETEMPFILENAME, "e_CCamC3GPDataSinkImp::M3GPMP4LibDeleteTempFileName 0" );
    }


// ---------------------------------------------------------------------------
// CCamC3GPDataSinkImp::IdleDelete
// ---------------------------------------------------------------------------
//
TInt CCamC3GPDataSinkImp::IdleDelete( TAny* aCont )
    {
    CCamC3GPDataSinkImp* appCont = static_cast<CCamC3GPDataSinkImp*>( aCont );
    return ( appCont->DoIdleDelete() );
    }

// ---------------------------------------------------------------------------
// CCamC3GPDataSinkImp::DoIdleDelete
// ---------------------------------------------------------------------------
//
TInt CCamC3GPDataSinkImp::DoIdleDelete()
    {
    OstTrace0( CAMERASRV_PERFORMANCE, CCAMC3GPDATASINKIMP_DOIDLEDELETE, "e_CCamC3GPDataSinkImp::DoIdleDelete 1" );
    PRINT((_L("CCamC3GPDataSinkImp::DoIdleDelete() in")));
    TInt err = KErrNone;
    MP4FileName tempFileName;
    TInt filesLeft = EFalse;

    // Delete one file from queue
    if ( iDeleteFileQueue )
        {
        PRINT((_L("CCamC3GPDataSinkImp::DoIdleDelete() file count=%d"), iDeleteFileQueue->Count()));    
        if ( iDeleteFileQueue->Count() )
            {
            tempFileName = *(*iDeleteFileQueue)[0];
            PRINT((_L("CCamC3GPDataSinkImp::DoIdleDelete index 0:tempFileName=%x %s"), tempFileName, tempFileName));
            delete (*iDeleteFileQueue)[0];
            iDeleteFileQueue->Remove( 0 );
            err = wremove( tempFileName );
            PRINT((_L("CCamC3GPDataSinkImp::DoIdleDelete wremove async err=%d, tempFileName=%x %s"), err, tempFileName, tempFileName));
            err++; // remove compiler warning
            free(tempFileName);
            tempFileName = 0;
            }

        // Start next deletion if queue is not empty
        if ( iDeleteFileQueue->Count() )
            {
            PRINT((_L("CCamC3GPDataSinkImp::DoIdleDelete() continue, file count=%d"), iDeleteFileQueue->Count()));
            filesLeft = ETrue;
            }
        }

    PRINT((_L("CCamC3GPDataSinkImp::DoIdleDelete() out")));
    OstTrace0( CAMERASRV_PERFORMANCE, DUP1_CCAMC3GPDATASINKIMP_DOIDLEDELETE, "e_CCamC3GPDataSinkImp::DoIdleDelete 0" );
    return ( filesLeft );
    }


//  End of File
