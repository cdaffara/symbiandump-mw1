/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CPSStateMapper class declaration.
*
*/



#ifndef C_PSSTATEMAPPER_H
#define C_PSSTATEMAPPER_H

#include <e32base.h>
#include <e32property.h>
#include <cfcontextobject.h>


class MCFContextInterface;

/**
* P&S state mapper wraps inside a selected P&S key from some category
* and maps this key to defined context.
* Currently only integer P&S values are handled. If other P&S values
* are wanted to map into contexts derive this class and provide support
* for this.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CPSStateMapper ): public CActive
    {
    public:
    
        // Mapper ids
        enum TPSStateMapperType
            {
            EPSIntMapper,
            EPSTextMapper
            };
    
    public:
            
        // Destructor
        ~CPSStateMapper();
        
    public: // New methods
    
    	virtual TInt MapperType() const = 0;
    
    	void SetPSCategory (TUid aPSCategory);
    	void SetPSKey (TUint32 aKey);
    	void SetSecurityPolicy (const TSecurityPolicy& aSecurityPolicy);
    	void SetSourceL ( const TDesC& aSource );
    	void SetTypeL ( const TDesC& aType );
    	void SetPassThrough(TBool aPassThrough);
    
        /**
        * Returns context source related to this mapping.
        * 
        * @since S60 4.0
        * @param None
        * @return const TDesC&
        */
        virtual const TDesC& Source() const;
        
        /**
        * Returns context type related to this mapping.
        * 
        * @since S60 4.0
        * @param None
        * @return const TDesC&
        */
        virtual const TDesC& Type() const;

        /**
        * Returns security policy related to this mapping.
        * 
        * @since S60 4.0
        * @param None
        * @return const TSecurityPolicy&
        */
        virtual const TSecurityPolicy& SecurityPolicy() const;
        
        virtual void Define() = 0;
        virtual void InitializeL() = 0;

    protected: // New methods
    
        /**
        * Subscribes into property.
        * 
        * @since S60 4.0
        * @param None
        * @return None
        */
        virtual void Subscribe();
    
    protected: // From base classes
    
        // @see CActive
        virtual void RunL();
    
        // @see CActive
        virtual void DoCancel();

        // @see CActive
        virtual TInt RunError( TInt aError );
        
    private: // New methods
    
        // Informs implementation that propery has changed
        virtual void ProperyChangedL() = 0;

    protected:
    
        CPSStateMapper( MCFContextInterface& aCF );
        virtual void ConstructL( );
        
    protected: // Data
    
        // Own: P&S property in hand.
        RProperty iProperty;
        
        
	public:
    
        // P&S category
        TUid iPSCategory;
        
        // P&S key from iPSCategory
        TUint32 iPSKey;

        // Context source to publish
        HBufC* iSource;
        
        // Context type to publish
        HBufC* iType;
        
        // Security policy
        TSecurityPolicy iSecurityPolicy;

		TBool iPassThrough;
        
        // CF interface
        MCFContextInterface& iCF;
        
        CCFContextObject* iContext;
        
    };

#endif
