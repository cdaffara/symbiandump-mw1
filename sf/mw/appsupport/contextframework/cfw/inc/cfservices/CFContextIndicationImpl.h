/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCFContextIndicationImpl class declaration.
*
*/


#ifndef C_CFCONTEXTINDICATIONIMPL_H
#define C_CFCONTEXTINDICATIONIMPL_H

#include "cfcontextindication.h"

class RWriteStream;
class RReadStream;
class CCFContextObjectImpl;

/**
* Context indication implementation.
*
* @lib CFServices.lib
* @since S60 4.0
*/
NONSHARABLE_CLASS( CCFContextIndicationImpl ): public CCFContextIndication
    {
    public:
    
        // Two phased constructors
        IMPORT_C static CCFContextIndicationImpl* NewL( const TUid& aUid );
        IMPORT_C static CCFContextIndicationImpl* NewLC( const TUid& aUid );
        
        // Two phased constructors
        IMPORT_C static CCFContextIndicationImpl* NewL(
            RReadStream& aStream );
        IMPORT_C static CCFContextIndicationImpl* NewLC(
            RReadStream& aStream );

        // Destructor
        ~CCFContextIndicationImpl();
        
    public: // From base classes
    
        // @see CCFContextIndication
        const CCFContextObject& Context() const;

        // @see CCFContextIndication
        const TUid& Uid() const;
        
        // @see CCFContextIndication
        void CreateDataObjectL( CCFContextDataObject& aDataObject );

    public: // New methods

        /**
        * Internalizes object.
        * 
        * @since S60 5.0
        * @param aStream Read stream.
        * @return None.
        */    
        IMPORT_C void InternalizeL( RReadStream& aStream );
        
        /**
        * Externalizes object.
        *
        * @since S60 5.0
        * @param aStream Write stream.
        * @return None.
        */    
        IMPORT_C void ExternalizeL( RWriteStream& aStream );

        /**
        * Returns size of the object in bytes.
        *
        * @since S60 5.0
        * @param None.
        * @return TInt.
        */    
        IMPORT_C TInt Size() const;
	        
        /**
        * Sets new context to this indication.
        * 
        * @since S60 4.0
        * @param aObject Context object.
        * @return None
        */
        IMPORT_C void SetContextL( const CCFContextObject& aContext );
        
        /**
        * Sets a UID of the data object.
        *
        * @since S60 5.0
        * @param aUid Uid of the data object.
        * @return None.
        */
        IMPORT_C void SetDataObjectUid( const TUid& aUid );
        
        /**
        * Associate a data object with the context indication.
        *
        * @since S60 5.0
        * @param aData Data object to associate.
        * @return None.
        */
        IMPORT_C void SetDataObject( HBufC8* aData );

    private:

        CCFContextIndicationImpl( const TUid& aUid );
        void ConstructL();

    private:

        /** Context object related to the indication */
        CCFContextObjectImpl* iContext;
        
        /** UID associated into the data */
        TUid iUid;
        
        /** Streamed data object */
        HBufC8* iData;
    };

#endif
