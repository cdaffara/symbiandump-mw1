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
* Description:  Declaration of CGSAccMusicStandContainer class
*
*/


#ifndef GSACCMUSICSTANDCONTAINER_H
#define GSACCMUSICSTANDCONTAINER_H

#include "gsaccbasecontainer.h"

/**
*  CGSAccMusicStandContainer container class
*
*  container class for Music Stand view
*/
class CGSAccMusicStandContainer : public CGSAccBaseContainer
    {

public: // Constructors and destructor

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccMusicStandContainer( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccMusicStandContainer();

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
    * Read automatic answer mode from model and update it to the listbox item.
    */
    void RefreshAutoAnswerL();

    /**
    * Read light setting from model and update it to the listbox item.
    */
    void RefreshLightModeL();

private: // data

    /** Text strings to show in the automatic answer mode item text box. Own. */
    CDesCArrayFlat* iAutoAnswerItems;

    /** Text strings to show in the lights mode item text box. Own. */
    CDesCArrayFlat* iLightModeItems;

    };

#endif // GSACCMUSICSTANDCONTAINER_H
