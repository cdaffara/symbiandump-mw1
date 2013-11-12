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


// INCLUDE FILES
#include "CCMRFifo.h"

// CLASS DECLARATION

/**
*  Fifo item, internal class used only by class CCMRFifo
*  @lib camcmediarecorder.lib
*  @since 2.1
*/
class CCMRFifoItem : public CBase
    {
    
    public:
        
        /**
        * C++ default constructor.
        */
        CCMRFifoItem();
        
        /**
        * Two-phased constructor.
        */
        static CCMRFifoItem* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CCMRFifoItem();
        
    public:  // New functions
        
        /**
        * Return the data pointer
        * @since 2.1
        * @return TAny*     Reference to data
        */
        const TAny* Data() const;

        /**
        * Return the data pointer
        * @since 2.1
        * @param TAny* 
        */
        void SetData(const TAny* aData);

    private:    // Data
    
        TSglQueLink     iNext;   // link to next element
        const TAny* iData;   // pointer to stored data

    private:  // Friend classes
        friend class CCMRFifo;
        
    };

// ================= MEMBER FUNCTIONS =======================

// CCMRFifoItem

// ---------------------------------------------------------
// CCMRFifoItem::CCMRFifoItem
// Default constructor
// ---------------------------------------------------------
//
CCMRFifoItem::CCMRFifoItem()
    {
    }

// ---------------------------------------------------------
// CCMRFifoItem::NewL
// "Two-phased" constructor
// ---------------------------------------------------------
//
CCMRFifoItem* CCMRFifoItem::NewL()
    {
    CCMRFifoItem* self = new (ELeave) CCMRFifoItem();
    return self;
    }

// ---------------------------------------------------------
// CCMRFifoItem::~CCMRFifoItem
// Destructor
// ---------------------------------------------------------
//
CCMRFifoItem::~CCMRFifoItem()
    {
    iData = NULL;
    }

// ---------------------------------------------------------
// CCMRFifoItem::Data()
// Returns the data pointer
// (other items were commented in a header).
// ---------------------------------------------------------
//
const TAny* CCMRFifoItem::Data() const
    {
    return iData;
    }

// ---------------------------------------------------------
// CCMRFifoItem::SetData()
// Sets the data pointer
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRFifoItem::SetData(const TAny* aData)
    {
    iData = aData;
    }




// CCMRFifo

// ---------------------------------------------------------
// CCMRFifo::CCMRFifo
// Default constructor
// ---------------------------------------------------------
//
CCMRFifo::CCMRFifo() : 
    iFifo( _FOFF(CCMRFifoItem,iNext) ), 
        iIter(iFifo), 
        iFifoEmpty( _FOFF(CCMRFifoItem,iNext) ), 
        iIterEmpty(iFifoEmpty), 
        iNumDataItemsStored(0) 
    { 
    }


// ---------------------------------------------------------
// CMRFifo::NewL
// "Two-phased" constructor
// ---------------------------------------------------------
//
CCMRFifo* CCMRFifo::NewL(TInt aNumItems)
    {
    CCMRFifo* self = new (ELeave) CCMRFifo();
    CleanupStack::PushL(self);
    self->ConstructL( aNumItems );
    CleanupStack::Pop(1);
    return self;
    }

// ---------------------------------------------------------
// CMRFifo::ConstructL
// 2nd phase constructor
// ---------------------------------------------------------
//
void CCMRFifo::ConstructL(TInt aNumItems)
    {
    TInt i;
    CCMRFifoItem* item;

    for ( i = 0; i < aNumItems; i++ )
        {
        item = CCMRFifoItem::NewL();
        iFifoEmpty.AddLast(*item);
        }
    }

// ---------------------------------------------------------
// CCMRFifo::~CCMRFifo
// Destructor
// ---------------------------------------------------------
//
CCMRFifo::~CCMRFifo()
    {
    CCMRFifoItem*  item;

    iIter.SetToFirst();     // rewind

    // delete all items
    while ((item = iIter++) != NULL)
        {
        iFifo.Remove(*item);
        delete item;
        }

    iIterEmpty.SetToFirst();     // rewind

    // delete all items
    while ((item = iIterEmpty++) != NULL)
        {
        iFifoEmpty.Remove(*item);
        delete item;
        }
    }

// ---------------------------------------------------------
// CCMRFifo::Get
// Returns the first element and removes it from fifo
// (other items were commented in a header).
// ---------------------------------------------------------
//
TAny* CCMRFifo::Get()
    {

    CCMRFifoItem* firstItem;
    if (iFifo.IsEmpty())
        {
        return NULL;
        }

    firstItem = iFifo.First();
    iFifo.Remove(*firstItem);  // remove
    iNumDataItemsStored--;
    iFifoEmpty.AddLast(*firstItem);
    return (const_cast<TAny*>(firstItem->Data()));
    }


// ---------------------------------------------------------
// CCMRFifo::PutL
// Puts an element to the fifo
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCMRFifo::PutL(TAny* aData)
    {
    CCMRFifoItem* item;
    if (iFifoEmpty.IsEmpty())
        {
        //allocate a new
        item = CCMRFifoItem::NewL();
        }
    else
        {
        item = iFifoEmpty.First();
        iFifoEmpty.Remove(*item);  // remove
        }

    item->SetData(aData);
    iFifo.AddLast(*item);  // add
    iNumDataItemsStored++;
    }


// ---------------------------------------------------------
// CCMRFifo::IsEmpty()
// Is it empty ?
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CCMRFifo::IsEmpty()
    {
    return iFifo.IsEmpty(); // is it empty ?
    }

// ---------------------------------------------------------
// CCMRFifo::NumberOfItems()
// Number of items stored in fifo
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCMRFifo::NumberOfItems()
    {
    return iNumDataItemsStored;
    }


//  End of File
