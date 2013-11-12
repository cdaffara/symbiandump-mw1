/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  tbselementiterator.h
*
*/




#ifndef TBSELEMENTITERATOR_H
#define TBSELEMENTITERATOR_H

#include <mbselement.h>

// CLASS DECLARATION

/**
*
*
*  @lib
*  @since
*/

class MBSElement;


class TBSElementIterator
    {
    public: // New functions

		IMPORT_C MBSElement& Next();

        /**
        * C++ default constructor.
        */
        IMPORT_C TBSElementIterator( const TDesC& aElementId );

    private:
    	TInt iIndex;
    	const TDesC& iElementId;
    	};

#endif      // TBSELEMENTITERATOR_H

// End of File
