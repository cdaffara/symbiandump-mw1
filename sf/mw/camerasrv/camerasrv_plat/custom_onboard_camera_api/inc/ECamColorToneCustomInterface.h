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
* Description:  Custom interface for Color Tone setting
*
*/


#ifndef ECAMCOLORTONECUSTOMINTERFACE_H
#define ECAMCOLORTONECUSTOMINTERFACE_H

#include <e32base.h>

// CONSTANTS

const TUid KCameraColorToneSettingsUid = { 0x101F87C0 };

// CLASS DECLARATION

/**
*  Custom interface for Color Tone setting.
*
*  @lib CamExt.lib
*/
class MCameraColorToneSettings
    {

    public: 

        /**
        * List of color tones
        */
        enum TColorTone
            {
            ENormal         = 0x00,
            ESepia          = 0x01,
            EGrayscale      = 0x02,
            ENegative       = 0x04
            };
 
        /**
        * Gives the current color tone setting value
        * @return TColorTone
        */
        virtual TColorTone ColorTone() const = 0;

        /**
        * Sets color tone
        * @param aValue new color tone
        * @return void
        */
        virtual void SetColorToneL( TColorTone aValue ) = 0;

        /**
        * Gives the bitfield of supported color tones
        * @return the bitfield that contains supported TColorTone features
        */
        virtual TUint32 SupportedColorTones() const = 0;

    };

#endif //ECAMCOLORTONECUSTOMINTERFACE_H
