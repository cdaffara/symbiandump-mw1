/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFServer utils
*
*/


#ifndef CFUTILS_H
#define CFUTILS_H


#include <e32base.h>

/**
* Context Framework utils class.
*/
NONSHARABLE_CLASS( CFUtils )
    {
    public:
    
        /**
        * Compares whether the thread is from CFW.
        * 
        * @since S60 5.0
        * @param aClientThread Thread to compare.
        * @return ETrue if thread from CFW process.
        */
        static TBool RequestFromSameProcess( const RThread& aClientThread );
    };

#endif  // CFUTILS_H
