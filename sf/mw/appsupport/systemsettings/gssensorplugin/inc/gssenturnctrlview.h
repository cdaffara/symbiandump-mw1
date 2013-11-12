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
* Description:  Sensors turn control view
*
*/



#ifndef GSSENTURNCTRLVIEW_H
#define GSSENTURNCTRLVIEW_H

// INCLUDES
#include "gssensorbaseview.h"

// FORWARD DECLARATIONS
class CGSSensorPluginModel;
class CGSSenTurnCtrlContainer;

// CLASS DECLARATION

/**
 *  CGSSenTurnCtrlView view class
 *
 *  view class for turn control options settings
 *  
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenTurnCtrlView : public CGSSensorBaseView
    {
    public:

        /**
         * Symbian OS two phased constructor.
         */
        static CGSSenTurnCtrlView* NewL( CGSSensorPluginModel* aModel );
        static CGSSenTurnCtrlView* NewLC( CGSSensorPluginModel* aModel );

        /**
         * Destructor.
         */
        virtual ~CGSSenTurnCtrlView();
        
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
         * From MGSSensorMskObserver.
         * Checks currently activated item in list and updates MSK label if needed
         */
        void CheckMiddleSoftkeyLabelL();
    
    private:

        /**
         * Symbian OS default constructor.
         */
        void ConstructL();

        /**
         * Default constructor
         */
        CGSSenTurnCtrlView( CGSSensorPluginModel* aModel );
        
        /**
         * Container of this view
         */
        CGSSenTurnCtrlContainer* Container();

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

#endif //GSSENTURNCTRLVIEW_H

// End of File
