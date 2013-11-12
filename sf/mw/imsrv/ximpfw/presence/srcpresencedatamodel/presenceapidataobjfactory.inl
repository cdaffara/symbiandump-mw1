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
* Description:  Inline methods for codec to pack and unpack data objects.
*
*/


#include <e32base.h>

#include "presencetypehelpers.h"
#include "presenceobjectfactory.h"
#include "ximpapidataobjbase.h"
//#include "ximpidentityimp.h"
#include "presentitygroupmemberinfoimp.h"
#include "presentitygroupinfoimp.h"
//#include "ximpfeatureinfoimp.h"
//#include "ximpcontextclientinfoimp.h"
#include "presencegrantrequestinfoimp.h"
#include "presencewatcherinfoimp.h"
//#include "ximpprotocolinfoimp.h"
//#include "ximpserviceinfoimp.h"
//#include "ximpstatusimp.h"
#include "presenceinfofieldimp.h"
#include "presenceinfofieldvaluetextimp.h"
#include "presenceinfofieldvaluebinaryimp.h"
#include "presenceinfofieldvalueenumimp.h"
#include "servicepresenceinfoimp.h"
#include "devicepresenceinfoimp.h"

//DATA TYPES


// ============================ MEMBER FUNCTIONS =============================

/**
 * Type definition for exact API data object constructor signature.
 */
typedef CXIMPApiDataObjBase* (*TApiDataObjConstructor)( RReadStream& );



/**
 * Structure for mapping data object interface IDs to
 * to corect API data object constructors.
 */
struct SApiDataObjConstructorMap
    {
    //Interface id
    TInt32    iInterfaceId;

    //Function pointer to data object interface implementation
    TApiDataObjConstructor iConstructorPtr;
    } ;



/**
 * Helper macro to initialise KApiDataObjConstructorTable members.
 *
 * Macro forces that each data object implementation class to have static
 * NewFromStreamLC() member function to instantiate the object.
 *
 * See TApiDataObjConstructor type definition for exact constructor
 * signature.
 */
#define CONSTRUCTOR_ENTRY( TheImplementedIf, TheClass ) \
    { TheImplementedIf::KInterfaceId, TheClass::NewFromStreamLC } \

/**
 * Constructor function mapping for data object interface implementations.
 *
 * When new data object types are implemented, add them here.
 */
const SApiDataObjConstructorMap KApiDataObjConstructorTable[] =
    {
    CONSTRUCTOR_ENTRY( MPresentityGroupMemberInfo, CPresentityGroupMemberInfoImp ),
    CONSTRUCTOR_ENTRY( MPresentityGroupInfo, CPresentityGroupInfoImp ),
    CONSTRUCTOR_ENTRY( MPresenceGrantRequestInfo, CPresenceGrantRequestInfoImp ),
    CONSTRUCTOR_ENTRY( MPresenceWatcherInfo, CPresenceWatcherInfoImp ),
    CONSTRUCTOR_ENTRY( MPresenceInfoField, CPresenceInfoFieldImp ),
    CONSTRUCTOR_ENTRY( MPresenceInfoFieldValueText, CPresenceInfoFieldValueTextImp ),
    CONSTRUCTOR_ENTRY( MPresenceInfoFieldValueBinary, CPresenceInfoFieldValueBinaryImp ),
    CONSTRUCTOR_ENTRY( MPresenceInfoFieldValueEnum, CPresenceInfoFieldValueEnumImp ),
    CONSTRUCTOR_ENTRY( MDevicePresenceInfo, CDevicePresenceInfoImp ),
    CONSTRUCTOR_ENTRY( MServicePresenceInfo, CServicePresenceInfoImp )
    };


/**
 * Count of constructor mappings.
 */
const TInt KApiDataObjConstructorCount = sizeof( KApiDataObjConstructorTable )
                                        / sizeof( SApiDataObjConstructorMap );



// ============================ HELPER FUNCTIONS =============================


namespace {

    /**
     * Helper function to locate constructor function for
     * data object interface ID.
     *
     * @param aDataObjInterfaceId The data object interface ID.
     * @return Data object constructor function.
     */
    TApiDataObjConstructor ConstructorForInterface( TInt32 aDataObjInterfaceId )
    {
        //Locate correct constructor
        for( TInt ix = 0; ix < KApiDataObjConstructorCount; ix++ )
        {
            const SApiDataObjConstructorMap map = KApiDataObjConstructorTable[ ix ];
            if( map.iInterfaceId == aDataObjInterfaceId )
            {
                return map.iConstructorPtr;
            }
        }


        //If assertion below fails, check that data object implementation
        //class implementing requested data object interface (aDataObjInterfaceId)
        //is registered to KApiDataObjConstructorTable
        __ASSERT_DEBUG( EFalse,
                User::Panic( _L("PresenceObjectFactory"), KErrUnknown ) );

        return NULL;
    }

    /**
     * Helper function to instantiate new data object object
     * of requested type and construct it from the stream.
     *
     * @param aDataObjInterfaceId
     * @return Data object constructor function.
     */
    CXIMPApiDataObjBase* NewDataObjectFromStreamLC( TInt32 aDataObjInterfaceId,
            RReadStream& aStream )
    {
        TApiDataObjConstructor newFromStreamLC = NULL;

        //Locate correct constructor for interface ID
        newFromStreamLC = ConstructorForInterface( aDataObjInterfaceId );

        //Instantiate the object
        CXIMPApiDataObjBase* dataObject = newFromStreamLC( aStream );

        return dataObject;
    }

} // namespace

// ---------------------------------------------------------------------------
// PresenceApiDataObjFactory::InternalizeL
// ---------------------------------------------------------------------------
//
template< class INFOIMP >
inline void CPresenceApiDataObjFactory::InternalizeL( 
        RReadStream& aStream,
        RPointerArray<INFOIMP>& aArray )
    {
    TInt32 arrLen = aStream.ReadInt32L();

    for ( TInt i = 0; i < arrLen; i++ )
        {
        TInt paramType = aStream.ReadInt32L();
        INFOIMP* infoObj = ( INFOIMP* ) NewDataObjectFromStreamLC( paramType, aStream );
        aArray.AppendL( infoObj );
        CleanupStack::Pop( infoObj );
        }
    }

// ---------------------------------------------------------------------------
// PresenceApiDataObjFactory::ExternalizeL
// ---------------------------------------------------------------------------
//
template <class INFOIMP>
inline void CPresenceApiDataObjFactory::ExternalizeL( 
        RWriteStream& aWs,
        const RPointerArray<INFOIMP>& aArray )
    {
    aWs.WriteInt32L( aArray.Count() ); // array length

    for ( TInt i = 0; i < aArray.Count(); i++ )
        {
        INFOIMP* infoField = aArray[ i ];

        // write the type
        aWs.WriteInt32L( infoField->Base().GetInterfaceId() );

        // write the actual object
        infoField->ExternalizeL( aWs );
        }

    aWs.CommitL();
    }

