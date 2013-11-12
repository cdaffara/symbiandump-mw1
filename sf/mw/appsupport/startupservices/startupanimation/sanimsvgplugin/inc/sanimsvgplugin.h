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
* Description:  Declaration of CSAnimSvgPlugin class
*
*/


#ifndef SANIMSVGPLUGIN_H
#define SANIMSVGPLUGIN_H

#include <SVGAnimationListener.h>

#include "sanimsvgpluginbase.h"

/**
*  Start-up Image plug-in using SVGT engine to load and display images /
*  animations in SVG-T and binary SVG-T formats.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimSvgPlugin )
  : public CSAnimSvgPluginBase,
    public MSvgAnimationListener
    {

public:

    /**
    * Constructs a CSAnimSvgPlugin object.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    * @return The new object
    */
    static CSAnimSvgPlugin* NewL( TAny* aConstructionParameters );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimSvgPlugin();

private:

    /**
    * Load the image.
    * From CSAnimPlugin.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aFileName Name of the image file to load.
    * @param aStatus Request to complete when image has been loaded.
    */
    virtual void Load(
        RFs& aFs,
        const TDesC& aFileName,
        TRequestStatus& aStatus );

    /**
    * Show the image/animation.
    * From CSAnimPlugin.
    *
    * @since S60 3.2
    *
    * @param aStatus Request to complete when image has been drawn / animation
    * has finished.
    */
    virtual void Start( TRequestStatus& aStatus );

    /**
    * Cancel loading / showing the image / animation.
    * From CSAnimPlugin.
    *
    * @since S60 3.2
    */
    virtual void Cancel();

    /**
    * Get the backgroud colour of the image.
    * Should only be called after successfully loading the image.
    * From CSAnimImagePlugin.
    *
    * @since S60 3.2
    *
    * @return The backgroud colour requested for the image.
    */
    virtual TRgb BackroundColour() const;

    /**
    * From MSvgRequestObserver
    *
    * @since S60 3.2
    */
    virtual void UpdateScreen();

    /**
    * From MSvgAnimationListener
    *
    * @since S60 3.2
    */
    virtual TBool AnimationStarted( TBool isAnimationIndefinite );

    /**
    * From MSvgAnimationListener
    *
    * @since S60 3.2
    */
    virtual TBool AnimationPaused();

    /**
    * From MSvgAnimationListener
    *
    * @since S60 3.2
    */
    virtual TBool AnimationEnded();

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    */
    CSAnimSvgPlugin( TAny* aConstructionParameters );

    /**
    * Second phase constructor.
    *
    * @since S60 3.2
    */
    void ConstructL();

    /**
    * Load the image from file.
    *
    * @since S60 3.2
    *
    * @param aFileName Name of the tone file to load.
    */
    void LoadL( const TDesC& aFileName );

private: // data

    /** Bitmap to hold the image to be shown. Owned. May be NULL. */
    CFbsBitmap* iImage;

    /** Bitmap to hold the mask of the image to be shown. Owned. May be NULL. */
    CFbsBitmap* iMask;

    };

#endif // SANIMSVGPLUGIN_H
