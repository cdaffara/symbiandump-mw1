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
* Description:  Main view for Sensor plug-in.
*
*/


#ifndef GSSENSORPLUGIN_H
#define GSSENSORPLUGIN_H

// INCLUDES
#include <gsbaseview.h>
#include <gspluginloader.h>
#include "gssensormskobserver.h"

// FORWARD CLASS DECLARATION
class CAknView;
class CAknViewAppUi;
class CGSSensorPluginContainer;
class CGSSensorPluginModel;


// CLASS DECLARATION

/**
 *  CGSSensorPlugin view class
 *
 *  Plugin implementation for sensor settings
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSensorPlugin : public CGSBaseView, 
                        public MGSPluginLoadObserver,
                        public MGSSensorMskObserver
    {
    public:

        /**
         * Symbian OS two-phased constructor
         *
         * @param aInitParams Initialization parameters
         */
        static CGSSensorPlugin* NewL( TAny* aInitParams );

        /**
         * Destructor.
         */
        virtual ~CGSSensorPlugin();

        /**
         * Returns view id. This is same as this plug-ins UID
         *
         * @return TUid
         */
        TUid Id() const;

        /**
         * Handles commands from user.
         *
         * @param aCommand Command to be handled.
         */
        void HandleCommandL( TInt aCommand );

        /**
         * From MEikCommandObserver, handles the commands.
         *
         * @param aCommand identifies the command given.
         */
        void ProcessCommandL( TInt aCommand );

        /**
         * @see CGSPluginInterface header file.
         */
        void GetCaptionL( TDes& aCaption ) const;

        /**
         * @see CGSPluginInterface header file.
         */
        TInt PluginProviderCategory() const;

        /**
         * @see CGSPluginInterface header file.
         */
        CGulIcon* CreateIconL( const TUid aIconType );

        /**
        * @see MGSPluginLoadObserver header file.
        */
        void HandlePluginLoaded( KGSPluginLoaderStatus aStatus );

        /**
         * Updates listbox's item's value.
         *
         * @param aItemId An item which is updated.
         */
        void UpdateListBoxL( TInt aItemId );

        /**
         * Get CGSSensorPlugin's container.
         */
        CGSSensorPluginContainer* Container();

        /**
         * From MGSSensorMskObserver.
         * Checks currently activated item in list and updates MSK label if needed
         */
        void CheckMiddleSoftkeyLabelL();

    protected:

        /**
         * From CAknView. Handles a view activation.
         *
         * @param aPrevViewId Specifies the view previously active.
         * @param aCustomMessageId Specifies the message type.
         * @param aCustomMessage The activation message.
         */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
         * Deactivates current view.
         */
        void DoDeactivate();

        /**
        * From MEikMenuObserver.
        * Changes MenuPane dynamically.
        */
        void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

        /**
         * From CGSBaseView. Override function to create container,
         */
        void CreateContainerL();

    protected:

        /**
         * C++ default constructor.
         */
        CGSSensorPlugin();

        /**
         * Symbian OS default constructor.
         */
        void ConstructL();

    private:

        /**
         * From CGSBaseView. Creates new container
         */
        void NewContainerL();

        /**
         * From CGSBaseView. Handles list box selection
         */
        void HandleListBoxSelectionL();

        /**
         * Called when plugin is not used anymore. Frees view UIDs this plugin has.
         */
        void RemoveLocalViews();

        /**
         * Creates local view.
         *
         * @param aLocalViewId Identifier of the local view to be created
         */
        void CreateLocalViewL( TUid aLocalViewId );

        /**
         * Selects and creates current view. Calls CreateLocalViewL
         */
        void SelectLocalViewL();

        /**
         * From CGSBaseView. Change sensors active status setting without 
         * displaying the setting page
         */
        void ChangeSensorActiveSettingL();

        /**
         * Removes current label from MSK
         */
        void RemoveCommandFromMSK();

        /**
         * Adds new label for MSK
         *
         * @param aResourceId Text of the new label
         * @param aCommandId Command id of the MSK
         */
        void SetMiddleSoftKeyLabelL( const TInt aResourceId, const TInt aCommandId );

        /**
         * Retreives Cba from base classes if iCba is NULL
         */
        CEikButtonGroupContainer* ButtonGroup();

        /**
         * Checks the exit status and shows query, note or neither
         */
        void CheckExitStatusL();

    private: // Data

        /**
         * resource loader
         */
        RConeResourceLoader iResourceLoader;

        /**
         * Pointer to model of this plug-in
         */
        CGSSensorPluginModel* iModel;

        /**
         * Extended plugin array for sensor specific plugins
         */
        CArrayPtrFlat<CGSPluginInterface>* iPluginArray;

        /**
         * Asynchronous loader for the GS plug-ins.
         */
        CGSPluginLoader* iPluginLoader;

        /**
         * All the views below are owned by iAppUi
         */
        CAknView* iTappingCtrlView;
        CAknView* iTurningCtrlView;
    };

#endif // GSSENSORPLUGIN_H
