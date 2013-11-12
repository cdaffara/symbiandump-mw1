/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Plugin interface
 *
*/


#ifndef C_CONTENTHARVESTERPLUGIN_H
#define C_CONTENTHARVESTERPLUGIN_H

#include <e32base.h>

// CLASS DECLARATION
/**
 * The interface class for all Content Harvester plugins.
 * Plugins are responsible for updating and synchronization with CPS.
 *
 */
class CContentHarvesterPlugin : public CBase
    {
public:

    /** 
     * Method called by Content Harvester in order to invoke Update operation.
     * Must be implemented by plugin provider.
     */
    virtual void UpdateL() = 0;

    };

#endif // C_CONTENTHARVESTERPLUGIN_H
