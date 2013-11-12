/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccBaseView class
*
*/


#ifndef CGSACCBASEVIEW_H
#define CGSACCBASEVIEW_H

#include <AccessorySettings.h>
#include <aknview.h>
#include <eikclb.h>

class CGSAccBaseContainer;
class CGSAccessoryPluginModel;

/**
*  Base class for GS Accessory settings views.
*
*  @since S60 3.1
*/
class CGSAccBaseView : public CAknView, public MEikListBoxObserver
    {

public:

    /**
    * Destructor.
    */
    virtual ~CGSAccBaseView();

protected: // From CAknView

    /**
    * @see class CAknView.
    */
    void HandleCommandL( TInt aCommand );

    /**
    * @see class CAknView.
    */
    void DoActivateL(
        const TVwsViewId& aPrevViewId,
        TUid aCustomMessageId,
        const TDesC8& aCustomMessage );

    /**
    * @see class CAknView.
    */
    void DoDeactivate();

protected: // From MEikMenuObserver

    /**
    * @see class MEikMenuObserver.
    */
    void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

protected: // From MEikListBoxObserver

    /**
    * @see class MEikListBoxObserver.
    */
    void HandleListBoxEventL(
        CEikListBox* aListBox,
        TListBoxEvent aEventType );

protected:

    /**
    * First phase constructor.
    *
    * @param aModel Data model for reading and writing settings.
    */
    CGSAccBaseView( CGSAccessoryPluginModel& aModel );

    /**
    * Updates listbox items value.
    *
    * @param aItemId Item which is updated.
    */
    void UpdateListBoxL( TInt aItemId );

    /**
    * Show a selection dialog with content specified in resources.
    *
    * @param aDlgResourceID Resource id for the dialog resource.
    * @param aItemsResourceID Resource id for the list of available items.
    * @param aCurrentSelectionIndex Index of the item which should be selected
    * initially. On return, contains the selection made by the user.
    * @return ETrue if the new value should be saved.
    */
    TBool ShowRadioButtonSettingsPageL(
        TInt aDlgResourceID,
        TInt aItemsResourceID,
		TInt& aCurrentSelectionIndex ) const;

    /**
    * Show a selection dialog with content given as parameter.
    *
    * @param aDlgResourceID Resource id for the dialog resource.
    * @param aItemArray Array of available items.
    * @param aCurrentSelectionIndex Index of the item which should be selected
    * initially. On return, contains the selection made by the user.
    * @return ETrue if the new value should be saved.
    */
    TBool ShowRadioButtonSettingsPageL(
        TInt aDlgResourceID,
		const MDesCArray& aItemArray,
		TInt& aCurrentSelectionIndex ) const;

    /**
    * Show the default profile selection dialog and store user selection.
    *
    * @param aProfileIdKey Cenrep key where the default profile is stored.
    * @param aListBoxId Identifies list box to update if profile is changed.
    */
    void ChangeDefaultProfileL(
        TInt aProfileKey,
        TInt aListBoxId );

    /**
    * Change automatic answer mode setting.
    *
    * @param aAutoAnswerKey Cenrep key where the auto answer mode is stored.
    * @param aListBoxId Identifies list box to update if mode is changed.
    * @param aSettingPage Show the setting page if ETrue, otherwise just flip
    * the value.
    */
    void ChangeAutoAnswerModeSettingL(
        TInt aAutoAnswerKey,
        TInt aListBoxId,
        TBool aUseSettingPage );

    /**
    * Change lights mode setting.
    *
    * @param aAccMode Identifies the accessory mode to change the light
    * setting for.
    * @param aAccSetting Identifies the light setting to change.
    * @param aListBoxId Identifies list box to update if mode is changed.
    * @param aSettingPage Show the setting page if ETrue, otherwise just flip
    * the value.
    */
    void ChangeLightModeSettingL(
        TAccMode aAccMode,
        TInt aAccSetting,
        TInt aListBoxId,
        TBool aUseSettingPage );

protected:

    /**
    * Functionality for creating a container. Called by DoActivateL().
    * Implemented by sub-class.
    */
    virtual void NewContainerL() = 0;

    /**
    * Handle selection of list box.
    * Called by HandleListBoxEventL
    *
    * @param aSelectedItem Currently selected item.
    */
    virtual void HandleListBoxSelectionL( TInt aSelectedItem ) = 0;

private:

    /**
    * Basic idea behind General Settings plugin framework is that view objects
    * stay alive even when they are not active, but container objects do not.
    * Create new container.
    */
    void CreateContainerL();

    /**
    * Delele the container.
    */
    void DestroyContainer();

protected: // data

    /** Data model. */
    CGSAccessoryPluginModel& iModel;

    /**
    * Container. Own.
    * Sub-classes are responsible for creating object of correct type.
    */
    CGSAccBaseContainer* iContainer;
    };

#endif // CGSACCBASEVIEW_H
