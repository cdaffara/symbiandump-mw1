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
* Description:  class ids
*
*/

#ifndef SEARCHAPIOBJBASE_H
#define SEARCHAPIOBJBASE_H


/**
 * search  plugin class implementation UIDs.
 *
 * These do not have to be "real" Symbian UIDs, but such ID's,
 * that they do not overlap with XIMP or IM Plugin API interface IDs.
 */
 
// 90xx - ximp related
// 91xx - im related
// 92xx - instant messaging related
// 93xx - search related


const TInt KArrayGranularity( 2 );

#define  IMP_CLSID_CSEARCHFEATUREIMP                   	9300
#define  IMP_CLSID_CSEARCHIMP    	        			9301

#define IMP_CLSID_CSEARCHOBJECTFACTORYIMP			    9302
#define IMP_CLSID_CSEARCHINFOIMP                    	9303
#define IMP_CLSID_CSEARCHEVENTIMP                   	9304
#define IMP_CLSID_CPROTOCOLIMDATAHOSTIMP              	9305

#define IMP_CLSID_CSEARCHOPERATIONFACTORYIMP          	9306
#define IMP_CLSID_CSEARCHDATACACHE                      9307
#define IMP_CLSID_CSEARCHELEMENTIMP                     9308
#define IMP_CLSID_CPROTOCOLSEARCHDATAHOSTIMP            9309
#define IMP_CLSID_CPROTOCOLSEARCHFEATUREDATAHOST        9310


#define IMP_CLSID_CSEARCHKEY_INFO_IMP                   9311
#define IMP_CLSID_CSEARCHKEYSEVENTIMP                   9312
#endif // SEARCHAPIOBJBASE_H
