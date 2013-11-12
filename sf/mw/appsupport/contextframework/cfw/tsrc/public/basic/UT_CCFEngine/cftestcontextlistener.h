/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFTestContextListener class declaration
*
*/


#ifndef C_CFTESTCONTEXTLISTENER_H
#define C_CFTESTCONTEXTLISTENER_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES
#include "CFContextSubscriptionListener.h"

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Implements MCFContextSubscriptionListener interface for test purposes.
 *
 * @lib lib
 * @since S60 5.0
 */
class CCFTestContextListener : public CBase,
    public MCFContextSubscriptionListener
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFTestContextListener
     */
    static CCFTestContextListener* NewL();
    static CCFTestContextListener* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~CCFTestContextListener();

public: // From MCFContextSubscriptionListener

    // @see MCFContextSubscriptionListener
    void ContextIndicationL( CCFContextIndication* aIndication );

    // @see MCFContextSubscriptionListener
    TInt Client( RThread& aThread ) const;

    // @see MCFContextSubscriptionListener
    void HandleContextSubscriptionError( TInt aError,
        const TDesC& aSource,
        const TDesC& aType );

private:

    CCFTestContextListener();
    void ConstructL();

public: // Data

    // The last received indication, owned.
    CCFContextIndication* iLastIndication;

    TBool iSubscriptionError;
    };

#endif // C_CFTESTCONTEXTLISTENER_H
