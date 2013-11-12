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
* Description:  CCFContextSubscriptionListener class declaration.
*
*/


#ifndef C_CFCONTEXTSUBSCRIPTIONLISTENER_H
#define C_CFCONTEXTSUBSCRIPTIONLISTENER_H

#include <e32base.h>

class RCFClientSession;
class MCFListener;

/**
 *  CCFContextSubscriptionListener class
 *
 *  @lib -
 *  @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextSubscriptionListener ): public CActive
    {
    public:
    
        // Two phased constructors
        static CCFContextSubscriptionListener* NewL( MCFListener& aListener,
            RCFClientSession& aSession );
        static CCFContextSubscriptionListener* NewLC( MCFListener& aListener,
            RCFClientSession& aSession );
    
        // Destructor
        ~CCFContextSubscriptionListener();
        
    public: // New methods
    
        /**
        * Start receiving context indication and subscription error
        * messages.
        *
        * @since S60 5.0
        * @param None
        * @return None
        */
        void ReceiveContextIndications();
        
    private: // From base classes

        // From CActive
        void RunL();
        
        // From CActive
        void DoCancel();
        
        // From CActive
        TInt RunError( TInt aError );
        
    private: // New methods
    
        // Interprets context indication from stream and sends it to client
        void NotifyContextIndicationL();
        
        // Interprets context indication from stream
        void NotifyContextIndicationErrorL();
        
        // Updates receive buffer - leavable version
        void UpdateBufferL();
        
        // Updates receive buffer - non leavable version
        void UpdateBuffer();
        
    private:
    
        CCFContextSubscriptionListener( MCFListener& aListener, 
            RCFClientSession& aSession );
        void ConstructL();
        
    private:

        // Ref: Context manager client session
        RCFClientSession& iSession;
        
        // Ref: Context manager client
        MCFListener& iListener;
        
        // Own: Indication receive buffer
        HBufC8* iBuffer;

        // Own: Indication receive buffer pointer
        TPtr8 iBufferPtr;

        // Own: Buffer size
        TInt iBufferSize;
        
        // Own: Buffer size pckg
        TPckg<TInt> iBufferSizePckg;

		// Flag indicating if we have been deleted from observer call back
        TBool* iDied;
    };

#endif
