/*
* Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Main view for Sensor plug-in.
*
*/


// INCLUDE FILES
#include <aknsettingpage.h>
#include <hlplch.h>             // For HlpLauncher
#include <bautils.h>
#include <featmgr.h>
#include <StringLoader.h>
#include <AknQueryDialog.h>
#include <aknnotedialog.h>
#include <aknradiobuttonsettingpage.h>

#include <gssensorpluginrsc.rsg>
#include <gsprivatepluginproviderids.h>
#include <gsfwviewuids.h>
#include <gscommon.hrh>

#include "gssensorplugin.h"
#include "gssensorplugincontainer.h"
#include "gssensorplugin.hrh"
#include "gssensorpluginlocalviewids.h"
#include "gssensorpluginmodel.h"
#include "gssensorplugin_debug.h"

// Local views
#include "gssensensorsview.h"
#include "gssentappingctrlview.h"
#include "gssenturnctrlview.h"
#include "gssenorientationview.h"

// LOCAL CONSTANTS AND MACROS
_LIT( KGSSensorPluginResourceFileName, "z:gssensorpluginrsc.rsc" );

// Panic
_LIT( KGSDoActivateError, "DoActivateL" );

// Child plugin count
const TInt KGSSenPluginCount = 10;

#include <gssenplugin.mbg>

#ifdef __SCALABLE_ICONS
// bitmap
_LIT( KGSSenPluginIconFileName, "\\resource\\apps\\gssenplugin.mif");
#else //__SCALABLE_ICONS
// svg file
_LIT( KGSSenPluginIconFileName, "\\resource\\apps\\gssenplugin.mbm");
#endif //__SCALABLE_ICONS


// ----------------------------------------------------------------------------
// CGSSensorPlugin::CGSSensorPlugin
// Constructor
// ----------------------------------------------------------------------------
//
CGSSensorPlugin::CGSSensorPlugin()
  : iResourceLoader( *iCoeEnv )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CGSSensorPlugin()" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::NewL
// Symbian OS default constructor
// ----------------------------------------------------------------------------
CGSSensorPlugin* CGSSensorPlugin::NewL( TAny* /*aInitParams*/ )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::NewL()" );
    CGSSensorPlugin* self = new( ELeave ) CGSSensorPlugin();
    CleanupStack::PushL( self );
    self->ConstructL();        
    CleanupStack::Pop( self );
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::NewL() - return" );
    return self;
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::ConstructL
// Symbian OS two-phased constructor
// ----------------------------------------------------------------------------
void CGSSensorPlugin::ConstructL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::ConstructL()" );
    FeatureManager::InitializeLibL();

    // Initialize the Model
    iModel = CGSSensorPluginModel::NewL();

    // assign view pointers to point to NULL.
    iTappingCtrlView = NULL;
    iTurningCtrlView = NULL;

    // Find the resource file
    TParse parse;
    parse.Set( KGSSensorPluginResourceFileName, 
               &KDC_RESOURCE_FILES_DIR, NULL );
    TFileName fileName( parse.FullName() );
    
    // Get language of resource file
    BaflUtils::NearestLanguageFile( iCoeEnv->FsSession(), fileName );

    // Open resource file
    iResourceLoader.OpenL( fileName );

    // Load extended plugins if any
    iPluginArray = new  ( ELeave ) CArrayPtrFlat<CGSPluginInterface>( KGSSenPluginCount );
    iPluginLoader = CGSPluginLoader::NewL( iAppUi );
    iPluginLoader->SetObserver( this );
    iPluginLoader->LoadAsyncL( KGSPluginInterfaceUid, Id(), iPluginArray );

    BaseConstructL( R_GS_SEN_VIEW );
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::ConstructL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::~CGSSensorPlugin 
// Destructor
// ----------------------------------------------------------------------------
CGSSensorPlugin::~CGSSensorPlugin()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::~CGSSensorPlugin()" );
    FeatureManager::UnInitializeLib();

    // close resource loader
    iResourceLoader.Close();

    // delete Model
    if ( iModel )
        {
        delete iModel;
        }

    // Delete plugin loader
    if ( iPluginLoader )
        {
        delete iPluginLoader;
        }

    // Delete possible plugins
    if ( iPluginArray )
        {
        // Since the plugins are actually avkon views, avkon is responsible
        // for owning the plugins. This means we do not reset and destroy
        // the contents of the array in which the plugins reside. We have to
        // leave it up to avkon to tidy up.
        delete iPluginArray;
        }

    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::~CGSSensorPlugin() - return" );
    }


// ----------------------------------------------------------------------------
// TUid CGSSensorPlugin::Id
// Returns plug-in's ID.
// ----------------------------------------------------------------------------
TUid CGSSensorPlugin::Id() const
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::Id() - ID: %i", KGSSensorPluginUid );
    return KGSSensorPluginUid;
    }


// ----------------------------------------------------------------------------
// CGSSensorPlugin::HandleCommandL 
// Handles commands directed to this class.
// ----------------------------------------------------------------------------
void CGSSensorPlugin::HandleCommandL( TInt aCommand )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::HandleCommandL() - command: %i", aCommand );

    switch ( aCommand )
        {
        case EGSSenMskGeneral:  // MSK, this is either 'Change' or 'Open'
        case EGSSenMenuOpen:    // 'Open' from options menu
            {
            SelectLocalViewL();
            break;
            }
        case EGSSenMenuChange:  // 'Change' from options menu
            {
            TInt current = iModel->SensorActiveStatusL();

            CDesCArrayFlat* items =
				iCoeEnv->ReadDesC16ArrayResourceL( R_GS_SENSOR_ACTIVE_STATUS );
			CleanupStack::PushL( items );

			CAknRadioButtonSettingPage* dlg =
				new ( ELeave ) CAknRadioButtonSettingPage(
					R_GS_SENSOR_ACTIVE_STATUS_SETTING_PAGE,
					current,
					items );

			if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
				{
				iModel->SetSensorActiveStatusL( current );
				UpdateListBoxL( EGSSensorsListItemId );
				}
		    CleanupStack::PopAndDestroy( items );
            break;
            }
        case EAknSoftkeyBack:
            // remove views
            RemoveLocalViews();
            // check exit status
            CheckExitStatusL();
            // Activate parent plugin
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
        case EGSSenMenuExit:
            {
            CheckExitStatusL();
			aCommand = EAknCmdExit; //"break" is removed to continue the flow to default case with the exit command
			// coverity[MISSING_BREAK] 
            }
        default:
            iAppUi->HandleCommandL( aCommand );
            break;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::HandleCommandL() - return" );
    }

// -----------------------------------------------------------------------------
// CGSSensorPlugin::CheckExitStatusL
// -----------------------------------------------------------------------------
//
void CGSSensorPlugin::CheckExitStatusL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CheckExitStatusL()" );
    // Check were there changes
    if ( iModel->ChangesMade() )
        {
        const TBool sensorStatus( iModel->SensorActiveStatusL() );
        const TBool settingsExists( iModel->SettingsExistsL() );

        // Check should we show dialog, note or neither
        if ( !sensorStatus && settingsExists )
            {
            // Query to turn sensors on
            CAknQueryDialog* dialog = CAknQueryDialog::NewL();
            dialog->PrepareLC( R_GS_SEN_CONFIRM_YES_NO_QUERY );
            TInt ret = dialog->RunLD();

            if ( ret == EAknSoftkeyYes )
                {
                // Set sensor status to active
                iModel->SetSensorActiveStatusL( ETrue );
                }
            }
        else if ( sensorStatus && !settingsExists )
            {
            // Set sensors off, as there are no sensor settings...
            iModel->SetSensorActiveStatusL( EFalse );
            // ...and inform user
            CAknNoteDialog* note = new ( ELeave ) CAknNoteDialog(
                REINTERPRET_CAST( CEikDialog**, &note ),
                CAknNoteDialog::ENoTone,
                CAknNoteDialog::ELongTimeout );

            note->PrepareLC( R_GS_SEN_INFORMATION_NOTE );
            note->RunLD(); // note will delete itself
            }
        else
            {
            // Everyting is OK, no need for actions
            }
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CheckExitStatusL() - return" );
    }


// -----------------------------------------------------------------------------
// CGSSensorPlugin::DynInitMenuPaneL
// -----------------------------------------------------------------------------
//
void CGSSensorPlugin::DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::Container()" );
    // Check resource
    if ( R_GS_SEN_MENU_ITEM_OPEN == aResourceId )
        {
        // First, resolve current item in listbox
        const TInt currentItem = Container()->CurrentFeatureId();

        if ( EGSSensorsListItemId == currentItem )
            {
            // Hide menu item 'Open'
            aMenuPane->SetItemDimmed( EGSSenMenuOpen, ETrue );
            aMenuPane->SetItemDimmed( EGSSenMenuChange, EFalse );
            }
        else
            {
            // Hide menu item 'Change'
            aMenuPane->SetItemDimmed( EGSSenMenuChange, ETrue );
            aMenuPane->SetItemDimmed( EGSSenMenuOpen, EFalse );
            }
        }

    // Forward to base class
    CGSBaseView::DynInitMenuPaneL( aResourceId, aMenuPane );

    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::Container() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::UpdateListBoxL
// ----------------------------------------------------------------------------
//
void CGSSensorPlugin::UpdateListBoxL( TInt aItemId )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::UpdateListBoxL() - item: %i", aItemId );
    Container()->UpdateListBoxL( aItemId );
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::UpdateListBoxL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::Container
// Return handle to container class.
// ----------------------------------------------------------------------------
//
CGSSensorPluginContainer* CGSSensorPlugin::Container()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::Container()" );
    return static_cast<CGSSensorPluginContainer*>( iContainer );
    }
    
// ----------------------------------------------------------------------------
// CGSSensorPlugin::NewContainerL
// Creates new iContainer.
// ----------------------------------------------------------------------------
//
void CGSSensorPlugin::NewContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::NewContainerL()" );
    // container takes a CGSSensorPluginModel pointer
    iContainer = new( ELeave ) CGSSensorPluginContainer( iModel, *this );
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::NewContainerL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::DoActivateL
// First method called by the Avkon framwork to invoke a view.
// ----------------------------------------------------------------------------
//
void CGSSensorPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                    TUid aCustomMessageId,
                                    const TDesC8& aCustomMessage )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::DoActivateL()" );
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    // Update MSK label
    CheckMiddleSoftkeyLabelL();
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::DoActivateL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::DoDeactivate
// Called by the Avkon view framework when closing.
// ----------------------------------------------------------------------------
//
void CGSSensorPlugin::DoDeactivate()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::DoDeactivateL()" );
    // Deactivate view
    CGSBaseView::DoDeactivate();
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::DoDeactivateL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::HandleListBoxSelectionL
// Handles events raised through a rocker key
// ----------------------------------------------------------------------------
void CGSSensorPlugin::HandleListBoxSelectionL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::HandleListBoxSelectionL()" );
    SelectLocalViewL();
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::HandleListBoxSelectionL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::SelectLocalViewL
// Selects a particular local view
// ----------------------------------------------------------------------------
void CGSSensorPlugin:: SelectLocalViewL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::SelectLocalViewL()" );
    const TInt currentFeatureId = Container()->CurrentFeatureId();

    switch ( currentFeatureId )
        {
        case EGSSensorsListItemId:
            ChangeSensorActiveSettingL();
            break;
        case EGSTapCtrlListItemId:
            CreateLocalViewL( KSenTapCtrlViewId );
            break;
        case EGSTurnCtrlListItemId:
            CreateLocalViewL( KSenTurnCtrlViewId );
            break;
        default:
            {
            // Not local view, try plugins
            CGSPluginInterface* selectedPlugin( Container()->SelectedPluginL() );

            switch( selectedPlugin->ItemType() )
                {
                // In these cases the plugin is a view:
                case EGSItemTypeSingleLarge:
                case EGSItemTypeSetting:
                case EGSItemTypeSettingIcon:
                    {
                    CleanupStack::PushL( selectedPlugin );
                    iAppUi->ActivateLocalViewL( selectedPlugin->Id() );
                    CleanupStack::Pop( selectedPlugin );
                    break;
                    }
                    
                // In these cases the plugin is a dialog:
                case EGSItemTypeSettingDialog:
                case EGSItemTypeSingleLargeDialog:
                     selectedPlugin->HandleSelection( EGSSelectionByMenu );
                     break;
                default:
                    break;
                }
            break;
            }
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::SelectLocalViewL() - return" );
    }

// -----------------------------------------------------------------------------
// CGSSensorPlugin::ProcessCommandL
// -----------------------------------------------------------------------------
void CGSSensorPlugin::ProcessCommandL( TInt aCommand )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::ProcessCommandL() - command: %i", aCommand );
    // Call base class
    CAknView::ProcessCommandL( aCommand );
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::ProcessCommandL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::RemoveLocalViews
// ---------------------------------------------------------------------------
//
void CGSSensorPlugin::RemoveLocalViews()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::RemoveLocalViews()" );
    // Remove view from iAppUi -> View is deleted by iAppUi automatically.
    if ( iTappingCtrlView )
        {
        // Also deletes view.
        iAppUi->RemoveView( KSenTapCtrlViewId );
        iTappingCtrlView = NULL;
        }
    if ( iTurningCtrlView )
        {
        // Also deletes view.
        iAppUi->RemoveView( KSenTurnCtrlViewId );
        iTurningCtrlView = NULL;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::RemoveLocalViews() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::CreateLocalViewL
// Initializes local views within this plug-in based on the view ID.
// ---------------------------------------------------------------------------
void CGSSensorPlugin::CreateLocalViewL( TUid aLocalViewId )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::CreateLocalViewL() - view: %i", aLocalViewId );

    // Check if the view exists. If view does not exist:
    // 1. Create the view
    // 2. Add view to iAppUi -> iAppUi takes the view ownership.
    //
    if ( aLocalViewId == KSenTapCtrlViewId && !iTappingCtrlView )
        {
        iTappingCtrlView = CGSSenTappingCtrlView::NewL( iModel );
        iAppUi->AddViewL( iTappingCtrlView );
        }
    else if ( aLocalViewId == KSenTurnCtrlViewId && !iTurningCtrlView )
        {
        iTurningCtrlView = CGSSenTurnCtrlView::NewL( iModel );
        iAppUi->AddViewL( iTurningCtrlView );
        }
    else
        {
        // View is already created
        TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CreateLocalViewL() - View already exists" );
        }

    iAppUi->ActivateLocalViewL( aLocalViewId );

    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CreateLocalViewL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::GetCaption
// Return application/view caption.
// ----------------------------------------------------------------------------
//
void CGSSensorPlugin::GetCaptionL( TDes& aCaption ) const
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::GetCaptionL()" );
    // the resource file is already opened.
    HBufC* result = StringLoader::LoadL( R_GS_SENSOR_VIEW_CAPTION );
    aCaption.Copy( *result );
    delete result;
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::GetCaptionL() - return" );
    }

// ----------------------------------------------------------------------------
// CGSSensorPlugin::PluginProviderCategory
// A means to identify the location of this plug-in in the framework.
// ----------------------------------------------------------------------------
//
TInt CGSSensorPlugin::PluginProviderCategory() const
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::PluginProviderCategory()" );
    //To identify internal plug-ins.
    return KGSPluginProviderInternal;
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::CreateIconL
// Return the icon, if has one.
// ---------------------------------------------------------------------------
//
CGulIcon* CGSSensorPlugin::CreateIconL( const TUid aIconType )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CreateIconL()" );
    CGulIcon* icon;
       
    if ( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(), 
        KAknsIIDQgnPropSetGeneSensor, 
        KGSSenPluginIconFileName,
        EMbmGssenpluginQgn_prop_set_gene_sensor,
        EMbmGssenpluginQgn_prop_set_gene_sensor_mask );
        }
     else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CreateIconL() - return" );
    return icon;
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::CreateContainerL
// Override function from CGSBaseView
// ---------------------------------------------------------------------------
//
void CGSSensorPlugin::CreateContainerL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CreateContainerL()" );
    NewContainerL();
    __ASSERT_ALWAYS( iContainer, User::Panic( KGSDoActivateError, EGSViewPanicNullPtr ) );
    iContainer->SetMopParent( this );

    TRAPD( error, Container()->ConstructL( ClientRect(), AppUi(), iPluginArray ) );

    if ( error )
        {
        delete iContainer;
        iContainer = NULL;
        User::Leave( error );
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CreateContainerL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::CheckMiddleSoftkeyLabelL
// ---------------------------------------------------------------------------
//
void CGSSensorPlugin::CheckMiddleSoftkeyLabelL() 
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CheckMiddleSoftkeyLabelL()" );
    // First, resolve current item
    const TInt currentItem = Container()->CurrentFeatureId();

    // First, remove current label
    RemoveCommandFromMSK();

    if ( EGSSensorsListItemId == currentItem )
        {
        // Change MKS to 'Change'
        SetMiddleSoftKeyLabelL( R_GS_MKS_LABEL_CHANGE, EGSSenMskGeneral );
        }
    else
        {
        // Change MSK to 'Open'
        SetMiddleSoftKeyLabelL( R_GS_MKS_LABEL_OPEN, EGSSenMskGeneral );
        }  
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::CheckMiddleSoftkeyLabelL() - return" );
    }

// -----------------------------------------------------------------------------
// CGSSensorPlugin::RemoveCommandFromMSK
// This method should do nothing but MSK issues.
// -----------------------------------------------------------------------------
//
void CGSSensorPlugin::RemoveCommandFromMSK()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::RemoveCommandFromMSK()" );
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        // Remove command from stack
        cbaGroup->RemoveCommandFromStack( KGSSenPluginMSKControlID, EGSSenMskGeneral );
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::RemoveCommandFromMSK() - return" );
    }

// ---------------------------------------------------------------------------
// Sets middle softkey label.
// ---------------------------------------------------------------------------
//
void CGSSensorPlugin::SetMiddleSoftKeyLabelL( 
    const TInt aResourceId, const TInt aCommandId )
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::SetMiddleSoftKeyLabelL()" );
    CEikButtonGroupContainer* cbaGroup = Cba();
    if ( cbaGroup )
        {
        HBufC* middleSKText = StringLoader::LoadLC( aResourceId );
        TPtr mskPtr = middleSKText->Des();
        cbaGroup->AddCommandToStackL( 
            KGSSenPluginMSKControlID, 
            aCommandId, 
            mskPtr );
        CleanupStack::Pop( middleSKText );
        delete middleSKText;
        }
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::SetMiddleSoftKeyLabelL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::ChangeSensorActiveSettingL
// Change sensors active status setting .
// ---------------------------------------------------------------------------
//
void CGSSensorPlugin::ChangeSensorActiveSettingL()
    {
    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::ChangeSensorActiveSettingL()" );
    TInt currentValue = iModel->SensorActiveStatusL();
    
    if( currentValue == KGSSenOff )
        {
        currentValue = KGSSenOn;
        }
    else
        {
        currentValue = KGSSenOff;
        }

    iModel->SetSensorActiveStatusL( currentValue );
    UpdateListBoxL( EGSSensorsListItemId );

    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::ChangeSensorActiveSettingL() - return" );
    }

// ---------------------------------------------------------------------------
// CGSSensorPlugin::HandlePluginLoaded
// Called by GSPluginLoader when plugins are loaded or error occured during that
// ---------------------------------------------------------------------------
//
void CGSSensorPlugin::HandlePluginLoaded( KGSPluginLoaderStatus aStatus )
    {
    TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::HandlePluginLoaded(): %i", aStatus );

    if ( iContainer && MGSPluginLoadObserver::EGSFinished == aStatus )
        {
        #ifdef __DEBUG
        TRAPD( ignore, Container()->UpdateListBoxL( EGSExtPluginsListItemId ); );
        TRACE_1( "[GSSensorPlugin] CGSSensorPlugin::HandlePluginLoaded() - Error: %i", ignore );
        #else
        TRAP_IGNORE( Container()->UpdateListBoxL( EGSExtPluginsListItemId ); );
        #endif // __DEBUG
        }

    TRACE_( "[GSSensorPlugin] CGSSensorPlugin::HandlePluginLoaded() - return" );
    }
