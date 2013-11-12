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
* Description:  Base class for XIMP API implementations.
*
*/

#ifndef IMAPIOBJBASE_H
#define IMAPIOBJBASE_H


/**
 * IM plugin class implementation UIDs.
 *
 * These do not have to be "real" Symbian UIDs, but such ID's,
 * that they do not overlap with XIMP or IM Plugin API interface IDs.
 */
 
// 90xx - ximp related
// 91xx - im related
// 92xx - instant messaging related


const TInt KArrayGranularity( 2 );

// IM plugin events
#define IMIMP_CLSID_CPROTOCOLIMCONVERSATIONDATAHOST     9200

// Im datamodel classes
#define IMIMP_CLSID_CIMDATACACHE                		9210
#define IMIMP_CLSID_CIMCONVERSATIONINFOIMP				9211
#define IMIMP_CLSID_CIMCONVERSATIONEVENTIMP				9212
#define IMIMP_CLSID_CIMOBJECTFACTORYIMP					9213
#define IMIMP_CLSID_CIMOPERATIONFACTORYIMP				9214
// Im plugin host environment classes
#define IMIMP_CLSID_CPROTOCOLIMDATAHOSTIMP				9230


// IM Management classes

#define IMIMP_CLSID_CIMSEARCHIMP    	        		9240
#define IMIMP_CLSID_CONVERSATIONIMP            			9241
#define IMIMP_CLSID_CIMGROUPSIMP						9242
#define IMIMP_CLSID_CIMINVITATIONIMP					9243
#define IMIMP_CLSID_CIMFEATURESIMP						9244



#endif // IMAPIOBJBASE_H
