/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextSourceCommandParameter class declaration.
 *
*/


#ifndef C_CFCONTEXTSOURCECOMMANDPARAMETER_H
#define C_CFCONTEXTSOURCECOMMANDPARAMETER_H


// SYSTEM INCLUDE FILES
#include <e32base.h>
#include <cfkeyvaluepair.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class CCFContextSourceCommandParameter;
class RWriteStream;
class RReadStream;

// DATA TYPES
typedef RPointerArray< CCFContextSourceCommandParameter > RSourceCommandParameterArray;

// CLASS DECLARATION

/**
 * Context source command parameter interface.
 *
 * @lib CFServices
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextSourceCommandParameter ) : public CBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFContextSourceCommandParameter
     */
    IMPORT_C static CCFContextSourceCommandParameter* NewL();
    IMPORT_C static CCFContextSourceCommandParameter* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~CCFContextSourceCommandParameter();

public: // New methods

    /**
    * Returns parameter name.
    * 
    * @since S60 5.0
    * @param None
    * @return const TDesC&
    */
    virtual const TDesC& Name() const = 0;

    /**
    * Sets parameter name.
    * 
    * @since S60 5.0
    * @param aName New parameter name.
    * @return None
    */
    virtual void SetNameL( const TDesC& aName ) = 0;

    /**
    * Returns parameter attributes.
    * 
    * @since S60 5.0
    * @param None.
    * @return const RKeyValueArray&
    */
    virtual const RKeyValueArray& Attributes() const = 0;

    /**
    * Adds new key value pair attribute.
    * Attribute's ownership is transferred.
    * 
    * @since S60 5.0
    * @param aAttribute Key value pair attribute. Ownership is transferred.
    * @return None
    */
    virtual void AddAttributeL( CCFKeyValuePair* aAttribute ) = 0;

    /**
    * Adds new key value pair attributes.
    * Ownership of the attributes conteined in the array is transferred.
    * 
    * @since S60 5.0
    * @param aAttributes Array of key value pair attributes. Ownership of the
    *   key value pairs (array content) is transferred. If a leave occurs the
    *   caller is responsible for the key value pairs left on the array.
    * @return None
    */
    virtual void AddAttributesL( RKeyValueArray& aAttributes ) = 0;

    /**
    * Returns nested parameters related to this parameter.
    * 
    * @since S60 5.0
    * @param None
    * @return const RSourceCommandParameterArray&
    */
    virtual const RSourceCommandParameterArray& Parameters() const = 0;

    /**
    * Adds new nested parameter.
    * Parameter's ownership is transferred.
    * 
    * @since S60 5.0
    * @param aParameter Nested parameter for this parameter. Ownership is
    *   transferred.
    * @return None
    */
    virtual void AddParameterL(
            CCFContextSourceCommandParameter* aParameter ) = 0;

    /**
    * Returns instance size in bytes.
    * 
    * @since S60 5.0
    * @param None
    * @return TInt
    */
    virtual TInt Size() const = 0;

    /**
    * Externalizes instance.
    * 
    * @since S60 5.0
    * @param aStream Write stream.
    * @return None
    */
    virtual void ExternalizeL( RWriteStream& aStream ) const = 0;

    /**
    * Internalizes instance.
    * 
    * @since S60 5.0
    * @param aStream Read stream.
    * @return None
    */
    virtual void InternalizeL( RReadStream& aStream ) = 0;

    /**
    * Returns a clone of this context source command parameter.
    * 
    * @since S60 5.0
    * @param None
    * @return CCFContextSourceCommandParameter that is left on the cleanup
    *   stack.
    */
    virtual CCFContextSourceCommandParameter* CloneLC() const = 0;

    };

#endif // C_CFCONTEXTSOURCECOMMANDPARAMETER_H
