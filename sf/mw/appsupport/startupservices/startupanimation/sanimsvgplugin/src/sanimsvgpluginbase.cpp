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
* Description:  Implementation of CSAnimSvgPluginBase class
*
*/


#include <SVGEngineInterfaceImpl.h>

#include "sanimsvgpluginbase.h"
#include "assert.h"
#include "trace.h"

/** Default SMIL fit value to use. */
_LIT( KDefaultSmilValue, "meetfill" );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::CSAnimSvgPluginBase
//
// ---------------------------------------------------------------------------
//
CSAnimSvgPluginBase::CSAnimSvgPluginBase( TAny* aConstructionParameters )
  : CSAnimImagePlugin( aConstructionParameters ),
    iEngine( NULL )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::~CSAnimSvgPluginBase
//
// ---------------------------------------------------------------------------
//
CSAnimSvgPluginBase::~CSAnimSvgPluginBase()
    {
    FUNC_LOG;

    delete iEngine;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::ScriptCall
//
// ---------------------------------------------------------------------------
//
TBool CSAnimSvgPluginBase::ScriptCall(
    const TDesC& /*aScript*/,
    CSvgElementImpl* /*aCallerElement*/ )
    {
    FUNC_LOG;
    return EFalse;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::FetchImage
//
// ---------------------------------------------------------------------------
//
TInt CSAnimSvgPluginBase::FetchImage(
    const TDesC& /*aUri*/,
    RFs& /*aSession*/,
    RFile& /*aFileHandle*/ )
    {
    FUNC_LOG;
    return KErrNotSupported;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::FetchFont
//
// ---------------------------------------------------------------------------
//
TInt CSAnimSvgPluginBase::FetchFont(
    const TDesC& /*aUri*/,
    RFs& /*aSession*/,
    RFile& /*aFileHandle*/ )
    {
    FUNC_LOG;
    return KErrNotSupported;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::GetSmilFitValue
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPluginBase::GetSmilFitValue( TDes& aSmilValue )
    {
    FUNC_LOG;
    // this is one of "fill", "meet", "slice", or "".
	aSmilValue = KDefaultSmilValue;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::UpdatePresentation
//
// ---------------------------------------------------------------------------
//
void CSAnimSvgPluginBase::UpdatePresentation( const TInt32& /*aNoOfAnimation*/ )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CSAnimSvgPluginBase::SvgToSymbianErr
//
// ---------------------------------------------------------------------------
//
TInt CSAnimSvgPluginBase::SvgToSymbianErr( MSvgError* aError )
    {
    FUNC_LOG;

    if ( aError && aError->HasError() )
        {
        INFO_1( "Svg error code: %d", aError->ErrorCode() );

        switch ( aError->ErrorCode() )
            {
            case ESvgNoError: return KErrNone;
            case ESvgFileNotFound: return KErrNotFound;
            case ESvgDocumentNotValid: return KErrCorrupt;
            case ESvgDocumentNotAvailable: return KErrNotFound;
            case ESvgNoMemory: return KErrNoMemory;
            case ESvgDiskFull: return KErrDiskFull;
            case ESvgUnknown: return KErrGeneral;
            case ESvgMissingRequiredAttribute: return KErrArgument;
            case ESvgInvalidAttributeValue: return KErrArgument;
            case ESvgbFileNotValid: return KErrCorrupt;
            case ESvgDRMFailure: return KErrPermissionDenied;
            case ESvgThumbNailRestriction: return KErrPermissionDenied;
            default: return KErrGeneral;
            };
        }

    return KErrNone;
    }
