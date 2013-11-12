/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CStartupMediatorObserver class
*
*/


#ifndef STARTUPMEDIATOROBSERVER_H
#define STARTUPMEDIATOROBSERVER_H

#include <MediatorCommandInitiator.h>
#include <MediatorEventProvider.h>

class CStartupAppUi;

class CStartupMediatorObserver : public CBase,
                                 public MMediatorCommandResponseObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ constructor.
        */
        CStartupMediatorObserver( CStartupAppUi* aStartupAppUi );

        /**
        * Two-phased constructor.
        */
        static CStartupMediatorObserver* NewL( CStartupAppUi* aStartupAppUi );

        /**
        * Destructor.
        */
        ~CStartupMediatorObserver();

        void IssueCommand(TInt aCommandId, TInt aData);

        void RaiseEvent( TUid aCategory,
                         TInt aEventId,
                         const TDesC8& aData );

    public: // MMediatorCommandResponseObserver

        void CommandResponseL( TUid aDomain,
                           TUid aCategory,
                           TInt aCommandId,
                           TInt aStatus,
                           const TDesC8& aData );

    private:

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

    private: //data

        CStartupAppUi*              iStartupAppUi; //uses
        CMediatorCommandInitiator*  iCommandInitiator;
        CMediatorEventProvider*     iEventProvider;
        TInt                        iSyncData;
    };

#endif      // STARTUPMEDIATOROBSERVER_H

// End of File
