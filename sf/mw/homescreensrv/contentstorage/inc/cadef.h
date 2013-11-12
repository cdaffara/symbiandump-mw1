/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  Definition of different constants
 *  Version     : %version: 10.1.30 % << Don't touch! Updated by Synergy at check-out.
 *
 */

#ifndef CADEF_H
#define CADEF_H

#include <e32base.h>
#include "cainternaltypes.h"


// database version
_LIT( KCaDbVersion, "00001" );



// ---------------------------------------------------------
//Ca constants

enum TChangeType
    {
    EAddChangeType,
    ERemoveChangeType,
    EUpdateChangeType
    };

enum TEntryRole
    {
    ENoneEntryRole = 0,
    EItemEntryRole = 1,
    EGroupEntryRole = 2
    };

//Entry flags
enum TEntryFlag
    {
    EUsed = 1,//Flag is set only for removable applications
    EUninstall = 2,
    ERemovable = 4,
    EVisible = 8,
    EReserved = 16, //flag is unused, stays for compatibility
    ENative = 32,
    EMissing = 64
    };

enum TItemAppearance
    {
    EItemAppearanceNotChanged,
    EItemDisappeared,
    EItemAppeared,
    EItemUninstallProgressChanged
    };

// Ca - Application

_LIT( KCaTypeMenuCollections, "menucollections" );
_LIT( KCaTypeFolder, "folder" );
_LIT( KCaTypeCollection, "collection" );
_LIT( KCaTypeApp, "application" );
_LIT( KCaTypeWidget, "widget" );
_LIT( KCaTypeUrl, "url" );
_LIT( KCaTypePackage, "package" );
_LIT( KCaAttrView, "view" ); ///< View.
_LIT( KCaAttrUrl, "url" );
_LIT( KCaTypeCollectionDownload, "collection::downloaded" );
_LIT( KCaPackageUid,"packageuid");
_LIT( KCaAttrInstallationTime, "installationTime");
_LIT( KCaTypeTemplatedApp, "templatedApplication" );
_LIT( KCaAttrAppType, "apptype");
_LIT( KCaAttrAppTypeValueJava, "java");
_LIT( KCaAttrAppTypeValueCWRT, "cwrt");
_LIT( KCaAttrAppTypeValueNative, "native");
_LIT( KCaAttrComponentId, "component_id" );
_LIT( KCaAttrDrivesIconIds, "drivesIconIds");
_LIT( KCaAttrAppSettingsPlugin, "app_settings_plugin");
_LIT( KCaScrPropertyAppSettings, "settingsName");
_LIT( KPreviewImageAttrName, "preview_image_name" );
_LIT( KCaAttrAppWidgetUri, "widget:uri");
_LIT( KCaAttrAppWidgetServiceXml, "widget:servicexml");

_LIT( KCaAttrAppWidgetUriCWRTValue, "wrtwidgetuiplugin");
_LIT( KCaAttrAppWidgetParamWebAppId, "widgetparam:webAppId");
_LIT( KCaScrPropertyAppId, "AppID");
_LIT( KCaScrPropertyIsMiniviewSupported, "isMiniviewSupported");

_LIT( KCaAppGroupName, "appgroup_name" );
_LIT( KCaAppUninstallProgress, "uninstall_progress" );

_LIT( KCollectionIconFileName, "qtg_large_applications_user");


_LIT(KLocalizationFilepathZ, "z:/resource/qt/translations");
_LIT(KLocalizationFilepathC, "c:/resource/qt/translations");
_LIT(KLocalizationFilepath, ":/resource/qt/translations");
_LIT( KLocalizationCaEntry, "CA_ENTRY" );
_LIT( KLocalizationEnText, "EN_TEXT" );
_LIT( KLocalizationEnDescription, "EN_DESCRIPTION" );
const TInt charsToFilename= 6;  // <loc://> loc://email/qtn_gmail_mail_account

_LIT( KCaQmFile, "contentstorage_" );//default content storage qm file
//Defaul localize string for collection
_LIT( KDefaultLocTitleName, "txt_applib_subtitle_collections_user" );


const TInt KGranularityOne = 1;
const TInt KUidChars = 10;
const TInt KDefaultGranularity = 5;
const TInt KDefaultExpandSize = 512;
const TInt KCaMaxAttrLenght = 255;
// MMC related constants
_LIT( KCaAttrMmcId, "mmc_id" );
_LIT( KCaMassStorage, "mass_storage" );
_LIT( KCaMmcHistoryFname, "mmchistory" );
// max(KCaMassStorage.Length(), KUidChars)
const TInt KMassStorageIdLength = 12;

//Ca constants
// ---------------------------------------------------------


// CONSTANTS

const TUid KSatUid =
    {
    0x101F4CE0
    };

const TUid KHsAppUid =
    {
    0x20022F35
    };

const TUid KMidletApplicationTypeUid =
    {
    0x10210E26
    };

const TUid KCWRTApplicationTypeUid =
    {
    0x10282821 
    };

const TInt KCenRepBufferSize = 255;
//
// Attributes commonly used for all items.
// These are the attributes that are normally used when displaying a menu of items
// in a menu UI. To get a consistent look of various items, it is recommended that
// some of these attributes are properly set (at least one of the xxx_name, and
// some kind of icon information for non-applications).
// Note, however, that none of these is guaranteed to be set, or be used by a menu UI.
// Menu clients always need to check that the necessary attributes are present.
//
_LIT( KCaAttrShortName, "short_name" ); ///< Short name.
_LIT( KCaAttrTitleName, "title_name" ); ///< Title name.
_LIT( KCaComponentId, "component_id" ); ///< Component Id.

// Built-in types and attributes.
//
//
// Command strings.
//

_LIT8( KCaCmdOpen, "open" ); ///< Open item.
_LIT8( KCaCmdRemove, "remove" ); ///< Remove item.


//Properties in database

_LIT( KCaDbPropVersion, "Version" ); /// db version
_LIT( KCaDbPropLanguage, "Language" ); // language of ca strings in db
_LIT( KCaDbPropQMfile, "QMfile" ); // prefix of qm file used in translation



_LIT(KComma, ",");
// Prefix hex format
_LIT(KHexFormat, "0x%X");

#endif // CADEF_H
