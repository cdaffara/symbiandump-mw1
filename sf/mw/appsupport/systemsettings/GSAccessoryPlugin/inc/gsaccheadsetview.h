/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccHeadsetView class
*
*/


#ifndef GSACCHEADSETVIEW_H
#define GSACCHEADSETVIEW_H

#include "gsaccbaseview.h"

/**
*  CGSAccHeadsetView view class
*
*  view class for Headset setting
*/
class CGSAccHeadsetView : public CGSAccBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccHeadsetView object.
    *
    * @param aModel Data model for reading and writing settings.
    * @return The new object
    */
    static CGSAccHeadsetView* NewLC( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccHeadsetView();

private: // From CGSAccBaseView

    /**
    * @see class CGSAccBaseView.
    */
    void HandleCommandL( TInt aCommand );

    /**
    * @see class CGSAccBaseView.
    */
    void NewContainerL();

    /**
    * @see class CGSAccBaseView.
    */
    void HandleListBoxSelectionL( TInt aSelectedItem );

private: // From CAknView

    /**
    * @see class CAknView.
    */
    TUid Id() const;

private:

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccHeadsetView( CGSAccessoryPluginModel& aModel );

    };

#endif // GSACCHEADSETVIEW_H
