/*
 * Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
 *  Description : Content Arsenal client session
 *
 */

#ifndef CACLIENTSESSION_H
#define CACLIENTSESSION_H

// INCLUDES
#include <e32base.h>

#include "caoperationparams.h"
#include "casrvdef.h"

//FORWARD DECLARATIONS
class CCaInnerEntry;
class CCaInnerQuery;
class RCaEntriesArray;
class RCaIdsArray;

// CLASS DECLARATION
/**
 * Client-side interface to Content Arsenal Server
 *
 * This class provides the client-side interface to the content arsenal server
 * session, it just passes requests to the server.
 *
 */
class RCaClientSession: public RSessionBase
    {
public:

    /**
     * Construct the object.
     */
    RCaClientSession();

    /**
     * Connects to the server and create a session.
     * @return error code
     */
    TInt Connect();

    /**
     * Starts the server if it is not running.
     * @return error code
     */
    TInt StartServer();

    /**
     * Create subsession.
     * @return error code
     */
    TInt Create( TInt aFunction, const TIpcArgs& aArgs );

    /**
     * Closes session
     *
     * @since S60 v 5.0
     */
    void Close();

    /**
     * Return version information
     */
    TVersion Version() const;

    /*
     * Gets entries list
     * @param aQuery query
     * @param aResult contains operation result
     */
    void GetListL( const CCaInnerQuery& aQuery, RCaEntriesArray& aResult );

    /*
     * Gets entries ids list
     * @param aQuery query
     * @param aResult contains operation result
     */
    void GetEntryIdsL( const CCaInnerQuery& aQuery, RCaIdsArray& aResult );

    /*
     * Adds entry to a storage
     * @param aEntry input entry
     * @param aResult output entry
     */
    void AddL( CCaInnerEntry& aEntry );

    /*
     * Removes entry from a storage
     * @param aEntries array containing entries ids
     */
    void RemoveL( const RCaIdsArray& aEntries );

    /*
     * Organizes entries in a storage
     * @param aEntries array containing entires to organize
     * @param aParams operation parameters
     */
    void OrganizeL( const RCaIdsArray& aEntries,
            TCaOperationParams aParams );

    /*
     * Touch operation for entry in storage
     * @param aEntry entry to be touched
     */
    void TouchL( const CCaInnerEntry& aEntry );
    
    /*
     * Custom sort in a storage.
     * @param aEntries array containing entries ids.
     * @param aGroupId The group Id to custom sort.
     */
    void CustomSortL( const RCaIdsArray& aEntries, const TInt aGroupId );

private:

    /*
     * Gets operation result
     * @param aSize size of descriptor
     * @param aRequest server request
     * @param aObject object to store result
     */
    template<typename SerializableObject>
    TInt GetResultL( TInt aSize, TCaServerRequests aRequest,
            SerializableObject& aObject );

    /*
     * GetSizeL method
     * Return error
     * @param aQuery query
     * @param aServerRequest
     * @param aResultSize
     */
    TInt GetSizeL( const CCaInnerQuery& aQuery,
            const TCaServerRequests aServerRequest, TInt& aResultSize );
    };

#endif // CACLIENTSESSION_H
// End of File
