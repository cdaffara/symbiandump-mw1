/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Header file for the UpnpAiwMoveToExternalService.
*
*/


#ifndef _UPNP_AIW_MOVE_TO_EXTERNAL_SERVICE_H
#define _UPNP_AIW_MOVE_TO_EXTERNAL_SERVICE_H

//  INCLUDES
#include <AiwServiceIfMenu.h>
#include "upnpaiwengineobserver.h"

// FUNCTION PROTOTYPES
class CUPnPAiwEngine;
class CUpnpDrmFilter;

// CLASS DECLARATION

/**
*  CUPnPAiwMoveToExternalService
*  The class implements the "Move to external" service of the
*  UPnP AIW Media Provider.
*  @since Series S60 3.0
*/
class CUPnPAiwMoveToExternalService : public CAiwServiceIfMenu,
                                      public MUPnPAiwEngineObserver
    {

    public: // Constructors and destructor

        // Two-phased constructor.
        static CUPnPAiwMoveToExternalService* NewL();

        // Destructor.
        virtual ~CUPnPAiwMoveToExternalService();

    public: // Methods from the CAiwServiceIfMenu

        // Unnecessary methods
        void InitialiseL(   MAiwNotifyCallback& /*aFrameworkCallback*/,
                            const RCriteriaArray& /*aInterest*/ ) {}
        void HandleServiceCmdL(
                            const TInt& /*aCmdId*/,
                            const CAiwGenericParamList& /*aInParamList*/,
                            CAiwGenericParamList& /*aOutParamList*/,
                            TUint /*aCmdOptions*/,
                            const MAiwNotifyCallback* /*aCallback*/) {}

        /**
        * Initialises menu pane by adding provider specific menu items.
        * The AIW Framework gives the parameters to be used in addition.
        * @param aMenuPane Menu pane handle
        * @param aIndex position of item where to add menu items.
        * @param aCascadeId ID of cascade menu item.
        * @param aInParamList input parameter list for provider's parameters 
        *        checking
        */
        void InitializeMenuPaneL( CAiwMenuPane& aMenuPane,
                                  TInt aIndex,
                                  TInt aCascadeId,
                                  const CAiwGenericParamList& aInParamList);

        /**
        * Handle a menu command invoked by the Handler.
        * @param aMenuCmdId Command ID for the menu command,
        *        defined by the provider when adding the menu commands.
        * @param aInParamList Input parameters, could be empty list
        * @param aOutParamList Output parameters, could be empty list
        * @param aCmdOptions Options for the command, see KAiwCmdOpt* 
        *        constants.
        * @param aCallback callback if asynchronous command handling is wanted
        *        by consumer. The provider may or may not support this, leaves
        *        with KErrNotSupported, it not.
        */
        void HandleMenuCmdL( TInt aMenuCmdId,
                             const CAiwGenericParamList& aInParamList,
                             CAiwGenericParamList& aOutParamList,
                             TUint aCmdOptions = 0,
                             const MAiwNotifyCallback* aCallback = NULL);

    public:  // MUPnPAiwEngineObserver

        // Unnecessary callback method
        void PlayCompleteL( TInt /*aStatus*/ ) {}

        /**
         * Indicates that the connection with the target UPnP
         * device has been lost.
         *
         * @since Series 60 3.0
         * @param None
         * @return None
         */
        void ConnectionLostL();

    private: // private methods

       /**
       * Deletes the contents of the iFileNames pointer array
       *
       * @since Series 60 3.0
       * @param None
       * @returns None
       */
       void EmptyFileNameArray();


    private: // construction

        // Constructor
        CUPnPAiwMoveToExternalService();

        // ConstructL
        void ConstructL();

    private: // Data members

        // reference to the engine component
        CUPnPAiwEngine* iEngine;            // not owned (singleton)
        
        // reference to the client (call back reference)
        MAiwNotifyCallback* iClient;        // not owned

        // The list of file names received as a parameter
        RPointerArray<TDesC16> iFileNames;  // owned
    };

#endif // _UPNP_AIW_MOVE_TO_EXTERNAL_SERVICE_H

// End of file
