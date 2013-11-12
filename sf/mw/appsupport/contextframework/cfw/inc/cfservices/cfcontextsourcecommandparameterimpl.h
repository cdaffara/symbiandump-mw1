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
* Description:  CCFContextSourceCommandParameterImpl class declaration.
*
*/


#ifndef C_CFCONTEXTSOURCECOMMANDPARAMETERIMPL_H
#define C_CFCONTEXTSOURCECOMMANDPARAMETERIMPL_H


// SYSTEM INCLUDE FILES
#include <cfcontextsourcecommandparameter.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Context source command parameter interface implementation header.
 *
 * @lib CFServices
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextSourceCommandParameterImpl ) :
    public CCFContextSourceCommandParameter
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFContextSourceCommandParameterImpl
     */
    static CCFContextSourceCommandParameterImpl* NewL();
    static CCFContextSourceCommandParameterImpl* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~CCFContextSourceCommandParameterImpl();

public: // From CCFContextSourceCommandParameter

    /** @see CCFContextSourceCommandParameter */
    virtual const TDesC& Name() const;

    /** @see CCFContextSourceCommandParameter */
    virtual void SetNameL( const TDesC& aName );

    /** @see CCFContextSourceCommandParameter */
    virtual const RKeyValueArray& Attributes() const;

    /** @see CCFContextSourceCommandParameter */
    virtual void AddAttributeL( CCFKeyValuePair* aAttribute );

    /** @see CCFContextSourceCommandParameter */
    virtual void AddAttributesL( RKeyValueArray& aAttributes );

    /** @see CCFContextSourceCommandParameter */
    virtual const RSourceCommandParameterArray& Parameters() const;

    /** @see CCFContextSourceCommandParameter */
    virtual void AddParameterL(
            CCFContextSourceCommandParameter* aParameter );

    /** @see CCFContextSourceCommandParameter */
    virtual TInt Size() const;

    /** @see CCFContextSourceCommandParameter */
    virtual void ExternalizeL( RWriteStream& aStream ) const;

    /** @see CCFContextSourceCommandParameter */
    virtual void InternalizeL( RReadStream& aStream );

    /** @see CCFContextSourceCommandParameter */
    virtual CCFContextSourceCommandParameter* CloneLC() const;

private:

    CCFContextSourceCommandParameterImpl();
    void ConstructL();

private: // Data

    // Parameter name; owned.
    HBufC* iName;

    // Parameter attributes; owned.
    RKeyValueArray iAttributes;

    // Nested parameters; owned.
    RSourceCommandParameterArray iParameters;
    };

#endif // C_CFCONTEXTSOURCECOMMANDPARAMETERIMPL_H
