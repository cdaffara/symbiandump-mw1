/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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


#ifndef __STARTUPMEDIATORPLUGINPROPERTYRESPONDER__
#define __STARTUPMEDIATORPLUGINPROPERTYRESPONDER__

// INCLUDES
#include <e32base.h>

class MStartupMediatorPluginPropertyResponder
    {
public:
    virtual void HandlePropertyChangedL( const TUid& aCategory, TUint aKey ) = 0;
    };
#endif //__STARTUPMEDIATORPLUGINPROPERTYRESPONDER__

// End of File
