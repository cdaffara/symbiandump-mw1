/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFContextSourceSettingsManager class implementation.
*
*/


// INCLUDE FILES

#include "cfcontextsourcesettingsmanager.h"
#include "cfcontextsourcesettingsmanagerimpl.h"

EXPORT_C CCFContextSourceSettingsManager* CCFContextSourceSettingsManager::NewL(
    RFs& aFs )
    {
    return CCFContextSourceSettingsManagerImpl::NewL( aFs );
    }

EXPORT_C CCFContextSourceSettingsManager* CCFContextSourceSettingsManager::NewLC(
    RFs& aFs )
    {
    return CCFContextSourceSettingsManagerImpl::NewLC( aFs );
    }
