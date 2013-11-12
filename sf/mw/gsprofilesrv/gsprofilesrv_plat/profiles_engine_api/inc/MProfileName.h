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
* Description:  Profile name interface.
*
*/



#ifndef __MPROFILENAME_H
#define __MPROFILENAME_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION

/**
*  Profile name interface.
*  This interface offers methods to get profile ID and profile name.
*
*  @lib N/A
*  @since 2.0
*/
class MProfileName
    {
    protected:  // Destructor

        virtual ~MProfileName() {};

    public:
        /**
        * Returns ID of the profile.
        * @since 2.0
        * @return Returns ID of the profile.
        */
        virtual TInt Id() const = 0;

        /**
        * Returns name of the profile.
        * @since 2.0
        * @return Returns name of the profile.
        */
        virtual const TDesC& Name() const = 0;

        /**
        * Returns the default short name of the profile. The localised string
        * returned by the method has the correct layout for navi pane with 5
        * tabs. If user has changed the name, this method returns the same as
        * Name().
        * @since 3.1
        * @return Returns short name of the profile.
        */
        virtual const TDesC& ShortName() const = 0;

        /**
        * Returns the name of the profile to be used in navi pane (that is,
        * the localised string has correct layout for navi pane). If user has
        * changed the name, this method returns the same as Name().
        * @since 3.2
        * @return Returns navi name of the profile.
        */
        virtual const TDesC& NaviName() const = 0;

        /**
        * Returns the name of the profile to be used in title pane (that is,
        * the localised string has correct layout for title pane). If user has
        * changed the name, this method returns the same as Name().
        * @since 3.2
        * @return Returns title name of the profile.
        */
        virtual const TDesC& TitleName() const = 0;
    };

#endif      // __MPROFILENAME_H

// End of File
