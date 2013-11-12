/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Constants header file
*
*/

// CONSTANTS

// default build language
_LIT( KDefaultLanguage,     "SC" );

// Characters to strip from xml text values
_LIT( KCommonControlCharacters, "\n\t\r" );
_LIT( KEndl,                    "\n\r" );

// === Directory & file constants =============================================
_LIT( KDirSeparator,        "\\" );
_LIT( KDefFileName,         "brandfile.bin.r" );
_LIT( KDefEventLogDir,      "BSImport" );
_LIT( KDefEventLogFile,     "BS_event_log.txt" );


// === XML tags ===============================================================
_LIT( KBrandingTag,         "branding" );
_LIT( KBrandTag,            "brand" );
_LIT( KBrandIDTag,          "brand_id" );
_LIT( KBrandLanguageTag,    "brand_language_id" );
_LIT( KBrandVersionTag,     "brand_version" );
_LIT( KBrandAppIdTag,       "application_id" );
_LIT( KBrandElementTag,     "element" );
_LIT( KElementValueTag,     "element_value" );

// Element attributes
// e.g. <element type="integer" id="id1">
//
_LIT( KElementTypeName,     "type" );
_LIT( KElementTypeID,       "id" );

// Element type attributes
_LIT( KElementTypeList,     "list" );
_LIT( KElementTypeBitmap,   "bitmap" );
_LIT( KElementTypeBuffer,   "buffer" );
_LIT( KElementTypeInt,      "integer" );
_LIT( KElementTypeText,     "text" );
_LIT( KElementTypeFile,     "file" );

// Bitmap element XML tags
_LIT( KBitmapIdTag,         "bitmap_id" );
_LIT( KBitmapFileIdTag,     "bitmap_file_id" ); /// XSP MOD
_LIT( KBitmapMaskIdTag,     "mask_id" );
_LIT( KBitmapSkinIdTag,     "skin_id_major" );
_LIT( KBitmapSkinMaskIdTag, "skin_id_minor" );

// File element XML tags
_LIT( KFileFileNameTag,     "file_name" );

// end of file
