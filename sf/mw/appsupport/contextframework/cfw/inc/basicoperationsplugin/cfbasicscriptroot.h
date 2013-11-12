/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFBasicScriptRoot class declaration.
*
*/



#ifndef C_CFBASICSCRIPTROOT_H
#define C_CFBASICSCRIPTROOT_H

// INCLUDES
#include <e32base.h>
#include <e32cmn.h>
#include <cfscriptroot.h>

#include "cfdelayexpired.h"

// FORWARD DECLARATION
class MCFOperationServices;
class CCFClauseNode;
class CCFDelay;

// CLASS DECLARATION

/**
*   This class implements script XML node of CFW script in Script Engine
*   scripts.
*
*  @lib CFScriptEngine
*  @since Series 60 2.6
*/
NONSHARABLE_CLASS( CCFBasicScriptRoot ): public CCFScriptRoot,
    public MCFDelayExpired
    {
public: // Constructors and destructor

    /**
    * Creates a CCFBasicScriptRoot object
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @return a pointer to the created instance of CCFBasicScriptRoot
    */
    static CCFBasicScriptRoot* NewL( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates a CCFBasicScriptRoot object
    * @param aServices is the operation services interface for nodes.
    * @param aParent parent of this node or NULL if this is a root. The
    *   ownership of the parent is not transferred.
    * @return a pointer to the created instance of CCFBasicScriptRoot
    */
    static CCFBasicScriptRoot* NewLC( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * Creates and parses a CCFBasicScriptRoot instance from an xml node.
    * @param aServices is the operation services interface for nodes.
    * @param aParent is the parent of this node or NULL if this is a root.
    *   The ownership of the parent is not transferred.
    * @param aNode is the node to be parsed.
    * @return a pointer to the created instance of CCFBasicScriptRoot, or NULL
    *   if the node is not basic script root node. May leave with any system
    *   wide error code.
    */
    static CCFBasicScriptRoot* ParseL( MCFOperationServices& aServices,
            CCFOperationNode* aParent,
            CMDXMLNode& aNode );

    /**
    * Destructor.
    */
    ~CCFBasicScriptRoot();

public: // Functions from CCFScriptRoot

    /**
    * Signals the root that evaluation was made with a context. Checks whether
    * delay should should be activated or actions fired immediately.
    * @param aContextLevelDelay is the context level delay requirement from the
    *   evaluation.
    * @return None.
    */
    void ContextEvaluatedL( TInt aContextLevelDelay );

    /**
    * Signals the root that evaluation was made without a context, i.e.
    * originating from a timer event.
    * @note If actions should be fired (any child is true), possible ongoing
    *   evaluation delay is cancelled before firing the actions.
    * @return None.
    */
    void EvaluatedL();

    /**
     * Determines whether the client has the needed security level
     * to upgrade script located in rom.
     * @param aSet Capability set needed for upgrade.
     * @return KErrNone if security check has passed.
     */
    const TCapabilitySet& UpgradeSecurity() const;

public: // Functions from CCFOperationNode

    /**
    * Activates this node by passing the activate call to its' children.
    * @return None.
    */
    void ActivateL();

    /**
    * Deactivates this node by passing the deactivate call to its' children.
    * @return None.
    */
    void Deactivate();

    /**
    * Asks this node to check its security (via services interface). For
    * example, the security of contexts to be subscribed or actions to be
    * performed.
    * @return KErrNone if security check passed, otherwise any of the system
    *   wide error codes.
    */
    TInt CheckSecurity();

    /**
    * Cleanup long term resources like persistent data files.
    * This is called by the context framework when the script is uninstalled
    * from the system.
    */ 
    void Cleanup();

public: // Functions from MCFDelayExpired

    /** @see MCFDelayExpired::ExpiredL. */
    void ExpiredL();

protected: // Functions from CCFOperationNode

    /**
    * Called by child node, to ask the parent to re-evaluate its value, since
    * the child's value has changed.
    * @return None.
    */
    void Evaluate();

private:

    /**
    * C++ default constructor.
    */
    CCFBasicScriptRoot( MCFOperationServices& aServices,
            CCFOperationNode* aParent );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // New functions

    /**
    * Fires actions of the first clause node having true value.
    * @return None.
    */
    void FireActionsL();
    
    /**
     * Add capability set for upgrading.
     * @param aCapabilitySet Set of capabilities.
     */
    void SetCapabilitites( TCapabilitySet& aSet );
    
    /**
     * Create a capability from descriptor.
     * @param aCapDesc Capability as descriptor.
     * @return TCapability Capability enum.
     */
    static TCapability CapabilityFromDesC( const TDesC8& aCapDesc );
    
    /**
     * Resolves upgrade security from the script XML.
     * @param aRoot Root where the security is associated.
     * @param aNode XML node where the security is parsed.
     */
    static void ResolveSecurity( CCFBasicScriptRoot& aRoot, CMDXMLNode& aNode );

private: // Data

    // Script level delay for firing actions after evaluation.
    TInt iScriptLevelDelay;

    // Delay action firing.
    CCFDelay* iDelayActionFiring;

    // Node children.
    RPointerArray<CCFClauseNode> iChildren;
    
    // Capability set
    TCapabilitySet iCapabilitySet;
    };

#endif // C_CFBASICSCRIPTROOT_H
