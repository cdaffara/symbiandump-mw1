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
* Description:  Imaging Configuration Manager Private CenRep keys
*
*/


#ifndef ICMCENREPKEYS_H
#define ICMCENREPKEYS_H

// Imaging Configuration Manager API

const TUid KCRUidImagingConfigManager = { 0x10282EDC };

// The following values are mainly for reference as they are actually parsed
// dynamically according to
// S60_3_2_Imaging_Configuration_Manager_Central_Repository_Guidance_C.doc

const TUint32 KICMVideoQualitySet001         = 0x00010001;
const TUint32 KICMVideoQualitySet002         = 0x00010002;
const TUint32 KICMVideoQualitySet003         = 0x00010003;
// etc...

const TUint32 KICMCamcorderMMFPluginSettings = 0x00020001;

const TUint32 KICMImageQualitySet001         = 0x00030001;
const TUint32 KICMImageQualitySet002         = 0x00030002;
const TUint32 KICMImageQualitySet003         = 0x00030003;
// etc...

#endif      // ICMCENREPKAYS_H
          
// End of File
