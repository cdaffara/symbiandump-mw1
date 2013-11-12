/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: This file defines the API for CaLocalizerScanner.dll
 *
 */

#ifndef __CALOCALCANNERPLUGIN_H__
#define __CALOCALCANNERPLUGIN_H__

#include <e32base.h>
#include <e32std.h>
#include "casrvplugin.h"


class CCaLocalizerScannerProxy;

class CCaLocalizerScanner: public CCaSrvPlugin
    {
public:

    /**
     * Two-phased constructor.
     */
    static CCaLocalizerScanner* NewL( TPluginParams* aPluginParams );
    
    /**
     * destructor.
     */
    virtual ~CCaLocalizerScanner( );

private:
    
    /**
     * Default constructor.
     */
    CCaLocalizerScanner( );
    
    /**
     * Two-phased constructor.
     * @param aPluginParams should contain CaStorageProxy
     */
    void ConstructL( TPluginParams* aPluginParams );

private:
    
    /**
     * Inner class of plugin. Owned
     */
    CCaLocalizerScannerProxy* iLocalizerScanner;
    };

#endif  // __CALOCALCANNERPLUGIN_H__
