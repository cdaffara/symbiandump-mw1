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
* Description:  Interface for Search info object.
*
*/

#ifndef MIMSEARCHINFO_H
#define MIMSEARCHINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <imdatamodelifids.hrh>
#include <badesca.h> 



class MXIMPIdentity;


/**
 * Interface for Search info object.
 *
 * Search info object presents single search. 
 * Search info object holds the identity,search limit and search results.
 *
 *
 * @ingroup Imdatamodelapi
 *
 */
class MImSearchInfo : public MXIMPBase
    {
public:

    /** Interface ID for the MImSearchInfo. */
    enum { KInterfaceId = IM_IF_ID_SEARCH_INFO };


public:

    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MImSearchInfo() {}


public:

    /**
     * Returns identity of the Search.
     *
     * @return The identity of the Search.
     */
	virtual const MXIMPIdentity& SearchId() const = 0;

	
    /**
     * Returns the search index.
     *
     * @return The search index.
     */
	virtual const TInt Index() const = 0;
	

    /**
     * Returns the number of search results.
     *
     * @return The number of search results.
     */
	virtual const TInt Results() const = 0;
	

    /**
     * Returns the search result.
     *
     * @return The search result.
     */
	virtual const CPtrC16Array& ResultArray() const = 0;


    /**
     * Sets identity of the search.
     *
     * @param [in] aIdentity
     *        Identity of the search.
     *        Ownership of the argument is transfered to
     *        callee, if method succeeds without a leave.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     * @leave KErrArgument if NULL argument were provided.
     */
	virtual void SetSearchIdL(
                MXIMPIdentity* aIdentity ) = 0;


    /**
     * Sets index of the search.
     *
     * @param [in] aIndex
     *        New search index value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
    virtual void SetIndex(
                const TInt aIndex ) = 0;                


    /**
     * Sets number of results.
     *
     * @param [in] aResult
     *        New search result value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
	virtual void SetResults(
				const TInt aResult ) = 0;
	

    /**
     * Sets the searched result.
     *
     * @param [in] aResultArray
     *        New searched result value.
     *
     * @leave KErrNoMemory if failed to allocate memory.
     */
	virtual void SetResultArray(
				CPtrC16Array& aResultArray ) = 0;
	
    };


#endif // MIMSEARCHINFO_H



