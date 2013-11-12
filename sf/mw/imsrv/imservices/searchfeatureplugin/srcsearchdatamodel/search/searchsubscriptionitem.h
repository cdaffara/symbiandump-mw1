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
* Description:   Search subscription implementation.
*
*/

#ifndef CSEARCHSUBSCRIPTIONITEM_H
#define CSEARCHSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "ximpsubscriptionitembase.h"
#include "searchtypehelpers.h"


class MXIMPItemParentBase;
class MXIMPPscContext;
class CXIMPPscContext;

class CSearchInfoImp;
class MSearchInfo;
/**
 * search  subscription implementation
 *
 * @lib searchdatamodel.dll
 */
class CSearchSubscriptionItem : public CXIMPSubscriptionItemBase
    {

	public: // Construction and destruction

	    /**
	     * Construction
	     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
	     */
	    static CSearchSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

	    /**
	     * Destruction
	     * - Remember to inform parent about delete.
	     */
	    IMPORT_C virtual ~CSearchSubscriptionItem();

	private: // Construction and destruction

	    /**
	     * Construction
	     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
	     */
	    CSearchSubscriptionItem( MXIMPItemParentBase& aParent );

	    /**
	     * Construction
	     */
	    void ConstructL();

	public: // New methods

	    /**
         *  Set the search results  takes ownership and given to search event
         */
	    IMPORT_C void  SetSearchResults(RSearchInfoImpArray*  aSearchResults);
	    /**
         *  Set the search id   takes ownership and given to search event
         */
	    IMPORT_C void SetSearchIdL( CXIMPIdentityImp& aSearchId );
	    
	public: // From MXIMPSubscriptionItem

	    /**
	     * @see MXIMPSubscriptionItem
	     */
	    void SynthesiseSubscriptionEventToL( MXIMPPscContext* aContext, TBool aForceEvent );
	private: // From CXIMPIMSubscriptionItemBase

	    /**
	     * @see CXIMPIMSubscriptionItemBase
	     */
	    void CleanExpired();

	private : 
	     /**	
	     *  Search info array :Not owned
	     */
		
	    RSearchInfoImpArray* iSearchResults;
	    
	    /**	
	     *  identity  :Not owned
	     */
	    CXIMPIdentityImp*    iSearchId ;

    };


/* ======================================================================== */



#endif // CPSCSearchSUBSCRIPTIONITEM_H
