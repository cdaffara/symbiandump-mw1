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
* Description:  Test support file tool
 *
*/


#ifndef MPRFWTESTFILEOBSERVER_H
#define MPRFWTESTFILEOBSERVER_H

/**
 * @since S60 v4.0
 */
class MXIMPTestFileObserver
    {
    protected:

        /**
         * Cannot be deleted through this interface
         */
        virtual ~MXIMPTestFileObserver(){}

    public: // Interface

        virtual void HandleL( TInt aCommandType, TDesC8& aCommand ) = 0;

    };

#endif // CPRFWTESTFILELOGICHELPER_H