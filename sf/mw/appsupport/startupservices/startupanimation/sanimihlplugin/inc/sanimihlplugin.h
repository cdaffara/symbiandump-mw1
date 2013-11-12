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
* Description:  Declaration of CSAnimIhlPlugin class
*
*/


#ifndef SANIMIHLPLUGIN_H
#define SANIMIHLPLUGIN_H

#include "sanimimageplugin.h"

class CSAnimIhlCtrl;

/**
*  Start-up Image plug-in using IHL to load and display images / animations.
*
*  @lib None
*  @since S60 3.2
*/
NONSHARABLE_CLASS( CSAnimIhlPlugin ) : public CSAnimImagePlugin
    {

public:

    /**
    * Constructs a CSAnimIhlPlugin object.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    * @return The new object
    */
    static CSAnimIhlPlugin* NewL( TAny* aConstructionParameters );

    /**
    * Destructor.
    *
    * @since S60 3.2
    */
    virtual ~CSAnimIhlPlugin();

    /**
    * Call-back function for CSAnimIhlCtrl requests.
    *
    * @since S60 3.2
    */
    void RequestComplete();

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

private:

    /**
    * First phase constructor.
    *
    * @since S60 3.2
    *
    * @param aConstructionParameters Construction parameters from ECom.
    * May NOT be NULL.
    */
    CSAnimIhlPlugin( TAny* aConstructionParameters );

    /**
    * Create objects needed for loading and showing an animation and start
    * the loading operation.
    *
    * @since S60 3.2
    *
    * @param aFs Fileserver reference.
    * @param aFileName Name of the image file to load.
    */
    void LoadL( RFs& aFs, const TDesC& aFileName );

private: // data

    /** Controls interaction with IHL. Owned. May be NULL. */
    CSAnimIhlCtrl* iIhlCtrl;

    };

#endif // SANIMIHLPLUGIN_H
