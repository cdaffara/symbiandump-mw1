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
* Description:  Interface for Search Feature Plugin object factory.
*
*/

#ifndef MSEARCHOBJECTFACTORY_H
#define MSEARCHOBJECTFACTORY_H

#include <ximpbase.h>
#include <ximpidentity.h>
#include <bamdesca.h>
#include <searchdatamodelifids.hrh>


class MSearchElement;
class MSearchInfo;
class MSearchKeyInfo;
/**
 * Interface for Search Feature Plugin search object factory.
 *
 * Search  object factory is used to instantiate
 * data objects declared in data model API.
 *
 * @ingroup Searchdatamodelapi
 *  
 */
class MSearchObjectFactory : public MXIMPBase
    {
	public:

	    /** Interface ID for the MSearchObjectFactory. */
	    enum { KInterfaceId = SEARCH_IF_ID_SEARCH_OBJECT_FACTORY };


	protected:

	    /**
	     * Protected destructor.
	     * Object instancies can't be deleted via this interface.
	     */
	    virtual ~MSearchObjectFactory() {}




	public: 

	 	/**
	     * Instantiates new search element object.
	     *
	     * @return Newsearch element object instance.
	     *         Object ownership is returned to caller.
	     *
	     * @leave KErrNoMemory if failed to allocate memory.
	     */
	   
	    virtual MSearchElement* NewSearchElementLC() = 0;
	    
	    
	    /**
	     * Instantiates new search info object.
	     *
	     * @return New search info object instance.
	     *         Object ownership is returned to caller.
	     *
	     * @leave KErrNoMemory if failed to allocate memory.
	     */
	    virtual MSearchInfo* NewSearchInfoLC() = 0;

	    /**
          * Instantiates new SearchKey info  object.
          *
          * @return new SearchKey   object instance.
          *         Object ownership is returned to caller.
          *
          * @leave KErrNoMemory if failed to allocate memory.
          */
       virtual MSearchKeyInfo* NewSearchKeyInfoLC() = 0;

    };



#endif // MSEARCHOBJECTFACTORY_H



