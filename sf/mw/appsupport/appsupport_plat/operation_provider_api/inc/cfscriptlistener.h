/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFScriptListener interface.
*
*/



#ifndef M_CFSCRIPTLISTENER_H
#define M_CFSCRIPTLISTENER_H

// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CCFContextObject;

// CLASS DECLARATION

/**
*  This is a listener interface for Script Engine operations. Context events
*  subscribed via MCFOperationServices interface will be notified via this
*  interface.
*
*  @lib CFScriptEngine
*  @since S60 v5.0
*/
class MCFScriptListener
    {
public: // New functions

    /**
    * Performs evaluation based on a context.
    * @param aContext is the context for evaluation.
    * @param aContextLevelDelay is storage for context level delay required by
    *   the evaluation.
    * @return ETrue if evaluation was performed and firing actions should be
    *   considered based on clause conditions, EFalse otherwise.
    */
    virtual TBool Evaluate( const CCFContextObject& aContext,
                            TInt& aContextLevelDelay ) = 0;

    /**
    * Returns a context source this listener is interested in.
    * @return the source of the context.
    */
    virtual TPtrC Source() const = 0;

    /**
    * Returns a context type this listener is interested in.
    * @return the type of the context.
    */
    virtual TPtrC Type() const = 0;

    /**
    * Tests if this listener requires notification of all contexts.
    * @return ETrue if this listener is interested in all contexts,
    *   EFalse otherwise.
    */
    virtual TBool IsAllRequired() const = 0;
    };


#endif // M_CFSCRIPTLISTENER_H
