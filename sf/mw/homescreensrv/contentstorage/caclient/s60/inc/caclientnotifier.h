/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
 *  Description : Content Arsenal client session
 *
 */

#ifndef CACLIENTNOTIFIER_H_
#define CACLIENTNOTIFIER_H_

#include <e32base.h>

class RCaClientSubSession;

class CCaClientNotifier: public CActive
    {
public:

    /**
     * Two-phased constructor.
     * @param aSubsession reference to a subsession.
     * @return pointer to a newly created instance of CCaClientNotifier.
     */
    static CCaClientNotifier* NewL( RCaClientSubSession* aSubsession );

    /**
     * Two-phased constructor. Leaving version.
     * @param aSubsession reference to a subsession.
     * @return pointer to a newly created instance of CCaClientNotifier.
     */
    static CCaClientNotifier* NewLC( RCaClientSubSession* aSubsession );

    /**
     * Desctructor.
     */
    ~CCaClientNotifier();

protected:

    /**
     * From CActive, RunL.
     * Handles the active object’s request completion event
     */
    void RunL();

    /**
     * From CActive, DoCancel.
     * Implements cancellation of an outstanding request.
     */
    void DoCancel();

    /**
     * From CActive, RunError.
     * Implements cancellation of an outstanding request.
     * @param aError error code.
     * return error code.
     */
    TInt RunError( TInt /*aError*/ );

private:

    /**
     * C++ default constructor.
     * @param aSubsession a pointer to a subsession.
     */
    CCaClientNotifier( RCaClientSubSession* aSubsession );

    /**
     * Perform the second phase construction of a CCaClientNotifier object.
     */
    void ConstructL();

private:

    /**
     * Client-server session
     * Not Own.
     */
    const RCaClientSubSession* iSubsession;

    };

#endif /* CACLIENTNOTIFIER_H_ */
