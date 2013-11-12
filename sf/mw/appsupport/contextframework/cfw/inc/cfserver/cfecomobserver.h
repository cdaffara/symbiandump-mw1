/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFEcomObserver class implementation.
 *
*/


#ifndef CFECOMOBSERVER_H
#define CFECOMOBSERVER_H

#include <e32base.h>	// For CActive, link against: euser.lib

#include <e32std.h>		
#include <ecom/ecom.h>

// FORWARD DECLARATIONS
class MCFStateObserver;
class CPeriodic;

class CCFEComObserver : public CActive
    {
public:
    // Cancel and destroy
    ~CCFEComObserver( );

    // Two-phased constructor.
    static CCFEComObserver* NewL( );

    // Two-phased constructor.
    static CCFEComObserver* NewLC( );

public: // New functions

    /**
     * Adds a new observer for plugin notifications.
     * @param aObserver Observer pointer.
     */
    void AddObserverL( MCFStateObserver* aObserver );

private:

    // Start observing ecom notification
    void StartObserving( );

private:

    CCFEComObserver( );
    void ConstructL( );

private: // From CActive

    // Handle completion
    void RunL( );

    // How to cancel me
    void DoCancel( );

    // Override to handle leaves from RunL(). Default implementation causes
    // the active scheduler to panic.
    TInt RunError( TInt aError );
    
private:

    // Notifies observers
    void NotifyObservers();

private:

    // Own: ECom session handle (must be closed, not deleted)
    REComSession* iEComSession;

    // Own: Observers - pointers not owned
    RPointerArray<MCFStateObserver> iObservers;
    };

#endif // CFECOMOBSERVER_H

