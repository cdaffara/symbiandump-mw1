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
* Description:  XIMP Framework store for filters
*
*/


#ifndef C_PRESENCEFILTERSTORE_H
#define C_PRESENCEFILTERSTORE_H

#include "ximpbase.h"
#include <e32base.h>

class CXIMPIdentityImp;
class CPresenceInfoFilterImp;

/**
 * Filter store.
 * Defines a one-to-one mapping of an identity to a presence filter.
 *
 * @since S60 v4.0
 */
class CPresenceFilterStore : public CBase
    {
    private: // helper class

    class CPresenceFilterStoreMapping : public CBase
        {
        public:

            /**
             * Takes ownership.
             * @param aIdentity The identity
             * @param aFilter The filter.
             */
            static CPresenceFilterStoreMapping* NewL();

            ~CPresenceFilterStoreMapping();

            /**
             * @return The identity
             */
            CXIMPIdentityImp& Identity();

            /**
             * @return The filter
             */
            CPresenceInfoFilterImp& Filter();

            static TInt OrderOfMappings(
                    const CPresenceFilterStoreMapping& aFirst, 
                    const CPresenceFilterStoreMapping& aSecond );

            static TInt OrderOfMappingsByIdentity(
                    const CXIMPIdentityImp* aFirst, 
                    const CPresenceFilterStoreMapping& aSecond );
                    
            /**
             * Set filter to mapping.
             */
            void SetPresenceInfoFilter( CPresenceInfoFilterImp* aFilter );
            void SetIdentity( CXIMPIdentityImp* aIdentity );

        private:

            CPresenceFilterStoreMapping();

            void ConstructL();

        public:
            // owns: the identity
            CXIMPIdentityImp* iIdentity;

            // owns: the filter
            CPresenceInfoFilterImp* iFilter;
        };

public:

    /**
     * Construction
     */
    IMPORT_C static CPresenceFilterStore* NewL();

    /**
     * Destruction
     */
    virtual ~CPresenceFilterStore();

private:

    /** 
     * Construction
     */
    CPresenceFilterStore();
    
    /** 
     * Construction
     */
    void ConstructL();


public: // New methods

    /**
     * Associate identity with filter
     * @param aIdentity The identity
     * @param aFilter The filter. Ownership is transferred to callee
     * @leave KErrAlreadyExists if the association exists
     */
    IMPORT_C void AssociateL( 
            const CXIMPIdentityImp& aIdentity, 
            CPresenceInfoFilterImp* aFilter );

    /**
     * Remove association.
     * @param aIdentity The identity
     * @param aFilter The filter
     * @leave KErrNotFound if the association does not exist
     */
    IMPORT_C void Disassociate( const CXIMPIdentityImp& aIdentity );
    
    /**
     * Find the filter associated with the given identity.
     * @param aIdentity The identity
     * @return The filter
     * @leave KErrNotFound if the association does not exist
     */
    IMPORT_C CPresenceInfoFilterImp* FilterForIdentity( const CXIMPIdentityImp& aIdentity );

private: // helpers

private: // data

    /**
     * Array of mappings. Items are owned
     */
    RXIMPObjOwningPtrArray< CPresenceFilterStoreMapping > iMappings;
    };


#endif // C_PRESENCEFILTERSTORE_H
