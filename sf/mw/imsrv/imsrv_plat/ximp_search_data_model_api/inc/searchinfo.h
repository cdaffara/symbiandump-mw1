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

#ifndef MSEARCHINFO_H
#define MSEARCHINFO_H

#include <e32std.h>
#include <ximpbase.h>
#include <searchdatamodelifids.hrh>
#include <badesca.h> 

class MXIMPIdentity;

/**
 * Defines Enums for information type.
 *
 * Information of following type  can be
 * stored and retrieve. 
 * 
 *
 */

enum TInfoType
	    {
	    
	    EEmailAddress,
	    EFirstName,
	    ELastName
	    
	    };	
	

/**
 * Interface for Search info object.
 *
 * Search info holds details of a single  search result.
 * Search results contains collection of searchinfo.
 *
 * @ingroup Searchdatamodelapi
 *
 */
class MSearchInfo : public MXIMPBase
    {
    
	    
	public:

	    /** Interface ID for the MImSearchInfo. */
	    enum { KInterfaceId = SEARCH_IF_ID_SEARCH_INFO };


	public:

	    /**
	     * Public destructor.
	     * Objects can be deleted through this interface.
	     */
	    virtual ~MSearchInfo() {}


	public:
	 	/**
	     * Returns the  information of particular information type.
	     *
	     * @param aType
	     *        TInfoType which information type (e.g. EFirstname, Elastname etc)
	     * 
	     * @return TDesc16&  information of the type  passed. 
	     */
	    virtual const TDesC16& GetFieldInfo(TInfoType aType ) const =0 ;
	    
	    
      	/**
	     * Sets the information of a particular type.
	     *
	     * @param aFieldInfo
	     *        information to be set.
	     *
	     * @param aType
	     *        type of the information.(e.g. EFirstname, Elastname etc)
	     *
	     */
	    virtual void SetFieldInfoL(const TDesC16& aFieldInfo, TInfoType aType)  = 0 ;
    };


#endif // MSEARCHINFO_H



