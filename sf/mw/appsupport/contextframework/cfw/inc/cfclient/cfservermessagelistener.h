/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFServerMessageListener class declaration.
*
*/


#ifndef C_CFSERVERMESSAGELISTENER_H
#define C_CFSERVERMESSAGELISTENER_H

#include <e32base.h>

class RCFClientSession;
class MCFListener;
class RReadStream;


/**
 *  CCFServerMessageListener class
 *
 *  @lib -
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CCFServerMessageListener ): public CActive
    {
    public:
    
        // Two phased constructors
        static CCFServerMessageListener* NewL( MCFListener& aListener,
            RCFClientSession& aSession );
        static CCFServerMessageListener* NewLC( MCFListener& aListener,
            RCFClientSession& aSession );
    
        // Destructor
        ~CCFServerMessageListener();
        
    public: // New methods
    
        /**
        * Start receiving server messages.
        *
        * @since S60 5.0
        * @param None
        * @return None
        */
        void ReceiveMessages();
        
    private: // From base classes

        // From CActive
        void RunL();
        
        // From CActive
        void DoCancel();
        
        // From CActive
        TInt RunError( TInt aError );
        
    private: // New methods
    
        // Interprets server message from stream and sends it to client
        void NotifyMessageL();
        
        // Sends context framework error to client
        void NotifyMessageErrorL();
        
        // Update buffer
        void UpdateBufferL();
        
        // Returns descriptor from a read stream;
        // format: [TInt32 des len][des len bytes]
        HBufC* DescriptorLC( RReadStream& aReadStream );
        
    private:
    
        CCFServerMessageListener( MCFListener& aListener, 
            RCFClientSession& aSession );
        void ConstructL();
        
    private:

        // Ref: Context manager client session
        RCFClientSession& iSession;
        
        // Ref: Context manager client
        MCFListener& iListener;
        
        // Own: Message receive buffer
        HBufC8* iBuffer;

        // Own: Message receive buffer pointer
        TPtr8 iBufferPtr;

        // Own: Buffer size
        TInt iBufferSize;
        
        // Own: Buffer size pckg
        TPckg<TInt> iBufferSizePckg;

		// Flag indicating if we have been deleted from observer call back
        TBool* iDied;

    };

#endif
