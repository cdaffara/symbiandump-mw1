/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFDelayExpired interface declaration.
*
*/



#ifndef M_CFDELAYEXPIRED_H
#define M_CFDELAYEXPIRED_H


/**
* Delay expired interface.
*
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( MCFDelayExpired )
    {
public:

    /**
    * Informs that the delay has expired.
    * @since Series 60 4.0
    * @return None.
    */
    virtual void ExpiredL() = 0;
    };

#endif // M_CFDELAYEXPIRED_H
