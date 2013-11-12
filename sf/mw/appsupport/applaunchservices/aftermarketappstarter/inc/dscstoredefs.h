// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __DSCSTOREDEFS_H__
#define __DSCSTOREDEFS_H__

#include "dscstorecommondefs.hrh"

/** TDscPosition is an indication used for insert DscItem to DscDatabase
@publishedPartner
@released
*/	
enum TDscPosition
		{
		/** Add item to the beginning of the DSC */
		EFirst=0,
		/** Add item to the end of the DSC */
		ELast=1
		};
			
#endif // __DSCSTOREDEFS_H__

