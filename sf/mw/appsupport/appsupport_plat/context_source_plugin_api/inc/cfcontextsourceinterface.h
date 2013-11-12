/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef M_CFCONTEXTSOURCEINTERFACE_H
#define M_CFCONTEXTSOURCEINTERFACE_H

// CONSTANTS
const TInt KCFContextSourceInterfaceUidValue = 0x2000B60F;
const TUid KCFContextSourceInterfaceUid = {KCFContextSourceInterfaceUidValue};

// FORWARD DECLARATIONS
class CCFContextSourceCommand;

/**
* ContextSourceInterface
*
* @since S60 5.0
* @lib None-
*/
class MCFContextSource
    {
public: // New functions

    /**
    * Informs that there are context subscribers for particular context.
    *
    * @param aContextSource Context Source.
    * @param aContextType Context Type.
    */
    virtual void Subscribers( const TDesC& aContextSource,
            const TDesC& aContextType ) = 0;

    /**
    * Informs that there are no context subscribers for particular context.
    *
    * @param aContextSource Context Source.
    * @param aContextType Context Type.
    */
    virtual void NoSubscribers( const TDesC& aContextSource,
            const TDesC& aContextType ) = 0;

    /**
    * Requests the source to handle a command.
    *
    * @param aCommand Context source command.
    */
    virtual void HandleCommand( const CCFContextSourceCommand& aCommand ) = 0;

protected:

    // Deny destruction through this class
    virtual ~MCFContextSource(){};

    };

#endif // M_CFCONTEXTSOURCEINTERFACE_H
