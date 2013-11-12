/*
* Copyright (c) 2007, 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPresenceBuddyInfoList implementation.
*
*/

#ifndef CXIMPPRESENCEBUDDYINFOLISTIMP_H
#define CXIMPPRESENCEBUDDYINFOLISTIMP_H

#include <s32buf.h>
#include <presencebuddyinfolist.h>

#include "presenceapiobjbase.h"
#include "presencetypehelpers.h"
#include "ximpapidataobjbase.h"

class MXIMPIdentity;
class MPresenceBuddyInfo;

/**
 * MPresenceBuddyInfoList API object implementation. By default the this
 * list owns the MPresenceBuddyInfo objects.
 * If the pointer array doesn't needed to own the objects, simply call
 * SetOwnObjects(EFalse) method before delete. 
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceBuddyInfoListImp ): public CXIMPApiDataObjBase,
                                                public MPresenceBuddyInfoList                                                
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CXIMPPRESENCEBUDDYINFOLIST };


public:
    
    //if aServiceName is KNullDesC then multiple services buddies are allowed
    IMPORT_C static CPresenceBuddyInfoListImp* NewLC(const TDesC& aServiceName);
    IMPORT_C static CPresenceBuddyInfoListImp* NewL(const TDesC& aServiceName);
    virtual ~CPresenceBuddyInfoListImp();

private:

    CPresenceBuddyInfoListImp();
    void ConstructL(const TDesC& aServiceName);
    void ConstructL(); // to use with internalize

public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS

public: // From CXIMPApiDataObjBase

    /**
     * @see CXIMPApiDataObjBase
     */
    XIMPIMP_DECLARE_DATAOBJ_BASE_METHODS

public: // From MPresenceBuddyInfoList

    RPointerArray<MPresenceBuddyInfo>& GetObjectCollection();
    
    TPtrC ServiceName() const;
    
    TInt Count() const;
    
    TInt AddOrReplace(MPresenceBuddyInfo* aPresenceBuddyInfo);
    
    TInt FindAndRemove(const MXIMPIdentity& aIdentity);
    
    MPresenceBuddyInfo* FindAndGet(const MXIMPIdentity& aIdentity) const;
    
public: // New functions

    /**
     * Internalizes object data from given stream. Must call 
     * SetCountForInternalize before calling this function.
     * @param aStream Stream to read.
     */
    IMPORT_C void InternalizeL( RReadStream& aStream );
    
    /**
     * Add buddy presence info object in this collection. Without any
     * check for duplicates. Takes ownership.
     * @param aPresenceBuddyInfo object to add.
     */
    IMPORT_C TInt BlindAddL(MPresenceBuddyInfo* aPresenceBuddyInfo);
    
    /**
     * Set to own/disown objects. Call this function with EFalse to
     * set this list not to own objects.
     */
    IMPORT_C void SetOwnObjects(TBool aOwnObjects);
    
private:

    /**
     * Extract service name from XspId
     * @param aXspId XspId URI
     * @return service name
     */
    TPtrC ExtractService( const TDesC& aXspId );    

private: // data


    /**
     * OWN if iOwnDataObjects is true.
     */
    RPointerArray< MPresenceBuddyInfo> iBuddyCollection;

    /**
     * OWN: 
     */
    RBuf16 iService;
    
    /**
    *OWN
    */
    TBool iOwnDataObjects;
    
    };


#endif // CXIMPPRESENCEBUDDYINFOLISTIMP_H
