/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Plugin controller
*
*/


#ifndef SENSORDATACOMPENSATORPLG_H
#define SENSORDATACOMPENSATORPLG_H

// INCLUDES
#include <e32base.h>
#include <cfclient.h>
#include <cflistener.h>
#include <sensrvtypes.h>
#include <sensordatacompensatorplugin.h>
#include <sensordatacompensatorplugintypes.h>

#include "sensordatacompensatoritem.h"
#include "sensordatacompensatorinitializer.h"
#include "sensordatacompensatorcfthread.h"

// FORWARD DECLARATIONS
class MSensorDataCompensatorObserver;

// CLASS DECLARATION

/**
 * Class implements sensor data compensator.
 *
 * @lib sensordatacompensatorplugin.lib
 * @since S60 5.1
 */
NONSHARABLE_CLASS( CSensorDataCompensatorPlg ) :
            public CSensorDataCompensatorPlugin,
            public MSensorDataCompensatorCfThreadController,
            public MCFListener
    {
    public:
        // Constructors and destructor

        /**
        * Two-phase constructor
        * @param aDataType: the type id of sensor channel data to be compensated
        * @param aType:     the compensation type to be used
        * @param aObserver: the observer
        */
        static CSensorDataCompensatorPlg* NewL(
            TSensrvChannelDataTypeId aDataType,
            TSensorCompensationType aType,
            MSensorDataCompensatorObserver& aObserver );

        /**
        * Destructor.
        */
        virtual ~CSensorDataCompensatorPlg();

    public:
        // From CSensorDataCompensatorPlugin

        /**
        * Returns the count of compensation items
        *
        * @param None
        * @return Compensation items count, if not ready, KErrNotReady
        */
        virtual TInt GetCompensationItemCount();

        /**
        * Returns pointer to compensation item of the type being compensated
        *
        * @param aIndex: the index of compensation item
        * @return Pointer to compensation item, KNullDesC8 if out of bound
        */
        virtual TPtrC8 GetCompensationItem( TInt aIndex );

    public:

        /**
        * Returns the count of compensation items
        *
        * @param None
        * @return Compensation items count, if not ready, KErrNotReady
        */
        void InitCfwL();

    private:

        /**
        * Cleans items
        */
        void CleanItems();

        /**
         * Default constructor
         */
        CSensorDataCompensatorPlg(
            TSensrvChannelDataTypeId aDataType,
            TSensorCompensationType aType,
            MSensorDataCompensatorObserver& aObserver );

        /**
         * 2nd phase of construction
         */
        void ConstructL();

    private:

        // From MSensorDataCompensatorCfThread
        
        void ThreadInitL();
        void ThreadRelease();
        void NotifyControllerL( const RKeyValueArray& aKeyValueArray );
        
        // From MCFListener

        /**
        * Indicates a change in context for subscribed clients.
        * If client has subscribed to partial context class path,
        * only the changed context will be indicated.
        *
        * @param aIndication: Context indication.
        * @return None
        */
        void ContextIndicationL(
            const CCFContextIndication& aChangedContext );

        /**
        * Indicates that specified action is needed to be executed.
        * When action indication is received certain rule has been triggered
        * and action is needed to be performed.
        *
        * @param aActionToExecute Action indication.
        * @return None
        */
        void ActionIndicationL(
            const CCFActionIndication& aActionToExecute );

        /**
        * Returns an extension interface.
        * The extension interface is mapped with the extension UID.
        *
        * If the client does not support the requested interface,
        * client must return NULL. Otherwise client needs to return
        * the correct interface combined with the UID.
        *
        * @since S60 5.0
        * @param aExtensionUid: The identifier of the extension.
        * @return Pointer to the extension.
        */
        TAny* Extension( const TUid& aExtensionUid ) const;

        /**
        * Indicates that error has occured.
        *
        * @param aError Error code.
        * @return None
        */
        void HandleContextFrameworkError( TCFError aError,
            const TDesC& aSource,
            const TDesC& aType );

    private:

        /**
        * Sets the values to the correct item.
        *
        * @param aKeyValueArray: key value pairs
        * @return None
        */
		void SetItemL( const RKeyValueArray& aKeyValueArray );

    private: // data

        /**
         * Observer
         */
        MSensorDataCompensatorObserver& iObserver;

        /**
         * Owned by CSensorDataCompensatorCfTread.
         * May not be deleted on destructor.
         */
        CSensorDataCompensatorInitializer* iInitializer;

        /**
         * Owned by CSensorDataCompensatorCfTread.
         * May not be deleted on destructor.
         */
        CCFClient* iCFClient;
        
        /**
         * Owned
         */
        RPointerArray<CSensorDataCompensatorItem> iArrayOfItems;

        /**
         * Requested data type
         */
        TSensrvChannelDataTypeId iDataType;

        /**
         * Requested compensation type
         */
        TSensorCompensationType iType;

        /**
         * AxisData compensation TPckgBuf
         */
        TPckgBuf<TCompensationTypeAxisData> iCompensationTypeAxisDataBuf;

        /**
         * AngleData compensation TPckgBuf
         */

        TPckgBuf<TCompensationTypeAngleData> iCompensationTypeAngleDataBuf;

        /**
         * DirectionData compensation TPckgBuf
         */
        TPckgBuf<TCompensationTypeDirectionData> iCompensationTypeDirectionDataBuf;

        /**
         * Only data types  TSensrvAccelerometerAxisData and TSensrvMagnetometerAxisData are compensated
         * based on superposition principle.
         */
        TBool iAllowMultiples;

        /**
         * Cf listener thread instance. Owned.
         */
        CSensorDataCompensatorCfTread* iCfThread;
    };

#endif // SENSORDATACOMPENSATORPLG_H
