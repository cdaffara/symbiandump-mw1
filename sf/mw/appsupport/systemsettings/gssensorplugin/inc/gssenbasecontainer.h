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
* Description:  Base container for multiselection containers
*
*/



#ifndef GSSENBASECONTAINER_H
#define GSSENBASECONTAINER_H

// INCLUDES
#include <gsbasecontainer.h>

// FORWARD DECLARATIONS
class CGSListBoxItemTextArray;
class CGSSensorPluginModel;
class MGSSensorMskObserver;

// CLASS DECLARATION

/**
 *  CGSSenBaseContainer container class
 *
 *  base container class for multiselection options settings view
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenBaseContainer : public CGSBaseContainer
    {
    public:

        /**
         * Destructor.
         */
        virtual ~CGSSenBaseContainer();

        /**
         * Default constructor
         */
        CGSSenBaseContainer( MGSSensorMskObserver& aMskObserver );

        /**
         * Retrieves the currently selected listbox feature id
         *
         * @return TInt Feature id.
         */
        TInt CurrentFeatureId() const;

        /**
         * Updates selected items checkbox icon
         *
         * @param aSelectedItem Checkbox to update
         */
        void UpdateCheckboxIconL( TInt aSelectedItem );

        /**
         * Stores currently made selections to Cenrep
         */
        void StoreSelectionsL();

        /**
         * Changes selection status bit in bitmask.
         *
         * @param aPosition Position of wanted selection in bitmask
         */
        void ChangeSelectionStatus( TInt aPosition );

        /**
         * Checks that selections are synchronized with the ones in the listbox
         */
        void CheckSelections();

        /**
         * Returns selected option value from selections bitmask
         *
         * @param aPosition Position of wanted selection in bitmask
         * @return 1: ON
         *         0: OFF
         */
        TInt SelectionStatus( TInt aPosition );

    protected:

        /**
         * Constructs listbox and initializes its sub-folders
         *
         * @param aResLbxId resource ID of the listbox item array
         */
        virtual void ConstructListBoxL( TInt aResLbxId ) = 0;

        /**
         * Update listbox icons according to previously set values
         */
        virtual void UpdateCheckboxIconsL() = 0;

        /**
         * Returns child classes interaction id
         *
         * @return TInt ID of the childs interaction from gssensorplugin.hrh file
         */
        virtual TInt InteractionId() = 0;

        /**
         * Constructs listbox and initializes its sub-folders
         *
         * @param aResLbxId resource ID of the listbox item array
         * @param aResMsId resource ID of the multiselection list
         * @param aInteraction ID of the interaction to get correct 
         *                     selections from CenRep
         */
        void ConstructListBoxL( TInt aResLbxId, TInt aResMsId );

        /**
         * Adds checkbox icons to multiselection list
         */
        void AddCheckboxIconsL();

        /**
         * Handles changes in MSK label between item changes in listbox
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );


    protected: // Data

        /**
         * Contains bitmask of selections
         */
        TUint32 iSelections;

        /**
         * Selection items from resource file
         */
        CDesCArrayFlat* iSelectionItems;

        /**
         * GS listbox model
         */
        CGSListBoxItemTextArray* iListboxItemArray;
        
        /**
         * Plugin model, does not own, so do not delete this
         */
        CGSSensorPluginModel* iModel;

        /**
         * Pointer to Middle soft key observer, not owned
         */
        MGSSensorMskObserver& iMskObserver;

    };

#endif // GSSENBASECONTAINER_H
