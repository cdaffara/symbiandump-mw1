/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFScriptInfo class declaration.
*
*/


#ifndef CFSCRIPTINFO_H
#define CFSCRIPTINFO_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// USER INCLUDE FILES

// FORWARD DECLARATIONS
class MCFScriptOwner;

// DATA TYPES

// CLASS DECLARATION

/**
 * Script information class.
 * Contains the basic information of the script.
 *
 * @lib None.
 * @since S60 5.0
 */
NONSHARABLE_CLASS( CCFScriptInfo ): public CBase
    {
public:

    /**
     * Symbian two phased constructors.
     *
     * @since S60 5.0
     * @param aName Script name.
     * @param aScriptId Script id.
     * @param aOwnerUid Script owner.
     * @param aLength Script length.
     * @param aScriptOwner Pointer to the owner session.
     * @return CCFScriptInfo*
     */
    static CCFScriptInfo* NewL( const TDesC& aName,
        TInt aScriptId,
        const TUid& aOwnerUid,
        TInt aLength,
        MCFScriptOwner* aScriptOwner );
    static CCFScriptInfo* NewLC( const TDesC& aName,
        TInt aScriptId,
        const TUid& aOwnerUid,
        TInt aLength,
        MCFScriptOwner* aScriptOwner );

    /**
     * C++ destructor.
     */
    virtual ~CCFScriptInfo( );

public:

    /**
     * Return script name.
     * @return TPtrC
     */
    TPtrC Name() const;
    
    /**
     * Returns script id.
     * @return TInt.
     */
    TInt Id() const;
    
    /**
     * Return owner uid.
     * @return const TUid&.
     */
    const TUid& OwnerUid() const;
    
    /**
     * Return script length.
     * @return TInt.
     */
    TInt Length() const;
    
    /**
     * Script owner session.
     * @return MCFScriptOwner*.
     */
    MCFScriptOwner* OwnerSession() const;
    
    /**
     * Change script owner session.
     * @param MCFScriptOwner*.
     */
    void SetOwnerSession( MCFScriptOwner* aScriptOwner );

private:

    CCFScriptInfo( TInt aScriptId,
        const TUid& aOwnerUid,
        TInt aLength,
        MCFScriptOwner* aScriptOwner );
    void ConstructL( const TDesC& aName );

private: // Data

    /** Script name */
    HBufC* iName;
    
    /** Script ID */
    TInt iScriptId;
    
    /** Script owner */
    TUid iOwnerUid;
    
    /** Script length */
    TInt iLength;
    
    /** Script owner session */
    MCFScriptOwner* iScriptOwner;
    };
    
// Info array typedef
typedef RPointerArray<CCFScriptInfo> RScriptInfoArray;

#endif // CFSCRIPTINFO_H

