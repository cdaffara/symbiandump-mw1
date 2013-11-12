/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Action indication impl class declaration.
*
*/


#ifndef C_CFACTIONINDICATIONIMPL_H
#define C_CFACTIONINDICATIONIMPL_H

#include "cfactionindication.h"
#include "cfkeyvaluepair.h"

class RWriteStream;
class RReadStream;

/**
* Action indication implementation.
* 
* @since S60 5.0
* @lib cfservices.lib
*/
NONSHARABLE_CLASS( CCFActionIndicationImpl ): public CCFActionIndication
    {
    public:

        // Two phased constructors
        IMPORT_C static CCFActionIndicationImpl* NewL ();
        IMPORT_C static CCFActionIndicationImpl* NewLC ();
        
        // Destructor
        ~CCFActionIndicationImpl();

    public: // From base classes
    
        // From CCFActionIndication
        void SetIdentifierL( const TDesC& aIdentifier );

        // From CCFActionIndication
        const TDesC& Identifier() const;
        
        // From CCFActionIndication
        void AddParameterL( const TDesC& aKey, const TDesC& aValue );
        
        // From CCFActionIndication    
        const RKeyValueArray& Parameters() const;

    public: // New methods
    
        /**
        * Gets object size in bytes.
        * 
        * @since S60 4.0
        * @param None
        * @return TInt
        */
        IMPORT_C TInt Size() const;
        
        /**
        * Externalizes object.
        * 
        * @since S60 4.0
        * @param aStream Write stream.
        * @return None
        */
        IMPORT_C void ExternalizeL( RWriteStream& aStream );

        /**
        * Internalizes object.
        * 
        * @since S60 4.0
        * @param aStream Read stream.
        * @return None
        */
        IMPORT_C void InternalizeL( RReadStream& aStream );
        
    private: // New methods
    
        // Creates a new buffer from stream
        HBufC* ReadBufferFromStreamLC( TInt aLength,
            RReadStream& aStream );
        
    private:
    
        CCFActionIndicationImpl();
        void ConstructL();

    private: // Data
    
        // Own: Parameters
        RKeyValueArray iParameters;

        // Own: Identifier
        HBufC* iIdentifier;
    };

#endif
