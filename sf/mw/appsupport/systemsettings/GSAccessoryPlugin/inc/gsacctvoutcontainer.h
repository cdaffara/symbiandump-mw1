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
* Description:  Declaration of CGSAccTvoutContainer class
*
*/


#ifndef GSACCTVOUTCONTAINER_H
#define GSACCTVOUTCONTAINER_H

#include "gsaccbasecontainer.h"

class CGSServerEngine;

/**
*  CGSAccTvoutContainer container class
*
*  container class for Tvout view
*/
class CGSAccTvoutContainer : public CGSAccBaseContainer
    {

public: // Constructors and destructor

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    * @param aServerEngine GS server engine for reading and writing settings.
    */
    CGSAccTvoutContainer(
        CGSAccessoryPluginModel& aModel,
        CGSServerEngine& aServerEngine );

    /**
    * Destructor.
    */
    virtual ~CGSAccTvoutContainer();

public: // From CGSAccBaseContainer

    /**
    * @see class CGSAccBaseContainer.
    */
    void UpdateListBoxL( TInt aFeatureId );

private: // From CGSBaseContainer

    /**
    * @see class CGSBaseContainer.
    */
    void ConstructL( const TRect& aRect );

    /**
    * @see class CGSBaseContainer.
    */
    void ConstructListBoxL( TInt aResLbxId );

private: // From CCoeControl

    /**
    * @see class CCoeControl.
    */
    void GetHelpContext( TCoeHelpContext& aContext ) const;

private:

    /**
    * Read default profile from model and update it to the listbox item.
    */
    void RefreshDefaultProfileL();

    /**
    * Read aspect ratio from GS engine and update it to the listbox item.
    */
    void RefreshAspectRatioL();

    /**
    * Read TV system from GS engine and update it to the listbox item.
    */
    void RefreshTvSystemL();

    /**
    * Read flicker filter from GS engine and update it to the listbox item.
    */
    void RefreshFlickerFilterL();

private: // data

    /** GS server reference. */
    CGSServerEngine& iServerEngine;

    /** Text strings to show in the aspect ratio item text box. Own. */
    CDesCArrayFlat* iAspectItems;

    /** Text strings to show in the TV system item text box. Own. */
    CDesCArrayFlat* iTvSystemItems;

    /** Text strings to show in the flicker filter item text box. Own. */
    CDesCArrayFlat* iFlickerItems;

    };

#endif // GSACCTVOUTCONTAINER_H
