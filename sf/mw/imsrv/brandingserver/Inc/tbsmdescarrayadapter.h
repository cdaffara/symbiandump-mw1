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
* Description:   One value adapter for MDesCArray.
*
*/


#ifndef TBSMDESCARRAYADAPTER_H__
#define TBSMDESCARRAYADAPTER_H__

//  INCLUDES
#include <E32Std.h>
#include <BaDesCa.h>


// CLASS DECLARATION
/**
 * One value adapter for MDesCArray.
 *
 * @since 3.0
 */
class TBSMDesCArrayAdapter : public MDesCArray
    {
    public: //Constructor & destructor

        /**
         * C++ constructor.
         * Takes the descriptor to adapt.
         */
        IMPORT_C TBSMDesCArrayAdapter( const TDesC& aDesc );


    public: //From MDesCArray

        IMPORT_C TInt MdcaCount() const;
        IMPORT_C TPtrC MdcaPoint( TInt aIndex ) const;


    private: //Data

        //OWN: Adapted descriptor
        TPtrC   iDesc;
    };


#endif      //TBSMDESCARRAYADAPTER_H__

//  End of File




