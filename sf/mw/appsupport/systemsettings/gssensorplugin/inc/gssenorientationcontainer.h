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
* Description:  Sensor orientation view container
*
*/



#ifndef GSSENORIENTATIONCONTAINER_H
#define GSSENORIENTATIONCONTAINER_H

// INCLUDES
#include "gssenbasecontainer.h"

// CLASS DECLARATION

/**
 *  CGSSenOrientationContainer container class
 *
 *  container class for orientation options settings view
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenOrientationContainer : public CGSSenBaseContainer
    {
    public:
        
        /**
         * Symbian OS default constructor.
         */
        void ConstructL( const TRect& aRect );

        /**
         * Destructor.
         */
        virtual ~CGSSenOrientationContainer();

        /**
         * Default constructor
         */
        CGSSenOrientationContainer( CGSSensorPluginModel* aModel );

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

#endif // GSSENORIENTATIONCONTAINER_H
