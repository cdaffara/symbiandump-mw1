/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Helper class to handle buffers in fifo
*
*/


#ifndef CCMRFIFO_H
#define CCMRFIFO_H

//  INCLUDES
#include <e32base.h>
#include <e32std.h>

// FORWARD DECLARATION
class CCMRFifoItem;

// CLASS DECLARATION

/**
*  Fifo, uses TSglQue to implement a singly-linked list
*
*  @lib camcdevvr.lib
*  @since 2.1
*/
class CCMRFifo : public CBase
    {

    public:
        
        /**
        * C++ default constructor.
        */
        CCMRFifo();
        
        /**
        * Two-phased constructor.
        */
        static CCMRFifo* NewL(TInt aNumItems);
        
        /**
        * Destructor.
        */
        virtual ~CCMRFifo();
        
    public:  // New functions
        
        /**
        * Adds an element to the end of the fifo
        * @since 2.1
        * @param TAny* aData    Pointer to data to be added
        * @return void
        */
        void PutL(TAny* aData);
        
        /**
        * Returns and remove the first element in fifo
        * @since 2.1
        * @return TAny*     Pointer to first element
        */
        TAny* Get(); 
        
      
        /**
        * Queries whether fifo is empty    
        * @since 2.1
        * @return TBool     ETrue if fifo is empty
        */
        TBool IsEmpty();

        /**
        * Return the size of the fifo (number of items stored)
        * @since 2.1
        * @return TInt      Number of items stored in fifo
        */
        TInt NumberOfItems();
        
    private: // Constructor

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL(TInt aNumItems);

    private:    // Data
        
        // the actual fifo for stored data items
        TSglQue<CCMRFifoItem> iFifo;
        // iterator for the actual fifo
        TSglQueIter<CCMRFifoItem> iIter;
        // fifo for empty items
        TSglQue<CCMRFifoItem> iFifoEmpty;
        // iterator for the empty-items fifo
        TSglQueIter<CCMRFifoItem> iIterEmpty;

        // the size of the fifo
        TInt iNumDataItemsStored;

    };

#endif // CCMRFIFO_H

//  End of File
