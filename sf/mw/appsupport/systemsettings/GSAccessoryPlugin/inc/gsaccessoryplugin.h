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
* Description:  Declaration of CGSAccessoryPlugin class
*
*/


#ifndef GSACCESSORYPLUGIN_H
#define GSACCESSORYPLUGIN_H

#include <gsbaseview.h>
#include <ConeResLoader.h>

class CGSAccessoryPluginModel;
class CGSAccBaseView;

/**
*  CGSAccessoryPlugin view class
*
*  @since S60 3.1
*/
class CGSAccessoryPlugin : public CGSBaseView
    {

public: // Constructors and destructor

    /**
    * Constructs a CGSAccessoryPlugin object.
    *
    * @return The new object
    */
    static CGSAccessoryPlugin* NewL( TAny* aInitParams );

    /**
    * Destructor.
    */
    virtual ~CGSAccessoryPlugin();

private: // from CGSBaseView

    /**
    * @see class CGSBaseView.
    */
    void NewContainerL();

    /**
    * @see class CGSBaseView.
    */
    void HandleListBoxSelectionL();

private: // From CGSPluginInterface

    /**
    * @see class CGSPluginInterface.
    */
    void GetCaptionL( TDes& aCaption ) const;

    /**
    * @see class CGSPluginInterface.
    */
    CGulIcon* CreateIconL( const TUid aIconType );

    /**
    * @see class CGSPluginInterface.
    */
    TInt PluginProviderCategory() const;

private: // from CAknView

    /**
    * @see class CAknView.
    */
    TUid Id() const;

    /**
    * @see class CAknView.
    */
    void ProcessCommandL( TInt aCommand );

    /**
    * @see class CAknView.
    */
    void HandleCommandL( TInt aCommand );

private: // From MEikMenuObserver

    /**
    * @see class MEikMenuObserver.
    */
    void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

private:

    /**
    * First phase constructor.
    */
    CGSAccessoryPlugin();

    /**
    * Second phase constructor.
    */
    void ConstructL();

    /**
    * Select and create local view based on current feature id.
    */
    void SelectLocalViewL();

    /**
    * Removes all local views that have been created.
    */
    void RemoveLocalViews();

    /**
    * Check if a local view already exists.
    *
    * @param aViewId Identifies the view.
    */
    TBool LocalViewExists( const TUid& aViewId ) const;

    /**
    * Change currently selected accessory to be the default accessory.
    */
    void ChangeDefaultAccessoryL();

private: // data

    /** Resource loader. */
    RConeResourceLoader iResourceLoader;

    /** Data model. Own. */
    CGSAccessoryPluginModel* iModel;

    /**
    * Identifiers of local view that have been created. These are stored to
    * be able to remove them later.
    */
    RArray<TUid> iSubViewIds;

    /** To check for the dynamic menu key event. */
    TBool iRockerPress;

    };

#endif // GSACCESSORYPLUGIN_H
