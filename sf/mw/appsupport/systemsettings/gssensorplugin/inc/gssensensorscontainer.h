/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sensors activation status view container
*
*/



#ifndef GSSENSENSORSCONTAINER_H
#define GSSENSENSORSCONTAINER_H

// INCLUDES
#include <msvstd.h>
#include <gsbasecontainer.h>

// FORWARD DECLARATIONS
class CGSListBoxItemTextArray;
class CGSSensorPluginModel;

// CLASS DECLARATION

/**
 *  CGSSenSensorsContainer container class
 *
 *  container class for Sensors activation status view
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenSensorsContainer : public CGSBaseContainer
    {
    public:
        
        /**
         * Symbian OS default constructor.
         */
        void ConstructL( const TRect& aRect );

        /**
         * Destructor.
         */
        virtual ~CGSSenSensorsContainer();

        /**
         * Default constructor
         */
        CGSSenSensorsContainer( CGSSensorPluginModel* aModel );
        
        /**
         * Updates the listbox items
         *
         * @param aFeatureId selected listbox item ID
         */
        void UpdateListBoxL( TInt aFeatureId );

        /**
         * Retrieves the currently selected listbox feature id
         *
         * @return TInt Feature id.
         */
        TInt CurrentFeatureId() const;

    protected:

        /**
         * Constructs listbox and initializes its sub-folders
         *
         * @param aResLbxId resource ID of the listbox item array
         */
        void ConstructListBoxL( TInt aResLbxId );

    private:

        /**
         * Creates list box items
         */
        void CreateListBoxItemsL();

        /**
         * Makes sensors active selection box
         */
        void MakeSensorsSelectionItemL();

        /**
         * Required for help.
         *
         * @param aContext Context to where to add help
         */
        void GetHelpContext( TCoeHelpContext& aContext ) const;
 
    private: // Data

        /**
         * Sensors active status items
         */
        CDesCArrayFlat* iSensorStatusItems;
        
        /**
         * GS listbox model
         */
        CGSListBoxItemTextArray* iListboxItemArray;
        
        /**
         * Plugin model, does not own, so do not delete this
         */
        CGSSensorPluginModel* iModel;

    };

#endif // GSSENSENSORSCONTAINER_H
