/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFCount class declaration.
*
*/



#ifndef C_CFCOUNT_H
#define C_CFCOUNT_H

// INCLUDES
#include "cfcontextoperation.h"

// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*  This class implements count -operation on Context Framework Scripts.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
NONSHARABLE_CLASS( CCFCount ): public CCFContextOperation
    {
public: // Public types

    /** Count types. */
    enum TCountType {
        ECountTypeRemain, /** Count value remains true after operation is evaluated to true. */
        ECountTypeChange, /** Count value is changed back from true to false after receiving the next counted context. */
        ECountTypeTrigger /** Count triggers one evaluation when count's value is true, after which count changes it's value back to false. */
    };

public: // Constructors and destructor

    /**
    * Creates and parses a CCFCount instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFCount, or NULL if the node
    *   is not this count node. May leave with any system wide error code.
    */
    static CCFCount* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFCount();

public: // From CCFOperationNode

    /**
    * Internalizes operation.
    * @param aStream Read stream.
    * @return None.
    */
    void InternalizeL( RReadStream& aStream );

    /**
    * Externalizes operation.
    * @param aStream Write stream.
    * @return None.
    */
    void ExternalizeL( RWriteStream& aStream );

    /**
    * Cleanup deletes possible persistent data file.
    * This is called by the context framework when the script is uninstalled
    * from the system.
    * @return None.
    */ 
    void Cleanup();

protected: // From CCFContextOperation

    /**
    * Tests if this listener requires notification of all contexts.
    * Count type trigger requires all contexts, other types do not.
    * @returns ETrue if this listener is interested in all contexts,
    *   EFalse otherwise.
    */
    TBool IsAllRequired() const;

    /**
    * Evaluates a node with a context.
    * @param aContext is the context for evaluation.
    * @param aContextLevelDelay contains the context level delay defined by
    *  contextRef after the call.
    * @return ETrue if the operation was evaluated, EFalse otherwise.
    */
    TBool Evaluate( const CCFContextObject& aContext,
            TInt& aContextLevelDelay );

protected: // From CCFOperationNode

    /**
    * Activates this node by subscribing to a context.
    * Overrides base class implementation because count must discard the
    * possible cached context used to pre-evaluate operations during subscribe
    * context. Otherwise count might be increased based on the cached context.
    * @return None.
    */
    void ActivateL();

    /**
    * Deactivates this node by removing context subscription.
    * @return None.
    */
    void Deactivate();

protected:

    /**
    * C++ default constructor.
    */
    CCFCount( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            HBufC* aName,
            HBufC* aSource,
            const CCFContextOperation::TCmpType aType,
            const TUint aCount,
            const TCountType aCountType );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL( const TDesC& aCmpVal );

private: // New functions

    /**
    * Evaluates a node with a context.
    * @param aContext is the context for evaluation.
    * @return ETrue if the operation was evaluated, EFalse otherwise.
    */
    TBool DoEvaluateL( const CCFContextObject& aContext );

    /**
    * Counts a context if appropriate.
    * @param aContext is the context for counting.
    * @return ETrue if the context was counted, otherwise EFalse.
    */
    TBool CountContextL( const CCFContextObject& aContext );

protected: // Data

    CCFContextOperation::TCmpType iType;
    TUint                         iCurrentCount;
    TBool                         iActive;
    TBool                         iFinished;
    TUint                         iCmpCount;
    TCountType                    iCountType;
    HBufC*                        iCmpValue;
    HBufC*                        iPersistencyFile;
    TBool                         iReEvaluateFromNextContext;
    };


#endif // C_CFCOUNT_H
