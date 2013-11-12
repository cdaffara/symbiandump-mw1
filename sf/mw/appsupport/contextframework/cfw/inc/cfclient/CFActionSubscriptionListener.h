/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef C_CFACTIONSUBSCRIPTIONLISTENER_H
#define C_CFACTIONSUBSCRIPTIONLISTENER_H

#include <e32base.h>

#include "cflistener.h"
#include "CFClientSession.h"

class CCFActionIndication;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
NONSHARABLE_CLASS( CCFActionSubscriptionListener ): public CActive
    {
    public:
    
        // Two phased constrcutors
        static CCFActionSubscriptionListener* NewL( MCFListener& aCFListener,
            RCFClientSession& aSession );
        static CCFActionSubscriptionListener* NewLC( MCFListener& aCFListener,
            RCFClientSession& aSession );
    
        // Destructor
        virtual ~CCFActionSubscriptionListener();
        
    public: // New methods
    
        /**
        * Activates action indication listening in CF server side.
        * Listening will be always re-activated when RunL is called.
        * Asynchronous.
        *
        * @since S60 4.0
        * @param None
        * @return None
        */
        void RequestActionIndication();
        
    private: // From base classes

        // From CActive
        void RunL();
        
        // From CActive
        void DoCancel();
        
    private: // New methods
    
        // Interprets action indication from data buffer
        CCFActionIndication* InterpretActionIndicationLC();
        
        // Notify client
        void NotifyActionIndicationL();
        
    private:
    
        CCFActionSubscriptionListener( MCFListener& aCFListener,
            RCFClientSession& aSession );
        void ConstructL();
        
    private:

        // Ref: Context manager client session
        RCFClientSession& iSession;
        
        // Ref: CF listener
        MCFListener& iCFListener;
        
        // Own: Indication buffer size
        TInt iIndicationBufferSize;
        
        // Own: Indication buffer size pckg
        TPckg<TInt> iIndicationBufferSizePckg;

        // Own: Indication buffer
        HBufC8* iIndicationBuffer;

        // Own: Indication buffer pointer
        TPtr8 iIndicationBufferPtr;
        
        // Flag indicating if we have been deleted from observer call back
        TBool* iDied;
    };

#endif//C_CFACTIONSUBSCRIPTIONLISTENER_H
