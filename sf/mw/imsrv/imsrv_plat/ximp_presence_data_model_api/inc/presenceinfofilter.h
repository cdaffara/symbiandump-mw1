/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence info filter object.
*
*/

#ifndef XIMPFWPRESENCEINFOFILTER_H
#define XIMPFWPRESENCEINFOFILTER_H

#include <e32std.h>
#include <ximpbase.h>
#include <presencedatamodelifids.hrh>


/**
 * Interface for presence info filter object.
 *
 * Presence information filter is used to define requested
 * presence information in presence subscriptions and
 * granted presence information in presence grants.
 *
 * Filtering is "inclusive": data is passed through if it matches the given
 * filtering criteria. Data with keys not matching to the filtering
 * criteria will be skipped by the filter.
 *
 * The Accept*FilterL methods can be called many times, to accept multiple
 * distinct fields. Wildcards "*" and "?" are allowed within the strings.
 *
 * @todo Interface will be extended.
 *
 * @ingroup presencedatamodelapi
 * @since S60 v3.2
 */
class MPresenceInfoFilter : public MXIMPBase
    {
public:

    /** Interface ID for the MPresenceInfoFilter. */
    enum { KInterfaceId = PRES_IF_ID_PRESENCE_INFO_FILTER };

public:

    /**
     * Public destructor. MPresenceInfoFilter object
     * can be deleted through this interface.
     */
    virtual ~MPresenceInfoFilter() {}

public:

    /**
     * Set filter for a field in the person presence component.
     * Wildcards "*" and "?" are allowed.
     * @param aFieldName The field name within the person info
     */
    virtual void AcceptPersonFilterL( const TDesC8& aFieldName ) = 0;

    /**
     * Set filter for the service presence component. 
     * Wildcards "*" and "?" are allowed.
     * @param aServiceName The service type
     * @param aFieldName The field name within the given service info
     */
    virtual void AcceptServiceFilterL( 
            const TDesC8& aServiceName,
            const TDesC8& aFieldName ) = 0;
  
    /**
     * Set filter for the device presence component.
     * Wildcards "*" and "?" are allowed.
     * @param aDeviceName The device name
     * @param aFieldName The field name within the given device info
     */
    virtual void AcceptDeviceFilterL( 
            const TDesC8& aDeviceName,
            const TDesC8& aFieldName ) = 0;

    /**
     * Try to match given field type against the filters
     * for the person presence component.
     * @param aFieldName The field to check
     * @return ETrue if the given field is accepted
     */
    virtual TBool MatchPersonFilter( const TDesC8& aFieldName ) const = 0;
    
    /**
     * Find out if the filter accepts the field in a given service info.
     * @param aServiceName The name of the service info to check
     * @param aFieldName The field name to check
     * @return ETrue if the given field within the named info is accepted
     */
    virtual TBool MatchServiceFilterL( 
            const TDesC8& aServiceName,
            const TDesC8& aFieldName ) const = 0;

    /**
     * Find out if the filter accepts the field in a given device info.
     * @param aDeviceName The name of the device info to check
     * @param aFieldName The field to check
     * @return ETrue if the given field within the named info is accepted
     */
    virtual TBool MatchDeviceFilterL( 
            const TDesC8& aDeviceName,
            const TDesC8& aFieldName ) const = 0;
    
    };

#endif //XIMPFWPRESENCEINFOFILTER_H



