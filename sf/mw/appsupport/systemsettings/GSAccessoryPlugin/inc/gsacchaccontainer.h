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
* Description:  Declaration of CGSAccHACContainer class
*
*/


#ifndef GSACCHACCONTAINER_H
#define GSACCHACCONTAINER_H

#include "gsaccbasecontainer.h"

/**
*  CGSAccHACContainer container class
*
*  container class for Loopset view
*/
class CGSAccHACContainer : public CGSAccBaseContainer
    {

public: // Constructors and destructor

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccHACContainer( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccHACContainer();

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
    * Read HAC mode from model and update it to the listbox item.
    */
    void RefreshHacModeL();

private: // data

    /** Text strings to show in the HAC mode item text box. Own. */
    CDesCArrayFlat* iHacItems;

    };

#endif // GSACCHACCONTAINER_H
