/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Context framework private CenRep keys.
*
*/


#ifndef CONTEXTFRAMEWORKPRIVATECRKEYS_H
#define CONTEXTFRAMEWORKPRIVATECRKEYS_H

// =============================================================================
// Context Source Configuration API
// =============================================================================

const TUid KCRUidCFContextSourceConf = {0x10282BCC};

/**
* Describes how many context source plug-ins are listed in the cenrep
* settings file.
* All the plug-in implementation UIDs will be listed after this key in the
* excel sheet.
* Type: int
*/
const TUint32 KContextSourceNumberOfMandatoryPlugIns = 0x00000001;

// =============================================================================
// Rule Script Configuration API
// =============================================================================

const TUid KCRUidCFRuleScriptConf = {0x10282BCD};

/**
* Describes how many scripts are listed in the cenrep settings file.
* All the script names will be listed after this key in the excel sheet.
* Type: int
*/
const TUint32 KCRuleScriptNumberOfMandatoryRules = 0x00000001;

// =============================================================================
// Action Configuration API
// =============================================================================

const TUid KCRUidCFActionConf = {0x10282BCE};

/**
* Describes how many action plug-ins are listed in the cenrep settings file.
* All the plug-in implementation UIDs will be listed after this key in the
* excel sheet.
* Type: int
*/
const TUint32 KActionNumberOfMandatoryPlugIns = 0x00000001;

// =============================================================================
// Context Framework Common Configuration API
// =============================================================================

const TUid KCRUidCFCommonConf = {0x2000B195};

/**
* Defines the maximum size for context data object.
* Since the data object can decrease CFW performance, the maximum size of
* the data object is restricted.
* The default value is 16kB.
* Type: int
*/
const TUint32 KContextDataObjectMaxSize = 0x00000001;

// =============================================================================
// Context Framework Default Application State API
// =============================================================================

const TUid KCRUidCFApplDefaultState = {0x2000CFD9};

/**
* Describes application default state for unlisted applications
* Possible values are:
* 65536  = Application is by default in portrait orientation
* 196608 = Application is by default in landscape orientation
* 327680 = Application follows the HW specified orientation
* Type: int
*/
const TUint32 KApplDefaultState = 0x00000001;

#endif
