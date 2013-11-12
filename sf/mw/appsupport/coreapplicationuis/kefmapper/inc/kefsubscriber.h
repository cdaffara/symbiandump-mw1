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
* Description:  Publish & subscribe property monitor.
*
*/


#ifndef KEFSUBSCRIBER_H
#define KEFSUBSCRIBER_H

// INCLUDES
#include <e32base.h>
#include <e32property.h>

// CLASS DECLARATION

/**
*  Interface for observers of property subscribers.
*  
*  @lib KeyEventFw.lib
*  @since Series 60 3.2 
*/
class MKefPropertyResponder
    {
    public: // New functions
    
        /**
        * This method is called when a subscribed variable is changed.
        * @param aCategory category of the variable.
        * @param aKey key of the variable.
        */
        virtual void HandlePropertyChangedL( 
            const TUid& aCategory, 
            const TUint aKey ) = 0;
    
    };

/**
*  Monitor for one Publish & Subscribe property.
*  
*  @lib KeyEventFw.lib
*  @since Series 60 3.2 
*/
NONSHARABLE_CLASS( CKefSubscriber )
    : public CActive
    {
    public: // Constructors and destructor
        
        /**
        * Two-phased constructor.
        * @param aKefPropertyResponser observer of this monitor.
        * @param aCategory category of the property to be monitored.
        * @param aKey key of the property to be monitored.
        */         
        static CKefSubscriber* NewL( 
            MKefPropertyResponder& aKefPropertyResponder, 
            const TUid& aCategory, 
            const TUint aKey );

        /**
        * Destructor.
        */
        ~CKefSubscriber();

    public: // New functions

        /**
        * Starts monitoring Publish & Subscribe property.
        */
        void Subscribe();

    public: // Functions from base classes
    
        /**
        * From CActive, this method is called when request is completed,
        * So it means that Publish & Subscribe property has been touched.
        */
        void RunL();
    
        /**
        * From CActive, handles exceptions from RunL.
        * @param aError error code which was raised in RunL.
        * @return overridden error code.
        */
        TInt RunError( TInt aError );

        /**
        * From CActive, cancels monitoring. 
        * This method is called by CActive::Cancel.
        */
        void DoCancel();

    private:
        
        /**
        * Constructor.
        * @param aKefPropertyResponder observer of this monitor.
        * @param aCategory category of the variable to be monitored.
        * @param aKey key of the variable to be monitored.
        */
        CKefSubscriber( 
            MKefPropertyResponder& aKefPropertyResponder, 
            const TUid& aCategory, 
            const TUint aKey );

        /**
        * Symbian OS constructor.
        */
        void ConstructL();

    private:

        // Observer of this monitor.
        MKefPropertyResponder& iKefPropertyResponder;

        // Owned Publish & Subscribe property.
        RProperty iProperty;

        // Category of the variable to be monitored.
        TUid iCategory;

        // Key of the variable to be monitored.
        TUint iKey;

    };

#endif // KEFSUBSCRIBER_H

// End of File
