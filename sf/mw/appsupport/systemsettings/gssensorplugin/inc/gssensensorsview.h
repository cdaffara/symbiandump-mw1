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
* Description:  Sensors active status view
*
*/



#ifndef GSSENSENSORSVIEW_H
#define GSSENSENSORSVIEW_H

// INCLUDES
#include "gssensorbaseview.h"
#include "gssensensorscontainer.h"
#include "gssensorpluginmodel.h"

// CLASS DECLARATION

/**
 *  CGSSenSensorsView view class
 *
 *  view class for Sensors activation setting
 *  
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenSensorsView : public CGSSensorBaseView
    {
    public:

        /**
         * Symbian OS two phased constructor.
         */
        static CGSSenSensorsView* NewL( CGSSensorPluginModel* aModel );
        static CGSSenSensorsView* NewLC( CGSSensorPluginModel* aModel );

        /**
         * Destructor.
         */
        virtual ~CGSSenSensorsView();
        
        /**
         * From CAknView. Returns views id
         *
         * @return TUid ID of this view defined in gssensorpluginlocalviewids.h
         */
        TUid Id() const;

        /**
         * From CAknView. This function processes user commands
         *
         * @param aCommand Identifier of the user command
         */
        void HandleCommandL( TInt aCommand );

        /**
         * Updates listbox's item's value.
         * 
         * @param aItemId An item which is updated.
         */
        void UpdateListBoxL( TInt aItemId );
            
    private:

        /**
         * Symbian OS default constructor.
         */
        void ConstructL();

        /**
         * Default constructor
         */
        CGSSenSensorsView( CGSSensorPluginModel* aModel );
        
        /**
         * Container of this view
         */
        CGSSenSensorsContainer* Container();

        /**
         * From CGSBaseView. Request to create new contrainer
         */
        void NewContainerL();

        /**
         * From CGSBaseView. Handles list box selection made by user
         */
        void HandleListBoxSelectionL();

        /**
         * From CGSBaseView. Change sensors active status setting without 
         * displaying the setting page
         */
        void ChangeSensorActiveSettingL();

     private: // data

        /**
         * Plugin model, does not own, so do not delete this
         */
        CGSSensorPluginModel* iModel;
        
    };

#endif //GSSENSENSORSVIEW_H

// End of File
