/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFContextSourceCommand interface.
 *
*/


#ifndef M_CFCONTEXTSOURCECOMMANDINTERFACE_H
#define M_CFCONTEXTSOURCECOMMANDINTERFACE_H


// SYSTEM INCLUDE FILES
#include <e32std.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFContextSourceCommand;

// DATA TYPES

// CLASS DECLARATION

/**
 * Context source command interface.
 *
 * @lib CFServer
 * @since S60 5.0
 */
class MCFContextSourceCommand
{
public:

    /**
    * Request context source to handle a command.
    *
    * @param aCommand is the context source command.
    * @return None.
    */
    virtual void HandleCommandL( const CCFContextSourceCommand& aCommand ) = 0;

};

#endif // M_CFCONTEXTSOURCECOMMANDINTERFACE_H
