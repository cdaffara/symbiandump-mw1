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
* Description:  Declaration of CGSAccWiredCarkitView class
*
*/


#ifndef GSACCWIREDCARKITVIEW_H
#define GSACCWIREDCARKITVIEW_H

#include "gsaccbaseview.h"

/**
*  CGSAccWiredCarkitView view class
*
*  view class for Music Stand settings
*/
class CGSAccWiredCarkitView : public CGSAccBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccWiredCarkitView object.
    *
    * @param aModel Data model for reading and writing settings.
    * @return The new object
    */
    static CGSAccWiredCarkitView* NewLC( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccWiredCarkitView();

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
    CGSAccWiredCarkitView( CGSAccessoryPluginModel& aModel );

    };

#endif // GSACCWIREDCARKITVIEW_H
