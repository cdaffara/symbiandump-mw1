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

#include <e32base.h>
#include <s32mem.h>

#include "casrvdef.h"

class RWriteStream;
class RReadStream;
class CCaInnerEntry;
class RIdsArray;

NONSHARABLE_CLASS( MenuUtils )
    {
public:
    /**
     * Gets TUint from a aStr - HEX or Decimal
     *
     * @return error code
     */
    IMPORT_C static TInt GetTUint( const TDesC& aStr, TUint& aInt );
    
    /**
     * Gets TUint from a aStr - HEX or Decimal
     */
    IMPORT_C static void InitTextResolverSymbianL( const TDesC& aFilename );

    /**
     * Template method for marshaling data.
     * @param aObject object supporting ExternalizeL method
     * @param aExpandSize granularity of the buffer
     * @return descriptor containing serialized object
     */
    template<typename SerializableObject>
    static HBufC8* MarshalDataL( const SerializableObject& aObject,
            TInt aExpandSize )
        {
        TIpcArgs args;
        CBufFlat* buf = CBufFlat::NewL( aExpandSize );
        CleanupStack::PushL( buf );
        RBufWriteStream stream( *buf );
        CleanupClosePushL( stream );
        aObject.ExternalizeL( stream );
        stream.CommitL();
        CleanupStack::PopAndDestroy( &stream );
        HBufC8* des = HBufC8::NewL( buf->Size() );
        TPtr8 ptr( des->Des() );
        buf->Read( 0, ptr, buf->Size() );
        CleanupStack::PopAndDestroy( buf );
        return des;
        }

    /**
     * Internalizes object from descriptor which is stored in aMessage
     * @param aMessage message containing descriptor with serialized object
     * @param aObject an object supporting InternalizeL method
     */
    template<typename SerializableObject>
    static void InternalizeObjectL( const RMessage2& aMessage,
            SerializableObject& aObject )
        {
        TInt deslen = aMessage.GetDesLength( KInputPosition1 );
        HBufC8* buffer = HBufC8::NewLC( deslen );
        TPtr8 tempDes = buffer->Des();
        aMessage.Read( KInputPosition1, tempDes );
        RDesReadStream datastrm( *buffer );
        CleanupClosePushL( datastrm );
        aObject.InternalizeL( datastrm );
        CleanupStack::PopAndDestroy( &datastrm );
        CleanupStack::PopAndDestroy( buffer );
        }
    };
