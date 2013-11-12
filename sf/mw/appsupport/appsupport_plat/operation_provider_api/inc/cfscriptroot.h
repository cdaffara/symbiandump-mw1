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
* Description:  Abstract CCFScriptRoot class declaration.
*
*/



#ifndef C_CFSCRIPTROOT_H
#define C_CFSCRIPTROOT_H

// INCLUDES
#include <e32base.h>
#include <cfoperationnode.h>

// FORWARD DECLARATIONS
class TSecurityPolicy;

// CLASS DECLARATION

/**
*   This class implements the interface for script XML node of CFW script in
*   Script Engine scripts.
*
*  @lib CFScriptEngine
*  @since Series 60 5.0
*/
class CCFScriptRoot : public CCFOperationNode
    {
public: // Constructors and destructor

    /**
    * Destructor.
    */
    IMPORT_C virtual ~CCFScriptRoot();

public: // New functions

    /**
    * Signals the root that evaluation was made with a context.
    * @return None.
    */
    virtual void ContextEvaluatedL( TInt aContextLevelDelay ) = 0;

    /**
    * Signals the root that evaluation was made without a context, i.e.
    * originating from a timer event.
    * @return None.
    */
    virtual void EvaluatedL() = 0;
    
    /**
     * Determines whether the client has the needed security level
     * to upgrade script located in rom.
     * @return const TCapabilitySet&.
     */
    virtual const TCapabilitySet& UpgradeSecurity() const = 0;

public: // Functions from CCFOperationNode

    /**
    * Activates this node by passing the activate call to its' children.
    * @return None.
    */
    virtual void ActivateL() = 0;

    /**
    * Deactivates this node by passing the deactivate call to its' children.
    * @return None.
    */
    virtual void Deactivate() = 0;

    /**
    * Asks this node to check its security (via services interface). For
    * example, the security of contexts to be subscribed or actions to be
    * performed.
    * @return KErrNone if security check passed, otherwise any of the system
    *   wide error codes.
    */
    virtual TInt CheckSecurity() = 0;

    /**
    * Called by child node, to ask the parent to re-evaluate its value,
    * since the child's value has changed.
    * @return None.
    */
    virtual void Evaluate() = 0;

protected:

    /**
    * C++ default constructor.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is parent node for this node. The ownership of the parent
    *   is not transffered.
    */
    IMPORT_C CCFScriptRoot( MCFOperationServices& aServices,
            CCFOperationNode* aParent );
    };

#endif // C_CFSCRIPTROOT_H
