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
* Description:  Declaration of CGSAccLoopsetContainer class
*
*/


#ifndef GSACCLOOPSETCONTAINER_H
#define GSACCLOOPSETCONTAINER_H

#include "gsaccbasecontainer.h"

/**
*  CGSAccLoopsetContainer container class
*
*  container class for Loopset view
*/
class CGSAccLoopsetContainer : public CGSAccBaseContainer
    {

public: // Constructors and destructor

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccLoopsetContainer( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccLoopsetContainer();

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
    * Constructs listbox and initializes its sub-folders
    *
    * @param aResLbxId Resource ID of the listbox item array
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

private: // data

    /** Text strings to show in the automatic answer mode item text box. Own. */
    CDesCArrayFlat* iAutoAnswerItems;

    };

#endif // GSACCLOOPSETCONTAINER_H
