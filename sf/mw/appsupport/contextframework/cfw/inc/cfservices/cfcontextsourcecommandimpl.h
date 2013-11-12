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
* Description:  CCFContextSourceCommandImpl class declaration.
*
*/


#ifndef C_CFCONTEXTSOURCECOMMANDIMPL_H
#define C_CFCONTEXTSOURCECOMMANDIMPL_H


// SYSTEM INCLUDE FILES
#include <cfcontextsourcecommand.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Context source command interface implementation header.
 *
 * @lib CFServices
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextSourceCommandImpl ) :
    public CCFContextSourceCommand
{
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFContextSourceCommandImpl
     */
    static CCFContextSourceCommandImpl* NewL();
    static CCFContextSourceCommandImpl* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~CCFContextSourceCommandImpl();

public: // From CCFContextSourceCommand

    /** @see CCFContextSourceCommand */
    virtual const TCFSourceCommandSenderId& Sender() const;

    /** @see CCFContextSourceCommand */
    virtual void SetSender( const TCFSourceCommandSenderId& aSender );

    /** @see CCFContextSourceCommand */
    virtual const TDesC& Name() const;

    /** @see CCFContextSourceCommand */
    virtual void SetNameL( const TDesC& aName );

    /** @see CCFContextSourceCommand */
    virtual TUid SourceUid() const;

    /** @see CCFContextSourceCommand */
    virtual const RKeyValueArray& Attributes() const;

    /** @see CCFContextSourceCommand */
    virtual void AddAttributeL( CCFKeyValuePair* aAttribute );

    /** @see CCFContextSourceCommand */
    virtual void AddAttributesL( RKeyValueArray& aAttributes );

    /** @see CCFContextSourceCommand */
    virtual const RSourceCommandParameterArray& Parameters() const;

    /** @see CCFContextSourceCommand */
    virtual void AddParameterL(
            CCFContextSourceCommandParameter* aParameter );

    /** @see CCFContextSourceCommand */
    virtual TInt Size() const;

    /** @see CCFContextSourceCommand */
    virtual void ExternalizeL( RWriteStream& aStream ) const;

    /** @see CCFContextSourceCommand */
    virtual void InternalizeL( RReadStream& aStream );

    /** @see CCFContextSourceCommand */
    virtual CCFContextSourceCommand* CloneL() const;

private:

    CCFContextSourceCommandImpl();
    void ConstructL();

private: // Data

    // Sender of the command.
    TCFSourceCommandSenderId iSender;

    // Command name; owned.
    HBufC* iName;

    // Command attributes; owned.
    RKeyValueArray iAttributes;

    // Command parameters; owned.
    RSourceCommandParameterArray iParameters;
};

#endif // C_CFCONTEXTSOURCECOMMANDIMPL_H
