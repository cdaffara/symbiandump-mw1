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
* Description:  Search Keys  subscription.
*
*/

#ifndef CSEARCHKEYSSUBSCRIPTIONITEM_H
#define CSEARCHKEYSSUBSCRIPTIONITEM_H

#include <e32base.h>
#include "ximpsubscriptionitembase.h"
#include "searchtypehelpers.h"


class MXIMPItemParentBase;
class MXIMPPscContext;
class CXIMPPscContext;


/**
 * search  subscription implementation
 *
 * @lib searchdatamodel.dll
 */
class CSearchKeysSubscriptionItem : public CXIMPSubscriptionItemBase
    {

	public: // Construction and destruction

	    /**
	     * Construction
	     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
	     */
	    static CSearchKeysSubscriptionItem* NewLC( MXIMPItemParentBase& aParent );

	    /**
	     * Destruction
	     * - Remember to inform parent about delete.
	     */
	    IMPORT_C virtual ~CSearchKeysSubscriptionItem();

	private: // Construction and destruction

	    /**
	     * Construction
	     * @param aParent. Parent has to be informed when this object is deleted by RemoveMe method.
	     */
	    CSearchKeysSubscriptionItem( MXIMPItemParentBase& aParent );

	    /**
	     * Construction
	     */
	    void ConstructL();

	public : 
	    /**
         *  Set the search key info array 
         *  takes ownership and will be transfered to search keys  event
         */ 
		IMPORT_C void SetKeyInfoArray( RSearchKeyInfoImpArray* aKeyInfoArray);
		
		
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

	private : // data
	
	    /**
         * RSearchKeyInfoImpArray : Not owned
         */	    
	    RSearchKeyInfoImpArray* iKeyInfoArray;
	    
    };


/* ======================================================================== */



#endif // CSEARCHKEYSSUBSCRIPTIONITEM_H
