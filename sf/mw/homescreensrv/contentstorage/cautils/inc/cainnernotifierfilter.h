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

#ifndef CAINNERNOTIFIERFILTER_H
#define CAINNERNOTIFIERFILTER_H

#include <e32base.h>
#include "cadef.h"

class CDesC16ArrayFlat;
class RWriteStream;
class RReadStream;

/**
 * Class represents Notifier filter
 */
NONSHARABLE_CLASS( CCaInnerNotifierFilter ): public CBase
    {
public:

    enum TNotifierType
        {
        EEntryChangedWithId = 0,
        EEntryChangedWithEntry,
        EEntryTouched,
        EGroupContentChanged
        };

    /**
     * Destructor.
     */
    virtual ~CCaInnerNotifierFilter();

    /**
     * Two-phased constructor.
     * @param aInnerNotifierType inner type of notifier.
     * @return The created object.
     */
    IMPORT_C static CCaInnerNotifierFilter* NewL(
        TNotifierType aInnerNotifierType );

    /**
     * Two-phased constructor.
     * @param aInnerNotifierType inner type of notifier.
     * @return The created object.
     */
    IMPORT_C static CCaInnerNotifierFilter* NewLC(
        TNotifierType aInnerNotifierType );

    /**
     *
     */
    IMPORT_C TInt GetParentId() const;
    /**
     *
     */
    IMPORT_C void SetParentId( TInt aParentId );
    /**
     *
     */
    IMPORT_C RArray<TInt> GetIds() const;
    /**
     *
     */
    IMPORT_C void SetIdsL( const RArray<TInt>& aIds );
    /**
     *
     */
    IMPORT_C TEntryRole GetEntryRole() const;
    /**
     *
     */
    IMPORT_C void SetEntryRole( TEntryRole aEntryRole );
    /**
     *
     */
    IMPORT_C CDesC16ArrayFlat* GetTypeNames() const;
    /**
     *
     */
    IMPORT_C void SetTypeNames( CDesC16ArrayFlat * aTypeNames );
    /**
     *
     */
    IMPORT_C TNotifierType GetNotifierType() const;

    /**
     *
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

    /**
     *
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

private:

    /**
     * Second phased constructor.
     */
    void ConstructL();

    /**
     * Constructor
     */
    CCaInnerNotifierFilter( TNotifierType aInnerNotifierType );

private:

    /**
     * Unique identifier of parent. Own
     */
    TInt iParentId;
    /**
     * Unique identifier of entries. Own
     */
    RArray<TInt> iIds;
    /**
     *
     */
    TEntryRole iEntryRole;
    /**
     * Own
     */
    CDesC16ArrayFlat* iTypeNames;
    /**
     * Notifier command
     */
    TNotifierType iNotifierType;

    };

#endif // CAINNERNOTIFIERFILTER_H
