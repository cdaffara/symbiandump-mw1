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
* Description:  CDisplayContextBase class declaration.
*
*/


#ifndef DISPLAYSERVICEBASE_H
#define DISPLAYSERVICEBASE_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFContextObject;
class MCFContextInterface;

// DATA TYPES

// CLASS DECLARATION

/**
 * Base class for all display type contexts.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CDisplayServiceBase ): public CBase
    {
public:
    
    // Display service type
    enum TDisplayServiceType
        {
        EDisplayLightStatus,
        EDisplayUILayout
        };
    
    virtual ~CDisplayServiceBase();
    
public:
    
    /**
     * Start the display service.
     * All the contexts must be defined here and also the initial value needs
     * to be set. This can also be done asynchronously.
     * 
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    virtual void StartL() = 0;
    
    /**
     * Stop the display service.
     * 
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    virtual void Stop() = 0;
    
public:
    
    /**
     * Return the display context type.
     * 
     * @since S60 5.0
     * @param None.
     * @return None.
     */
    TInt DisplayServiceType() const;
    
protected:

    /**
     * Default C++ constructor.
     */
    CDisplayServiceBase( TDisplayServiceType aType,
        MCFContextInterface& aCF );
    
    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();

protected: // Data
    
    /** Context associated with the display service */
    CCFContextObject* iContext;
    
    /** Reference to the Context Access API */
    MCFContextInterface& iCF;
    
    /** Handle to the current thread */
    RThread iThread;

private: // Private data
    
    /** Display context type */
    TDisplayServiceType iType;
    };

#endif // DISPLAYCONTEXTBASE_H
