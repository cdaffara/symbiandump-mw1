/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Action indication class declaration.
*
*/


#ifndef C_CFACTIONINDICATION_H
#define C_CFACTIONINDICATION_H

#include <e32base.h>
#include <cfkeyvaluepair.h>

/**
* Encapsulates inside an action indication. Action indication consists of
* an action identifier and a set of paramters.
* When an action indication is received the rule inside a script has
* been evaluated to true. Action with identified by the action ID is
* needed to be executed with the parameters assocaited with the action
* indication.
* 
* @since S60 5.0
* @lib cfservices.lib
*/
NONSHARABLE_CLASS( CCFActionIndication ): public CBase
    {
    public:

        // Two phased constructors
        IMPORT_C static CCFActionIndication* NewL();
        IMPORT_C static CCFActionIndication* NewLC();

    public: // New methods
    
        /**
        * Sets identifier for the action.
        * 
        * @since S60 5.0
        * @param aIdentifier New identifier.
        * @return None
        */
        virtual void SetIdentifierL( const TDesC& aIdentifier ) = 0;
        
        /**
        * Returns action identifier associated with the indication.
        * 
        * @since S60 5.0
        * @param None
        * @return const TDesC&
        */
        virtual const TDesC& Identifier() const = 0;
        
        /**
        * Adds new key value pair parameter.
        * 
        * @since S60 5.0
        * @param aKey Name of the key.
        * @param aValue Value for the key.
        * @return None
        */
        virtual void AddParameterL( const TDesC& aKey, const TDesC& aValue ) = 0;

        /**
        * Returns parameters related to the action in a key
        * value pair array.
        * 
        * @since S60 5.0
        * @param None
        * @return const RKeyValueArray&
        */
        virtual const RKeyValueArray& Parameters() const = 0;
    };

#endif
