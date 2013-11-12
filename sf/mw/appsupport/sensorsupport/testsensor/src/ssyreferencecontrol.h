// ssyreferencecontrol.h

/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/




/**
 @file
 @internalComponent
*/

#ifndef SSYREFERENCECONTROL_H
#define SSYREFERENCECONTROL_H

#include <sensors/spi/ssycontrol.h>
#include "ssyreferenceconfig.h"

// FORWARD DECLARATIONS
class CSsyReferenceChannel;

/**
 *  Main control class for SSY. Controls sensor basic functionality and provides mandatory 
 *  ECOM interface specific things.
 */
class CSsyReferenceControl : public CSsyControl
    {

public:

    /**
     * Two-phase constructor
     *
     * @param[in] aSsyCallback Reference to SSY callback instance.
     * @return Pointer to created CSsyReferenceControl object
     */
    static CSsyReferenceControl* NewL( MSsyCallback& aSsyCallback );

    /**
     * Virtual destructor
     */
    virtual ~CSsyReferenceControl();

// from base class CSsyControl

    /**
     * From CSsyControl
     * Request for SSY to open a sensor channel asynchronously. 
     * Response to the request is delivered through MSsyCallback::ChannelOpened().
     * Initilizes SSY (and the sensor) to be ready for other control commands via
     * data and property providers. Multiple OpenChannel()-requests can be
     * active for different channels at the same time.
     *
     * @param[in] aChannelID Channel that is requested to be opened
     * @return Symbian error code
     */
    void OpenChannelL( TSensrvChannelId aChannelID );

    /**
     * From CSsyControl
     * Request to close a sensor channel asynchronously. 
     * Response to the request is delivered through MSsyCallback::ChannelClosed().
     * Multiple CloseChannel()-requests can be active for different channels 
     * at the same time.
     *
     * @param[in] aChannelID Channel that is reqeusted to be closed
     * @leave  Any One of the system wide error codes
     */  
    void CloseChannelL( TSensrvChannelId aChannelID );

    /**
     * Reference to SSY Callback instance
     */
    MSsyCallback& SsyCallback() const;

    /**
     * Reference to SSY Config file
     */
    CSsyReferenceConfig& SsyConfig() const;
    /**
     * Handles response to CSsyReferenceCmdHandler::ProcessCommand
     *
     * @param[in] aMessage Contains information of the response
     */
    void ProcessResponse( TSsyReferenceMsg* aMessage );

    /**
     * Search property of given property id from the channel properties and 
     * returns reference to that. Leaves with KErrNotFound if property is not found
     *
     * @param[in]  aPropertyId Property ID to locate
     * @param[in]  aArrayIndex Propertys array index
     * @param[out] aProperty Contains found property
     */
    void FindPropertyL( const TSensrvPropertyId aPropertyId, 
                        const TInt aArrayIndex,
                        TSensrvProperty& aProperty );

private:

    /**
     * C++ constructor.
     *
     * @param[in] aSsyCallback Reference to SSY callback instance.
     */
    CSsyReferenceControl( MSsyCallback& aSsyCallback );

    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();

    /**
     * Finds and returns pointer to channel
     */
    CSsyReferenceChannel* FindChannelL( TSensrvChannelId aChannelID );

private: // data

    /**
     * Reference to SSY CallBack to send responses to Sensor Server
     */
    MSsyCallback& iSsyCallback;

    /**
     * Pointer array of all channels provided by this SSY
     */
    CArrayPtrFlat<CSsyReferenceChannel>* iChannelArray;

    /**
     * Pointer to config file parser
     */
    CSsyReferenceConfig* iConfigFile;

    /**
     * Property list of general properties of this SSY
     */
    RSensrvPropertyList iProperties;
    };

#endif // SSYREFERENCECONTROL_H
