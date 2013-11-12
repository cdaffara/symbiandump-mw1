/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CAknAnimDll class definition 
*
*/


#ifndef NSPSWSPLUGINDLL_H
#define NSPSWSPLUGINDLL_H

//  INCLUDES
#include <e32std.h>             // TInt
#include <w32adll.h>            // CAnimDll

/**
*  CAknAnimDll class definition 
*  Animation plug-in (Avkon Anim Dll Factory)
*
*  @lib   nspswsplugin
*  @since 1.0
*/

class CAknAnimDll : public CAnimDll
    {
    public:// From CAnimDll
    /** Creates instance of plugin.
     *   @see CAnimDll::CreateInstanceL 
     */
    virtual CAnim* CreateInstanceL(TInt aType);
    };

/** 
 *  Create AnimDll
 *  Factory function 
 */

IMPORT_C CAnimDll* CreateCAnimDllL();

#endif
// End of File

