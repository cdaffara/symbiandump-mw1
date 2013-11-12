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
* Description:  Declaration of CSAnimImagePlugin class
*
*/


#ifndef SANIMIMAGEPLUGIN_H
#define SANIMIMAGEPLUGIN_H

#include <e32base.h>
#include <gdi.h>
#include "sanimobserver.h"
#include "sanimplugin.h"

/**
*  Base class for all Startup Image plug-in implementations.
*  ECom interface UID for Start-up Image Plug-in API is 0x2000B118.
*
*  @lib None
*  @since S60 3.2
*/
class CSAnimImagePlugin : public CSAnimPlugin
    {

protected:

    /**
    * Constructor.
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    *
    * Implement a factory method with this signature, and give the parameter
    * to this constructor.
    *
    * Example:
    *
    * CMyImagePlugIn* CMyImagePlugIn::NewL( TAny* aConstructionParameters )
    *     {
    *     CMyImagePlugIn* self =
    *         new( ELeave ) CMyImagePlugIn( aConstructionParameters );
    *     CleanupStack::PushL( self );
    *     self->ConstructL();
    *     CleanupStack::Pop( self );
    *     return self;
    *     }
    *
    * CMyImagePlugIn::CMyImagePlugIn( TAny* aConstructionParameters )
    *  : CSAnimImagePlugin( aConstructionParameters )
    *    {
    *    }
    */
    IMPORT_C CSAnimImagePlugin( TAny* aConstructionParameters );

public:

    /**
    * Set requested display mode.
    *
    * @since S60 3.2
    *
    * @param aDisplayMode Display mode to use, if supported by the image.
    */
    IMPORT_C void SetDisplayMode( const TDisplayMode aDisplayMode );

    /**
    * Set requested size for the image. If the image is scalable, it will be
    * scaled as near this size as possible.
    *
    * @since S60 3.2
    *
    * @param aSize Requested size for the image.
    */
    IMPORT_C void SetSize( const TSize& aSize );

    /**
    * Set frame delay for the animation. The same value is used for all the
    * frames in the animation. The value is only used if frame delays are not
    * specified in the animation file.
    *
    * @since S60 3.2
    *
    * @param aFrameDelay Frame delay to use.
    */
    IMPORT_C void SetFrameDelay( const TTimeIntervalMicroSeconds32& aFrameDelay );

    /**
    * Set scaling enabled/disabled for the animation.
    * The setting has only effect for those image formats for which scaling
    * can be enabled/disabled.
    *
    * @since S60 3.2
    *
    * @param aEnable Indicates whether scaling should be enabled or disabled.
    */
    IMPORT_C void SetScalingEnabled( const TBool aEnable );

    /**
    * Get the backgroud colour of the image.
    * Should only be called after successfully loading the image.
    *
    * @since S60 3.2
    *
    * @return The backgroud colour requested for the image.
    */
    virtual TRgb BackroundColour() const = 0;

private:

    /** Default constructor. */
    CSAnimImagePlugin();
    /** Copy constructor. */
    CSAnimImagePlugin( const CSAnimImagePlugin& );
    /** Assignment operator. */
    CSAnimImagePlugin& operator=( const CSAnimImagePlugin& );

protected: // data

    /** Observer for animation events. */
    MSAnimObserver& iObserver;

    /** Display mode to use, if supported by the image. */
    TDisplayMode iDisplayMode;

    /** Requested size of the image. */
    TSize iSize;

    /**
    * Frame delay for animation, unless specified in the animation file itself.
    */
    TTimeIntervalMicroSeconds32 iFrameDelay;

    TAny* iReserved1; /** Reserved for future use. */
    TAny* iReserved2; /** Reserved for future use. */

    /** If ETrue, try to scale the image to fit the screen if possible. */
    TBool iScalingEnabled;

    TInt iReserved4; /** Reserved for future use. */

    };

#endif // SANIMIMAGEPLUGIN_H
