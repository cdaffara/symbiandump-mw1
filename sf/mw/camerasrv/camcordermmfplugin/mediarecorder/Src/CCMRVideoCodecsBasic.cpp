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
*                basic variant (no MPEG4-support enabled in the platform)
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


    CleanupStack::PopAndDestroy( &encoders ); // calls also encoders.Close()
    CleanupStack::PopAndDestroy( tmpDevvr ); 
    }
