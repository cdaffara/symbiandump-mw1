/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence Service Connection observer for events from Globals
 *
*/


#ifndef XIMPCPSCGLOBALSOBSERVER_H
#define XIMPCPSCGLOBALSOBSERVER_H

#include <e32base.h>

/**
 * Observer interface for events originating from the singleton "globals"
 * holder.
 *
 * @lib ximpoperation.dll
 * @since S60 v4.0
 */
class MXIMPGlobalsObserver
    {
    public:

		/**
		 * Signal about host deletion.
         * The server can shutdown itself after this.
	 	 */
		virtual void AllHostsDied() = 0;
    };

#endif      // XIMPCPSCGLOBALSOBSERVER_H


