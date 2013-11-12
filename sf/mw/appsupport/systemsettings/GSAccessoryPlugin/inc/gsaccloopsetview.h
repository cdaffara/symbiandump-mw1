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
* Description:  Declaration of CGSAccLoopsetView class
*
*/


#ifndef GSACCLOOPSETVIEW_H
#define GSACCLOOPSETVIEW_H

#include "gsaccbaseview.h"

/**
*  CGSAccLoopsetView view class
*
*  view class for Loopset settings
*/
class CGSAccLoopsetView : public CGSAccBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccLoopsetView object.
    *
    * @param aModel Data model for reading and writing settings.
    * @return The new object
    */
    static CGSAccLoopsetView* NewLC( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccLoopsetView();

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
    CGSAccLoopsetView( CGSAccessoryPluginModel& aModel );

    };

#endif // GSACCLOOPSETVIEW_H
