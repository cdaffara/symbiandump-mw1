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
* Description:  Sensors orientation view
*
*/



#ifndef GSSENORIENTATIONVIEW_H
#define GSSENORIENTATIONVIEW_H

// INCLUDES
#include "gssensorbaseview.h"

// FORWARD DECLARATIONS
class CGSSensorPluginModel;
class CGSSenOrientationContainer;

// CLASS DECLARATION

/**
 *  CGSSenOrientationView view class
 *
 *  view class for orientation options settings
 *  
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenOrientationView : public CGSSensorBaseView
    {
    public:

        /**
         * Symbian OS two phased constructor.
         */
        static CGSSenOrientationView* NewL( CGSSensorPluginModel* aModel );
        static CGSSenOrientationView* NewLC( CGSSensorPluginModel* aModel );

        /**
         * Destructor.
         */
        virtual ~CGSSenOrientationView();
        
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
    
    private:

        /**
         * Symbian OS default constructor.
         */
        void ConstructL();

        /**
         * Default constructor
         */
        CGSSenOrientationView( CGSSensorPluginModel* aModel );
        
        /**
         * Container of this view
         */
        CGSSenOrientationContainer* Container();

        /**
         * From CGSBaseView. Request to create new contrainer
         */
        void NewContainerL();

        /**
         * From CGSBaseView. Handles list box selection made by user
         */
        void HandleListBoxSelectionL();

     private: // data

        /**
         * Plugin model, does not own, so do not delete this
         */
        CGSSensorPluginModel* iModel;
        
    };

#endif //GSSENORIENTATIONVIEW_H

// End of File
