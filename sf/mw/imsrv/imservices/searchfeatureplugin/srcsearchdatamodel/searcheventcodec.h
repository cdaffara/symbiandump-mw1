/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SEARCHEVENTCODEC_H
#define SEARCHEVENTCODEC_H

#include <e32base.h>

#include "eventcodec.h"

class CXIMPApiEventBase;
class RReadStream;


/**
 * Codec to pack and unpack event objects.
 *
 * @lib searchdatamodel.dll
 */
NONSHARABLE_CLASS( CSearchEventCodec ): public MEventCodec
    {
    
public:
    static CSearchEventCodec* NewL();
    
    ~CSearchEventCodec();
    
    
    /**
     * packs and allocates memory for the  
     * event object and event type
     *
     * @param aEventObj  
     * event object derived from CXIMPApiEventBase
     * 
     * @param aTypeOfEvent 
     * holds  type of the event after execution from aEventObj
     * 
     * @return
     * heap allocated which contains both
     * event and event type
     */
    HBufC8* PackL( CXIMPApiEventBase& aEventObj,
                                   TInt32& aTypeOfEvent );
                                   
    /**
     * packs  
     * event object and event type into the buffer
     *
     * @param aEventObj  
     * event object derived from CXIMPApiEventBase
     * 
     * @param aTypeOfEvent 
     * holds  type of the event after execution from aEventObj 
     * 
     * @param
     * buffer to be packed  with
     * event and event type
     */                               
                                   
    void PackL( CXIMPApiEventBase& aEventObj,
                         TInt32& aTypeOfEvent,
                         CBufFlat* aBuffer );

    /**
     *
     * @param aEventData
     * data desc which has the event type and id
     *
     * @param aTypeOfEvent 
     * holds  type of the event after execution from aEventData 
     *
     * @return new eventbase object constructed from aEventData
     *
     */
    CXIMPApiEventBase* UnPackL( const TDesC8& aEventData,
                                                TInt32& aTypeOfEvent );
protected:
    void ConstructL();

private:

    CSearchEventCodec();


    };


#endif // SEARCHEVENTCODEC_H
