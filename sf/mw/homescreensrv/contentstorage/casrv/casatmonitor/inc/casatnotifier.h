/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: casatnotifier.h
*
*/

#ifndef CCASATNOTIFIER_H
#define CCASATNOTIFIER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

//Forward declatations
class RProperty;

/*
 *   Callback for notifier.
 */
class MCaSatNotifierCallback
    {
public:

    /**
     * SatChangeL is called when the subscribed key has been changed
     *
     */
    virtual void SatChangeL() = 0;

    };

/**
 * Central repository notifier
 */
class CCaSatNotifier: public CActive
    {
public:
    // Construction and destruction
    /**
     * Creates an instance of CCaSatNotifier implementation.
     * @param aCallBack Reference to notifier interface
     * @param aCategory application uid
     * @param aKey key for central repository
     */
    static CCaSatNotifier* NewLC(
            MCaSatNotifierCallback* aCallback, TUid aCategory, TUint aKey );

    /**
     * Creates an instance of CCaSatNotifier implementation.
     * @param aCallBack Reference to notifier interface
     * @param aCategory application uid
     * @param aKey key for central repository
     */
    static CCaSatNotifier* NewL(
            MCaSatNotifierCallback* aCallback, TUid aCategory, TUint aKey );

    /**
     * Destructor.
     */
    virtual ~CCaSatNotifier();

private:
    // From base classes

    /**
     * From CActive.
     */
    void DoCancel();
    void RunL();
    TInt RunError( TInt aError );

private:

    // Constructor
    CCaSatNotifier( MCaSatNotifierCallback* aCallback, TUid aCategory,
            TUint aKey );

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    // Data

    // User side interface to Publish & Subscribe.
    RProperty iProperty;

    // Interface for notifying changes in SAT
    MCaSatNotifierCallback* iCallback;

    // category uid
    TUid iCategory;

    // key identifier
    TUint iKey;

    };

#endif // CCASATNOTIFIER_H
// End of File
