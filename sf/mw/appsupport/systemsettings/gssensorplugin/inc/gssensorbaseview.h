/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Base view in GS Sensor Plugin.
*
*/


#ifndef GSSENSORBASEVIEW_H
#define GSSENSORBASEVIEW_H

// INCLUDES
#include <aknview.h>
#include <eikclb.h>
#include "gssensormskobserver.h"

// FORWARD DECLARATIONS
class CGSBaseContainer;
class CAknViewAppUi;

/**
 *  Base view class for GS Sensor plug-in views
 *
 *  @lib gssensorplugin.lib
 *  @since S60 5.0
 */
class CGSSensorBaseView : public CAknView,
                          public MEikListBoxObserver,
                          public MGSSensorMskObserver
    {
    public:

        // Panic code used in this class
        enum TGSViewPanicCodes
            {
            EGSViewPanicNullPtr = 1
            };

        /**
         * C++ Constructor 
         */
        CGSSensorBaseView();

        /**
         * Destructor.
         */
        virtual ~CGSSensorBaseView();

        /**
         * This function is used to set the current item in the listbox.
         * @param aIndex Current item. Default value is 0 i.e. the first item of the list.
         */
        virtual void SetCurrentItem( TInt aIndex = 0 );

    protected:

        /**
         * From CAknView. Handles client rect change.
         */
        void HandleClientRectChange();

        /**
         * From CAknView. Handles a view activation.
         *
         * @param aPrevViewId Specifies the view previously active.
         * @param aCustomMessageId Specifies the message type.
         * @param aCustomMessage The activation message.
         */
        void DoActivateL( const TVwsViewId& aPrevViewId,
                          TUid aCustomMessageId,
                          const TDesC8& aCustomMessage );

        /**
         * From CAknView. Deactivates current view.
         */
        void DoDeactivate();

        /**
         * From MEikListBoxObserver. Handles listbox events.
         *
         * @param aListBox Listbox where the event occurred.
         * @param aEventType Event type.
         */
        void HandleListBoxEventL( CEikListBox* aListBox,
                                  TListBoxEvent aEventType );
        /**
         * Creates new container. Subclasses must implement this function
         */
        virtual void NewContainerL() = 0;

        /**
         * Creates new container. This is called from DoActivateL and this one
         * calls NewContainerL from subclass.
         */
        virtual void CreateContainerL();

        /**
         * Removes current label from MSK
         */
        void RemoveCommandFromMSK();

        /**
         * Adds new label for MSK
         *
         * @param aResourceId Text of the new label
         * @param aCommandId Command id of the MSK
         */
        void SetMiddleSoftKeyLabelL( const TInt aResourceId, const TInt aCommandId );


    private:

        /**
         * Handles selection of list box. Called by HandleListBoxEventL.
         */
        virtual void HandleListBoxSelectionL() = 0;

    protected: // Member variables
        
        /**
         * Application UI
         */
        CAknViewAppUi* iAppUi;

        /**
         * Currently selected listbox item
         */
        TInt iCurrentItem;

        /**
         * Pointer to the container
         */
        CGSBaseContainer* iContainer;
    };

#endif //GSSENSORBASEVIEW_H
