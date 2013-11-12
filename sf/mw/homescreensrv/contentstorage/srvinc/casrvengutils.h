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
 * Description : The API supports item information that is not supported by
 *                the content arsenal server itself
 *
 */

#ifndef CASRVENGUTILS_H
#define CASRVENGUTILS_H

#include <e32base.h>
#include <apgcli.h>
/// Number of characters in UID and in MMC-id strings.
class TApaAppInfo;
class CCaInnerEntry;

NONSHARABLE_CLASS( CCaSrvEngUtils ): public CBase
    {
public:

    virtual ~CCaSrvEngUtils();
    static CCaSrvEngUtils* NewL();

    static TInt GetAppUid( const CCaInnerEntry& aEntry, TUid& aAppUid );

    // Caption support

    IMPORT_C TInt GetAppInfo( const CCaInnerEntry& aEntry,
            TApaAppInfo& aAppInfo ) const;

    IMPORT_C TInt GetApaAppInfo( TUid aUid, TApaAppInfo& aInfo ) const;

    /**
     * Fetches aEngObj native attribute value.
     * @param aEngObj engine object.
     * @param aAttrExists ETrue if attribute exist.
     * @param aAttrVal attribute value.
     */
    TBool IsNativeL( const CCaInnerEntry& aEntry );

private:

    CCaSrvEngUtils();

    void ConstructL();

    /**
     * Checks wether application is native
     * @since S60 v5.0.
     * @param aAppUid applications uid
     * @param aIsNative ETrue if apppication is native
     */
    void IsNativeL( TUid aAppUid, TBool& aIsNative );

private:
    // Data


    RApaLsSession iApaLsSession;
    };

#endif // CASRVENGUTILS_H
