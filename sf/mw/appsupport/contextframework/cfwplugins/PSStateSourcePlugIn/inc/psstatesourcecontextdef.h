/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Call state source context definition.
*
*/



#ifndef PSSTATESOURCECONTEXTDEF_H
#define PSSTATESOURCECONTEXTDEF_H

// Context values definition array
// Values in this array can be compared to descriptors:
// @<code>
//      TBuf<10> buf = _L( "Value" );
//      _LIT_CONTEXT_VALUES( KValues ) =
//          {
//          L"Value"
//          }; 
//      if( buf == TPtrC( KValues[0] )
//          {
//          // Do something
//          }
// @</code>
//
#ifndef _LIT_CONTEXT_VALUES
#define _LIT_CONTEXT_VALUES( x ) static const TText* const x[]
#endif

//------------------------------------------------------------------------------
// Application state source.
// Defines application specific internal states like view state.
// These contexts are mapped from P&S category: KPSStateSourceAppStatePSUid
//------------------------------------------------------------------------------
//

/**
* Source: 'Application'
* Specifies application specific internal states.
*/
_LIT( KPSAppStateSource, "Application" );

/**
* Type: 'MusicPlayer.View'
* Describes in which view Music player application currently is.
*
* Security:
* - ECapabilitySwEvent
* - ECapabilityUserEnvironment
*
* Values:
* - 'Now'
* - 'Other'
*/
_LIT( KPSAppStateTypeMusicPlayerView, "MusicPlayer.View" );

_LIT_SECURITY_POLICY_C2( KPSAppStateTypeMusicPlayerViewSecurity,
    ECapabilitySwEvent,
    ECapabilityUserEnvironment );

// Map these with KPSStateSourceAppStatePSMusicPlayerState
// @see PSStateSourceAppStateDomainPSKeys.h
const TInt KPSAppStateTypeMusicPlayerViewValueCount = 2;
_LIT_CONTEXT_VALUES( KPSAppStateTypeMusicPlayerViewValues ) =
    {
    _S("Now"),
    _S("Other")
    };
    
/**
* Type: 'Gallery.View'
* Describes in which view Gallery application currently is.
*
* Security:
* - ECapabilitySwEvent
* - ECapabilityUserEnvironment
*
* Values:
* - 'Image'
* - 'Other'
*/
_LIT( KPSAppStateTypeGalleryView, "Gallery.View" );

_LIT_SECURITY_POLICY_C2( KPSAppStateTypeGalleryViewSecurity,
    ECapabilitySwEvent,
    ECapabilityUserEnvironment );

// Map these with KPSStateSourceAppStatePSGalleryState
// @see PSStateSourceAppStateDomainPSKeys.h
const TInt KPSAppStateTypeGalleryViewValueCount = 2;
_LIT_CONTEXT_VALUES( KPSAppStateTypeGalleryViewValues ) =
    {
    _S("Image"),
    _S("Other")
    };

/**
* Type: 'NGI.State'
* Describes in NGI inside states.
*
* Security:
* - ECapabilitySwEvent
* - ECapabilityUserEnvironment
*
* Values:
* - 'ForegroundLandscape'
* - 'ForegroundLandscapeUpsideDown'
* - 'ForegroundPortrait'
* - 'Background'
*/
_LIT( KPSAppStateTypeNGIState, "NGI.State" );

_LIT_SECURITY_POLICY_C2( KPSAppStateTypeNGIStateSecurity,
    ECapabilitySwEvent,
    ECapabilityUserEnvironment );

// Map these with KPSStateSourceAppStatePSNGIState
// @see PSStateSourceAppStateDomainPSKeys.h
const TInt KPSAppStateTypeNGIStateValueCount = 4;
_LIT_CONTEXT_VALUES( KPSAppStateTypeNGIStateValues ) =
    {
    _S("ForegroundLandscape"),
    _S("ForegroundLandscapeUpsideDown"),
    _S("ForegroundPortrait"),
    _S("Background")
    };

#endif
