/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __SYSAPPROPERTYRESPONDER__
#define __SYSAPPROPERTYRESPONDER__

// INCLUDES
#include <e32base.h>

class MSysApPropertyResponder
    {
public:
    virtual void HandlePropertyChangedL( const TUid& aCategory, const TUint aKey ) = 0;
    };

#endif

// End of File
