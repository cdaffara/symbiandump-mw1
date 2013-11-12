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
* Description:  Action owner interface.
 *
*/


#ifndef CFACTIONOWNER_H
#define CFACTIONOWNER_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

/**
 * Action owner interface.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( MCFActionOwner )
    {
public:

    // Action owner type
    enum TCFActionOwner
        {
        // Action defined by an action plug-in
        ECFInternalAction,
        // Action defined by a extermnal client
        ECFExternalAction 
        };

public:

    /**
     * Action owner.
     * @return TCFActionOwner
     */
    virtual TCFActionOwner ActionOwner() const = 0;
    };

#endif // CFACTIONOWNER_H

