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
* Description:  Animation plugin implementation file
*       
*/


// INCLUDE FILES

#include "keypublisherdll.h"
#include "KeyPublisher.h"

// -----------------------------------------------------------------------
// CKeyPublisherDll::CreateInstanceL
// Description: A virtual factory function to create a new instance of the 
//              animation object of the type CAnim.
// return: Animation object derived from CAnim.
// -----------------------------------------------------------------------
//
CAnim* CKeyPublisherDll::CreateInstanceL( TInt aType )
    {
    switch (aType)
        {
        case ESVKAnimKeySound:
            return new(ELeave)CKeyPublisher();
        default:
            return NULL;
        }
    }
// ========================== OTHER EXPORTED FUNCTIONS ======================    
// -----------------------------------------------------------------------
// CreateCAnimDllL ()
// Description: This is the first function called by the windows server. 
// return: Animation object derived from CAnimDll.
// -----------------------------------------------------------------------
//
EXPORT_C CAnimDll* CreateCAnimDllL()
    {
    return new(ELeave)CKeyPublisherDll();
    }



// End of File
