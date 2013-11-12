/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  RCFClientSession class declaration.
*
*/


#ifndef R_CFCLIENTSESSION_H
#define R_CFCLIENTSESSION_H

#include <e32std.h>

/**
* Client side session.
*
* @since S60 5.0
* @lib -
*/
NONSHARABLE_CLASS( RCFClientSession ) : public RSessionBase
    {
    public:
    
        /**
        * C++ constructor.
        */
        RCFClientSession();

        /**
        * Connects the client session to the Context Manager server.
        * ConnectL() should be the first function called on an
        * RCFClientSession object after it is created.
        * This function will start the server if it is not already running.
        */
        void ConnectL();
        
        /** 
        * Gets the version number of the Context Manager server.
        */
        TVersion Version() const;
        
        /**
         * Sends a synchronous request to cfserver.
         * 
         * @since S60 5.0
         * @param aFunction IPC function id.
         * @return Message completion code.
         */
        TInt SendSync( TInt aFunction ) const;
        
        /**
         * Sends a synchronous request to cfserver.
         * 
         * @since S60 5.0
         * @param aFunction IPC function id.
         * @param aArgs Message arguments.
         * @return Message completion code.
         */
        TInt SendSync( TInt aFunction, const TIpcArgs &aArgs ) const;
        
        /**
         * Sends an asynchronous request to cfserver.
         * 
         * @since S60 5.0
         * @param aFunction IPC function id.
         * @param aArgs Message arguments.
         * @param aStatus Status of the sync request.
         * @return None.
         */
        void SendAsync( TInt aFunction,
            const TIpcArgs &aArgs,
            TRequestStatus &aStatus ) const;        

    private: // New functions

        // Start server process
        TInt StartServer();
    };

#endif
