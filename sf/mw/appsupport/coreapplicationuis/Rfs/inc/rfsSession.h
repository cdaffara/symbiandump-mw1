/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*  Session to RFS server.
*
*
*/


#ifndef RFS_SESSION_H
#define RFS_SESSION_H

//  INCLUDES

#include <e32base.h>
#include "rfsClient.h"
#include "rfsCompleter.h"

// CONSTANTS

enum TRfsScriptLocation
    {
    EScriptInFile,
    EScriptInDescriptor
    };

const TInt KRfsRequestCompleted = -200;

// FORWARD DECLARATIONS

class CRfsIsiMsg;
class CRfsServer;

// CLASS DECLARATION

/**
*  This class represents a session in the server.
*  Functions are provided to respond appropriately to client messages.
*/

class CRfsSession : public CSession2
    {
    public:  // Constructors and destructor

        static CRfsSession* NewL();

        /**
        * Destructor.
        */
        virtual ~CRfsSession();

    public: // New functions

        /**
        * Handles a client request; tests the opcode and gives
        * service according to that.
        *
        * @param aMessage service request message from client
        *
        * @return standard Symbian OS errorcode
        *
        * Leaves if:
        * - a service routine leaves
        */
        TInt DispatchMessageL( const RMessage2& aMessage );

        /**
        * Below are handlers for different services.
        * They get the parameters from client message and
        * call the desired service routine.
        * 
        * They also do the necessary inter-thread data transfers.
        * 
        * Leave if:
        * - a service routine leaves
        *
        * Leaves are trapped by ServiceL() which returns the
        * errorcode to the client.
        */

        // services available

            TInt RunScriptL( TRfsScriptLocation aLocation );
            TInt PerformRfsL();    
            TInt PerformPostEikonRfs();

        /**
        * Returns the epoc RFS completer object owned by the session.
        */
        inline CRfsCompleter* EpocCompleter();

        /**
        * Returns pointer of the request status of the Symbian OS completer object
        * owned by the session.
        */
        inline TRequestStatus* EpocCompleterRequestStatus();

        /**
        * Returns the DOS RFS completer object owned by the session.
        */
        inline CRfsCompleter* DosCompleter();

        /**
        * Returns pointer of the request status of the DOS completer object
        * owned by the session.
        */
        inline TRequestStatus* DosCompleterRequestStatus();

        /**
        * Gives information of whether the client request has been handled
        * regarding both Symbian OS and DOS sides.
        */
        inline TBool RequestHandled();

        /**
        * Completes the client's asynchronous request. This is called from
        * completer object's RunL() when both Symbian OS and DOS sides have been
        * completed.
        */
        inline void CompleteMessage();

		/**
        * Executes the DOS side Rfs.
        */

        /**
        * Returns the last message arrived from the client
        *
        * @return                 Current message
        */
        RMessage2& Message();

    public: // Functions from base classes

        /**
        * From CSession2 (pure virtual)
        * Handles service request messages from clients.
        * @param aMessage message from client
        */
        void ServiceL( const RMessage2& aMessage );

    private:

        /**
        * C++ default constructor.
        */
        CRfsSession();

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

        /**
        * Panics the client thread
        * @param aPanic panic reason
        */      
        void PanicClient( TRfsPanic aPanic ) const;

        /**
        * server down-cast
        */
        inline CRfsServer* Server();

        /**
        * Checks security of coming message
        * @param aMessage message from client
        */
        TInt CheckSecurity( const RMessage2 &aMessage );

    private:    // Data

        CRfsCompleter* iEpocCompleter;      // owned
        CRfsCompleter* iDosCompleter;       // owned
        TInt iChangeDirStatus;

        /** 
        * The last message arrived from the client
        *   
        */
        RMessage2 iCurrentMessage;

    private:    // friend classes

        friend class CRfsServer;
        friend class CRfsCommand;
    };

    #include "rfsSession.inl"

#endif      // RFS_SESSION_H   
            
// End of File
