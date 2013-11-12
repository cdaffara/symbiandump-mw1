/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for helper class to check supported video codecs,
*                MPEG-4 variant
*
*/


#include "CCMRVideoCodecs.h"
#include "CCMRSupportedCodecs.h"
#include "CCMRVideoSettings.h"
#include <mmf/devvideo/devvideorecord.h>


// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif


// ================= LOCAL FUNCTIONS =======================


// ---------------------------------------------------------
// CheckIfSupported
// Helper function to check if mimetype x is supported in 
// the system
// 
// ---------------------------------------------------------
//
static TBool CheckIfSupportedL( const TDes8& aMimeType, RArray <TUid>& aEncoders, CMMFDevVideoRecord* aDevvr )
    {
    TBool r = EFalse;
    TRAPD( err, aDevvr->FindEncodersL( aMimeType, 0, aEncoders, EFalse ));

    if ( (err == KErrNone) && (aEncoders.Count() > 0) )
        {
        r = ETrue;
        }
    aEncoders.Reset();

    return r;
    }


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCMRVideoCodecs::NewL
//
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCMRVideoCodecs* CCMRVideoCodecs::NewL()
    {
    
    CCMRVideoCodecs* self = new (ELeave) CCMRVideoCodecs;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    
    }


// -----------------------------------------------------------------------------
// CCMRVideoCodecs::ConstructL
//
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CCMRVideoCodecs::ConstructL()
    {
    iVideoMimeTypes = new(ELeave) CDesC8ArrayFlat( 2 ); // 2 is the granularity of the array
    // check what we support and have installed
    GenerateListOfSupportedVideoCodecsL();
    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecs::~CCMRVideoCodecs
// Destructor.
// -----------------------------------------------------------------------------
//
CCMRVideoCodecs::~CCMRVideoCodecs()
    {
    delete iVideoMimeTypes;
    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecs::GetSupportedVideoCodecsL
// Get the supported & installed video codecs.
// This can be called also when the recorder is not open
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoCodecs::GetSupportedVideoCodecsL(CDesC8Array& aVideoMimeTypes) const
    {
    aVideoMimeTypes.Reset();

    // copy from iVideoMimeTypes
    TInt i;
    for ( i = 0; i < iVideoMimeTypes->Count(); i++ )
        {
        aVideoMimeTypes.AppendL( iVideoMimeTypes->MdcaPoint(i) );
        }

    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecs::DoWeSupportThisL
// Check if the given codec is supported & installed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRVideoCodecs::DoWeSupportThisL(const TDesC8& aMimeType) const
    {
    if ( aMimeType == KNullDesC8 )
        {
        // asking to use default codec; we should ensure that all classes use 
        // the same default
        TInt i;
        for ( i = 0; i < iVideoMimeTypes->Count(); i++ )
            {
            if ( iVideoMimeTypes->MdcaPoint(i) == KCMRMimeTypeH263BaselineProfile )
                {
                PRINT((_L("CCMRVideoCodecs::DoWeSupportThisL() - Null codec was given and checked that we support it"))); 
                return ETrue;
                }
            }
        }
    else
        {
        TInt i;
        for ( i = 0; i < iVideoMimeTypes->Count(); i++ )
            {
            if ( iVideoMimeTypes->MdcaPoint(i) == aMimeType )
                {
                PRINT((_L("CCMRVideoCodecs::DoWeSupportThisL() - a MIME-type was given and checked that we support it") )); 
                return ETrue;
                }
            }
        }
    PRINT((_L("CCMRVideoCodecs::DoWeSupportThisL() - given MIME-type is not supported") )); 
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL
// Generate list of supported & installed video codecs.
// Called internally in the construction
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL()
    {
    MMMFDevVideoRecordObserver* nullObserver = NULL;
    CMMFDevVideoRecord* tmpDevvr = NULL;
    tmpDevvr = CMMFDevVideoRecord::NewL( *nullObserver );   //NULL should be acceptable since we are not creating any HW devices here
    CleanupStack::PushL( tmpDevvr );

    RArray <TUid> encoders;
    CleanupClosePushL( encoders );

    iVideoMimeTypes->Reset();

    // check that each supported encoder also exists in the system
    // H.263 with default profile & level (profile 0; duplicate of the next item).
    TBool defH263 = CheckIfSupportedL( (const TDes8&)KCMRMimeTypeH263, encoders, tmpDevvr);
    if ( defH263 )
        {
        // encoder exists => add to the list
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.263 default parameters found"))); 
        iVideoMimeTypes->AppendL( KCMRMimeTypeH263 );
        }
    // profile 0
    TBool profileH263 = CheckIfSupportedL( (const TDes8&)KCMRMimeTypeH263BaselineProfile, encoders, tmpDevvr);
    if ( profileH263 )
        {
        // encoder exists => add to the list
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.263 profile 0 and default level found"))); 
        iVideoMimeTypes->AppendL( KCMRMimeTypeH263BaselineProfile );
        }

    // check what levels are specified in the codec's registration file
    TBool levelsH263 = EFalse;

    TBuf8<256> h263mime;
    h263mime = KCMRMimeTypeH263BaselineProfile;
    TBuf8<256> h263string;
    TPtrC8 levels[KCMRNumSupportedH263Levels] = KCMRH263Levels;

    // First a special case, we have '*' after profile; assume it means any level is supported
    h263string = h263mime;
    h263string += _L8("*");
    levelsH263 = CheckIfSupportedL( (const TDes8&)h263string, encoders, tmpDevvr);
    if ( levelsH263 )
        {
        // found, all the levels we support are supported also by the encoder
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.263 profile 0 and all levels found"))); 
        h263mime += _L8("; level=");
        TInt j;
        for ( j = 0; j < KCMRNumSupportedH263Levels; j++ )
            {
            // encoder exists => add to the list
            h263string = h263mime;
            h263string += levels[j];
            iVideoMimeTypes->AppendL( h263string );
            }
        }
    else
        {
        // go through the levels
        h263mime += _L8("; level=");
        TInt i;

        for ( i = 0; i < KCMRNumSupportedH263Levels; i++ )
            {
            h263string = h263mime;
            h263string += levels[i];
            levelsH263 = CheckIfSupportedL( (const TDes8&)h263string, encoders, tmpDevvr);
            if ( levelsH263 )
                {
                // found, all the levels below this one are supported
                PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.263 profile 0 and level from level-array at position %d found"), i )); 
                TInt j;
                for ( j = i; j < KCMRNumSupportedH263Levels; j++ )
                    {
                    // encoder exists => add to the list
                    h263string = h263mime;
                    h263string += levels[j];
                    iVideoMimeTypes->AppendL( h263string );
                    }
                // done, also the levels below this one were already listed
                break;
                }
            }
        }

    // complement the list with variants of the default, need depends on how plugins were registered & what client is asking
    if ( (!defH263) && ( profileH263 || levelsH263 ) )
        {
        // no short version (default) registered, add also it to the list
        iVideoMimeTypes->AppendL( KCMRMimeTypeH263 ); 
        }
    if ( (!profileH263) && (defH263 || levelsH263) )
        {
        // add also version with details of the default profile
        iVideoMimeTypes->AppendL( KCMRMimeTypeH263BaselineProfile ); 
        }
    if ( (!levelsH263) && (defH263 || profileH263) )
        {
        // add also version with details of the default profile & level
        h263string = h263mime;
        h263string += _L8("; level=10");
        iVideoMimeTypes->AppendL( h263string ); 
        }

    // MPEG-4
    TBool defMPEG4 = CheckIfSupportedL( (const TDes8&)KCMRMimeTypeMPEG4V, encoders, tmpDevvr);
    if ( defMPEG4 )
        {
        // encoder exists => add to the list
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with MPEG4 default parameters found"))); 
        iVideoMimeTypes->AppendL( KCMRMimeTypeMPEG4V );
        }

    TBool levelsMPEG4 = EFalse;

    TInt i;
    TPtrC8 mlevels[KCMRNumSupportedMPEG4ProfileLevels] = KCMRMPEG4Levels;
    // First a special case, we have '*' after profile; assume it means any level is supported
    TBuf8<256> mpeg4mime;
    TBuf8<256> mpeg4string;
    mpeg4mime = KCMRMimeTypeMPEG4VSP;
    mpeg4string = mpeg4mime;
    mpeg4string += _L8("*");
    levelsMPEG4 = CheckIfSupportedL( (const TDes8&)mpeg4string, encoders, tmpDevvr);
    if ( levelsMPEG4 )
        {
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with MPEG4 all profile-level-id's found"))); 
        TInt j;
        for ( j = 0; j < KCMRNumSupportedMPEG4ProfileLevels; j++ )
            {
            // encoder exists => add to the list
            mpeg4string = mpeg4mime;
            mpeg4string += mlevels[j];
            iVideoMimeTypes->AppendL( mpeg4string );
            }
        }
    else
        {
        // go through the levels
        for ( i = 0; i < KCMRNumSupportedMPEG4ProfileLevels; i++ )
            {
            mpeg4string = mpeg4mime;
            mpeg4string += mlevels[i];
            levelsMPEG4 = CheckIfSupportedL( (const TDes8&)mpeg4string, encoders, tmpDevvr);
            if ( levelsMPEG4 )
                {
                // found, all the levels below this one are supported
                PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with MPEG4 profile-level-id from level-array at position %d found"), i )); 
                TInt j;
                for ( j = i; j < KCMRNumSupportedMPEG4ProfileLevels; j++ )
                    {
                    // encoder exists => add to the list
                    mpeg4string = mpeg4mime;
                    mpeg4string += mlevels[j];
                    iVideoMimeTypes->AppendL( mpeg4string );
                    }
                // done, also the levels below this one were already listed
                break;
                }
            }
        }    

    if ( (!defMPEG4) && levelsMPEG4 )
        {
        // no short version registered, only with profile-level-id, add also the short one
        iVideoMimeTypes->AppendL( KCMRMimeTypeMPEG4V ); 
        }
    else if ( defMPEG4 && (!levelsMPEG4) )
        {
        // only short version registered, add also version with details of the default profile & level
        iVideoMimeTypes->AppendL( KCMRMimeTypeMPEG4VSPL1 ); 
        iVideoMimeTypes->AppendL( KCMRMimeTypeMPEG4VSPL0B );    // assuming level 1 implies support for 0b also; this is not clear from MPEG-4 specs
        iVideoMimeTypes->AppendL( KCMRMimeTypeMPEG4VSPL0 ); 
        }

    // H.264 AVC
    TBool defH264AVC = CheckIfSupportedL( (const TDes8&)KCMRMimeTypeH264AVC, encoders, tmpDevvr);
    if ( defH264AVC )
        {
        // encoder exists => add to the list
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.264 AVC default parameters found"))); 
        iVideoMimeTypes->AppendL( KCMRMimeTypeH264AVC );
        }

    TBool levelsH264AVC = EFalse;
    TPtrC8 h264avclevels[KCMRNumSupportedH264AVCProfileLevels] = KCMRH264AVCLevels;
    // First a special case, we have '*' after profile; assume it means any level is supported
    TBuf8<256> h264avcmime;
    TBuf8<256> h264avcstring;
    h264avcmime = KCMRMimeTypeH264AVCProfileId;
    h264avcstring = h264avcmime;
    h264avcstring += _L8("*");
    levelsH264AVC = CheckIfSupportedL( (const TDes8&)h264avcstring, encoders, tmpDevvr);
    if ( levelsH264AVC )
        {
        PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.264 AVC all profile-level-id's found"))); 
        TInt j;
        for ( j = 0; j < KCMRNumSupportedH264AVCProfileLevels; j++ )
            {
            // encoder exists => add to the list
            h264avcstring = h264avcmime;
            h264avcstring += h264avclevels[j];
            iVideoMimeTypes->AppendL( h264avcstring );
            }
        }
    else
        {
        // go through the levels
        for ( i = 0; i < KCMRNumSupportedH264AVCProfileLevels; i++ )
            {
            h264avcstring = h264avcmime;
            h264avcstring += h264avclevels[i];
            levelsH264AVC = CheckIfSupportedL( (const TDes8&)h264avcstring, encoders, tmpDevvr);
            if ( levelsH264AVC )
                {
                // found, all the levels below this one are supported
                PRINT((_L("CCMRVideoCodecs::GenerateListOfSupportedVideoCodecsL() - a codec with H.264 AVC profile-level-id from level-array at position %d found"), i )); 
                TInt j;
                for ( j = i; j < KCMRNumSupportedH264AVCProfileLevels; j++ )
                    {
                    // encoder exists => add to the list
            		h264avcstring = h264avcmime;
            		h264avcstring += h264avclevels[j];
                    iVideoMimeTypes->AppendL( h264avcstring );
                    }
                // done, also the levels below this one were already listed
                break;
                }
            }
        }    

    if ( (!defH264AVC) && levelsH264AVC )
        {
        // no short version registered, only with profile-level-id, add also the short one
        iVideoMimeTypes->AppendL( KCMRMimeTypeH264AVC ); 
        }
    else if ( defH264AVC && (!levelsH264AVC) )
        {
        // only short version registered, add also version with details of the default profile & level
        iVideoMimeTypes->AppendL( KCMRMimeTypeH264AVCBPL10 ); 
        iVideoMimeTypes->AppendL( KCMRMimeTypeH264AVCBPL10b );
        }

    CleanupStack::PopAndDestroy( &encoders ); // calls also encoders.Close()
    CleanupStack::PopAndDestroy( tmpDevvr ); 
    }
