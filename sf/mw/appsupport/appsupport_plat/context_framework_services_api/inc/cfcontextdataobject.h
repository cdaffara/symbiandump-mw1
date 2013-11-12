/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextDataObject class declaration.
*
*/


#ifndef C_CFCONTEXTDATAOBJECT_H
#define C_CFCONTEXTDATAOBJECT_H

#include <e32base.h>

class RReadStream;
class RWriteStream;

/**
* Abstract context data object class which encapsulates
* inside data which is associated with a specific context object.
* Context data object can be received as a part of context
* indication.
* 
* @lib -
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextDataObject ): public CBase
    {
    public:
    
        /**
        * Unique UID of the context data object.
        * 
        * @since S60 5.0
        * @param None.
		* @return const TUid&
        */
        virtual const TUid& Uid() const = 0;

        /**
        * Externalize context data object into stream.
        * 
        * @since S60 5.0
        * @param aStream Write stream.
		* @return None.
        */
        virtual void ExternalizeL( RWriteStream& aStream ) = 0;

        /**
        * Internalizes context data object from stream.
        * 
        * @since S60 5.0
        * @param aStream Read stream.
		* @return None.
        */
        virtual void InternalizeL( RReadStream& aStream ) = 0;

        /**
        * Returns size of the data object in bytes.
        * Size must represent the real size of the object
        * since Context Framework uses this size when creating
        * a buffer for the object.
        * 
        * @since S60 5.0
        * @param None.
		* @return TInt.
        */
        virtual TInt Size() const = 0;
    };
    
#endif
