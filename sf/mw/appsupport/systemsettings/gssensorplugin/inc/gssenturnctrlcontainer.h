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
* Description:  Sensor turn control view container
*
*/



#ifndef GSSENTURNCTRLCONTAINER_H
#define GSSENTURNCTRLCONTAINER_H

// INCLUDES
#include "gssenbasecontainer.h"

// CLASS DECLARATION

/**
 *  CGSSenTurnCtrlContainer container class
 *
 *  container class for turn control options settings view
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSenTurnCtrlContainer : public CGSSenBaseContainer
    {
    public:
        
        /**
         * Symbian OS default constructor.
         */
        void ConstructL( const TRect& aRect );

        /**
         * Destructor.
         */
        virtual ~CGSSenTurnCtrlContainer();

        /**
         * Default constructor
         */
        CGSSenTurnCtrlContainer( CGSSensorPluginModel* aModel, 
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

#endif // GSSENTURNCTRLCONTAINER_H
