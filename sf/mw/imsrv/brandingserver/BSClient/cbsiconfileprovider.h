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
* Description:   Icon file provider.
 *
*/




#ifndef CBSICONFILEPROVIDER_H
#define CBSICONFILEPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <AknsItemID.h>
#include <AknIconUtils.h>


// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 *  Icon file provider.
 *  See MAknIconFileProvider.h for comments.
 *
 *  @lib chat.app
 *  @since 3.0
 */
class CBSIconFileProvider : public CBase, public MAknIconFileProvider
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CBSIconFileProvider* NewL( RFile& aFile );
        /**
         * Destructor.
         */
        virtual ~CBSIconFileProvider();

    public: // Functions from base classes

        /**
         * @see MAknIconFileProvider
         */
        void RetrieveIconFileHandleL(
                RFile& aFile, const TIconFileType aType );

        /**
         * @see MAknIconFileProvider
         */
        void Finished();

    private:

        /**
         * C++ default constructor.
         */
        CBSIconFileProvider();
        
        /**
         * By default Symbian 2nd phase constructor is private.
         */
		void ConstructL(RFile& aFile);

    private:    // Data

        // owned. filename of this icon. must be a copy,
        // a reference could end up pointing into another file.
        HBufC* iFilename;
        
        RFile iFile;
    };

#endif      // CBSICONFILEPROVIDER_H

// End of File
