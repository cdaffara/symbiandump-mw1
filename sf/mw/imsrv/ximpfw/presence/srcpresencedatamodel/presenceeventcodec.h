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

#ifndef PRESENCEEVENTCODEC_H
#define PRESENCEEVENTCODEC_H

#include <e32base.h>

#include "eventcodec.h"
//#include "ximpapieventbase.h"
class CXIMPApiEventBase;
class RReadStream;


/**
 * Codec to pack and unpack event objects.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceEventCodec ): public MEventCodec
    {
public:

    static CPresenceEventCodec* NewL();
    
    ~CPresenceEventCodec();
    
    
    /**
     *
     *
     *
     * @param
     *
     * @return
     *
     */
    HBufC8* PackL( CXIMPApiEventBase& aEventObj,
                                   TInt32& aTypeOfEvent );
    void PackL( CXIMPApiEventBase& aEventObj,
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
    CXIMPApiEventBase* UnPackL( const TDesC8& aEventData,
                                                TInt32& aTypeOfEvent );
protected:
    void ConstructL();

private:

    CPresenceEventCodec();


    };


#endif // PRESENCEEVENTCODEC_H
