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
* Description:  Configuration item base for configuration items.
*
*/

#ifndef CXIMPPSCCONFIGURATIONITEMBASE_H
#define CXIMPPSCCONFIGURATIONITEMBASE_H

#include <e32base.h>
#include "ximpconfigurationitem.h"
#include "ximpsubscriptionitembase.h"

/**
 * Configuration item base
 *
 * @since S60 v3.2
 */
class CXIMPConfigurationItemBase : public CXIMPSubscriptionItemBase, 
                                   public MXIMPConfigurationItem
    {
public: // Definitions
        
    /** 
     * Authorization status
     */
    enum TConfigurationStatus
        {
        ENotConfiguredAtAll,
        EConfiguredForOtherCtxOnly,
        EConfiguredForCtxOnly,
        EConfiguredForCtxAndOthers
        };
                
public: // Construction and destruction

    /**
     * Destruction
     * - Remember to inform parent about delete.
     */
    IMPORT_C virtual ~CXIMPConfigurationItemBase();

    /**
     * Construction
     */
    IMPORT_C CXIMPConfigurationItemBase( MXIMPItemParentBase& aParent );
        
    /**
     * Base construction. Must be called from inherited classes ConstructL
     */
    IMPORT_C void BaseConstructL();
     
    
public: // New methods
    
    /**
     * Authorizationstatus for context. 
     *
     *   ENotConfiguredAtAll            - Configure not done by any context.
     *   EConfiguredForOtherCtxOnly     - Configure done by others but not aContext
     *   EConfiguredForCtxOnly          - Configure done by aContext only.
     *   EConfiguredForCtxAndOthers     - Configure done by others and aContext.
     *
     * @param aContext Check authorization status for aContext.
     */
    IMPORT_C TConfigurationStatus ConfigurationStatus( MXIMPPscContext* aContext );
    
    /**
     * Add configurator for item.
     * @param aContext. Context to be added.
     */
    IMPORT_C void AddConfiguratorL( MXIMPPscContext* aContext );
    
    /**
     * Remove configurator from item
     * @param aContext. Context to be removed.
     */
    IMPORT_C void RemoveConfigurator( MXIMPPscContext* aContext );
    
    /** 
     * Add configurator for item.
     * @param aItem. Item to be added.
     */
    IMPORT_C void AddConfiguratorL( CXIMPConfigurationItemBase* aItem );

    /**
     * Remove configurator from item
     * @param aContext. Item to be removed.
     */
    IMPORT_C void RemoveConfigurator( CXIMPConfigurationItemBase* aItem );
    
    
    
public: // From CXIMPSubscriptionItemBase

    /**
     * @see CXIMPSubscriptionItemBase
     */
    IMPORT_C TBool IsContext( MXIMPPscContext* aContext ) const;    

    /**
     * @see CXIMPSubscriptionItemBase
     */
    IMPORT_C void ForceClose();

 
public: // From MXIMPConfigurationItem

    
protected: // Helpers

    /**
     * For linear ordering by pointers
     */
    static TInt OrderOfItems( const CXIMPConfigurationItemBase& aFirst, 
                              const CXIMPConfigurationItemBase& aSecond );
        
protected: // New interface methods.
        
    
protected: // Data

    /**
     * Configured contexts.
     */
    RPointerArray< MXIMPPscContext > iConfigContexts;
    RPointerArray< MXIMPPscContext > iPreConfigContexts;
    RPointerArray< CXIMPConfigurationItemBase > iConfigItems;
    RPointerArray< CXIMPConfigurationItemBase > iPreConfigItems;
    };


/* ======================================================================== */



#endif // CXIMPPSCCONFIGURATIONITEMBASE_H
