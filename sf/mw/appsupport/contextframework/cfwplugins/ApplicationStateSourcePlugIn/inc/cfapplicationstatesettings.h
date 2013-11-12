/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsulates information about application and its views.
*
*/


#ifndef CFAPPLICATIONSTATESETTINGS_H
#define CFAPPLICATIONSTATESETTINGS_H


#include <e32base.h>

#include "cfuidnamepair.h"

/**
*
*/
/**
*  Encapsulates information about application and its views.
*
*  Contains uid name pairs of application itself and its views.
*
*  @lib
*  @since S60 v4.0
*/
class CCFApplicationStateSettings : public CBase
    {
    public:

        /**
        * Two phased constructor.
        *
        * @since S60 4.0
        * @param aUid Uid.
        * @param aName Name.
        * @return CCFApplicationStateSettings*
        */
        static CCFApplicationStateSettings* NewL(
            const TUid& aUid,
            const TDesC& aName );
        static CCFApplicationStateSettings* NewLC(
            const TUid& aUid,
            const TDesC& aName );

        virtual ~CCFApplicationStateSettings();

    public:

        /**
        * Returns application uid.
        *
        * @since S60 4.0
        * @param None
        * @return const TUid&
        */
        const TUid& Uid() const;

        /**
        * Returns application name.
        *
        * @since S60 4.0
        * @param None
        * @return const TDesC&
        */
        const TDesC& Name() const;

        /**
        * Adds view settings.
        *
        * @since S60 4.0
        * @param aViewSettings Uid and name of the view, ownership is
        *  transferred.
        * @return None
        */
        void AddViewSettingsL( CCFUidNamePair* aViewSettings );

        /**
        * Gets view name.
        *
        * @since S60 4.0
        * @param aUid Defines the view whose name is got.
        * @param aName Pointer for the view name. Is modified only if the view
        *  is found.
        * @return None
        */
        void GetViewName( const TUid& aUid, TPtrC& aName ) const;

    private:

        CCFApplicationStateSettings();

        void ConstructL( const TUid& aUid, const TDesC& aName );

    private: // Data

        // Application uid name pair.
        CCFUidNamePair* iApplicationInfo;

        // Application view's uid name pairs.
        RUidNamePointerArray iViewInfo;
    };

typedef RPointerArray< CCFApplicationStateSettings > RApplicationStateSettingsPointerArray;

#endif // CFAPPLICATIONSTATESETTINGS_H
