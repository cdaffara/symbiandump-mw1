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
* Description:  Handles reactivation of the previous profile when the scheduler
*                runs.
*
*/



#ifndef CPROFILECHANGEEVENT_H
#define CPROFILECHANGEEVENT_H

//  INCLUDES
#include <e32base.h>
#include <f32file.h>

// FORWARD DECLARATIONS
class MProfileEngine;

// CLASS DECLARATION

/**
* Profile Change event run by the scheduler when the timer expires.
*
*  @lib ProfileChangeEvent.lib
*  @since S60 3.2
*/
class CProfileChangeEvent : public CBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CProfileChangeEvent* NewL();

        static CProfileChangeEvent* NewLC();

        /**
        * Destructor.
        */
        virtual ~CProfileChangeEvent();

    public: // New functions

        /**
         * Creates an instance of CProfileChangeEvent and calls ChangeProfileL
         * from it.
         */
        static void ExecuteL();

    protected:  // New functions

        /**
         * Reads the ID of the profile to be activated after expiration, shows
         * the note about expiration and activates the new profile. Everything
         * as an atomic operation. If the ID of the profile to be activated is
         * invalid (means: someone just called SetActiveProfileL() after the
         * scheduler started running this dll) does nothing.
         */
        void DoProfileExpirationL();

        /**
        * Reads the ID of the profile which was active before current timed
        * profile.
        * @return ETrue if a valid ID was found.
        */
        TBool ReadProfileIdL();

        /**
         * Activates the profile which was active before timed profile.
         */
        void ChangeProfileL();

        /**
         * Shows the confirmation note about the expiration of the timed
         * profile.
         */
        void ShowNoteL();

        /**
         * Reads the resource for the confirmation note.
         */
        void ReadResourcesL();

    private:

        /**
        * C++ default constructor.
        */
        CProfileChangeEvent();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private:    // Data
        // Own: Profiles Engine for profile activation
        MProfileEngine* iProfileEngine;

        // File server session needed for resource reading:
        RFs iFs;

        // Mutex controlling access to profiles data
        RMutex iMutex;

        // The ID of the profile which should be reactivated
        TInt iPreviousId;
    };

#endif      // CPROFILECHANGEEVENT_H

// End of File
