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
* Description:  This class implements MProEngNotifyHandler interface.
*
*/



#ifndef CPROENGNOTIFYHANDLERIMPL_H
#define CPROENGNOTIFYHANDLERIMPL_H

// INCLUDES
#include <MProEngNotifyHandler.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CProEngProfileActivationDelegate;
class CProEngActiveProfileDelegate;
class CProEngProfileNameArrayEventDelegate;
class CProEngProfileEventDelegate;

// CLASS DECLARATION

/**
* This class implements MProEngNotifyHandler interface.
*
*  @lib ProfileEngine.lib
*  @since 3.1
*/
NONSHARABLE_CLASS( CProEngNotifyHandlerImpl ) : public CBase,
                                                public MProEngNotifyHandler
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CProEngNotifyHandlerImpl* NewL();
        
        /**
         * Destructor.
         */
          ~CProEngNotifyHandlerImpl();

    public:

        /**
         * From MProEngNotifyHandler
         */
        TInt RequestProfileActivationNotificationsL(
                MProEngProfileActivationObserver& aObserver );

        /**
         * From MProEngNotifyHandler
         */
        TInt RequestActiveProfileNotificationsL(
                MProEngActiveProfileObserver& aObserver );

        /**
         * From MProEngNotifyHandler
         */
        TInt RequestProfileNotificationsL(
                MProEngProfileObserver& aObserver,
                TInt aProfileId );

        /**
         * From MProEngNotifyHandler
         */
        TInt RequestProfileNameArrayNotificationsL(
                MProEngProfileNameArrayObserver& aObserver );

        /**
         * From MProEngNotifyHandler
         */
        void CancelProfileActivationNotifications();

        /**
         * From MProEngNotifyHandler
         */
        void CancelActiveProfileNotifications();

        /**
         * From MProEngNotifyHandler
         */
        void CancelProfileNotifications( TInt aProfileId );

        /**
         * From MProEngNotifyHandler
         */
        void CancelProfileNameArrayNotifications();

        /**
         * From MProEngNotifyHandler
         */
        void CancelAll();

    private:

        /**
         * C++ default constructor.
         */
        CProEngNotifyHandlerImpl();

    private:    // Data

        // Own: Delegate for active profile change events
        CProEngProfileActivationDelegate* iActiveIdEventDelegate;

        // Own: Delegate for active profile modification events
        CProEngActiveProfileDelegate* iActiveProfileEventDelegate;

        // Own: Delegate for profile name array modification events
        CProEngProfileNameArrayEventDelegate* iNameArrayEventDelegate;

        // Own: Array of profile change observers
        RPointerArray< CProEngProfileEventDelegate > iProfileEventDelegates;

    };

#endif      //  CPROENGNOTIFYHANDLERIMPL_H

// End of File

