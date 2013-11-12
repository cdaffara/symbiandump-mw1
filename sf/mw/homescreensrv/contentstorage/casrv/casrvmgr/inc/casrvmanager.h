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
 * Description: casrvmanager.h
 *
 */

#ifndef CA_SRV_MANAGER_H
#define CA_SRV_MANAGER_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32hashtab.h>
#include "casrvplugin.h"


class CCaSrvEngUtils;
namespace Usif{
    class RSoftwareComponentRegistry;
}

/**
 *
 * @since S60 5.0
 * @ingroup group_mmextensions
 */
NONSHARABLE_CLASS( CCaSrvManager ): public CBase
    {
public:
    // Constructors and destructor

    /**
     * Standard C++ Destructor.
     */
    ~CCaSrvManager();

    /**
     * Two-phased constructor.
     * @return Instance of the CCaSrvManager class.
     * @param aCaStorageProxy storage.
     * @param aUtils storage utils.
     */
    IMPORT_C static CCaSrvManager* NewL( CCaStorageProxy& aCaStorageProxy,
            Usif::RSoftwareComponentRegistry* aSoftwareRegistry,
            CCaSrvEngUtils* aUtils = NULL );

    /**
     * Two-phased constructor.
     * @return Instance of the CCaSrvManager class.
     * @param aCaStorageProxy storage.
     * @param aUtils storage utils.
     */
    static CCaSrvManager* NewLC( CCaStorageProxy& aCaStorageProxy,
            Usif::RSoftwareComponentRegistry* aSoftwareRegistry,
            CCaSrvEngUtils* aUtils = NULL );

    /**
     * Load operation error code
     * @return error code for load plugins operation
     */
    IMPORT_C TInt LoadOperationErrorCodeL();
    
private:

    /**
     * Constructor for performing 1st stage construction.
     * @param aCaStorageProxy Interface enabling to issues request to App UI.
     */
    CCaSrvManager();

    /**
     * EPOC default constructor for performing 2nd stage construction.
     * @param aCaStorageProxy storage.
     * @param aUtils storage utils.
     */
    void ConstructL( CCaStorageProxy& aCaStorageProxy,
            Usif::RSoftwareComponentRegistry* aSoftwareRegistry,
            CCaSrvEngUtils* aUtils = NULL );

    /**
     * Loads MultimediaMenu plugins implementations.
     */
    void LoadPluginsL();

private:

    /**
     * Pointer to plugin map.
     * Own.
     */
    RHashMap<TInt32, CCaSrvPlugin*> iPluginMap;

    /**
     * Not Own.
     * Reference to the Storage Proxy, Storage Utils.
     */
    TPluginParams* iPluginParams;
    
    /**
     * Error code for load plugins operation
     */
    TInt iErrorCode;
    };

#endif // C_MM_EXTENSION_MANAGER_H
// End of file
