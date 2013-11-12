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
* Description:  Declaration of CGSAccMusicStandView class
*
*/


#ifndef GSACCMUSICSTANDVIEW_H
#define GSACCMUSICSTANDVIEW_H

#include "gsaccbaseview.h"

/**
*  CGSAccMusicStandView view class
*
*  view class for Music Stand settings
*/
class CGSAccMusicStandView : public CGSAccBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccMusicStandView object.
    *
    * @param aModel Data model for reading and writing settings.
    * @return The new object
    */
    static CGSAccMusicStandView* NewLC( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccMusicStandView();

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
    CGSAccMusicStandView( CGSAccessoryPluginModel& aModel );

    };

#endif // GSACCMUSICSTANDVIEW_H
