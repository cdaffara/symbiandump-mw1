/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Declaration of class CKeyPublisherDll .
*       
*
*/


#ifndef __KEYPUBLISHERDLL_H__
#define __KEYPUBLISHERDLL_H__

//  INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <w32std.h>
#include <w32adll.h>

// DATA TYPES
enum TKeyPublisherAnimType
    {
    ESVKAnimKeySound
    };   

/**
*  CKeyPublisherDll 
*  Used by the Window server for launching Animation plugin
*
*  @lib KeyPublisherPlugin.dll
*  @since 3.0
*/
class CKeyPublisherDll : public CAnimDll
    {
public:
    /** @see CAnimDll::CreateInstanceL(TInt aType) */
    CAnim* CreateInstanceL(TInt aType);
    };

/**
* Entry point function to Create the animation plugin
* Returns the object of the CKeyPublisherDll class
*/
IMPORT_C CAnimDll* CreateCAnimDllL();


#endif  // __KEYPUBLISHERDLL_H__

// End of File 
