/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextIndication class declaration.
*
*/


#ifndef C_CFCONTEXTINDICATION_H
#define C_CFCONTEXTINDICATION_H

#include <e32base.h>

class CCFContextObject;
class CCFContextDataObject;

/**
* Encapsulates one context indication.
* Context indication contains the related context object and a possible
* data object.
*
* @lib CFServices.lib
* @since S60 5.0
*/
NONSHARABLE_CLASS( CCFContextIndication ): public CBase
    {
    public:

        // Two phased constructors
        IMPORT_C static CCFContextIndication* NewL( const TUid& aUid );
        IMPORT_C static CCFContextIndication* NewLC( const TUid& aUid );
        
    public:
    
        /**
        * Returns context that has been changed.
        * 
        * @since S60 5.0
        * @param None.
        * @return const CCFContextObject&.
        */
        virtual const CCFContextObject& Context() const = 0;

        /**
        * Returns UID of the context data object associated into
        * this indication.
        * If the context indication does not have data associated to
        * KNullUid is returned.
        * 
        * @since S60 5.0
        * @param None.
        * @return const TUid&.
        */
        virtual const TUid& Uid() const = 0;

        /**
        * Creates a data object.
        * If indication has UID set for the data it has data associated to.
        * Client can create a correct data object and pass it as a parameter.
        * Data object will be internalized from the data stream.
        * If the indication does not have data associated to the method
        * leaves with code KErrArgument.
        * 
        * @since S60 5.0
        * @param aDataObject Data object associated with UID.
        * @return None.
        */
        virtual void CreateDataObjectL( CCFContextDataObject& aDataObject ) = 0;
    };

#endif
