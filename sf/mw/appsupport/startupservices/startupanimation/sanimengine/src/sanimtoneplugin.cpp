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
* Description:  Implementation of CSAnimTonePlugin class
*
*/


#include "sanimtoneplugin.h"

#include "trace.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// Constructor
//
// -----------------------------------------------------------------------------
//
EXPORT_C CSAnimTonePlugin::CSAnimTonePlugin()
  : iVolume( 0 ),
    iVolumeRamp( 0 )
    {
    FUNC_LOG;
    }


// -----------------------------------------------------------------------------
// CSAnimTonePlugin::SetVolume
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSAnimTonePlugin::SetVolume( const TInt aVolume )
    {
    FUNC_LOG;

    iVolume = aVolume;
    }


// -----------------------------------------------------------------------------
// CSAnimTonePlugin::SetVolumeRamp
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSAnimTonePlugin::SetVolumeRamp(
    const TTimeIntervalMicroSeconds& aVolumeRamp )
    {
    FUNC_LOG;

    iVolumeRamp = aVolumeRamp;
    }
