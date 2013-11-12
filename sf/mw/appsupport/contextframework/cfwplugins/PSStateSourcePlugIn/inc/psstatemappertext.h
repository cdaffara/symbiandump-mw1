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
* Description:  CPSStateMapperText class declaration.
*
*/



#ifndef C_PSSTATEMAPPERTEXT_H
#define C_PSSTATEMAPPERTEXT_H

#include <badesca.h>

#include "psstatemapper.h"
#include "textpair.h"

/**
* P&S state mapper for text type keys.
* 
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( CPSStateMapperText ): public CPSStateMapper
    {
    public:
    
        /**
        * Creates a new P&S mapper.
        * 
        * @since S60 40
        * @param aParams P&S state mapper construction parameters.
        * @return CPSStateMapperText*
        */
        static CPSStateMapperText* NewL( MCFContextInterface& aCF );
        static CPSStateMapperText* NewLC( MCFContextInterface& aCF );
            
        // Destructor
        ~CPSStateMapperText();
        
    public:
    	TInt MapperType() const;
    	
    	void AddMappingL(const TDesC& aPSValue, const TDesC& aContextValue);
        
        // from PsStateMapper
		void Define();
		void InitializeL();
        
    private: // From base classes
    
        // @see CPSStateMapper
        void ProperyChangedL();
        
    private:
    
        CPSStateMapperText( MCFContextInterface& aCF );
        
        static TInt StartCallback(TAny* aPtr);
        void DoStart();
        
    private: // Data
    
        RTextPairPointerArray iTextMappings;
        
        CAsyncCallBack *iStartCB;
    };


#endif // C_PSSTATEMAPPERTEXT_H
