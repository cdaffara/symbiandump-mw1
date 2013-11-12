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

#ifndef C_CAUSIFUNINSTALLOPERATION_H
#define C_CAUSIFUNINSTALLOPERATION_H

#include <e32base.h>
#include <usif/sif/sif.h>

class CaUninstallerObserver;

using namespace Usif;

/**
 * Uninstall Operation
 * Encapsulates the functionality of uninstalling an application.
 *
 *  @lib caclient.lib
 */
NONSHARABLE_CLASS(CCaUsifUninstallOperation): public CActive {

public:
    /**
    * Destructor
    */
    virtual ~CCaUsifUninstallOperation();
    
    /**
     * Two-phased constructor. Leaves on failure.
     * Allocates memory for and constructs an uninstaller object. After that
     * starts asynchronous uninnstallation of the requested entry
     * @param aEntry entry to be uninstalled
     * @param aPriority priority of the active scheduler responsible for handling
     * asynchronous installation operation
     * @return The constructed object.
     */
    static CCaUsifUninstallOperation *NewL(TComponentId, TInt aPriority =
            CActive::EPriorityStandard);
    
    void AddObserver(CaUninstallerObserver* aCaUninstallerObserver);

private:
    // construction
    /**
     * Constructor.
     * Starts active scheduler.
     * @param aEntry entry to be uninstalle
     * @param aPriority priority of the active scheduler responsible for handling
     * asynchronous installation operation
     */
    CCaUsifUninstallOperation(TInt aPriority);
    
    /**
     * Initializes the object and starts asynchronous uninstallation process.
     * @param aEntry entry to be uninstalle
     * asynchronous installation operation
     */
    void ConstructL(TComponentId aComponentId);
    


private:
    // from CActive

    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);

private:
    COpaqueNamedParams* iArguments;
    COpaqueNamedParams* iResults;

    RSoftwareInstall iUninstaller;
    
    CaUninstallerObserver* iCaUninstallerObserver; // not owned
};

#endif // C_CAUSIFUNINSTALLOPERATION_H
