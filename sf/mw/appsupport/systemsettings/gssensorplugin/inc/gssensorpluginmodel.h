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
* Description:  Sensor Settings model.
*
*/


#ifndef GSSENSORPLUGINMODEL_H
#define GSSENSORPLUGINMODEL_H

// INCLUDES
#include <e32base.h>
#include <badesca.h>
#include <centralrepository.h>

// CONSTANTS
// maximum string sizes
const TInt KGSBufSize128 = 128;
const TUid KGSSensorPluginUid = { 0x10282DED };


// CLASS DEFINITION

/**
 *  CGSSensorPluginModel is the model class of GS sensor plugin.
 *  It provides functions to get and set setting values.
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSensorPluginModel : public CBase
    {
    public:

        /**
         * Two-phased constructor
         */
        static CGSSensorPluginModel* NewL();

        /**
         * Destructor
         */
        virtual ~CGSSensorPluginModel();

        /**
         * Returns sensor active status.
         *
         * @return 1: sensor active status ON
         *         0: sensor active status OFF
         */
        TInt SensorActiveStatusL();

        /**
         * Sets sensor active status.
         *
         * @param aStatus sensor active status.
         */
        void SetSensorActiveStatusL( TInt aStatus );

        /**
         * Gets user selected options for interactions
         *
         * @param aInteraction Interaction for which settings to get
         * @return TUint32 contains a bitmaks of user selections
         */
        TUint32 SelectionListL( TInt aInteraction );

        /**
         * Sets user selected options for interaction
         *
         * @param aInteraction Interaction for which settings to set
         * @param aList updated bitmask of user selected interactions
         */
        void SetSelectionListL( TInt aInteraction, TInt aList );

        /**
         * Indicates is any changes made during this objects lifetime
         */
        TBool ChangesMade();

        /**
         * Indicates are there any settings set on
         */
        TBool SettingsExistsL();

        /**
         * Gets the variation bitmask of a selected interaction
         */
        TUint32 VariationListL( const TInt aInteraction );

    private:

        /**
         * Default C++ contructor
         */
        CGSSensorPluginModel();

        /**
         * Symbian OS default constructor
         */
        void ConstructL();

    private: // data

        /**
         * Sensor settings repository
         */
        CRepository* iSensorRepository;

        /**
         * Flag that has user made any changes to settings. This value is 
         * updated whenever settings are stored into Central Repository.
         */
        TBool iChangesMade;
    };

#endif //GSSENSORPLUGINMODEL_H
