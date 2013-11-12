/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CFServer utils
*
*/


#ifndef CFSERVICEUTILS_H
#define CFSERVICEUTILS_H


#include <e32base.h>

class RWriteStream;
class RReadStream;

/**
* Context Framework Services utils class.
*/
NONSHARABLE_CLASS( CFServiceUtils )
    {
    public:
    
        /**
        * Updates buffer with new data.
        * Creates a new buffer if needed.
        * 
        * @since S60 5.0
        * @param aBuffer Buffer where to copy.
        * @param aNewData New data what to copy.
        * @return None.
        */
        static void UpdateBufferL( HBufC*& aBuffer, const TDesC& aNewData );

        /**
        * Writes descriptor into stream.
        * First is written descriptor length and then the descriptor itself.
        * 
        * @since S60 5.0
        * @param aDesc Descriptor to write into stream.
        * @param aStream Write stream to do the writing.
        * @return None.
        */
        static void WriteIntoStreamL( const HBufC* const aBuf, RWriteStream& aStream );

        /**
        * Read from stream into descriptor.
        * First is read descriptor length and then the descriptor itself.
        * 
        * @since S60 5.0
        * @param aDesc Descriptor to write into stream.
        * @param aStream Write stream to do the writing.
        * @return None.
        */
        static void ReadFromStreamL( HBufC*& aBuffer, RReadStream& aStream );
    };

#endif
