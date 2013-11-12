/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SysAp drive list implementation
 *
*/


#ifndef SYSAPDRIVELIST_H
#define SYSAPDRIVELIST_H

// INCLUDES
#include <e32std.h>
#include "SysAp.hrh"

// FORWARD DECLARATIONS
class RFs;

// CLASS DECLARATION
class CSysApDriveList : public CBase
    {
public:

    /**
     * Two-phased constructor.
     */
    static CSysApDriveList* NewL( RFs& aFs );

    /**
     * Destructor
     */
    ~CSysApDriveList();

public: // New methods

    /**
    * Mounts drive
    *
    * @param aDrive Drive to mount
    * @return System wide error code
    */
    TInt MountDrive( TInt aDrive );

    /**
    * Dismounts drive
    *
    * @param aDrive Drive to dismount
    * @return System wide error code
    */
    TInt DismountDrive( TInt aDrive );

    /**
     * Updates drive state stored in the list
     *
     * @param aDrive Drive to update
     * @param aForceUpdate ETrue if forced update. Otherwise EFalse.
     */
    void UpdateDrive( TInt aDrive, TBool aForceUpdate );

    /**
     * Returns the status of the memory card
     *
     * @param aDrive Drive what status to get
     * @return The memory card status
     */
    TSysApMemoryCardStatus MemoryCardStatus( TInt aDrive );

    /**
     * Returns the drive id of default memory card
     */
    TInt DefaultMemoryCard() const;

    // For entry data used by GetMemoryCardsL
    class TMemoryCardEntry
        {        
        public:
            /**
             * Compare for RArray::Find()
             */
            static TInt CompareFind(
                const TInt* aDrive,
                const TMemoryCardEntry& aEntry );

        public:
            TInt iDrive;
            TSysApMemoryCardStatus iStatus;
            TUint iDriveStatus;
        };
    
    // For inclusition mask used by GetMemoryCardsL
    enum TMemoryCardInclusionMask
        {
        EIncludeAll = 0x0,
        EIncludeInserted = 0x1,
        EIncludeNonInserted = 0x2,
        EIncludeLocked = 0x4
        };

    /**
     * Gets the memory cards with requested status
     *
     * @param aDriveArray Array to store memory cards
     * @param aInclusionMask Memory card inclusion mask 
     *                       specified by TMemoryCardInclusionMask
     */
    void GetMemoryCardsL(
        RArray< TMemoryCardEntry >& aDriveArray,
        TUint aInclusionMask );

    /**
     * Gets the drive name with text formatting
     *
     * @param aDrive Drive that name is requested
     * @param aTextIdForDefaultName Text id containing %U
     * @param aTextIdForName Text id containing %0U and %1U
     * @return Pointer to allocated text
     */
    HBufC* GetFormattedDriveNameLC(
        const TInt aDrive,
        const TInt aTextIdForDefaultName,
        const TInt aTextIdForName = 0 ) const;

    /**
     * Returns drive index from given array or KErrNotFound if not found
     */
    static TInt Find(
        const RArray< TMemoryCardEntry >& aDriveArray,
        TInt aDrive );

    // Drive eject type used in MarkDriveToEject
    enum TDriveEjectType
        {
        EEjectNone = 0,
        EEjectFromMenu,
        EEjectRemovedWithoutEject
        };
    /**
     * Marks drive to be ejected
     */
    void MarkDriveToEject( TInt aDrive, TDriveEjectType aEjectUsed );

    /**
     * Resets drive marked to be ejected
     */
    void ResetDriveToEject( TInt aDrive );

    /**
     * Resets all drives marked to be ejected
     */
    void ResetDrivesToEject();

    /**
     * Gets next drive to be ejected, returns KErrNotFound if none marked
     */
    TInt DriveToEject( TDriveEjectType& aEjectUsed );

    /**
     * Checks is drive marked to be ejected
     */
    TBool IsDriveToEject( TInt aDrive );

    /**
     * Marks drive insert beeps to be ignored
     */
    void MarkDrivesInsertBeepIgnored( const RArray< TMemoryCardEntry >& aArray );

    /**
     * Mark drive insert beep to be ignored
     */
    void MarkDriveInsertBeepIgnored( TInt aDrive );

    /**
     * Resets drive insert beep marked to be ignored
     */
    void ResetDriveInsertBeepIgnored( TInt aDrive );

    /**
     * Resets all drives marked inserts beeps
     */
    void ResetDrivesInsertBeepIgnored();

    /**
     * Checks is insert beep ignored
     */
    TBool IsDriveInsertBeepIgnored( TInt aDrive );

    /**
     * Marks locked drive's unlock query status as shown
     */
    void MarkDriveUnlockQueryShown( TInt aDrive );

    /**
     * Resets locked drive's unlock query shown status
     */
    void ResetDriveUnlockQueryShown( TInt aDrive );

    /**
     * Gets next locked drive to be unlocked. Returns KErrNotFound if none.
     * The locked drives where unlock query has been shown are ignored.
     */
    TInt DriveToUnlock();

    /**
     * Returns number of non USB drives in the array
     */
    static TInt NonUsbDriveCount( const RArray< TMemoryCardEntry >& aDriveArray );

private:

    /**
    * Constructors.
    */
    CSysApDriveList( RFs& aFs );

    void ConstructL();

    // Used internally for drive states
    enum TDriveState
        {
        EStateStatusUpdated = 0x1,
        EStateEjectedFromMenu = 0x2,
        EStateRemovedWithoutEject = 0x4,
        EStateBeepIgnored = 0x8,
        EStateUnlockQueryShown = 0x10,
        EStateAllEjected = EStateEjectedFromMenu | EStateRemovedWithoutEject
        };

    // Used internally for drive status caching
    class TDriveStateEntry
        {
        public:
            TSysApMemoryCardStatus iStatus;
            TUint iState;
            TUint iDriveStatus;
        };

private: // Data
    // Reference to open file server session
    RFs& iFs;

    // The default memory card drive
    TInt iDefaultMemoryCard;

    // Array for drive state caching
    TFixedArray< TDriveStateEntry, KMaxDrives > iStatusCache;

    };

#endif // SYSAPDRIVELIST_H

// End of File
