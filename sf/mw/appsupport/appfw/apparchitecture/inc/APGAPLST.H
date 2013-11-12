// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apgaplst.h
//

#ifndef __APGAPLST_H__
#define __APGAPLST_H__

#include <apaid.h>
#include <apmstd.h>
#include <badesca.h>
#include <s32file.h>



/**
KApaMaxAppIcons

@publishedPartner
@released
*/
const TInt KApaMaxAppIcons=3;

/**
KApaIconIndexSmall

@publishedPartner
@released
*/
const TInt KApaIconIndexSmall=0;

/**
KApaIconIndexMedium

@publishedPartner
@released
*/
const TInt KApaIconIndexMedium=1;

/**
KApaIconIndexLarge

@publishedPartner
@released
*/
const TInt KApaIconIndexLarge=2;


_LIT(KDefaultAppIconMbm,"Z:\\Resource\\Apps\\default_app_icon.mbm");
_LIT(KLitPathForUntrustedRegistrationResourceFiles, "?:\\private\\10003a3f\\import\\apps\\*");


#endif //__APGAPLST_H__
