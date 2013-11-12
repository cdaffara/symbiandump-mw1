/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFListener interface declaration.
*
*/


#ifndef M_CFLISTENER_H
#define M_CFLISTENER_H

class CCFContextIndication;
class CCFActionIndication;

/**
* Interface for CF listener.
* MCFListener receives indications of context changes and action indications
* according to the subscriptions. Also possible error situations are notified
* through MCFListener interface.
*
* Context indication:
* To receive context indications client first needs to subscribe a context.
* See cfclient.h how to subscribe contexts.
* When a context which is affected by the context subscription made by the client
* is updated a context indication is informed to client. Context indication consists
* of the changed context:
* @code
*  void CClient::ContextIndicationL( const CCFContextIndication& aChangedContext )
*    {
*    _LIT( KSomeSource, "Source" );
*    _LIT( KSomeType, "Type" );
*    const CCFContextObject& context = aChangedContext.Context();
*    if( context.Source() == KSomeSource && context.Type() == KSomeType )
*      {
*      // Handle context indication...
*      }
*    }
* @endcode
*
* Action indication:
* To receive action indications client first needs to subscribe a certain action.
* See cfclient.h how to subscribe actions.
* When a rule which has an action, sbscribed by the client, associated to is
* evaluated to true client will receive an action indication.
* Action indication contains the action ID which is needed to be executed and
* all the parameters defined in the rule:
* @code
*  void CClient::ActionIndicationL( const CCFActionIndication& aActionToExecute )
*    {
*    _LIT( KSomeActionId, "ActionId" );
*    if( aActionToExecute.Identifier() == KSomeActionId )
*      {
*      const RKeyValueArray& params = aActionToExecute.Parameters();
*      TInt paramCount = params.Count();
*      for( TInt i = 0; i < paramCount; i++ )
*        {
*        // Process parameters
*        const CCFKeyValuePair& parameter = params[i];
*        ...
*        // Execute action
*        ...
*        }
*      }
*    }
* @endcode
*
* Subscription error:
* If client is subcribing a context which it does not have sufficient
* capabilities this will lead to a context subscription error.
* Since contexts can be subscribed before they have been defined
* context subscription errors will be asynchronously notified everytime:
* @code
*  void CClient::HandleContextFrameworkError( TCFError aError,
*    const TDesC& aSource,
*    const TDesC& aType )
*    {
*    _LIT( KSubscribedSource, "Source" );
*    _LIT( KSubscribedType, "Type" );
*    if( aError == MCFListener::ESecurityCheckErrorForContextSubscription )
*      {
*      if( aSource == KSubscribedSource && aType == KSubscribedType )
*        {
*        // Handle error...
*        }
*      }
*    }
* @endcode
*
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( MCFListener )
    {
    public:
    
        // Error codes from Context Framework
        enum TCFError
            {
            // System wide error code that cannot be interpreted to any
            // Context Framework specific error (e.g OOM)
            EGeneralError,
            
            // Indicates that context subscription has failed
            // due insufficient security level.
            // On other words client process does not have needed
            // capabilities for a context.
            // This error occurs when subscription fails due to
            // security error
            ESecurityCheckErrorForContextSubscription
            };
        
    public:
    
        /**
        * Indicates a change in context for subscribed clients.
        * If client has subscribed to partial context class path,
        * only the changed context will be indicated.
        * 
        * @since S60 5.0
        * @param aIndication: Context indication.
        * @return None
        */
        virtual void ContextIndicationL(
            const CCFContextIndication& aChangedContext ) = 0;
            
        /**
        * Indicates that specified action is needed to be executed.
        * When action indication is received certain rule has been triggered
        * and action is needed to be performed.
        *
        * @since S60 5.0
        * @param aActionToExecute Action indication.
        * @return None
        */
        virtual void ActionIndicationL(
            const CCFActionIndication& aActionToExecute ) = 0;
            
        /**
        * Indicates that error has occured.
        * 
        * @since S60 5.0
        * @param aError Error code.
        * @return None
        */
        virtual void HandleContextFrameworkError( TCFError aError,
            const TDesC& aSource,
            const TDesC& aType ) = 0;

        /**
        * Returns an extension interface.
        * The extension interface is mapped with the extension UID.
        *
        * If the client does not support the requested interface,
        * client must return NULL. Otherwise client needs to return
        * the correct interface combined with the UID.
        *
        * @since S60 5.0
        * @param aExtensionUid: The identifier of the extension.
        * @return Pointer to the extension.
        */
        virtual TAny* Extension( const TUid& aExtensionUid ) const = 0;
    };

#endif
