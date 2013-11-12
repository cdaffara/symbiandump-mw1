/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Profile change notify handler.
*
*/



#ifndef __CPROFILECHANGENOTIFYHANDLER_H__
#define __CPROFILECHANGENOTIFYHANDLER_H__

//  INCLUDES
#include <e32base.h>
#include <e32property.h>

// FORWARD DECLARATIONS
class CRepository;

class MProfileChangeObserver;

// CLASS DECLARATION

/**
*  Profile change notify handler.
*  How to use:
*  class CMyClass : public CBase, public MProfileChangeObserver
*      {
*      ...
*      virtual void HandleActiveProfileEventL(
*              TProfileEvent aProfileEvent, TInt aProfileId );
*      ...
*      private:    // data
*          CProfileChangeNotifyHandler* iHandler;
*
*      }
*  void CMyClass::ConstructL()
*      {
*      iHandler = CProfileChangeNotifyHandler::NewL( this );
*      }
*
*  void CMyClass::HandleActiveProfileEventL(
*              TProfileEvent aProfileEvent,
*              TInt aProfileId )
*      {
*      // handle event
*      }
*
*  @lib ProfileEng.lib
*  @since 2.0
*/

NONSHARABLE_CLASS(CProfileChangeNotifyHandler) : public CBase
    {
    private: // class definitions

        NONSHARABLE_CLASS(CProfilePSObserver) : public CActive
            {
            public:  // constructor and destructor
                CProfilePSObserver( TUint32 aPSKey );
                ~CProfilePSObserver(); 

            public: // New functions:

                /**
                 * Requests notifications from P&S of KProEngActiveProfile-
                 * Modified value changes.
                 */
                void RequestNotificationL();

            private: // Functions from base classes

                /**
                 * From CActive.
                 */
                void RunL();
                void DoCancel();

            public: // Data:

                // The P&S key to observe
                TUint32 iPSKey;

                // Ref:
                CRepository* iCenRep;

                // Own:
                RProperty iProperty;

                // Ref: Active profile change observer
                MProfileChangeObserver* iProfileChangeObserver;

                TBool iAttached;

            };

    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        * @param aProfileChangeObserver Profile change observer.
        */
        IMPORT_C static CProfileChangeNotifyHandler* NewL(
            MProfileChangeObserver* aProfileChangeObserver );

        /**
        * Destructor.
        */
        virtual ~CProfileChangeNotifyHandler();

    private:

        /**
        * C++ default constructor.
        * @param aProfileChangeObserver Profile change observer.
        */
        CProfileChangeNotifyHandler(
            MProfileChangeObserver* aProfileChangeObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data

        // Ref: Active profile change observer
        MProfileChangeObserver* iProfileChangeObserver;

        // Own:
        CProfilePSObserver* iActiveProfileObserver;

        // Own:
        CProfilePSObserver* iPSObserver;

        // Own:
        CRepository* iRepository;
    };

#endif      //  __CPROFILECHANGENOTIFYHANDLER_H__

// End of File
