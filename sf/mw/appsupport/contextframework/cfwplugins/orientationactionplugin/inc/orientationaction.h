/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  COrientationAction class declaration.
*
*/


#ifndef C_ORIENTATIONACTION_H
#define C_ORIENTATIONACTION_H

#include <e32base.h>
#include <e32property.h>
#include <AknCapServerClient.h>
#include <w32std.h>

class CAknLayoutConfig;
class CWsScreenDevice;

/**
* Orientation action.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( COrientationAction ): public CBase
    {
public:

    // Two phased constructors
    static COrientationAction* NewL();
    static COrientationAction* NewLC();

    // Destructor
    ~COrientationAction();

public: // New methods

    /**
     * Executes orientation action.
     * Basically switches current orientation if not already
     * in the orientation to be switched.
     * 
     * @since S60 5.0
     * @param aAction Orientation action to execute
     * @return KErrNone if no errors.
     */
    TInt Execute( const TDesC& aMode );

private:

    COrientationAction();
    void ConstructL();
    void RotateL( const TDesC& aMode );

private: // Data
    /** Handle to window server session **/
    RWsSession iWsSession;
    };

#endif
