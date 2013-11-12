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
* Description:  Declaration of CSAnimMifPlugin class
*
*/


#ifndef SANIMMIFPLUGIN_H
#define SANIMMIFPLUGIN_H

#include "sanimsvgpluginbase.h"

/**
*  Start-up Image plug-in for images and animations in MIF format.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimMifPlugin ) : public CSAnimSvgPluginBase
    {

public:

    /**
    * Constructs a CSAnimMifPlugin object.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    * @return The new object
    */
    static CSAnimMifPlugin* NewL( TAny* aConstructionParameters );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimMifPlugin();

    /**
    * Call-back function for call-back timer.
    *
    * @since S60 3.2
    */
    void TimerExpired();

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

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    */
    CSAnimMifPlugin( TAny* aConstructionParameters );

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
    * @param aFs Fileserver reference.
    * @param aFileName Name of the image file to load.
    */
    void LoadL( RFs& aFs, const TDesC& aFileName );

    /**
    * Read array of images from file.
    *
    * @since S60 3.2
    *
    * @param aFile File to read from.
    * @param aPosition Position in the file where the array starts.
    * @param aLength Length of the array data in the file.
    */
    void ReadIconArrayL(
        RFile& aFile,
        const TInt32 aPosition,
        const TInt32 aLength );

    /**
    * Read icon data from file from the position pointed by the parameter data.
    *
    * @since S60 3.2
    *
    * @param aFile File to read from.
    * @param aData Information on the position and length of the icon data in
    * the file.
    */
    void ReadIconOffsetElementL( RFile& aFile, const TDesC8& aData );

    /**
    * Inform observer to refresh its view.
    *
    * @since S60 3.2
    */
    void RefreshView();

private:

    /** Default constructor. */
    CSAnimMifPlugin();
    /** Copy constructor. */
    CSAnimMifPlugin( const CSAnimMifPlugin& );
    /** Assignment operator. */
    CSAnimMifPlugin& operator=( const CSAnimMifPlugin& );

private: // data

    /** Timer for timing the frame showing durations. Owned. May not be NULL. */
    CPeriodic* iTimer;

    /** Array of animation frames. */
    RPointerArray<CFbsBitmap> iFrames;

    /** Bitmap to hold the mask of the image to be shown. Owned. May not be NULL. */
    CFbsBitmap* iDummy;

    /** Indicates which frame is currently shown. */
    TInt iCurrentFrame;

    };

#endif // SANIMMIFPLUGIN_H
