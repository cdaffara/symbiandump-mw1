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
* Description:  Declaration of CGSAccTvoutView class
*
*/


#ifndef GSACCTVOUTVIEW_H
#define GSACCTVOUTVIEW_H

#include "gsaccbaseview.h"

class CGSServerEngine;

/**
*  CGSAccTvoutView view class
*
*  view class for Tvout setting
*/
class CGSAccTvoutView : public CGSAccBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccTvoutView object.
    *
    * @param aModel Data model for reading and writing settings.
    * @return The new object
    */
    static CGSAccTvoutView* NewLC( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccTvoutView();

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
    CGSAccTvoutView( CGSAccessoryPluginModel& aModel );

    /**
    * Second phase constructor.
    */
    void ConstructL();

    /**
    * Change aspect ratio setting.
    *
    * @param aSettingPage Show the setting page if ETrue, otherwise just flip
    * the value.
    */
    void ChangeAspectRatioSettingL( TBool aUseSettingPage );

    /**
    * Change flicker filter setting.
    *
    * @param aSettingPage Show the setting page if ETrue, otherwise just flip
    * the value.
    */
    void ChangeFlickerFilterSettingL( TBool aSettingPage );

    /**
    * Show TV system setting page and store the users selection.
    */
    void ChangeTvSystemSettingL();

    /**
    * Display note that some of the settings has changed.
    */
    void DisplaySettingsChangeNoteL();

private: // data

    /** GS Server Engine. Own. */
    CGSServerEngine* iServerEngine;

    /** Indicates if any of the settings was changed. */
    TBool iSettingChanged;

    };

#endif // GSACCTVOUTVIEW_H
