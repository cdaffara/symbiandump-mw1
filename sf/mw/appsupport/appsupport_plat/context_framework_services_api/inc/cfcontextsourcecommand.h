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
* Description:  CCFContextSourceCommand class declaration.
 *
*/


#ifndef C_CFCONTEXTSOURCECOMMAND_H
#define C_CFCONTEXTSOURCECOMMAND_H


// SYSTEM INCLUDE FILES
#include <e32base.h>
#include <cfkeyvaluepair.h>
#include <cfcontextsourcecommandparameter.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS

// DATA TYPES

// CLASS DECLARATION

/**
 * Source command sender identity.
 * Identifies the sender of a context source command via sender's secure id and
 * script id. Script id is valid when secure id is CFServer's id, otherwise
 * script id should be KErrNotFound.
 *
 * @lib CFServices
 * @since S60 5.0
 */
NONSHARABLE_CLASS( TCFSourceCommandSenderId )
{
public: // Data

    /**
    * Secure id of the sender thread.
    * The value is CFServer's secure id when the command is coming from a script
    * action, otherwise it is the client's secure id.
    */
    TSecureId iSender;

    /**
    * Id of the script firing the command as an action.
    * The value is KErrNotFound (-1) if the command is not from a script.
    */
    TInt iScriptId;
};


/**
 * Context source command interface that can be used, for example, to activate
 * or deactivate context sources.
 *
 * @lib CFServices
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFContextSourceCommand ) : public CBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param None.
     * @return CCFContextSourceCommand
     */
    IMPORT_C static CCFContextSourceCommand* NewL();
    IMPORT_C static CCFContextSourceCommand* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~CCFContextSourceCommand();

public: // New methods

    /**
    * Returns context source command sender identity.
    * 
    * @since S60 5.0
    * @param None
    * @return const TCFSourceCommandSenderId&
    */
    virtual const TCFSourceCommandSenderId& Sender() const = 0;

    /**
    * Sets context source command sender identity.
    * 
    * @since S60 5.0
    * @param aSender is the new sender identity for the command.
    * @return None
    */
    virtual void SetSender( const TCFSourceCommandSenderId& aSender ) = 0;

    /**
    * Returns context source command name.
    * 
    * @since S60 5.0
    * @param None
    * @return const TDesC&
    */
    virtual const TDesC& Name() const = 0;

    /**
    * Sets command name.
    * 
    * @since S60 5.0
    * @param aName New command name.
    * @return None
    */
    virtual void SetNameL( const TDesC& aName ) = 0;

    /**
    * Returns uid of the context source who is the command target.
    * Uid is taken from the first key value pair attribute whose key is uid.
    * 
    * @since S60 5.0
    * @param None
    * @return TUid converted from hexadecimal string, Null-valued uid if
    *   conversion fails or the attribute does not exist.
    */
    virtual TUid SourceUid() const = 0;

    /**
    * Returns context source command attributes.
    * 
    * @since S60 5.0
    * @param None
    * @return const RKeyValueArray&
    */
    virtual const RKeyValueArray& Attributes() const = 0;

    /**
    * Adds new key value pair attribute for the command.
    * Attribute's ownership is transferred.
    * 
    * @since S60 5.0
    * @param aAttribute Key value pair attribute. Ownership is transferred.
    * @return None
    */
    virtual void AddAttributeL( CCFKeyValuePair* aAttribute ) = 0;

    /**
    * Adds new key value pair attributes for the command.
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
    * Returns parameters related to the context source command.
    * 
    * @since S60 5.0
    * @param None
    * @return const RSourceCommandParameterArray&
    */
    virtual const RSourceCommandParameterArray& Parameters() const = 0;

    /**
    * Adds new parameter for the command.
    * Parameter's ownership is transferred.
    * 
    * @since S60 5.0
    * @param aParameter Parameter for this command. Ownership is transferred.
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
    * Returns a clone of this context source command.
    * 
    * @since S60 5.0
    * @param None
    * @return CCFContextSourceCommand
    */
    virtual CCFContextSourceCommand* CloneL() const = 0;

    };

#endif // C_CFCONTEXTSOURCECOMMAND_H
