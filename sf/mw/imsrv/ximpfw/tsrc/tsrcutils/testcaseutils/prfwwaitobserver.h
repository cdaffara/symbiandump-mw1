/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  XIMP Framework Test Code prfwwaitobserver.h
*
*/



#ifndef T_PRFWWAITOBSERVER
#define T_PRFWWAITOBSERVER


/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class T_MWaitObserver
    {

public:

    /**
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
    virtual void WaitCompleted( TInt aRetVal ) = 0;


protected:

    /**
     * Protected destructor.
     * Instancies can't be destroyed via this interface.
     */
    virtual ~T_MWaitObserver() {};

    };



#endif // T_PRFWWAITOBSERVER


