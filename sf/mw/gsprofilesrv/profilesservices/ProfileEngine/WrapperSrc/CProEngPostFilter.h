/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is used to filter all the files that are of a forbidden type from
*               the list returned by Content Listing Framework
*
*/



#ifndef CPROENGPOSTFILTER_H
#define CPROENGPOSTFILTER_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>
#include <MCLFPostFilter.h>

// FORWARD DECLARATIONS
class MCLFItem;
class CProEngDrmCommonWrapper;
class CProEngMediaVariation;

// CLASS DECLARATION

/**
* This class is used to filter all the files that are of a forbidden type from
* the list returned by Content Listing Framework.
*/
NONSHARABLE_CLASS( CProEngPostFilter )
    : public CBase,
      public MCLFPostFilter
    {
    public:  // Constructors and destructor

		/**
		 * Creates and returns a new instance of this class.
		 * @return Pointer to the CProEngPostFilter object
		 */
	    static CProEngPostFilter* NewL();

        /**
        * Destructor.
        */
        virtual ~CProEngPostFilter();

    private:    // Constructors
        /**
        * C++ default constructor.
        */
        CProEngPostFilter();

        /**
        * Symbian 2nd phase constructor.
        */
        void ConstructL();

    protected:  // Functions from base classes

        /**
        * From MCLFPostFilter. Method for filtering the source list. Will get 
        * called when post filtering operation is executed by calling refresh 
        * for the list model.
        * @since Series 60 3.0
        * @param aItemList Source item list
        * @param aFilteredItemList Filtered/destination item list
        */
        void FilterItemsL( const TArray<MCLFItem*>& aItemList,
                           RPointerArray<MCLFItem>& aFilteredItemList );

    private:  // Data

    /// Own: Needed to check whether a file is protected or not
    CProEngDrmCommonWrapper* iDrmWrapper;

    /// Own: Handles MIME-type checking
    CProEngMediaVariation* iMediaVariation;
    };

#endif // CPROENGPOSTFILTER_H

// End of File
