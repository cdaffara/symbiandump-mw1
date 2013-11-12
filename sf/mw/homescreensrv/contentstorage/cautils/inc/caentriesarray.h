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

#ifndef __CAENTRIESARRAY_H__
#define __CAENTRIESARRAY_H__

#include <e32base.h>

//FORWARD DECLARATIONS
class RWriteStream;
class RReadStream;
class CCaInnerEntry;
/**
 * Entries array.
 *  @lib camenu.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( RCaEntriesArray ): public RPointerArray<CCaInnerEntry>
    {
public:

    /**
     * Externalizes array to the stream
     * @param aStream a write stream
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;

    /**
     * Internalizes stream to the array
     * @param aStream a read stream
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );

    };

#endif // __CAENTRIESARRAY_H__
