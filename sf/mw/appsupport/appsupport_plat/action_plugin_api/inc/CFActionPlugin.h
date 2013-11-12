/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFActionPlugIn class declaration.
*
*/



#ifndef C_CFACTIONPLUGIN_H
#define C_CFACTIONPLUGIN_H

#include <e32base.h>
#include <badesca.h>

#include <cfactionpluginconst.hrh>

class CCFActionPlugInImpl;
class CCFActionIndication;

/**
* Action plug in Ecom interface Uid
*/
const TUid KActionPluginUid = {KActionPluginInterfaceUid};

/**
* Base class for all Action plugins.
*
* For every action plug-in implementation there will be also an action
* execution thread.
* Action execution thread uses shared heap with Context Framework
* Server main thread. Cleanup stack and active scheduler will be created
* for Action execution thread.
*
* Actions are exeucted in three phase:
* - PrepareExecutionL
* - ExecuteL (Mandatory)
* - FinishedExecution
* Implementation needs to return correct TExecutionTime from ExecuteL if
* the action being performed is asynchronous an will not be completed when
* ExecuteL returns.
* 
* NOTE:
* It is crucial to only return ENone from ExecuteL if
* the action is really completed. Any active objects, timers or asynchronous
* requests won't be run since the action execution thread starts to wait
* in semaphore for a new action signalation when ExecuteL returns with ENone.
* In other cases action execution thread starts to wait for action to be completed.
* When the action is completed call AsyncExecutionCompleted to stop the wait.
*
* @lib CFActivatorEngine.lib
* @since S60 5.0
*/
class CCFActionPlugIn : public CBase
    {
    public:
    
        // Execution time.
        // Execution time defines aprroximately how long
        // it takes to complete async request. Active scheduler
        // will wait the specified amount of time before it will be
        // stopped automatically.
        enum TExecutionTime
            {
            // ENone = Not async, will return right away
            ENone,
            
            // ESmall = 5 seconds reserved
            ESmall,
            
            // EMedium = 15 seconds reserved
            EMedium,
            
            // ELong = 30 seconds reserved
            ELong
            };
    
    public:
    
        /**
        * Symbian two phased contrstuctors.
        *
        * @since S60 5.0
        * @param aImplementationUid Implementation to create.
        * @return CCFActionPlugIn*
        */
        IMPORT_C static CCFActionPlugIn* NewL( const TUid& aImplementationUid );
        IMPORT_C static CCFActionPlugIn* NewLC( const TUid& aImplementationUid );
            
        /**
        * Destructor.
        */
        IMPORT_C ~CCFActionPlugIn();
        
    public: // New methods
    
        /**
        * Notifies the base implementation that async request has been
        * completed. All errors will be ignored.
        * If the operation executed is not async then this method
        * is not needed to be called.
        * Note:
        * - If this method is not called scheduler is blocked maximum of
        *   30 seconds before it will be automatically stopped.
        * 
        * @since S60 5.0
        * @param None
        * @return None
        */
        IMPORT_C void AsyncExecutionCompleted();

        /**
        * Prepares execution.
        * If PrepareExecutionL leaves, ExecuteL and FinishedExecution won't
        * be called.
        * Default implementation is empty.
        *
        * @since S60 5.0
        * @param None
        * @return None
        */
        IMPORT_C virtual void PrepareExecutionL();

        /**
        * Finishes execution.
        * Default implementation is empty.
        *
        * @since S60 5.0
        * @param None
        * @return None
        */
        IMPORT_C virtual void FinishedExecution();
        
        /**
        * Returns an extension interface.
        * The extension interface is mapped with the extension UID.
        *
        * The default implemementation returns NULL.
        *
        * @since S60 5.0
        * @param aExtensionUid: The identifier of the extension.
        * @return Pointer to the extension.
        */
        IMPORT_C virtual TAny* Extension( const TUid& aExtensionUid ) const;

    public: // Implementation specific
    
        /**
        * Plug-in is allowed to reserve memory.
        * It is nessecary that no memory is allocated before
        * InitializeL is called.
        * 
        * @since S60 5.0
        * @param None
        * @return None
        */
        virtual void InitializeL() = 0;
    
        /**
        * Executes action.
        * If ExecuteL leaves, FinishedExecution won't be called.
        * Note:
        * - Do not delete aActionIndication. aActionIndication will be deleted
        *   automatically after the execution is ended.
        *
        * @since S60 5.0
        * @param aActionIndication Action indication.
        * @return None
        */
        virtual TExecutionTime ExecuteL(
            CCFActionIndication* aActionIndication ) = 0;
			
        /**
        * Get actions the plugin can perform.
        * Activator Engine calls this when it wants to know which
        * actions the module supports.
        *
        * @since S60 5.0
        * @param aActionList Supported actions by the plug-in.
        * @return None
        */
        virtual void GetActionsL( CDesCArray& aActionList ) const = 0;

        /**
        * Returns the security policy assigned with the plugin.
        *
        * @since S60 5.0
        * @param None
        * @return const TSecurityPolicy&
        */
        virtual const TSecurityPolicy& SecurityPolicy() const = 0;
        
    protected:

        // C++ constrcutor
        IMPORT_C CCFActionPlugIn();
        
    private: // New methods

        // Friend classes
        friend class CCFActivatorEngineActionPluginManager;

        // Appends new action in the queue
        // Returns ETrue if action thread is elligble to run
        TBool AppendQueue( CCFActionIndication* aAction );
        
        // Executes action
        void ExecuteAction();
    
    private:
    
        // Second phase constructor
        void ConstructL();

    private:
    
        /** Destructor Id */
        TUid iDtorKey;

        /** Plug-in logic */
        CCFActionPlugInImpl* iImpl;
        
        /** Reserved */
        TAny* iReserved1;
        TAny* iReserved2;
        TAny* iReserved3;
    };
    
#endif
