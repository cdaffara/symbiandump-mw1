/*
* Copyright (c) 2009-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CDisplayServiceLight class declaration.
*
*/


#ifndef DISPLAYSERVICELIGHT_H
#define DISPLAYSERVICELIGHT_H

// SYSTEM INCLUDE FILES
#include <hwrmlight.h>

// USER INCLUDE FILES
#include "displayservicebase.h"

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Display light service provides context information about
 * the display light status.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CDisplayServiceLight ): public CDisplayServiceBase,
public MHWRMLightObserver
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aCF Interface to Context Access API.
     * @return CDisplayServiceLight*
     */
    static CDisplayServiceLight* NewL( MCFContextInterface& aCF );
    static CDisplayServiceLight* NewLC( MCFContextInterface& aCF );

    /**
     * C++ destructor.
     */
    virtual ~CDisplayServiceLight();
    
public: // From base classes
    
    // @see CDisplayServiceBase
    virtual void StartL();

    // @see CDisplayServiceBase
    virtual void Stop();
    
    // @see MHWRMLightObserver
    virtual void LightStatusChanged( TInt aTarget,
        CHWRMLight::TLightStatus aStatus );

private: // New methods

    // Publishes the display state value
    void PublishContext( CHWRMLight::TLightStatus aStatus );

private:

    CDisplayServiceLight( MCFContextInterface& aCF );
    void ConstructL();

private: // Data

    /** HWRM Light client connection */
    CHWRMLight* iLight;

    /** Last light status stored to avoid publishing state status twice */
    CHWRMLight::TLightStatus iCurrentStatus;
    };

#endif // DISPLAYSERVICELIGHT_H
