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
* Description:  Implementation of CSAnimImagePlugin class
*
*/


#include "sanimimageplugin.h"

#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Constructor
//
// -----------------------------------------------------------------------------
//
EXPORT_C CSAnimImagePlugin::CSAnimImagePlugin( TAny* aConstructionParameters )
  : iObserver(
        *( static_cast<MSAnimObserver*>( aConstructionParameters ) ) ),
    iDisplayMode( EGray2 ),
    iFrameDelay( 0 ),
    iScalingEnabled( ETrue )
    {
    FUNC_LOG;
    }


// -----------------------------------------------------------------------------
// CSAnimImagePlugin::SetDisplayMode
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSAnimImagePlugin::SetDisplayMode( const TDisplayMode aDisplayMode )
    {
    FUNC_LOG;

    iDisplayMode = aDisplayMode;
    }


// -----------------------------------------------------------------------------
// CSAnimImagePlugin::SetSize
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSAnimImagePlugin::SetSize( const TSize& aSize )
    {
    FUNC_LOG;

    iSize = aSize;
    }


// -----------------------------------------------------------------------------
// CSAnimImagePlugin::SetFrameDelay
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSAnimImagePlugin::SetFrameDelay(
    const TTimeIntervalMicroSeconds32& aFrameDelay )
    {
    FUNC_LOG;

    iFrameDelay = aFrameDelay;
    }


// -----------------------------------------------------------------------------
// CSAnimImagePlugin::SetScalingEnabled
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSAnimImagePlugin::SetScalingEnabled( const TBool aEnable )
    {
    FUNC_LOG;

    iScalingEnabled = aEnable;
    }
