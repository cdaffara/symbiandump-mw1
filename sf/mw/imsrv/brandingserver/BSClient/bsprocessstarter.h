/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  bsprocessstarter.h
*
*/

#ifndef __PROCESSSTARTER_H__
#define __PROCESSSTARTER_H__

#include <e32std.h>
class TProcessStartupParam;



/**
 * Generic process starter.
 */
class BSProcessStarter
    {
    public:     // New methods

        static void FullExePathForClienLocation(
                        const TDesC& aExeName,
                        TFileName& aFullExePath );

        static TInt StartInstance(
                        const TDesC& aFullExePath,
                        const TDesC& aCommand,
                        const TArray< TProcessStartupParam >* aParams );

        static TInt ConnectToServer(
                        const TDesC& aFullExePath,
                        const TDesC& aCommand,
                        const TArray< TProcessStartupParam >* aParams,
                        RSessionBase& aSessionToConnect,
                        const TDesC& aServerName,
                        const TVersion& aClientVersion,
                        TInt aAsyncMessageSlots );



    private:    // Helpers
        static TInt DoStartInstance(
                        const TDesC& aFullExePath,
                        const TDesC& aCommand,
                        const TArray< TProcessStartupParam >* aParams );

        static TInt ApplyParameters(
                        const TArray< TProcessStartupParam >* aParams,
                        RProcess& aProcess );

    };




/**
 * Generic process startup parameters.
 */
class TProcessStartupParam
    {
    public:

        TProcessStartupParam();
        void Set( TInt aSlot, const RHandleBase& aHandle );
        void Set( TInt aSlot, const RSubSessionBase& aSubSession );
        void Set( TInt aSlot, const TDesC16& aDes );
        void Set( TInt aSlot, const TDesC8& aDes );
        void Set( TInt aSlot, TInt aInt );
        TInt ApplyParam( RProcess& aProcess ) const;



    private: //data

        enum TParamType{ EUnknown, EHandle, ESubSession,
                         EDes16, EDes8, EInt };

        TParamType iType;
        TInt iSlot;
        const RHandleBase* iHandle;
        const RSubSessionBase* iSubSession;
        TPtrC16 iDes16;
        TPtrC8 iDes8;
        TInt iInt;
    };


#endif      //  __PROCESSSTARTER_H__


//END OF FILE


