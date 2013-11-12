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
* Description:  CUidOrientationPair class declaration.
*
*/


#ifndef UIDORIENTATIONPAIR_H
#define UIDORIENTATIONPAIR_H

// SYSTEM INCLUDE FILES
#include <e32std.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * This class wraps inside an application UID and its orientation.
 *
 * @lib None.
 * @since S60 5.0
 */
class TUidOrientationPair
    {
public:

    /**
     * C++ constructor.
     */
    TUidOrientationPair( const TUid& aUid, TInt aOrientation );

public: // Data
    
    /** Application UID */
    TUid iUid;
    
    /** Application orientation */
    TInt iOrientation;
    };

// List definition
typedef RArray<TUidOrientationPair> RUidOrientationList;

#endif // UIDORIENTATIONPAIR_H
