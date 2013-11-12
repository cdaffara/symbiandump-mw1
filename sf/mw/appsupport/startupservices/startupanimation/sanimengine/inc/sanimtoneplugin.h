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
* Description:  Declaration of CSAnimTonePlugin class
*
*/


#ifndef SANIMTONEPLUGIN_H
#define SANIMTONEPLUGIN_H

#include <e32base.h>
#include "sanimplugin.h"

/**
*  Base class for all Start-up Tone plug-in implementations.
*  ECom interface UID for Start-up Tone Plug-in API is 0x2000B119.
*
*  @lib None
*  @since S60 3.2
*/
class CSAnimTonePlugin : public CSAnimPlugin
    {

protected:

    /**
    * Constructor.
    *
    * @since S60 3.2
    */
    IMPORT_C CSAnimTonePlugin();

public:

    /**
    * Set requested tone volume.
    *
    * @since S60 3.2
    *
    * @param aVolume Volume level to use.
    */
    IMPORT_C void SetVolume( const TInt aVolume );

    /**
    * Set requested volume ramp value.
    *
    * @since S60 3.2
    *
    * @param aVolumeRamp Volume ramp: the period over which the volume level is
    * to rise smoothly from nothing to the normal volume level.
    */
    IMPORT_C void SetVolumeRamp( const TTimeIntervalMicroSeconds& aVolumeRamp );

private:

    /** Copy constructor. */
    CSAnimTonePlugin( const CSAnimTonePlugin& );
    /** Assignment operator. */
    CSAnimTonePlugin& operator=( const CSAnimTonePlugin& );

protected: // data

    /** Volume level to use. */
    TInt iVolume;

    /**
    * Volume ramp: the period over which the volume level is to rise smoothly
    * from nothing to the normal volume level.
    */
    TTimeIntervalMicroSeconds iVolumeRamp;

    TAny* iReserved1; /** Reserved for future use. */
    TAny* iReserved2; /** Reserved for future use. */
    TInt iReserved3; /** Reserved for future use. */
    TInt iReserved4; /** Reserved for future use. */

    };

#endif // SANIMTONEPLUGIN_H
