/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccBaseContainer class
*
*/


#ifndef GSACCBASECONTAINER_H
#define GSACCBASECONTAINER_H

#include <gsbasecontainer.h>

class CGSAccessoryPluginModel;
class CGSListBoxItemTextArray;

/**
*  CGSAccBaseContainer container class.
*  Base class for different containers in GSAccessoryPlugin.
*/
class CGSAccBaseContainer : public CGSBaseContainer
    {

public:

    /**
    * Destructor.
    */
    virtual ~CGSAccBaseContainer();

public:

    /**
    * Retrieves the currently selected listbox feature id.
    *
    * @return feature id.
    */
    TInt CurrentFeatureId() const;

    /**
    * Updates the listbox item identified by the parameter.
    *
    * @param aFeatureId Selected listbox item id.
    */
    virtual void UpdateListBoxL( TInt aFeatureId ) = 0;

protected: // from CGSBaseContainer

    /**
    * @see class CGSBaseContainer.
    */
    void ConstructListBoxL( TInt aResLbxId );

protected:

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccBaseContainer( CGSAccessoryPluginModel& aModel );

    /**
    * Constructs listbox object.
    */
    void CreateListBoxL();

    /**
    * Sets listbox item text.
    *
    * @param aItemId Identifies the item.
    * @param aText Text to set to the item.
    */
    void SetItemTextL( TInt aItemId, const TDesC& aText );

    /**
    * Makes a listbox item visible.
    *
    * @param aItemId Identifies the item to make visible.
    */
    void MakeItemVisibleL( TInt aItemId );

protected: // data

    /** Data model. */
    CGSAccessoryPluginModel& iModel;

    /** GS listbox model. Own. */
    CGSListBoxItemTextArray* iListboxItemArray;

    };

#endif // GSACCBASECONTAINER_H
