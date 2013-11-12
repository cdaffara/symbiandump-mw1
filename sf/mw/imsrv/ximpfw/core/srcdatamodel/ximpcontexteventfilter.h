/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Presence context event filter.
*
*/

#ifndef CXIMPPRESENCECONTEXTEVENTFILTER_H
#define CXIMPPRESENCECONTEXTEVENTFILTER_H

#include "ximpapiobjbase.h"
#include <s32strm.h>

/**
 * Presence Context event filter.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextEventFilter ) : public CBase
    {
public:

    /**
     * Instantiates new event filter object.
     *
     * Initializes the list of accepted events 
     * from given array.
     * 
     * @param aAcceptedEvents List of events to accept.
     *        If NULL, all events are accepted. Default value NULL
     */
    IMPORT_C static CXIMPContextEventFilter* NewLC( 
            const TArray< TInt32 >* aAcceptedEvents = NULL );

    IMPORT_C static CXIMPContextEventFilter* NewL( 
            const TArray< TInt32 >* aAcceptedEvents = NULL );


    /**
     * Instantiates new event filter object.
     * 
     * Collects the accepted events from given event 
     * filter objects.
     * 
     * @param aFilters List of events filters 
     *        from where to gather accepted events.
     */
    IMPORT_C static CXIMPContextEventFilter* NewCombinedLC( 
            const TArray< CXIMPContextEventFilter* >& aFilters );


    /**
     * Destructor.
     */
    virtual ~CXIMPContextEventFilter();


private:
    CXIMPContextEventFilter();



public:


    /**
     * Checks is the given event accepted or not.
     * 
     * @param aEvent The event to check.
     * @return ETrue if given event type is accepted, 
     *         else EFalse.
     */
    IMPORT_C TBool IsEventAccepted( TInt32 aEvent ) const;
    
    
    /**
     * Extenalizes object data to given stream.
     * @param aStream Stream to write.
     */
    IMPORT_C void ExternalizeL( RWriteStream& aStream ) const;


    /**
     * Internalizes object data from given stream.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );


private:
    void SetFilterL( const TArray< TInt32 >* aAcceptedEvents );

    void CombineFilterL( 
         const TArray< CXIMPContextEventFilter* >& aFilters );

    void CopyUniqueEventsL( const TArray< TInt32 >& aSource, 
                            CArrayFix< TInt32 >& aTarget );

    TBool FindEventPosition( const TInt32 aEvent, 
                             const CArrayFix< TInt32 >& aEvents, 
                             TInt& aPos ) const; 



private: // data

    /**
     * The list of accepted events.
     * NULL if all events are accepted.
     * Own.
     */
    CArrayFix< TInt32 >* iAcceptedEvents;

    };


#endif // CXIMPPRESENCECONTEXTEVENTFILTER_H
