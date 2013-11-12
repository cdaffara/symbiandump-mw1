/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*
*/




#include "RfsHelixPlugin.h"


CRfsHelixPlugin::CRfsHelixPlugin()
    {
    }

CRfsHelixPlugin::CRfsHelixPlugin(TAny* /*aInitParams*/)
    {
    }


CRfsHelixPlugin::~CRfsHelixPlugin()
    {
    }


// ---------------------------------------------------------
// NewL
// ---------------------------------------------------------
//
CRfsHelixPlugin* CRfsHelixPlugin::NewL(TAny* aInitParams)
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsHelixPlugin::NewL()"));
    #endif
    CRfsHelixPlugin* self = new (ELeave) CRfsHelixPlugin(aInitParams);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// ---------------------------------------------------------
// ConstructL
// ---------------------------------------------------------
//

void CRfsHelixPlugin::ConstructL()
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsHelixPlugin::ConstructL()"));
    #endif
    }

// ---------------------------------------------------------
// RestoreFactorySettingsL
// ---------------------------------------------------------
//

#ifdef _DEBUG
void CRfsHelixPlugin::RestoreFactorySettingsL( const TRfsReason aType )
#else
void CRfsHelixPlugin::RestoreFactorySettingsL( const TRfsReason /* aType */)
#endif
    {
    #ifdef _DEBUG
        RDebug::Print(_L("CRfsHelixPlugin::RestoreFactorySettingsL(%d)"), aType);
    #endif
    RFs fsrv;

    _LIT(KRNUserConfigNameAndPath,     "c:\\data\\R1_Mobile_4_0.cfg");
    _LIT(KRNPluginArchiveNameAndPath,  "c:\\data\\plugin_archive.txt");
	_LIT(KRNMDEngineNameAndPath,  "c:\\data\\HXMDEngine_3_2.cfg");
	_LIT(KRNMetadataArchiveNameAndPath,  "c:\\data\\hxmetadata_archive.txt");
	_LIT(KRNThumbnailArchiveNameAndPath,  "c:\\data\\hxthumbnail_archive.txt");
	_LIT(KRNTNEngineNameAndPath,  "c:\\data\\HXTNEngine_3_2.cfg");
	_LIT(KRNMdfPluginArchiveNameAndPath,  "c:\\data\\MdfPluginArchive.txt");

    TInt err = KErrNone;

    CleanupClosePushL( fsrv );

    err = fsrv.Connect();
    if ( err != KErrNone )
        {
        User::Leave( err );
        }
    // Delete all the cfg-files.
    //
    fsrv.Delete(KRNUserConfigNameAndPath);
    fsrv.Delete(KRNPluginArchiveNameAndPath);
	fsrv.Delete(KRNMDEngineNameAndPath);
	fsrv.Delete(KRNMetadataArchiveNameAndPath);
	fsrv.Delete(KRNThumbnailArchiveNameAndPath);
	fsrv.Delete(KRNTNEngineNameAndPath);
	fsrv.Delete(KRNMdfPluginArchiveNameAndPath);

    CleanupStack::PopAndDestroy( 1 );
    }



void CRfsHelixPlugin::GetScriptL( const TRfsReason /*aType*/, TDes& /*aPath*/ )
    {

    }



void CRfsHelixPlugin::ExecuteCustomCommandL( const TRfsReason /*aType*/,
                                           TDesC& /*aCommand*/ )
    {

    }


