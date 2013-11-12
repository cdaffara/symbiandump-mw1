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
* Description:  Codec to pack and unpack event objects.
*
*/

#ifndef XIMPEVENTCODEC_H
#define XIMPEVENTCODEC_H

#include <e32base.h>

class CXIMPApiEventBase;
class RReadStream;


/**
 * Codec to pack and unpack event objects.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( XIMPEventCodec )
    {
public:

    /**
     *
     *
     *
     * @param
     *
     * @return
     *
     */
    IMPORT_C static HBufC8* PackL( CXIMPApiEventBase& aEventObj,
                                   TInt32& aTypeOfEvent );
    IMPORT_C static void PackL( CXIMPApiEventBase& aEventObj,
                         TInt32& aTypeOfEvent,
                         CBufFlat* aBuffer );

    /**
     *
     *
     *
     * @param
     *
     * @return
     *
     */
    IMPORT_C static CXIMPApiEventBase* UnPackL( const TDesC8& aEventData,
                                                TInt32& aTypeOfEvent );


private:

    XIMPEventCodec();
    ~XIMPEventCodec();

    };


#endif // XIMPEVENTCODEC_H
