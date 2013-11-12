/*
 * Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:  ?Description
 *
 */

#ifndef CASRV_H
#define CASRV_H

//  INCLUDES
#include <e32base.h>
#include <usif/scr/scr.h>

// FORWARD DECLARATION
class CCaSrvEngUtils;
class CCaSrvManager;
class CCaStorageProxy;
class CCaBackupNotifier;

// GLOBAL FUNCTIONS
/**
 * Run the Menu Server (entry function).
 * @return Error code.
 */
IMPORT_C TInt RunCaServer();

// CLASS DECLARATION

/**
 * Menu Server.
 */
NONSHARABLE_CLASS( CCaSrv ): public CServer2
    {
public:
    
    /**
     * Destructor.
     */
    virtual ~CCaSrv();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed server.
     */
    static CCaSrv* NewL();

public:
    
    /**
     * Storage proxy getter.
     * @return storage proxy.
     */
    CCaStorageProxy* GetStorageProxy();

    /*
     * Increases session count.
     */
    void IncreaseSessionCount();

    /*
     * Decreases session count. If session count reaches 0 server is stoped.
     */
    void DecreaseSessionCount();

private:
    // Constructors and destructor

    /**
     * Constructor.
     */
    CCaSrv();

    /**
     * Second-phase constructor.
     */
    void ConstructL();

private:
    // From CServer2

    /**
     * Create new session.
     * @param aVersion Version.
     * @param aMessage Message (unused).
     * @return New session.
     */
    CSession2* NewSessionL( const TVersion& aVersion,
            const RMessage2& aMessage ) const;

private:
    // Data
    
    /**
     * Data storage proxy. Own.
     */
    CCaStorageProxy* iStorageProxy;

    /*
     * Session count. Own.
     */
    TInt iSessionCount;

    /*
     * Utils. Own.
     */
    CCaSrvEngUtils* iSrvEngUtils;
    
    /*
     * Software Component Registry. Own.
     */
    Usif::RSoftwareComponentRegistry iSoftwareRegistry;

    /*
     * Srv plugin manager. Own.
     */
    CCaSrvManager* iSrvManager;
    
    /*
     * Srv plugin manager. Own.
     */
    CCaBackupNotifier* iBackupNotifier;

    };

#endif // CASRV_H
// End of File
