/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
*/



#if (!defined __DSTUNITSTEP_H__)
#define __DSTUNITSTEP_H__

#include "DstStep.h"
#include "DstUnitServer.h"
#include <tz.h>

class CDstUnitStep : public CDstStep
	{
public:
	CDstUnitStep(CDstUnitServer& aServer);
	CDstUnitServer& Server();

private:
	CDstUnitServer& iServer;		
	};

#endif



