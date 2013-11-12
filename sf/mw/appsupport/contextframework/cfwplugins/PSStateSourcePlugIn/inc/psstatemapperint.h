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
* Description:  CPSStateMapperInt class declaration.
*
*/



#ifndef C_PSSTATEMAPPERINT_H
#define C_PSSTATEMAPPERINT_H

#include <badesca.h>

#include "psstatemapper.h"
#include "intnamepair.h"

/**
* P&S state mapper for int type keys.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CPSStateMapperInt ): public CPSStateMapper
    {
    public:
    
        /**
        * Creates a new P&S mapper.
        * 
        * @since S60 40
        * @param aParams P&S state mapper construction parameters.
        * @return CPSStateMapperInt*
        */
        static CPSStateMapperInt* NewL( MCFContextInterface& aCF );
        static CPSStateMapperInt* NewLC( MCFContextInterface& aCF );
            
        // Destructor
        ~CPSStateMapperInt();
        
    public:
    	TInt MapperType() const;
    	
    	
    	void AddMappingL(TInt aPSValue, const TDesC& aContextValue);
        
        // from PsStateMapper
		void Define();
		void InitializeL();
        
    private: // From base classes
    
        // @see CPSStateMapper
        void ProperyChangedL();
        
    private:
    
        CPSStateMapperInt( MCFContextInterface& aCF );
        
        
        static TInt StartCallback(TAny* aPtr);
        void DoStart();
        
    private: // Data
    
        RIntNamePointerArray iIntNameMappings;
        
        CAsyncCallBack *iStartCB;
    };


#endif
