/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Document utils
*
*/

#ifndef CDOCUMENTUTILS_H
#define CDOCUMENTUTILS_H

#include <e32std.h>

class CPresenceInfoImp;
class CPresenceInfoFilterImp;

/**
 * Document utils
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
class DocumentUtils
    {

public:

    /**
     * Create union of two source info filters.
     * @param aSource1 First source filter
     * @param aSource2 Second source filter
     * @return New created filter. Ownership is transferred to caller.
     */
    IMPORT_C static CPresenceInfoFilterImp* InfoFilterUnionL(
                                                CPresenceInfoFilterImp& aSource1,
                                                CPresenceInfoFilterImp& aSource2 );
    /**
     * Create union of filters
     * @param aSources Array of sources for union.
     * @return New created filter. Ownership is transferred to caller.
     */
    IMPORT_C static CPresenceInfoFilterImp* InfoFilterUnionL(
                    const RPointerArray< CPresenceInfoFilterImp >& aSources );

    /**
     *
     */
    IMPORT_C static CPresenceInfoImp* CollectFilteredPresenceInfoLC(
                                                const CPresenceInfoFilterImp& aPif,
                                                const CPresenceInfoImp& aSource );
private:


    };


#endif // CDOCUMENTUTILS_H


