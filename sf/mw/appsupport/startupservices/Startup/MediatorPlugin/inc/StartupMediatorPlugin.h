/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  
*
*/


#ifndef STARTUPMEDIATORPLUGIN_H
#define STARTUPMEDIATORPLUGIN_H

// INCLUDES
#include <MediatorCommandResponder.h>
#include <MediatorPluginBase.h>
#include <MediatorEventProvider.h>
#include <SecondaryDisplay/SecondaryDisplaySystemStateAPI.h>

// FORWARD DECLARATIONS

// TYPEDEFS

/**
* 
*
* @since S60 3.1
*/
class CStartupMediatorPluginPSObserver;

class CStartupMediatorPlugin : public CMediatorPluginBase,
                               public MMediatorCommandObserver
    {
    public:

        /**
         * Creates an instance of the implementation.
         *
         * @since S60 3.1
         * @return instance of this class.
         */
        static CStartupMediatorPlugin* NewL();

        /**
         * Destructor.
         */
        ~CStartupMediatorPlugin();

        /**
        * A Mediator Service event.
        *
        * @since S60 3.1        
        * @param aDomain    The identifier of the domain.
        * @param aCategory  The identifier of the category.
        * @param aEventId   The identifier of the event.
        * @param aData      The parameters of the event.
        * @return None.
        */
        void RaiseEvent( TUid aDomain,
                         TUid aCategory,
                         TInt aEventId,
                         TVersion aVersion,
                         const TDesC8& aData );

    public:
        /**
        *  Gets global system state
        *
        *  @return TInt
        */
        TInt GetSystemStateL();

        /**
        *  Gets autolock state
        *
        *  @return TInt
        */
        TInt GetAutolockStateL();

        /**
        *  Gets keyguard state
        *
        *  @return TInt
        */
        TInt GetKeyGuardStateL();


    public: // From Base class

        void StartL();

    public: // MMediatorCommandResponseObserver
        void CommandResponseL( TUid aDomain,
                               TUid aCategory, 
                               TInt aCommandId,
                               TInt aStatus, 
                               const TDesC8& aData );

    public: // MMediatorCommandObserver
        void MediatorCommandL( TUid aDomain,
                               TUid aCategory, 
                               TInt aCommandId,
                               TVersion aVersion, 
                               const TDesC8& aData );

    public: // MMediatorCommandObserver
        void CancelMediatorCommand( TUid aDomain,
                                    TUid aCategory, 
                                    TInt aCommandId );

    private:

        CStartupMediatorPlugin();
        
        void ConstructL(); 

        /**
        *  Checks if DOS is in Offline Mode
        *
        *  @return    TBool
        */
        TBool DosInOfflineModeL();

        /**
        * Check Off-line Mode status from the UI side 
        * @param None
        * @return TBool : Off-line Mode status
        */
        TBool UiInOfflineMode();

    private: // data

        CStartupMediatorPluginPSObserver*   iStartupMediatorPluginPSObserver;
        CMediatorCommandResponder*          iCommandResponder;
        CMediatorEventProvider*             iEventProvider;
    };

#endif //STARTUPMEDIATORPLUGIN_H

