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
* Description:  Declaration of CGSAccessoryPluginContainer class
*
*/


#ifndef GSACCESSORYPLUGINCONTAINER_H
#define GSACCESSORYPLUGINCONTAINER_H

#include <gsbasecontainer.h>

class CGSAccessoryPluginModel;
class CGSListBoxItemTextArray;

/**
*  GSAccessoryPluginContainer container class
*
*  Container class for Accessory Settings view
*/
class CGSAccessoryPluginContainer : public CGSBaseContainer
    {

public: // Constructors and destructor

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccessoryPluginContainer( CGSAccessoryPluginModel& aModel );

    /**
    * Destructor.
    */
    virtual ~CGSAccessoryPluginContainer();

public:

    /**
    * Retrieves the currently selected listbox feature id
    *
    * @return Feature id of the item which is currently selected in the list
    * box.
    */
    TInt CurrentFeatureId() const;

    /**
    * Changes the default item - clears the dynamic text from the old item and
    * adds it to the new item.
    *
    * @param aNewDefaultItem New default item index
    * @param aOldDefaultItem Previous default item index
    */
    void ChangeDefaultItemL(
	    const TInt aNewDefaultItem,
	    const TInt aOldDefaultItem );

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
    * Checks if accessory is supported and makes its listbox item visible if
    * it is.
    *
    * @param aAccessory Identifies the accessory whose support status to check.
    * @param aItemId Identifies the item to make visible.
    */
    void MakeVisibleIfSupportedL( TInt aAccessory, TInt aItemId );

    /**
    * Adds a label to an item.
    *
    * @param aFeatureId Identifies the item to set the text to.
    * @param aLabel Label to show.
    */
    void ShowLabelL( TInt aFeatureId, const TDesC& aLabel );

protected: // data

    /** Data model. */
    CGSAccessoryPluginModel& iModel;

    /** GS listbox model. Own. */
    CGSListBoxItemTextArray* iListboxItemArray;

    /** Label for the default accessory. Own. */
    HBufC* iDefaultText;

    };

#endif // GSACCESSORYPLUGINCONTAINER_H
