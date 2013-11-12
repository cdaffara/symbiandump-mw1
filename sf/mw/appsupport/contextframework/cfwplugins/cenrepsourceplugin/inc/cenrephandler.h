/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCenRepHandler class declaration.
*
*/


#ifndef C_CENREPHANDLER_H
#define C_CENREPHANDLER_H

#include <e32base.h>

#include "cenrepattributes.h"

class CRepository;
class MCFContextInterface;
// class CCenRepSourcePluginTester;

/**
 *  Handler class for listening CenRep variables changes
 *
 *  @lib CenRepSourcePlugIn.dll
 *  @since S60 5.0
 */

NONSHARABLE_CLASS( CCenRepHandler ) : public CActive
    {

public:

    /**
     * Two-phased constructor.
     *
     * @since S60 5.0
     * @param aCF Interface for contexts
     * @param aSettings Sensor source settings
     * @param aIndex Index of RContextSourceSettingArray item
     */
    static CCenRepHandler* NewL( MCFContextInterface& aCF,
                                 CCFContextSourceSettingArray& aSettings,
                                 TInt aIndex );

    virtual ~CCenRepHandler();

protected:

private:

    CCenRepHandler();

    void ConstructL( MCFContextInterface& aCF, CCFContextSourceSettingArray& aSettings, TInt aIndex);

    void QueryAndUpdateContext();

// from base class CActive

    /**
    * From CActive
    * Handles RunL's leave cases
    *
    * @since S60 5.0
    * @param aError Error code of leave
    * @return KErrNone
    */
    TInt RunError( TInt aError );

    /**
    * From CActive
    * Handles an active object's request completion event.
    *
    * @since S60 5.0
    * @param
    * @return void
    */
    void RunL();

    /**
    * From CActive
    * Notifies CenRep to cancel listening key changes
    *
    * @since S60 5.0
    * @param
    * @return void
    */
    void DoCancel();

public:    // Data

protected: // Data

private:   // Data

    // Own: Pointer to a Central repository object
    CRepository *iCrep;

    // Own: Pointer to a attribute handler object
    CCenRepAttributes* iCenRepAttributes;

    // Own: Pointer to a test class
    // CCenRepSourcePluginTester* iTester;

    // Central repository key
    TInt iKey;

    };

#endif // C_CENREPHANDLER_H
