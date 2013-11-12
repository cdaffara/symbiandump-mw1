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
* Description:  CCMRAudioCodecs functions for checking supported codecs etc
*                and CCMRAudioCodecData base class implementation
*
*/



// INCLUDE FILES
#include "CCMRAudioCodecs.h"
#include "CCMRAudioCodecData.h"
#include "CCMRAMRNBCodecData.h"
#include "CCMRAacCodecData.h"

#include "CCMRSupportedCodecs.h"
#include <mmf/common/mmfcontrollerpluginresolver.h>
#include <ecom/ecom.h>
#include <mmfplugininterfaceuids.hrh>
#include <mmf/server/sounddevice.h> // devsound
#include <AudioPreference.h>  // For MMF audio preference definitions.


// MACROS

// Debug print macro
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x
#else
#define PRINT(x)
#endif

// ============================ CCMRAudioCodecs MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CCMRAudioInput::NewL
// -----------------------------------------------------------------------------
//
CCMRAudioCodecs* CCMRAudioCodecs::NewL()
    {
    CCMRAudioCodecs* self = new (ELeave) CCMRAudioCodecs;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecs::~CCMRAudioCodecs
// -----------------------------------------------------------------------------
//
CCMRAudioCodecs::~CCMRAudioCodecs()
    {
    PRINT((_L("CCMRAudioCodecs::~CCMRAudioCodecs() in")));
    iAudioTypesHW.Close();
    iAudioTypesSW.Close();

    delete iAudioCodec;
    PRINT((_L("CCMRAudioCodecs::~CCMRAudioCodecs() out")));
    }


// -----------------------------------------------------------------------------
// CCMRAudioInput::ConstructL
// -----------------------------------------------------------------------------
//
void CCMRAudioCodecs::ConstructL()
    {
    PRINT((_L("CCMRAudioCodecs::ConstructL() in")));
    iAudioTypesHW.Reset();
    iAudioTypesSW.Reset();

    // AMR-NB
    CCMRAMRNBCodecData amr(ECodecTypeNA);

    if ( amr.Supported() )
        {
        // 1st check HW codecs
        if ( CheckIfInstalledL(TFourCC(KCMRFourCCIdAMRNB), KMmfUidPluginInterfaceHwDevice, KNullUid) )
            {
            PRINT((_L("CCMRAudioCodecs::GetSupportedAudioCodecsL() AMR HW codec found")));
            iAudioTypesHW.AppendL( TFourCC(KCMRFourCCIdAMRNB) );
            }
        else if ( CheckIfInstalledL(TFourCC(KCMRFourCCIdAMRNB), KMmfUidPluginInterfaceCodec, amr.SWCodecUid() ) )
        // then check for SW codecs
            {
            PRINT((_L("CCMRAudioCodecs::GetSupportedAudioCodecsL() AMR SW codec found")));
            iAudioTypesSW.AppendL( TFourCC(KCMRFourCCIdAMRNB) );
            }
        // else no AMR codecs => nothing to add to the list
        }

    //AAC
    CCMRAACCodecData aac(ECodecTypeNA);

    if ( aac.Supported() )
        {
        // 1st check HW codecs
        if ( CheckIfInstalledL(TFourCC(KCMRFourCCIdMPEG4AAC), KMmfUidPluginInterfaceHwDevice, KNullUid ) )
            {
            PRINT((_L("CCMRAudioCodecs::GetSupportedAudioCodecsL() AAC HW codec found")));
            iAudioTypesHW.AppendL( TFourCC(KCMRFourCCIdMPEG4AAC) );
            }
        else if ( CheckIfInstalledL(TFourCC(KCMRFourCCIdMPEG4AAC), KMmfUidPluginInterfaceCodec, aac.SWCodecUid() ) )
        // then check for SW codecs
            {
            PRINT((_L("CCMRAudioCodecs::GetSupportedAudioCodecsL() AAC SW codec found")));
            iAudioTypesSW.AppendL( TFourCC(KCMRFourCCIdMPEG4AAC) );
            }
        // else no AAC codecs => nothing to add to the list
        }

    PRINT((_L("CCMRAudioCodecs::ConstructL() out")));
    }

// ---------------------------------------------------------
// CCMRAudioCodecs::CheckIfInstalledL
// Helper function to check if given codec FourCC is supported
// in the system, either HW or SW
//
// ---------------------------------------------------------
//
TBool CCMRAudioCodecs::CheckIfInstalledL( const TFourCC& aFourCC, const TInt aUidMmfPluginInterfaceCodec, const TUid& aCodecUid )
    {
    PRINT((_L("CCMRAudioCodecs::CheckIfInstalledL() in") ));
    TBool found = EFalse;

	// Create a match string using a FourCC code
	_LIT8(emptyFourCCString, "    ,    ");
	TBufC8<9> fourCCString(emptyFourCCString);
	TPtr8 fourCCPtr = fourCCString.Des();
	TPtr8 fourCCPtr1(&fourCCPtr[0], 4);
	TPtr8 fourCCPtr2(&fourCCPtr[5], 4 );

	TFourCC srcFourCC(' ','P','1','6');
	srcFourCC.FourCC(&fourCCPtr1);
	aFourCC.FourCC(&fourCCPtr2);

	// Create a TEcomResolverParams structure.
	TEComResolverParams resolverParams;
	resolverParams.SetDataType( fourCCPtr );
	resolverParams.SetWildcardMatch( EFalse );

	RImplInfoPtrArray plugInArray; // Array to return matching decoders in (place on cleanupstack _after_ ListImplementationsL() )

  	TUid UidMmfPluginInterfaceCodec = {aUidMmfPluginInterfaceCodec};

	// ListImplementationsL leaves if it cannot find anything so trap the error and ignore it.
	TRAPD( err, REComSession::ListImplementationsL(UidMmfPluginInterfaceCodec, resolverParams, plugInArray ) );

	CleanupResetAndDestroyPushL(plugInArray);

	if (err == KErrNone)
		{
		if ( plugInArray.Count() != 0 )
            {
            if ( aCodecUid == KNullUid )
                {
			    //match found and accepted; for a HW codec the Uid is not that critical, since the API is more fixed (no ConfigureL with undefined argument),
                //and licensees should be able to use their optimized HW codecs
			    found = CheckIfHWInstalledL( aFourCC );
                }
            else
                {
                for ( TInt i = 0; i < plugInArray.Count(); i++)
                    {
                    // there is a match, but 1st we need to ensure it is the one we have tested with, and that have compatible implementation of ConfigureL
                    PRINT((_L("CCMRAudioCodecs::CheckIfInstalledL() plugin found with Uid 0x%x"), plugInArray[i]->ImplementationUid().iUid ));
                    if ( plugInArray[i]->ImplementationUid() == aCodecUid )
                        {
			            //match accepted
			            found = ETrue;
                        }
                    }
                }
			}
		else
			{
			//no match
			found = EFalse;
			}
		}
	else
		{
		//no match
		found = EFalse;
		}
	CleanupStack::PopAndDestroy();	//plugInArray
    PRINT((_L("CCMRAudioCodecs::CheckIfInstalledL() out, found[%d]"), found));
    return found;
    }

// ---------------------------------------------------------
// CCMRAudioCodecs::CheckIfHWInstalledL
// Helper function to check if given codec FourCC is supported
// by devsound
//
// ---------------------------------------------------------
//
TBool CCMRAudioCodecs::CheckIfHWInstalledL( const TFourCC& aFourCC )
    {
    PRINT((_L("CCMRAudioCodecs::CheckIfHWInstalledL() in") ));
    TBool found = EFalse;

    CMMFDevSound* dev;
    dev = CMMFDevSound::NewL();

    TMMFPrioritySettings prioritySettings;
    prioritySettings.iPriority = KAudioPriorityVideoRecording;
    prioritySettings.iPref =  TMdaPriorityPreference( KAudioPrefVideoRecording );

    RArray<TFourCC> supportedHWTypes;
    CleanupStack::PushL( dev );
    dev->GetSupportedOutputDataTypesL( supportedHWTypes, prioritySettings );
    CleanupStack::PopAndDestroy(dev);	//dev

    for ( TInt i = 0; i < supportedHWTypes.Count(); i++)
        {
        TFourCC fourccFromDevsound = supportedHWTypes[i];
        PRINT((_L("CCMRAudioCodecs::CheckIfHWInstalledL() FourCC supported by devsound: %d"), fourccFromDevsound.FourCC() ));
        if ( fourccFromDevsound == aFourCC )
            {
            PRINT((_L("CCMRAudioCodecs::CheckIfHWInstalledL() FourCC match found to %d"), fourccFromDevsound.FourCC() ));
            //match accepted
            found = ETrue;
            }
        }
    supportedHWTypes.Close();
    PRINT((_L("CCMRAudioCodecs::CheckIfHWInstalledL() out, found[%d]"), found));
    return found;
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecs::GetSupportedAudioCodecsL
// Get the supported & installed audio codecs.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCMRAudioCodecs::GetSupportedAudioCodecsL( RArray<TFourCC>& aAudioTypes )
    {
    TInt i;

    for( i = 0; i < iAudioTypesHW.Count(); i++ )
        {
        PRINT((_L("CCMRAudioCodecs::GetSupportedAudioCodecsL() HW codec found, FourCC %d"), iAudioTypesHW[i].FourCC() ));
        aAudioTypes.AppendL( iAudioTypesHW[i] );
        }
    for( i = 0; i < iAudioTypesSW.Count(); i++ )
        {
        PRINT((_L("CCMRAudioCodecs::GetSupportedAudioCodecsL() SW codec found, FourCC %d"), iAudioTypesSW[i].FourCC() ));
        aAudioTypes.AppendL( iAudioTypesSW[i] );
        }
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecs::CheckIfSupportedL
// Check if the given codec FourCC is supported & installed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCMRAudioCodecs::CheckIfSupportedL( const TFourCC& aAudioCodec )
    {
    TInt i;
    TFourCC tmpFourCC = aAudioCodec;

    if ( aAudioCodec == KFourCCNULL )
        {
        // use default
        tmpFourCC = TFourCC(KCMRFourCCIdAMRNB);
        }

    // search from the lists
    for( i = 0; i < iAudioTypesHW.Count(); i++ )
        {
        if ( iAudioTypesHW[i] == tmpFourCC )
            {
            PRINT((_L("CCMRAudioCodecs::CheckIfSupportedL() the codec is in HW")));
            return ETrue;
            }
        }
    for( i = 0; i < iAudioTypesSW.Count(); i++ )
        {
        if ( iAudioTypesSW[i] == tmpFourCC )
            {
            PRINT((_L("CCMRAudioCodecs::CheckIfSupportedL() the codec is in SW")));
            return ETrue;
            }
        }

    // no match
    PRINT((_L("CCMRAudioCodecs::CheckIfSupportedL() no codec exist with given fourCC")));
    return EFalse;
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecs::GetCodecSWHWTypeL
// Returns the codec type (SW/HW)
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TCMRCodecType CCMRAudioCodecs::GetCodecSWHWTypeL( const TFourCC& aCodec )
    {

    TInt i;
    // search from the lists
    for( i = 0; i < iAudioTypesHW.Count(); i++ )
        {
        if ( iAudioTypesHW[i] == aCodec )
            {
            PRINT((_L("CCMRAudioCodecs::GetCodecTypeL() the codec is in HW")));
            return ECodecTypeHW;
            }
        }
    for( i = 0; i < iAudioTypesSW.Count(); i++ )
        {
        if ( iAudioTypesSW[i] == aCodec )
            {
            PRINT((_L("CCMRAudioCodecs::GetCodecTypeL() the codec is in SW")));
            return ECodecTypeSW;
            }
        }

    // no match
    PRINT((_L("CCMRAudioCodecs::GetCodecTypeL() no codec exist with given fourCC")));
    return ECodecTypeNA;
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecs::SetAudioCodecL
// Set the audio codec to use
// -----------------------------------------------------------------------------
//
void CCMRAudioCodecs::SetAudioCodecL( const TFourCC& aAudioCodec )
    {
    if ( !CheckIfSupportedL( aAudioCodec ) )  // CheckIfSupportedL can interpret KFourCCNULL as default AMR-NB
        {
        // the requested codec does not exist or is not supported
        User::Leave( KErrNotSupported );
        // The original functionality was so that codec's existence was checked only in PrepareL,
        // and if it didn't exist, recording was video-only. The same behaviour is not any more supported,
        // mainly because the old behaviour was a restriction from lower layer components (Symbian datapath)
        // and was considered problematic if the leave with KErrNotSupported occurred only in PrepareL
        // although OpenL or SetCodecL were successful. It is now considered better to leave in setup phase
        // if the requested codec doesn't exist or is not supported
        }
    // All ok, we can change the codec
    delete iAudioCodec;
    iAudioCodec = NULL;

    if ( aAudioCodec == KCMRFourCCIdMPEG4AAC )
        {
        PRINT((_L("CCMRAudioCodecs::SetAudioCodecL() AAC selected")));
        iAudioCodec = new (ELeave) CCMRAACCodecData(GetCodecSWHWTypeL(aAudioCodec));
        }
    else if ( aAudioCodec == KCMRFourCCIdAMRNB )
        {
        PRINT((_L("CCMRAudioCodecs::SetAudioCodecL() AMR-NB selected")));
        iAudioCodec = new (ELeave) CCMRAMRNBCodecData(GetCodecSWHWTypeL(aAudioCodec));
        }
    else    // AMR-NB is the default
        {
        PRINT((_L("CCMRAudioCodecs::SetAudioCodecL() default codec selected, use AMR-NB")));
        iAudioCodec = new (ELeave) CCMRAMRNBCodecData(GetCodecSWHWTypeL(TFourCC(KCMRFourCCIdAMRNB)));
        }
    }


// -----------------------------------------------------------------------------
// CCMRAudioCodecs::GetCodecDataL
// Get the audio codec data object
// -----------------------------------------------------------------------------
//
CCMRAudioCodecData* CCMRAudioCodecs::GetCodecDataL()
    {
    return iAudioCodec;
    }







// ============================ CCMRAudioCodecData base class MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCMRAudioCodecData::~CCMRAudioCodecData
// -----------------------------------------------------------------------------
//
CCMRAudioCodecData::~CCMRAudioCodecData()
    {
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecData::GetCodecFourCCL
// Returns the FourCC of the codec
// -----------------------------------------------------------------------------
//
TFourCC CCMRAudioCodecData::GetCodecFourCCL() const
    {
    return iFourCC;
    }

// -----------------------------------------------------------------------------
// CCMRAudioCodecData::BufferType
// Returns the CCMRMediaSink/CCMRMediaBuffer compatible type of the codec buffer
// -----------------------------------------------------------------------------
//
CCMRMediaBuffer::TBufferType CCMRAudioCodecData::BufferType() const
    {
    return iBufferType;
    }


// -----------------------------------------------------------------------------
// CCMRAudioCodecData::GetDecoderConfigInfoLC
// Get the decoder configuration info == NULL by default, inherited classes may
// provide real info if needed
// -----------------------------------------------------------------------------
//
HBufC8* CCMRAudioCodecData::GetDecoderConfigInfoLC()
    {
    return NULL;
    }
