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
* Description:  CCFContextSubscription class declaration.
*
*/


#ifndef C_CFCONTEXTSUBSCRIPTION_H
#define C_CFCONTEXTSUBSCRIPTION_H

#include <e32base.h>

/**
* Subscription to get information of context changes.
*
* @lib cfservices.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextSubscription ): public CBase
    {
    public:
 
        // Two phased constructors
        IMPORT_C static CCFContextSubscription* NewL();
        IMPORT_C static CCFContextSubscription* NewLC();

    public: // New methods
    
        /**
        * Sets desired context type.
        * Copies the entire argument to the context type member variable. 
        *
        * @since S60 5.0
        * @param aContextType 
        *  A complete or partial context type from the ontology.
        *  If the context type is partial all the context changes
        *  starting with this type are indicated. <br>
        *  Example: Environment.Temperature<br>
        *  Example2: Environment.<br>
        * @return None.
        */
        virtual void SetContextTypeL( const TDesC& aContextType ) = 0;

        /**
        * Returns a reference to the context type descriptor.
        *
        * @since S60 5.0
        * @param None.
        * @return A reference to the context type descriptor.
        */
        virtual const TDesC& ContextType() const = 0;

        /**
        * Sets desired context source.
        *  Copies the entire argument to the context source member variable.
        *
        * @since S60 5.0
        * @param aSource Desired context source or empty
        *  string if source doesn't matter.
		* @return None.
        */
        virtual void SetContextSourceL( const TDesC& aSource ) = 0;

        /**
        * Returns a reference to the context source descriptor.
        *
        * @since S60 5.0
        * @param None.
        * @return A reference to the context source descriptor.
        */
        virtual const TDesC& ContextSource() const = 0;

		/**
		* Returns whether subtypes of the context type are indicated.
		* 
		* @since S60 5.0
		* @param None.
		* @return ETrue if subtypes are indicated by this subscription,
		*/ 
		virtual TBool SubTypeMatch() const = 0;
		
		/**
		* Set whether subtypes of the context type are indicated.
		* The default value for a new CCFContextSubscription is EFalse.
		* 
		* @since S60 5.0
		* @param aMatchSubTypes ETrue = subtypes are indicated by this
		*  subscription, EFalse = only type defined in this subscription
		*  is indicated.
		* @return None.
		*/ 
		virtual void SetSubTypeMatch( TBool aMatchSubTypes ) = 0;

		/**
		* Returns whether only context value changes are indicated.
		* 
		* @since S60 5.0
		* @param None.
		* @return EFalse = all context publications are
		*  indicated, ETrue = only publications where context value changes
		*  form the previous value are indicated.
		*/ 
		virtual TBool OnlyChangesIndicated() const = 0;

		/**
		* Set indication to happen only when context value changes.
		* The default value for a new CCFContextSubscription is EFalse.
		* 
		* @since S60 5.0
		* @param aIndicateChangesOnly EFalse = all context publications are
		*  indicated, ETrue = only publications where context value changes
		*  form the previous value are indicated.
		* @return None.
		*/
		virtual void SetOnlyChangesIndicated( TBool aIndicateChangesOnly ) = 0;
		
		/**
		* Set indication to contain also possible data object if any.
		* By default data objects are not subscribed.
		*
		* @since S60 5.0
		* @param aEnabled Enable data objects.
		* @retrun None.
		*/
		virtual void SetDataObjectEnabled( TBool aEnabled ) = 0;
		
		/**
		* Returns if this subscription also subscribes possible
		* data object associated with the context.
		*
		* @since S60 5.0
		* @param None.
		* @retrun ETrue if data object enabled.
		*/
		virtual TBool DataObjectEnabled() const = 0;
    };

#endif
