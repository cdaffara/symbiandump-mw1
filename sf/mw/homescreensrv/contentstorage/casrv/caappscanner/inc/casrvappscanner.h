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
* Description: casrvappscanner.h
*
*/

#ifndef CASRVAPPSCANNER_H_
#define CASRVAPPSCANNER_H_

#include <e32base.h>
#include <apgcli.h>
#include <apgnotif.h>
#include <driveinfo.h>
#include <usif/scr/scr.h>
#include "cainstallnotifier.h"
#include "castorage_global.h"

class CCaSrvMmcHistory;
class CCaInnerEntry;
class CCaStorageProxy;
class CCaSrvEngUtils;


/**
 * Application scanner.
 * Using AppArc application information, keeps menu application
 * items up-to-date.
 *
 * Operation is asynchronous (because AppArc startup is asynchronous):
 * 1. ConstructL(): invokes iApaLsSession to build app list (async).
 * 2. RunL(): App list is complete - appscanner update entries with application.
 * 4. HandleAppListEvent(): change in app list, go back to step 2.
 *
 * App scanning does the following:
 * - Applications present in the system but not appearing in the menu are added
 *   to the appropriate folder.
 * - Hide hidden applications, show non-hidden applications (flag "hidden").
 * - Hide / show MMC-related items, according to whether MMC is present or not
 *   (flag "missing"). Keep an MMC history, purge old MMC-based menu items
 *   (== missing menu items whose MMC card has not been seen for long).
 * - Observing AppArc (and MMC, etc etc) to keep the menu data up
 *   to date continuously.
 */

NONSHARABLE_CLASS( CCaSrvAppScanner ) : public CActive
    {

public:
    // Constructor

    /**
     * Destructor.
     */
    virtual ~CCaSrvAppScanner();

    /**
     * Two-phased constructor.
     * @param aCaStorageProxy storage.
     * @param aSoftwareRegistry software component registry.
     * @param aUtils storage utils.
     */
    static CCaSrvAppScanner* NewL( CCaStorageProxy& aCaStorageProxy,
            Usif::RSoftwareComponentRegistry& aSoftwareRegistry,
            CCaSrvEngUtils& aUtils );

private:
    // Constructors and destructor

    /**
     * Constructor.
     * @param aCaStorageProxy storage.
     * @param aSoftwareRegistry software component registry.
     * @param aUtils storage utils.
     */
    CCaSrvAppScanner( CCaStorageProxy& aCaStorageProxy,
            Usif::RSoftwareComponentRegistry& aSoftwareRegistry,
            CCaSrvEngUtils& aUtils );

    /**
     * Second phased constructor.
     */
    void ConstructL();

private:
    // from CActive

    /**
     * Request completed, schedule engine operation.
     */
    void RunL();

    /**
     * Cancel request.
     */
    void DoCancel();

    /**
     * Error handling: ignore error.
     * @param aError Error code.
     */
    TInt RunError( TInt aError );

private:
    // new methods

    /**
     * Gets collectionId of download collection.
     * @result id of download collection.
     */
    TInt GetCollectionDownloadIdL();

    /**
     * Gets collectionId of all collection.
     * @result id of all collection.
     */
    TInt GetAllCollectionIdL();

    /**
     * Adds application entry to downloaded collection.
     * @param aEntryId application entry id.
     */
    void AddEntryToDownloadedCollectionL( TInt aEntryId );

    /**
     * Add application entry to predefined collection.
     * @param aEntry application entry.
     * @param aUpdate is entry updated by a client.
     */
    void AddEntryToPredefinedCollectionL( CCaInnerEntry* aEntry,
            TBool aUpdate = EFalse );

    /**
     * Create predefined collection.
     * @param aGroupName group name.
     * @result id of a newly created collection.
     */
    TInt CreatePredefinedCollectionL( const TDesC& aGroupName );

    /**
     * Add collection to all collection.
     * @param aCollectionId collection id to add.
     */
    void AddCollectionToAllCollectionL( TInt aCollectionId );

    /**
     * Removes application entry from downloaded collection.
     * @param aEntryId application entry id.
     */
    void RemoveEntryFromDownloadedL( TInt aEntryId );

    /**
     * Updates application's removable and visible flags.
     * @param aItem menu item.
     * @param aMmcId MMC ID of currently inserted MMC, or 0.
     * @return ETrue if flags were updated.
     */
    TBool HandleRemovableFlagAndMmcAttrUpdateL( CCaInnerEntry* aItem, TUint aMmcId );

    /**
     * Removes application unninstall flag.
     * @param aItem menu item.
     * @return ETrue if flags were updated.
     */
    TBool RemoveUninstallFlagL( CCaInnerEntry* aItem );

    /**
     * Updates application's missing and visible flags.
     * @param aItem menu item.
     * @return ETrue if flags were updated.
     */
    TBool HandleMissingFlagsUpdate( CCaInnerEntry* aItem );

    /**
     * Updates visible flag.
     * @param aItem menu item.
     * @return ETrue if visible flag was updated.
     */
    TBool HandleVisibleFlagUpdate( CCaInnerEntry* aItem );

    /**
     * Updates used flag.
     * @param aItem menu item.
     * @return ETrue if used flag was updated.
     */
    TBool HandleUsedFlagUpdate( CCaInnerEntry* aItem );

    /**
     * Updates mmc attribute.
     * @param aItem menu item.
     * @param aMmcId MMC ID of currently inserted MMC, or 0.
     * @return ETrue if mmc attribute was updated.
     */
    TBool HandleMmcAttrUpdateL( const Usif::CComponentEntry* aEntry, 
            CCaInnerEntry* aItem, TUint aMmcId );

    /**
     * Get applications from AppArc.
     * @param aArray RArray with applications' uids.
     */
    void GetApaItemsL( RArray<TUint>& aArray );

    /**
     * Remove application from array (found in AppArc).
     * @param aArray RArray with applications' uids.
     * @param aUid uid of application to remove.
     */
    void RemoveApp( RArray<TUint>& aArray, TInt32 aUid );

    /**
     * Get applications from CaStorage.
     * @param aArray RPointerArray with CCaInnerEntries.
     * contains applications.
     */
    void GetCaAppEntriesL( RPointerArray<CCaInnerEntry>& aArray );

    /**
     * Remove sat application from array(found in CaStorage).
     * @param aArray RPointerArray with CCaInnerEntries
     * contains applications.
     */
    void RemoveSatAppL( RPointerArray<CCaInnerEntry>& aArray );

    /**
     * Ensure that HsApplicaiton is not visible: Add HsApplication.
     * as hidden to CaStorage or remove HsApplication entry
     * from array (found in CaStorage).
     * @param aArray RPointerArray with CCaInnerEntries
     * contains application.
     */
    void HandleHsAppEntryL( RPointerArray<CCaInnerEntry>& aArray );

    /**
     * Get application with specified uid from CaStorage.
     * @param aUid RPointerArray with CCaInnerEntries.
     * @param aArray RPointerArray with CCaInnerEntries
     * contains application.
     */
    void GetCaAppEntriesL( TInt aUid, RPointerArray<CCaInnerEntry>& aArray );

    /**
     * New application is found, no menu item refers to it.
     * Add new menu item referring to this application.
     * @param aUid Application capability.
     * @param aCurrentMmcId MMC ID of currently inserted MMC, or 0.
     * @return added item id
     */
    TInt AddAppEntryL( TUint aUid, TUint aCurrentMmcId );

    /**
     * Set CWRT ettributes to entry.
     * @param aEntry CCaInnerEntry with application.
     * @return ETrue if app info was updatedd
     */
    TBool SetCWRTAppL( CCaInnerEntry* aEntry );

    /**
     * Set Java attributes to entry.
     * @param aEntry CCaInnerEntry with application.
     * @return ETrue if app info was updatedd
     */
    TBool SetJavaAppL( CCaInnerEntry* aEntry );

    /**
     * Set information from TApaAppInfo to entry.
     * @param aEntry CCaInnerEntry with application.
     * @param info application informations from AppArc.
     * @return ETrue if app info was updatedd
     */
    TBool SetAppCaptionL( CCaInnerEntry* aEntry, TApaAppInfo* info );

    /**
     * Gets component id from entry.
     * @param aEntry CCaInnerEntry with application.
     * @return component id
     */
    TInt GetComponentIdFromEntryL( CCaInnerEntry* aEntry );

    /**
     * Sets new attribute value.
     * @param aEntry CCaInnerEntry with application.
     * @param aKey attribute name.
     * @param aValue attribute value.
     * @return ETrue if attribute was updated or added
     */
    TBool AddAttributeL(
            CCaInnerEntry* aEntry,
            const TDesC& aKey,
            const TDesC& aValue );
    
    /**
     * Removes attribute from entry.
     * @param aEntry CCaInnerEntry with application.
     * @param aKey attribute name.
     * @return ETrue if attribute was removed
     */
    TBool RemoveAttributeL( CCaInnerEntry* aEntry, const TDesC& aKey );

    /**
     * Set information from TApaAppInfo to entry.
     * @param aEntry CCaInnerEntry with application.
     * @return ETrue if app info was updatedd
     */
    TBool SetApaAppInfoL( CCaInnerEntry* aEntry );

    /**
     * Handle items which are in the menu but no corresponding app is present:
     * - Items that are on a recently used MMC get "missing" flag but kept.
     * - Items that are not on MMC or the MMC is not recently used, are
     *   removed from the menu.
     * @param aItems Missing items.
     */
    void HandleMissingItemsL( RPointerArray<CCaInnerEntry>& aCaEnties );

    /**
     * Set "missing" flag.
     * @param aEntry entry.
     */
    void SetMissingFlagL( CCaInnerEntry* aEntry );

    /**
     * Clear all flags.
     * @param aEntry entry.
     */
    void ClearAllFlagsL( CCaInnerEntry* aEntry );

    /**
     * Check currently inserted MMC card, update and save MMC history.
     * @return Current MMC id, or 0 if no MMC is inserted.
     */
    TUint UpdateMmcHistoryL();

    /**
     * Get current MMC id. 0 if no MMC is inserted, or in case of any errors.
     * @return Current MMC id or 0.
     */
    TUint CurrentMmcId() const;
    
    /**
     * Check if drive letter is on a given default drive type.
     * @param aDriveLetter drive letter .
     * @param aDefaultDrive default drive type
     * @return ETrue if aFileName is on a given default drive type.
     */
    TBool IsCharInDrive( const TChar& aDriveLetter,
            const DriveInfo::TDefaultDrives& aDefaultDrive ) const;

    /**
     * Check if drive's status is EDriveInUse.
     * @param aDefaultDrive default drive type.
     * @return ETrue if drive is in use.
     */
    TBool IsDriveInUse( const DriveInfo::TDefaultDrives& aDefaultDrive );

    /**
     * Add application to storage or update entry if necessary.
     * @param aCaEnties with applications from storage.
     * @param aAppUid application uid.
     * @param aMmcId unique mmc id.
     */
    void UpdateApplicationEntryL( RPointerArray<CCaInnerEntry>& aCaEnties,
            TUint aAppUid, TUint aMmcId );

    /**
     * Update entry from storage.
     * @param aEntry application entry.
     * @param aMmcId unique mmc id.
     * @param aAlwaysUpdate item always should be updated.
     */
    void UpdateAppEntryL( CCaInnerEntry* aEntry,
            TUint aMmcId,
            TBool aAlwaysUpdate = EFalse );

    /**
     * Get all applications from storage and visible from apparc
     * and update if necessary.
     */
    void UpdateApplicationEntriesL();

    /**
     * Make not empty collections with not hidden apps visible.
     */
    void MakeNotEmptyCollectionsVisibleL();

    /**
     * Make collection visible if has visible entry.
     * @param aEntry application entry.
     */
    void MakeCollectionVisibleIfHasVisibleEntryL( CCaInnerEntry* aEntry );

    /**
     * Adds or updates component id attribute in entry based on SCR provided data
     * @param aEntry entry being updated with component id attribute.
     * @return ETrue when component id attribute of the entry has been added or changed.
     */
    TBool UpdateComponentIdL( CCaInnerEntry& aEntry );

private:
    // data

    RApaLsSession iApaLsSession; ///< AppArc session. Own.
    CCaSrvMmcHistory* iMmcHistory; ///< MMC history. Own.
    RFs iFs; ///< File Server Session. Own.
    
    CCaStorageProxy& iCaStorageProxy; ///< Not own
    Usif::RSoftwareComponentRegistry& iSoftwareRegistry; ///< Not own
    CCaSrvEngUtils& iSrvEngUtils; ///< Not own

    TInt iCollectionDownloadId;
    TInt iAllCollectionId;

CA_STORAGE_TEST_FRIEND_CLASS    (T_casrvAppScaner)

    };

#endif /* CASRVAPPSCANNER_H_ */
