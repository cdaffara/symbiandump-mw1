/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  View for Shortcuts sub-folder
*
*/


#include <bautils.h>
#include <eikmenup.h>
#include <featmgr.h>
#include <gsaccessoryplugin.rsg>
#include <gsaccplugin.mbg>
#include <gscommon.hrh>
#include <gsfwviewuids.h>
#include <gsprivatepluginproviderids.h>
#include <hlplch.h> // For HlpLauncher
#include <StringLoader.h>

#include "acclocalviewids.h"
#include "gsaccessoryplugin.h"
#include "gsaccessoryplugin.hrh"
#include "gsaccessorypluginconst.hrh"
#include "gsaccessoryplugincontainer.h"
#include "gsaccessorypluginmodel.h"
#include "gsacchacview.h"
#include "gsaccheadphonesview.h"
#include "gsaccheadsetview.h"
#include "gsaccloopsetview.h"
#include "gsaccmusicstandview.h"
#include "gsacctextphoneview.h"
#include "gsacctvoutview.h"
#include "gsaccwiredcarkitview.h"
#include "gsaccwirelesscarkitview.h"
#include "trace.h"

// LOCAL CONSTANTS AND MACROS
_LIT( KGSAccessoryPluginResourceFileName, "z:gsaccessoryplugin.rsc" );

#ifdef __SCALABLE_ICONS
_LIT( KGSAccPluginIconFileName, "\\resource\\apps\\gsaccplugin.mif" ); // svg file
#else //__SCALABLE_ICONS
_LIT( KGSAccPluginIconFileName, "\\resource\\apps\\gsaccplugin.mbm" ); // bitmap
#endif //__SCALABLE_ICONS

// ========================= LOCAL FUNCTIONS ================================

// ---------------------------------------------------------------------------
// ListIdToViewId
// ---------------------------------------------------------------------------
//
static TUid ListIdToViewId( TInt aListId )
    {
    switch ( aListId )
        {
        case EGSHACListItemId:            return KAccHACViewId;
        case EGSHeadphonesListItemId:     return KAccHeadphonesViewId;
        case EGSHeadsetListItemId:        return KAccHeadsetViewId;
        case EGSLoopsetListItemId:        return KAccLoopsetViewId;
        case EGSMusicStandListItemId:     return KAccMusicStandViewId;
        case EGSTextPhoneListItemId:      return KAccTextPhoneViewId;
        case EGSTvOutListItemId:          return KAccTvoutViewId;
        case EGSWiredCarKitListItemId:    return KAccWiredCarkitViewId;
        case EGSWirelessCarKitListItemId: return KAccWirelessCarkitViewId;
        default:
            break;
        }

    return TUid::Null();
    }


// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::NewL()
// ---------------------------------------------------------------------------
//
CGSAccessoryPlugin* CGSAccessoryPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSAccessoryPlugin* self = new( ELeave ) CGSAccessoryPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::~CGSAccessoryPlugin
// ---------------------------------------------------------------------------
//
CGSAccessoryPlugin::~CGSAccessoryPlugin()
    {
    FUNC_LOG;

    // Do not call RemoveLocalViews(), iAppUi has already been deleted.
    iSubViewIds.Close();
    delete iModel;
    iResourceLoader.Close();
    FeatureManager::UnInitializeLib();
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::NewContainerL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::NewContainerL()
    {
    FUNC_LOG;

    iContainer = new( ELeave ) CGSAccessoryPluginContainer( *iModel );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::HandleListBoxSelectionL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::HandleListBoxSelectionL()
    {
    FUNC_LOG;

    iRockerPress = ETrue;
    TRAPD( error, SelectLocalViewL() );
    ERROR( error, "Failed to select local view" );
    iRockerPress = EFalse;
    User::LeaveIfError( error );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::GetCaption
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::GetCaptionL( TDes& aCaption ) const
    {
    FUNC_LOG;

    // The resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_ACC_MAIN_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::CreateIconL
// ---------------------------------------------------------------------------
//
CGulIcon* CGSAccessoryPlugin::CreateIconL( const TUid aIconType )
    {
    FUNC_LOG;

    CGulIcon* icon( NULL );
    if ( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
            AknsUtils::SkinInstance(),
            KAknsIIDQgnPropSetAccessorySub,
            KGSAccPluginIconFileName,
            EMbmGsaccpluginQgn_prop_set_accessory_sub,
            EMbmGsaccpluginQgn_prop_set_accessory_sub_mask );
        }
     else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    return icon;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::PluginProviderCategory
// ---------------------------------------------------------------------------
//
TInt CGSAccessoryPlugin::PluginProviderCategory() const
    {
    // To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }


// ---------------------------------------------------------------------------
// TUid CGSAccessoryPlugin::Id()
// ---------------------------------------------------------------------------
//
TUid CGSAccessoryPlugin::Id() const
    {
    return TUid::Uid( KGSAccessoryPluginUid );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::ProcessCommandL
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::ProcessCommandL( TInt aCommand )
    {
    FUNC_LOG;

    if ( aCommand == EGSCmdAppOpen )
        {
        SelectLocalViewL();
        }
    else
        {
        CAknView::ProcessCommandL( aCommand );
        }
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::HandleCommandL
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::HandleCommandL( TInt aCommand )
    {
    FUNC_LOG;

    iRockerPress = EFalse;

    switch ( aCommand )
        {
        case EAknSoftkeyOpen:
            SelectLocalViewL();
            break;
        case EAknSoftkeyBack:
            RemoveLocalViews();
            iAppUi->ActivateLocalViewL( KGSGenPluginUid );
            break;
        case EAknCmdHelp:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), iAppUi->AppHelpContextL() );
                }
            break;
            }
        case EGSCmdAppSetasDefault:
            ChangeDefaultAccessoryL();
           	break;
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::DynInitMenuPaneL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::DynInitMenuPaneL(
    TInt aResourceId,
    CEikMenuPane* aMenuPane )
    {
    FUNC_LOG;

    if ( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );

        if ( iRockerPress || !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            // Remove help if rocker key is pressed or help is not supported
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        }
    else if ( aResourceId == R_GS_MENU_ITEM_EXIT )
        {
        User::LeaveIfNull( aMenuPane );

        if ( iRockerPress )
            {
            // Remove exit if rocker key is pressed
            aMenuPane->SetItemDimmed( EAknCmdExit, ETrue );
            }
        }
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::CGSAccessoryPlugin()
// ---------------------------------------------------------------------------
//
CGSAccessoryPlugin::CGSAccessoryPlugin()
  : iResourceLoader( *iCoeEnv )
    {
    FUNC_LOG;
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::ConstructL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::ConstructL()
    {
    FUNC_LOG;

    FeatureManager::InitializeLibL();

    // Initialize the Model
    iModel = CGSAccessoryPluginModel::NewL();

    // TParse uses a lot of stack space, so allocate it from heap.
    TParse* parse = new ( ELeave ) TParse;
    CleanupStack::PushL( parse );
    TInt errorCode = parse->Set(
        KGSAccessoryPluginResourceFileName,
        &KDC_RESOURCE_FILES_DIR,
        NULL );
    ERROR( errorCode, "parse::Set() failed" );
    User::LeaveIfError( errorCode );

    // TFileName uses a lot of stack space, so allocate it from heap.
    TFileName* fileName = new ( ELeave ) TFileName( parse->FullName() );
    CleanupStack::PushL( fileName );

    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), *fileName );

    // Open resource file
    iResourceLoader.OpenL( *fileName );

    CleanupStack::PopAndDestroy( fileName );
    CleanupStack::PopAndDestroy( parse );

    BaseConstructL( R_ACC_MAIN_VIEW );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::SelectLocalViewL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::SelectLocalViewL()
    {
    FUNC_LOG;

    CGSAccessoryPluginContainer* container =
        static_cast<CGSAccessoryPluginContainer*>( iContainer );
    TUid viewId = ListIdToViewId( container->CurrentFeatureId() );

    if ( !LocalViewExists( viewId ) )
        {
        CGSAccBaseView* view( NULL );
        // Unsupported items are already hidden - no need to check them here.
        switch ( container->CurrentFeatureId() )
            {
            case EGSHeadsetListItemId:
                view = CGSAccHeadsetView::NewLC( *iModel );
                break;
            case EGSHeadphonesListItemId:
                view = CGSAccHeadphonesView::NewLC( *iModel );
                break;
            case EGSLoopsetListItemId:
                view = CGSAccLoopsetView::NewLC( *iModel );
                break;
            case EGSTextPhoneListItemId:
                view = CGSAccTextPhoneView::NewLC( *iModel );
                break;
            case EGSMusicStandListItemId:
                view = CGSAccMusicStandView::NewLC( *iModel );
                break;
            case EGSWiredCarKitListItemId:
                view = CGSAccWiredCarkitView::NewLC( *iModel );
                break;
            case EGSWirelessCarKitListItemId:
                view = CGSAccWirelessCarkitView::NewLC( *iModel );
                break;
            case EGSTvOutListItemId:
                view = CGSAccTvoutView::NewLC( *iModel );
                break;
            case EGSHACListItemId:
                view = CGSAccHACView::NewLC( *iModel );
                break;
            default:
                break;
            }

        if ( view )
            {
            iSubViewIds.AppendL( view->Id() ); // Store id
            iAppUi->AddViewL( view );
            CleanupStack::Pop( view );
            }
        }

    iAppUi->ActivateLocalViewL( viewId ); // Activate the view
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::RemoveLocalViews()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::RemoveLocalViews()
    {
    FUNC_LOG;

    TInt count = iSubViewIds.Count();
    for ( TInt i = 0; i < count; i++ )
        {
        iAppUi->RemoveView( iSubViewIds[i] ); // Deletes the object also.
        }
    iSubViewIds.Reset(); // No need to close after this.
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::LocalViewExists()
// ---------------------------------------------------------------------------
//
TBool CGSAccessoryPlugin::LocalViewExists( const TUid& aViewId ) const
    {
    FUNC_LOG;

    return ( iSubViewIds.Find( aViewId ) != KErrNotFound );
    }


// ---------------------------------------------------------------------------
// CGSAccessoryPlugin::ChangeDefaultAccessoryL()
// ---------------------------------------------------------------------------
//
void CGSAccessoryPlugin::ChangeDefaultAccessoryL()
    {
    FUNC_LOG;

    CGSAccessoryPluginContainer* container =
        static_cast<CGSAccessoryPluginContainer*>( iContainer );

    TInt currentFeatureId = container->CurrentFeatureId();
    TInt oldDefaultItem = iModel->DefaultAccessory();
    if ( oldDefaultItem != currentFeatureId )
        {
        // Here we set the item selected as "Set as Default"
        container->ChangeDefaultItemL( currentFeatureId, oldDefaultItem );
        iModel->SetDefaultAccessoryL( currentFeatureId );
        container->DrawNow();
        }
    }
