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
* Description:  Declaration of CGSAccHACView class
*
*/


#ifndef GSACCHACVIEW_H
#define GSACCHACVIEW_H

#include "gsaccbaseview.h"

/**
*  CGSAccHACView view class
*
*  View class for HAC settings
*/
class CGSAccHACView : public CGSAccBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccHACView object.
    *
    * @param aModel Data model for reading and writing settings.
    * @return The new object
    */
    static CGSAccHACView* NewLC( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccHACView();

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
    CGSAccHACView( CGSAccessoryPluginModel& aModel );

    /**
    * Change HAC mode setting.
    *
    * @param aSettingPage Show the setting page if ETrue, otherwise just flip
    * the value.
    */
    void ChangeHACModeSettingL( TBool aUseSettingPage );

    };

#endif // GSACCHACVIEW_H
