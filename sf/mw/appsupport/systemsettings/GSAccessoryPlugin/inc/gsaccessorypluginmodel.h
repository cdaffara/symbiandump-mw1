/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccessoryPluginModel class
*
*/


#ifndef GSACCESSORYPLUGINMODEL_H
#define GSACCESSORYPLUGINMODEL_H

#include <e32base.h>
#include <AccessorySettings.h>
#include <AccessoryServer.h>

class CRepository;
class CGSAccProfileList;

/**
*  CGSAccessoryPluginModel is the model class of GS accessory plugin.
*  It provides functions to get and set setting values.
*  @since S60 3.1
*/
class CGSAccessoryPluginModel : public CBase
    {
public:  // Constructor and destructor

    /**
    * Constructs a CGSAccessoryPluginModel object.
    *
    * @return The new object
    */
    static CGSAccessoryPluginModel* NewL();

    /**
    * Destructor.
    */
    virtual ~CGSAccessoryPluginModel();

public:

    /**
    * Gets accessory support status.
    *
    * @param aAccessory Identifies accessory to check.
    * @return ETrue if accessory is supported, EFalse otherwise.
    */
    TBool AccessorySupportedL( TInt aAccessory ) const;

    /**
    * Gets the accessory which has been selected as default.
    *
    * @return Default accessory identifier.
    */
    TInt DefaultAccessory() const;

    /**
    * Sets the accessory which has been selected as default.
    *
    * @param aValue Value to be stored.
    */
    void SetDefaultAccessoryL( TInt aValue );

    /**
    * Read profile id from specified Cenrep key.
    *
    * @param aKey Key id.
    * @return 0: active
    *         1: general
    *         2: silent
    *         3: meeting
    *         4: outdoor
    *         5: pager
    *         negative value on error.
    */
    TInt DefaultProfileByKey( TInt aKey ) const;

    /**
    * Set default profile to specific Cenrep key.
    *
    * @param aKey Key id.
    * @param aProfile Default profile to save.
    */
    void SetDefaultProfileByKeyL( TInt aKey, TInt aNewProfile );

    /**
    * Read automatic answer mode from specified Cenrep key.
    *
    * @param aKey Key id.
    * @return List box index for automatic answer mode setting
    *         0: automatic answer mode on.
    *         1: automatic answer mode off.
    *         Negative value on error.
    */
    TInt AutoAnswerModeByKey( TInt aKey ) const;

    /**
    * Read automatic answer mode from specified Cenrep key.
    *
    * @param aKey Key id.
    * @param aMode List box index for automatic answer mode mode settings
    *         0: automatic answer mode on.
    *         1: automatic answer mode off.
    */
    void SetAutoAnswerModeByKeyL( TInt aKey, TInt aMode ) const;

    /**
    * Read accessory light mode from Accessory Server.
    *
    * @param aAccMode Identifies the accessory.
    * @param aAccSetId Identifies the accessory setting to read.
    * @return List box index for light mode setting
    *         0: light mode on.
    *         1: light mode off.
    *         Negative value on error.
    */
    TInt LightMode( TAccMode aAccMode, TInt aAccSetId ) const;

    /**
    * Store accessory light mode to Accessory Server.
    *
    * @param aAccMode Identifies the accessory.
    * @param aAccSetId Identifies the accessory setting to read.
    * @param aLightMode List box index for light mode setting
    *         0: light mode on.
    *         1: light mode off.
    */
    void SetLightModeL(
        TAccMode aAccMode,
        TInt aAccSetId,
        TInt aLightMode );

    /**
    * Read HAC mode value from Central Repository.
    *
    * @return List box index for HAC mode settings
    *         0: HAC mode on.
    *         1: HAC mode off.
    *         Negative value on error.
    */
    TInt HacMode();

    /**
    * Write HAC mode value to Central Repository.
    *
    * @param aMode List box index for HAC mode settings
    *         0: HAC mode on.
    *         1: HAC mode off.
    */
    void SetHacModeL( TInt aMode );

    /**
    * Check if PALM TV system is supported.
    *
    * @return ETrue if PALM is supported
    *         EFalse if PALM is not supported
    */
    TBool PalmSupport();

    /**
    * Get handle to profile list.
    *
    * @return Handle to profile list.
    */
    CGSAccProfileList& ProfileList() const;

    /**
    * Switches from 0 to 1 and vice versa. Used for changing on/off selections
    * status.
    *
    * @param aValue Value will be switched (0<->1).
    */
    void FlipValue( TInt& aValue );

private:

    /**
    * First phase constructor.
    */
    CGSAccessoryPluginModel();

    /**
    * Second phase constructor.
    */
    void ConstructL();

private: // data

    /** Internal accessory repository. Own. */
    CRepository* iAccessoryRepository;

    /** Internal profile repository Own. */
    CRepository* iProfileRepository;

    /** Hardware settings repository: for HAC feature Own. */
    CRepository* iHwSettingsRepository;

    /** Profiles repository: for HAC feature Own. */
    CRepository* iProfVariationRepository;

    /** Accessory server connection. */
    RAccessoryServer iAccServer;

    /** Accessory settings connection. */
    RAccessorySettings iAccessorySettings;

    /** PALM TV system support. */
    TBool iPalmSupport;

    /** List of profiles available for different accessories. Own. */
    CGSAccProfileList* iProfileList;

    };

#endif // GSACCESSORYPLUGINMODEL_H
