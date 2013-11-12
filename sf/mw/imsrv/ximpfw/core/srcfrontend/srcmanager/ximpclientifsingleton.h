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
* Description:  Reference counted singleton for XIMP client side.
*
*/

#ifndef CXIMPCLIENTIFSINGLETON_H
#define CXIMPCLIENTIFSINGLETON_H

#include <e32base.h>


class RXIMPSrvRootClient;



/**
 * Reference counted singleton for XIMP Framework
 * client IF library.
 *
 * @lib ximpclientinterface.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPClientIfSingleton ) : public CObject
    {
    /** Private nested structure to hold global objects */
    struct SGlobals;


public:  // singleton access

    /**
     * Singleton access method.
     *
     * Gets pointer to XIMP Client Interface library singleton.
     */
    static CXIMPClientIfSingleton* InstanceL();



private:

    /**
     * Two-phased constructor.
     * Leaves created object on the CleanupStack.
     */
    static CXIMPClientIfSingleton* NewLC();


    /**
     * C++ default constructor.
     */
    CXIMPClientIfSingleton();


    /**
     * Private destructor.
     *
     * Singleton holder is reference counted.
     * It is automaticly destroyed when last
     * holded object or reference is destroyed.
     */
    ~CXIMPClientIfSingleton();



    /**
     * Symbian 2nd phase constructor
     */
    void ConstructL();



public:


    /**
     * Gets reference to XIMP client side Root Server session.
     *
     * @return Connected XIMP Root Server session.
     */
    RXIMPSrvRootClient& RootClient();


private:    // Data


    /**
     * Object holder having pointers to XIMP client side global objects.
     * Both holder and pointed objects own.
     */
    SGlobals* iGlobals;
    };

#endif      // CXIMPCLIENTIFSINGLETON_H


