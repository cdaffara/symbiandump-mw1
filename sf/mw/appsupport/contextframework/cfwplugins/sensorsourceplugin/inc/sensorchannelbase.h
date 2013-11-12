/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSensorChannelBase class declaration.
*
*/



#ifndef C_SENSORCHANNELBASE_H
#define C_SENSORCHANNELBASE_H

#include <e32base.h>
#include <sensrvtypes.h>
#include <sensrvdatalistener.h>
#include <cfcontextobject.h>

#include "sensorsourceclient.h"
class CSensrvChannel;
class CCFContextSourceCommand;
class CSensorSourceCommand;
class MCFContextInterface;
class TSensrvChannelInfo;

/**
* Sensor channel listener base class.
*
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( CSensorChannelBase ):
    public CBase,
    public MSensrvDataListener
    {
    public:
    
        // Sensor channels
        enum TSensorChannelId
            {
            // Listens double tap event channel
            EChannelDoubleTap = 0x00,
            
            // Listens orientation event channel
            EChannelOrientation = 0x01,
            
            // Listens proximity event channel
            EChannelProximity = 0x02,
            
            // Number of channels supported
            EChannelCount
            };
            
        // Sensor channel states
        enum TSensorChannelState
            {
            // Idle state, channels are not opened
            EChannelStateIdle,
            // Channel is initializing i.e. getting initial value
            EChannelStateInitializing,
            // Channel is open and data listening is active
            EChannelStateActive
            };
    
    public:
    
        // Destructor
        ~CSensorChannelBase();
        
    public: // New methods
    
        /**
        * Requests channel to define contexts.
        * 
        * @since S60 5.0
        * @param None.
        * @return None.
        */
        virtual void DefineContextsL() = 0;
    
    public: // New methods
    
        /**
        * Starts listening data channel.
        *
        * @since S60 5.0
        * @param None.
        * @return None.
        */
        virtual void StartListeningL();
    
        /**
        * Stops data channel listening.
        * 
        * @since S60 5.0
        * @param None.
        * @return None.
        */
        virtual void StopListening();
        
        /**
         * Fetch initial value from the sensor channel.
     	 * @param None.
     	 * @return None.
         */
        virtual void FetchInitialValueL();
        
    	 /**
     	 * Handles source command.
     	 * @param aCommand New source command.
     	 * @return None.
     	 */
    	 virtual void HandleSensorCommandL( const CSensorSourceCommand& aCommand );

    public: // New methods
    
        /**
        * Stops data channel listening.
        * 
        * @since S60 5.0
        * @param None.
        * @return None.
        */
        TSensorChannelId SensorChannelId() const;
        
        /**
     	 * Checks if we have active clients.
     	 * 
     	 * @since S60 5.0
     	 * @param None.
     	 * @return ETrue if we have active clients
         */
        TBool SensorActive() const;

    protected: // New methods
    
        /**
        * Handles received data.
        * 
        * @since S60 5.0
        * @param aChannel Channel through data is received.
        * @param aCount Number of data structs received.
        * @param aDataLost How many data structs lost.
        * @return None.
        */
        virtual void HandleDataReceivedL( CSensrvChannel& aChannel, 
            TInt aCount, 
            TInt aDataLost ) = 0;
        
    protected: // From base classes

        // @see MSensrvDataListener
        void DataReceived( CSensrvChannel& aChannel, 
            TInt aCount, 
            TInt aDataLost );

        // @see MSensrvDataListener
        void DataError( CSensrvChannel& aChannel, 
            TSensrvErrorSeverity aError );

        // @see MSensrvDataListener
        void GetDataListenerInterfaceL( TUid aInterfaceUid, 
            TAny*& aInterface );

    protected: // New methods
    
        /**
        * Searches for a sensor channel.
        * 
        * @since S60 5.0
        * @param aChannelTypeId Channel type id.
        * @param aLocation Channel location.
        * @param aVendorId Channel vendor id.
        * @param aChannelInfo Channel info found.
        * @return ETrue if channel found.
        */
        TBool FindChannelL( const TSensrvChannelTypeId& aChannelTypeId,
            const TDesC8& aLocation,
            const TDesC8& aVendorId,
            TSensrvChannelInfo& aChannelInfo );
        
        /**
        * Deletes current channel, reopens channel
        * and starts listening if needed.
        * 
        * @since S60 5.0
        * @param None.
        * @return None.
        */
        void DoReopenChannel();

    /**
     * Switches the channel state.
     * @param aState The new state.
     */
    void ChangeState( TSensorChannelState aState );
    
    /**
     * Search a command by feature id.
     * @param aFeatureId Feature id to search.
     * @return TInt Index where the command is in array.
     *  KErrNotFound if the command was not found.
     */
    TInt SearchCommandByFeatureId( const TPtrC& aFeatureId ) const;

    /**
     * Search a command by feature id.
     * @param aScriptId Script id to search.
     * @return TInt Index where the command is in array.
     *  KErrNotFound if the command was not found.
     */
    TInt SearchCommandByScriptId( TInt aScriptId ) const;

    /**
     * Search a command by Sid.
     * @param aSid Sid to search.
     * @return TInt Index where the command is in array.
     *  KErrNotFound if the command was not found.
     */
    TInt SearchCommandBySid( const TSecureId& aSid ) const;

    protected:
    
        /**
        * Constrcutor.
        * 
        * @since S60 5.0
        * @param aChannelInfo Info related to channel to be opened.
        * @return None.
        */        
        CSensorChannelBase( MCFContextInterface& aCF, TSensorChannelId aSensorChannel );
            
        /**
        * 2nd phase constructor.
        * 
        * @since S60 5.0
        * @param aChannelInfo Info related to channel to be opened.
        * @return None.
        */        
        void BaseConstructL( const TSensrvChannelInfo& aChannelInfo );
        
    private:
    
        // Opens the channel and starts data listening
        void DoStartL( TSensorChannelState aState );
        
        // Stop data listening and close channel
        void DoStop();
    
    // Handles activate sensor command
    void HandleActivateCommandL( const CSensorSourceCommand& aCommand );

    // Handles activate sensor command
    void HandleDeactivateCommandL( const CSensorSourceCommand& aCommand );

    // Add new client in list and activate sensor channel if needed
    void AddClientL( CSensorSourceClient* aClient );

    // Remove a command from list and deactivate sensor channel if needed
    void RemoveClient( TInt aIndex );

    protected: // Data
    
        // Own: Sensor channel
        CSensrvChannel* iSensorChannel;
        
        // Channel info
        TSensrvChannelInfo iChannelInfo;

        // Own: Context object
        CCFContextObject* iContext;
        
        // Channel Id
        TSensorChannelId iChannelId;
        
        // Ref: CF interface for contexts
        MCFContextInterface& iCF;
        
        // Channel state
        TSensorChannelState iState;
    
    // Own: Sensor source client list
    RSensorSourceClientList iClients;
    };
    
#endif
