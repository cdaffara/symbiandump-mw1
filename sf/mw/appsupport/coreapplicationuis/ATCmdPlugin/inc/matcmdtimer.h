
/*
 * Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 * Description :Abstract interface for MATCmdTimer callback functionality
 *
 */



#ifndef __MATCMDTIMER_H
#define __MATCMDTIMER_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS

// CLASS DECLARATION

class MATCmdTimer
    {
    public:
        virtual void TimerExpiredL( TInt aRelatedCommand ) = 0;
    };

#endif // __MATCMDTIMER_H

            
// End of File
