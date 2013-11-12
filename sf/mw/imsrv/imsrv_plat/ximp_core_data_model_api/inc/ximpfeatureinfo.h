/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Interface for presence feature info object.
*
*/

#ifndef MXIMPFWFEATUREINFO_H
#define MXIMPFWFEATUREINFO_H

#include <e32std.h>
#include <bamdesca.h>
#include <ximpbase.h>
#include <ximpdatamodelifids.hrh>


/**
 * Literal feature IDs for XIMP Framework features.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 *
 * @todo Exact features & their IDs need to be defined.
 */
namespace NXIMPFeature
    {
    namespace Presence
        {
        _LIT8( KPublish,        "feat/pres/pub");
        _LIT8( KFetch,          "feat/pres/fetch");
        _LIT8( KSubscribe,      "feat/pres/subs");
        _LIT8( KUnsubscribe,    "feat/pres/unsubs");
        _LIT8( KAddContact,     "feat/pres/addcnt");
        _LIT8( KDeleteContact,  "feat/pres/delcnt"); 
        _LIT8( KBlock,          "feat/pres/block"); 
        _LIT8( KUnBlock,        "feat/pres/unblock");
        _LIT8( KAvatar,         "feat/pres/avatar");
        }

    namespace PresentityGroups
        {
        _LIT8( KCreate,         "feat/entgroup/create");
        _LIT8( KDelete,         "feat/entgroup/delete");
        }
    namespace InstantMessage
        {
        _LIT8( KInstantMessage, "feat/im");
        }
    namespace Search
        {
        _LIT8( KSearch,         "feat/search");
        }
    }



/**
 * Interface for XIMP feature info object.
 *
 * @TODO Improve comments: Container used to present either XIMP Framework,
 * Presence Protocol or Presence Context features.
 *
 * @ingroup ximpdatamodelapi
 * @since S60 v3.2
 */
class MXIMPFeatureInfo : public MXIMPBase
    {
public:

    /** The MXIMPFeatureInfo interface ID. */
    enum { KInterfaceId = XIMP_IF_ID_FEATURE_INFO };


public:


    /**
     * Public destructor.
     * Objects can be deleted through this interface.
     */
    virtual ~MXIMPFeatureInfo() {}


public:


    /**
     * Gets the list of feature IDs.
     *
     * @return The list of features. Depending from where
     *         the MXIMPFeatureInfo object was retrieved,
     *         list contains either XIMP Framework,
     *         Presence Protocol or Presence Context features.
     */
    virtual const MDesC8Array& FeatureIds() const = 0;



    /**
     * Checks has the MXIMPFeatureInfo object the given feature
     * ID or not.
     *
     * @param [in] aFeatureId
     *        The feature ID which status to check.
     *        XIMP Framework feature IDs are defined in
     *        NXIMPFeature namespace.
     *
     * @return KErrNone if the ID is found from the feature ID list.
     *         KErrNotFound if the ID is not found from feature ID list.
     */
    virtual TInt HasFeatureId(
                const TDesC8& aFeatureId ) = 0;

    };


#endif // MXIMPFWFEATUREINFO_H


