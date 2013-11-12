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
* Description:  CCenRepSourcePlugIn class declaration.
*
*/



#ifndef C_CENREPSOURCEPLUGIN_H
#define C_CENREPSOURCEPLUGIN_H

#include <cfcontextsourceplugin.h>

class CCenRepHandler;

/**
 *  CenRepSource plugin.
 *
 *  @lib CenRepSourcePlugIn.dll
 *  @since S60 5.0
 */

NONSHARABLE_CLASS( CCenRepSourcePlugIn ) : public CCFContextSourcePlugIn
    {

public:

    /**
     * Two-phased constructors.
     *
     * @since S60 5.0
     * @param aParams Context source plug-in construction parameters
     */
    static CCenRepSourcePlugIn* NewL( TContextSourceInitParams* aParams );

    static CCenRepSourcePlugIn* NewLC( TContextSourceInitParams* aParams );

    virtual ~CCenRepSourcePlugIn();

protected:

private:

    // Subscribes for specified contexts
    void SubscribeL();

    CCenRepSourcePlugIn( TContextSourceInitParams* aParams );

// From base classes

    // @see CCFContextSourcePlugIn
    void HandleSettingL( CCFContextSourceSettingArray* aSettingList );

    // @see CCFContextSourcePlugIn
    void DefineContextsL();

    // @see CCFContextSourcePlugIn
    void InitializeL();

public:    // Data

protected: // Data

private:   // Data

    // Own: Array of CenRepHandler objects
    RPointerArray<CCenRepHandler> iCenRepHandlerArray;

    // Own: CenRep source settings
    CCFContextSourceSettingArray* iSettings;
    };

#endif // C_CENREPSOURCEPLUGIN_H
