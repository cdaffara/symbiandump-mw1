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
* Description:  XIMP Framework process starter.
*
*/

#ifndef XIMPPROCESSSTARTER_H
#define XIMPPROCESSSTARTER_H

#include <e32std.h>
class TXIMPProcessStartupParam;



/**
 *
 *
 *
 *
 * @lib ?library
 * @since S60 v4.0
 */
class XIMPProcessStarter
    {
public:     // New methods
    /**
     *
     *
     *
     *
     */
    IMPORT_C static void FullExePathForClienLocation(
                    const TDesC& aExeName,
                    TFileName& aFullExePath );

    /**
     *
     * Usage example:
     * TFixedArray< TXIMPProcessStartupParam, 2 > params;
     * params[ 0 ].Set( 1, 1001 );
     * params[ 2 ].Set( 2, _L("value") );
     * TArray< TXIMPProcessStartupParam> paramsArray = params.Array();
     * TInt err = XIMPProcessStarter::StartInstance( KMySrvServerExe,
     *                                          _L("command line"),
     *                                           &paramsArray );
     */
    IMPORT_C static TInt StartInstance(
                    const TDesC& aFullExePath,
                    const TDesC& aCommand,
                    const TArray< TXIMPProcessStartupParam >* aParams );


    /**
     *
     *
     *
     *
     */
    IMPORT_C static TInt ConnectToServer(
                    const TDesC& aFullExePath,
                    const TDesC& aCommand,
                    const TArray< TXIMPProcessStartupParam >* aParams,
                    RSessionBase& aSessionToConnect,
                    const TDesC& aServerName,
                    const TVersion& aClientVersion,
                    TInt aAsyncMessageSlots );



private:    // Helpers
    static TInt DoStartInstance(
                    const TDesC& aFullExePath,
                    const TDesC& aCommand,
                    const TArray< TXIMPProcessStartupParam >* aParams );

        static TInt ApplyParameters(
                        const TArray< TXIMPProcessStartupParam >* aParams,
                        RProcess& aProcess );

    };




/**
 * Generic process startup parameters.
 *
 *
 *
 */
class TXIMPProcessStartupParam
    {
public:

    /**
     * C++ constructor
     */
    IMPORT_C TXIMPProcessStartupParam();

    /**
     *
     */
    void Set( TInt aSlot, const RHandleBase& aHandle );
    void Set( TInt aSlot, const RSubSessionBase& aSubSession );
    void Set( TInt aSlot, const TDesC16& aDes );
    void Set( TInt aSlot, const TDesC8& aDes );
    IMPORT_C void Set( TInt aSlot, TInt aInt );


    /**
     *
     *
     * @return
     */
    TInt ApplyParam( RProcess& aProcess ) const;



private: //data

    enum TParamType{ EUnknown, EHandle, ESubSession,
                     EDes16, EDes8, EInt };
    //OWN:
    TParamType iType;

    //OWN:
    TInt iSlot;

    //OWN:
    const RHandleBase* iHandle;
    const RSubSessionBase* iSubSession;
    TPtrC16 iDes16;
    TPtrC8 iDes8;
    TInt iInt;
    };


#endif      //  XIMPPROCESSSTARTER_H


