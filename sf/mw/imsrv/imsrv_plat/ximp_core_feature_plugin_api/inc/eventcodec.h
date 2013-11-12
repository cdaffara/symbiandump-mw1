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
* Description:  Header for MEventCodec
*
*/



#ifndef M_EVENTCODEC_H
#define M_EVENTCODEC_H

#include <e32std.h>

class CXIMPApiEventBase;


/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class MEventCodec
    {  
public:
    
    virtual HBufC8* PackL( CXIMPApiEventBase& aEventObj,
                           TInt32& aTypeOfEvent ) = 0;
    
    virtual void PackL( CXIMPApiEventBase& aEventObj,
                        TInt32& aTypeOfEvent,
                        CBufFlat* aBuffer ) = 0;

    virtual CXIMPApiEventBase* UnPackL( const TDesC8& aEventData,
                                        TInt32& aTypeOfEvent ) = 0;

    };

#endif // M_EVENTCODEC_H

