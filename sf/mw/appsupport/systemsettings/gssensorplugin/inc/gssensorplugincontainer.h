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
* Description:  Container for the sensor settings
*
*/


#ifndef GSSENSORPLUGINCONTAINER_H
#define GSSENSORPLUGINCONTAINER_H

// INCLUDES
#include <gsbasecontainer.h>

// FORWARD DECLARATIONS
class CGSListBoxItemTextArray;
class CGSSensorPluginModel;
class CAknViewAppUi;
class CGSPluginInterface;
class MGSSensorMskObserver;

// CLASS DECLARATION

/**
 *  GSSensorPluginContainer container class
 *
 *  container class for sensor settings view
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSensorPluginContainer : public CGSBaseContainer
    {
    public:
        
        /**
         * Symbian 2nd phase constructor.
         * This constructL is not used, but it needs to be here
         * as it is derived from CGSbaseContainer
         *
         * @param aRect Listbox's rectancle
         */
        void ConstructL( const TRect& aRect );

        /**
         * Symbian 2nd phase constructor.
         *
         * @param aRect Rectangle
         * @param aAppUi Application UI
         * @param aPluginArray Contains information of child plugins
         */
        void ConstructL(
            const TRect& aRect,
            CAknViewAppUi* aAppUi,
            CArrayPtrFlat<CGSPluginInterface>* aPluginArray );
            
        /**
         * Destructor.
         */
        virtual ~CGSSensorPluginContainer();
        
        /**
         * C++ constructor
         *
         * @param aModel Model of this plug-in
         */
        CGSSensorPluginContainer( CGSSensorPluginModel* aModel, 
                                  MGSSensorMskObserver& aMskObserver );

        /**
         * Updates listbox's item's value.
         *
         * @param aFeatureId An item which is updated.
         */
        void UpdateListBoxL( TInt aFeatureId );

		/**
         * Retrieves the currently selected listbox feature id
         *
         * @return TInt Current feature id.
         */
        TInt CurrentFeatureId() const;

        /**
         * Sets Dynamic Text value
         * 
         * @param aValue Sensors status value
         */
	    void SetSensorStatusTextL( TInt aValue );

        /**
         * Finds user selected plugin from plugin array
         *
         * @return Plugin which is selected in the lbx or leaves 
         * with KErrnotFound if no plugins exist.
         */
        CGSPluginInterface* SelectedPluginL();

        /**
         * Makes extended plugins item
         */
        void MakeExtendedPluginsItemL();

        /**
         * Checks are there extended plugins and activates if only one exists
         */
        void ActivateExtendedPluginL();

    protected:

        /**
         * From CGSBaseContainer. Constructs listbox and initializes its sub-folders
         *
         * @param aResLbxId: resource ID of the listbox item array
         */
        void ConstructListBoxL( TInt aResLbxId );

    private:

        /**
         * Create listbox from resource
         */
        void CreateListBoxItemsL();

        /**
         * Makes Sensors view visible
         */
        void MakeSensorsItemL();

        /**
         * Makes Tapping Control view visible
         */
        void MakeTapCtrlItemL();

        /**
         * Makes Turning Control view visible
         */
        void MakeTurnCtrlItemL();

        /**
         * Makes Orientation view visible
         */
        void MakeOrientationItemL();

        /**
         * From CGSBaseContainer. Required for help.
         *
         * @param aContext Contains help context
         */
        void GetHelpContext( TCoeHelpContext& aContext ) const;

        /**
         * From CGSBaseContainer. Constructs correct textformat for one list box item 
         * and adds it in listbox
         *
         * @param aPos Position of text in listbox
         * @param aFirstLabel Text in first row of listbox item
         * @param aSecondLabel Text in second row of listbox item
         */
        void AppendListItemL( const TInt aPos, 
                              const TDesC16& aFirstLabel, 
                              const TDesC16& aSecondLabel );

        /**
         * Handles changes in MSK label between item changes in listbox
         */
        TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    private: // Data

        /**
         * listbox item array. Contains listbox items
         */
		CGSListBoxItemTextArray* iListboxItemArray;

        /**
         * Listbox item text array. Contains listbox item texts
         */
        CDesCArray* iListBoxTextArray;

        /** 
         * General list box items. Contains localized listbox texts
         * from resource file.
         */
        CDesCArrayFlat* iListItems;

        /**
         * Model pointer. Does not own it, so do not delete this pointer.
         */
        CGSSensorPluginModel* iModel;

        /** 
         * Sensors ON / OFF text
         */
        CDesCArrayFlat* iSensorStatusText;

        /**
         * Application UI. Not owned.
         */
        CAknViewAppUi* iAppUi;

        /**
         * Array of the child plugins. Owned by CGSSensorPlugin.
         */
        CArrayPtrFlat<CGSPluginInterface>* iPluginArray;

        /**
         * Reference to main view. Does not own
         */
        MGSSensorMskObserver& iMskObserver;

        /**
         * General item count. Used when calculating item indexes to extedned plugins
         */
        TInt iGeneralItemCount;
    };

#endif //GSSENSORPLUGINCONTAINER_H
