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
* Description: caappscanner.h
*
*/


#ifndef __C_APPSCANNER_HANDLER_H__
#define __C_APPSCANNER_HANDLER_H__

#include "casrvplugin.h"
#include "casrvappscanner.h"

/**
 *  App Scanner Handler.
 *  @lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaAppScannerHandler ): public CCaSrvPlugin
    {
public:

    /**
     * Two-phased constructor. Leaves on failure.
     * @param aPLuginParam plugin param consist of storage, storage utils.
     * @return The constructed object.
     */
    static CCaAppScannerHandler* NewL( TPluginParams* aPLuginParam );

    /**
     * Destructor.
     * @since S60 v5.0
     * @capability None.
     * @throws None.
     * @panic None.
     */
    virtual ~CCaAppScannerHandler();

private:

    /**
     * Constructor.
     * @since S60 v5.0
     * @param aPLuginParam plugin param consist of storage, storage utils.
     */
    CCaAppScannerHandler( TPluginParams* aPLuginParam );

    /**
     * Symbien 2nd phase constructor.
     * @param aCaStorageProxy storage.
     * @param aSoftwareRegistry software component registry.
     * @param aUtils storage utils.
     */
    void ConstructL( CCaStorageProxy& aCaStorageProxy,
            Usif::RSoftwareComponentRegistry& aSoftwareRegistry,
            CCaSrvEngUtils& aUtils );

private:
    // data

    /**
     * App scanner.
     * Own.
     */
    CCaSrvAppScanner* iScanner;

    CA_STORAGE_TEST_FRIEND_CLASS ( T_casrvAppScaner)

    };

#endif // __C_APPSCANNER_HANDLER_H__
    // End of File
