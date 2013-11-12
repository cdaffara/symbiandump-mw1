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
* Description:  Sensor tapping control view container
*
*/



#ifndef GSSENTAPPINGCTRLCONTAINER_H
#define GSSENTAPPINGCTRLCONTAINER_H

// INCLUDES
#include "gssenbasecontainer.h"

// CLASS DECLARATION

/**
 *  CGSSenTappingCtrlContainer container class
 *
 *  container class for tapping control options settings view
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenTappingCtrlContainer : public CGSSenBaseContainer
    {
    public:
        
        /**
         * Symbian OS default constructor.
         */
        void ConstructL( const TRect& aRect );

        /**
         * Destructor.
         */
        virtual ~CGSSenTappingCtrlContainer();

        /**
         * Default constructor
         */
        CGSSenTappingCtrlContainer( CGSSensorPluginModel* aModel, 
                                    MGSSensorMskObserver& aMskObserver );

    protected:

        /**
         * Constructs listbox and initializes its sub-folders
         *
         * @param aResLbxId resource ID of the listbox item array
         */
        void ConstructListBoxL( TInt aResLbxId );

        /**
         * Update listbox icons according to previously set values
         */
        void UpdateCheckboxIconsL();

        /**
         * Returns interaction id
         *
         * @return TInt ID of the interaction from gssensorplugin.hrh file
         */
        TInt InteractionId();
    };

#endif // GSSENTAPPINGCTRLCONTAINER_H
