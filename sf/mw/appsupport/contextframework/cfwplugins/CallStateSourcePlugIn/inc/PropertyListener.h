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
* Description:  CPropertyListener class declaration.
*
*/



#ifndef C_PROPERTYLISTENER_H
#define C_PROPERTYLISTENER_H

#include <e32base.h>
#include <e32property.h>

class MPropertyListenerCallBack;

/**
* P&S property listener.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CPropertyListener ): public CActive
    {
    public:
    
        // Two phased constructors
        static CPropertyListener* NewL( const TUid& aCategory,
            const TUint32& aKey,
            MPropertyListenerCallBack& aCallBack );
        static CPropertyListener* NewLC( const TUid& aCategory,
            const TUint32& aKey,
            MPropertyListenerCallBack& aCallBack );
            
        // Destructor
        ~CPropertyListener();
        
    public: // New methods
    
        /**
        * Returns handle to the property.
        *
        * @since S60 4.0
        * @param None
        * @return RProperty&
        */
        RProperty& Property();
        
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
        
    private:
    
        CPropertyListener( MPropertyListenerCallBack& aCallBack );
        void ConstructL( const TUid& aCategory,
            const TUint32& aKey );  
        
        static TInt StartCallback(TAny* aPtr);
        void DoStart();
        
    private:
    
        // Own: P&S property
        RProperty iProperty;
        
        // Ref: Call back
        MPropertyListenerCallBack& iCallBack;
        
        CAsyncCallBack *iStartCB;
    };
    
#endif//C_PROPERTYLISTENER_H
