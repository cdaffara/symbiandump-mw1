/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFPropertyListener class declaration.
*
*/



#ifndef C_CFPROPERTYLISTENER_H
#define C_CFPROPERTYLISTENER_H

#include <e32base.h>
#include <e32property.h>

class MCFPropertyListenerCallBack;

/**
* P&S property listener.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CCFPropertyListener ): public CActive
    {
    public:
    
        // Two phased constructors
        static CCFPropertyListener* NewL( const TUid& aCategory,
            const TUint32& aKey,
            MCFPropertyListenerCallBack& aCallBack );
        static CCFPropertyListener* NewLC( const TUid& aCategory,
            const TUint32& aKey,
            MCFPropertyListenerCallBack& aCallBack );
            
        // Destructor
        ~CCFPropertyListener();
        
    public: // New methods
    
        /**
        * Returns handle to the property.
        *
        * @since S60 4.0
        * @param None
        * @return RProperty&
        */
        RProperty& Property();
        
        /**
        * Stops listening property.
        * 
        * @since S60 4.0
        * @param None.
        * @return None.
        */
        void StopListening();
        
    private: // From base classes
    
        // @see CActive
        void RunL();
    
        // @see CActive
        void DoCancel();
    
        // @see CActive
        TInt RunError( TInt aError );
        
    private: // New methods
    
        // Start listening
        void Listen();
        
        static TInt StartCallback(TAny* aPtr);
        void DoStart();

    private:
    
        CCFPropertyListener( MCFPropertyListenerCallBack& aCallBack );
        void ConstructL( const TUid& aCategory,
            const TUint32& aKey );  
        
    private:
    
        // Own: P&S property
        RProperty iProperty;
        
        // Ref: Call back
        MCFPropertyListenerCallBack& iCallBack;
        
        CAsyncCallBack *iStartCB;
    };
    
#endif
