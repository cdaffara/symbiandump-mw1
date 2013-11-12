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
 * Description: causifscanner.h
 *
 */

#ifndef CAUSIFSCANNER_H
#define CAUSIFSCANNER_H

#include <e32base.h>
#include <usif/sif/sifcommon.h>
#include <usif/scr/scr.h>

#include "casrvplugin.h"
#include "cainstallnotifier.h"
#include "cammcwatcher.h"

class CCaStorageProxy;
class CCaInnerEntry;
class CCaMmcWatcher;
class TPluginParams;

namespace Usif
    {
    class CComponentEntry;
    }

using namespace Usif;

/*
 An implementation of the CCaSrvPlugin interface for the Reference Installer.

 The CaSrvManager loads and uses this plug-in when the MIME type of a package
 is 'binary/sif-refpkg'.
 */
NONSHARABLE_CLASS(CCaUsifScanner): public CCaSrvPlugin,
        public MCaInstallListener, public MMmcWatcherCallback
    {
public:

    /**
     * Two-phased constructor.
     * @param aPLuginParam plugin param consist of storage, storage utils and scr.
     * @retval New object.
     */
    static CCaUsifScanner* NewL( TPluginParams* aPluginParams );

    /**
     * Destructor.
     */
    ~CCaUsifScanner();

public:
    // From MCaInstallListener.

    /**
     * Handle Install/Uninstall Event.
     * @since S60 v5.0
     * @param aUid installed uid.
     */
    void HandleInstallNotifyL();

public:
    // From MMmcWatcherCallback.

    /**
     * MmcChangeL is called when the MMC is removed or inserted.
     */
    void MmcChangeL();

private:

    /**
     * Constructor
     * @param aStorageProxy Storage handler.
     * @param aSoftwareRegistry software component registry.
     */
    CCaUsifScanner( CCaStorageProxy& aStorageProxy,
	        RSoftwareComponentRegistry& aSoftwareRegistry );

    /**
     * Second phased constructor.
     */
    void ConstructL();

    /**
     * Creates AppLib's entry from Usif's entry
     * @param aEntry Usif entry obtained from Sif database.
     * @param aCaEntry AppLib entry to be filled from Usif entry.
     */
    void CreateCaEntryFromEntryL( const CComponentEntry* aEntry,
            CCaInnerEntry* aCaEntry );

    /**
     * Adds package to database. Scans Sif db and adds entries to AppLib db.
     */
    void AddPackageL();

    /**
     * Updates package in the database.
     */
    void UpdatePackagesL();

    /**
     * Gets entries from AppLib story.
     * @param aArray Array of entries to be obtained from db.
     */
    void GetCaPackageEntriesL( RPointerArray<CCaInnerEntry>& aArray );

    /**
     * Gets entries from Usif story.
     * @param aArray Array of entries to be obtained from db.
     */
    void GetUsifPackageEntriesL( RPointerArray<CComponentEntry>& aArray );

    /**
     *  Checks if entry exists in AppLib db.
     *  @param aArray Array of entries from AppLib db.
     *  @param aEntry Entry data to be checked if exists.
     *  @retval Returns array index if exists, otherwise KErrNotFound.
     */
    TInt PackageExists( RPointerArray<CCaInnerEntry>& aArray,
            const CComponentEntry* aEntry );

    /**
     *  Finds deleted entries from Usif. Updates aCaArray.
     *  @param aCaArray Entries from AppLib db.
     *  @param aUsifArray Entries from Usif db.
     */
    void FindDeletedEntriesL( RPointerArray<CCaInnerEntry>& aCaArray,
            const RPointerArray<CComponentEntry>& aUsifArray );

    /**
     * Removes entries from db.
     * @param aCaArray Entries to be removed.
     */
    void RemoveEntriesFromDbL( RPointerArray<CCaInnerEntry>& aCaArray );

    /**
     * Update usif list.
     */
    void UpdateUsifListL();

private:
    /**
     * not own
     * Storage proxy.
     */
    CCaStorageProxy& iStorageProxy;

    /**
     * Install notifier. Own.
     */
    CCaInstallNotifier *iInstallNotifier;

    /**
     * File session. Own
     */
    RFs iFs;

    /**
     * Mmc watcher. Own.
     */
    CCaMmcWatcher* iMmcWatcher;

    /**
     * Software component registry. Not own.
     */
    RSoftwareComponentRegistry& iSoftwareRegistry;

    };

#endif // CAUSIFSCANNER_H
