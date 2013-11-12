/*
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
* Definition of the missing dependency dialog commandd handler and command request object.
*
*/


/**
 @file 
 @internalComponent
 @released
*/
 
#ifndef __MISSINGDEPENDENCY_H__
#define __MISSINGDEPENDENCY_H__

#include "uisscommand.h"

namespace Swi
{
class MUiHandler;

/**
 * Command handler for the missing dependency dialog.
 */
NONSHARABLE_CLASS(CMissingDependencyDialogCmdHandler) : public CUissCmdHandler
	{
public:
	//virtual void HandleMessageL(const RMessage2& aMessage);
	virtual void HandleMessageL(const TDesC8& aInBuf, TDes8& aOutBuf);
	CMissingDependencyDialogCmdHandler(MUiHandler& aUiHandler);
	};

} // namespace Swi

#endif // #ifndef __MISSINGDEPENDENCY_H__
