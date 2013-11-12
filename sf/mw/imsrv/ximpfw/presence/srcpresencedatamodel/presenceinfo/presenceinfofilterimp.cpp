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
* Description:  MPresenceDocument API object implementation.
*
*/


#include "presenceinfofilterimp.h"
#include "ximprbufhelpers.h"
#include "ximpobjecthelpers.h"
#include "presenceinfo.h"
#include "badesca.h"

const TInt KFilterGranularity = 3;
const TInt KFieldGranularity  = 3;

using namespace NPresenceInfo;

// ============================ LOCAL FUNCTIONS ==============================

// ============================ HELPER CLASS =================================

// ---------------------------------------------------------------------------
// CFilterField::NewL
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp::CFilterField* 
CPresenceInfoFilterImp::CFilterField::NewL(
        const TDesC8& aInfoName )
    {
    CFilterField* self = new ( ELeave ) CFilterField();
    CleanupStack::PushL( self );
    self->ConstructL( aInfoName );
    CleanupStack::Pop( self ); 
    return self;
    }

// ---------------------------------------------------------------------------
// CFilterField::NewLC
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp::CFilterField* 
CPresenceInfoFilterImp::CFilterField::NewLC()
    {
    CFilterField* self = new ( ELeave ) CFilterField();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CFilterField::ConstructL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::CFilterField::ConstructL(
        const TDesC8& aInfoName )
    {
    HBufC8* tmp = aInfoName.AllocL();
    iInfoName.Assign( tmp );
    
    iFieldNames = new ( ELeave ) CDesC8ArraySeg( KFieldGranularity ); 
    }

// ---------------------------------------------------------------------------
// CFilterField::ConstructL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::CFilterField::ConstructL()
    {
    iInfoName.CreateL( 0 );
    iFieldNames = new ( ELeave ) CDesC8ArraySeg( KFieldGranularity ); 
    }

// ---------------------------------------------------------------------------
// CFilterField::CFilterField
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp::CFilterField::CFilterField() 
    {
    }

// ---------------------------------------------------------------------------
// CFilterField::~CFilterField
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp::CFilterField::~CFilterField()
    {
    iInfoName.Close();
    delete iFieldNames;
    }

// ---------------------------------------------------------------------------
// CFilterField::InfoName
// ---------------------------------------------------------------------------
//
const TDesC8& CPresenceInfoFilterImp::CFilterField::InfoName()
    {
    return iInfoName;
    }

// ---------------------------------------------------------------------------
// CFilterField::AddFieldNameL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::CFilterField::AddFieldNameL(
        const TDesC8& aFieldName )
    {
    iFieldNames->InsertIsqL( aFieldName );
    }

// ---------------------------------------------------------------------------
// CFilterField::OrderOfFields()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFilterImp::CFilterField::OrderOfFields( 
        const CFilterField& aFirst, 
        const CFilterField& aSecond )
    {
    // order is based on the info name
    return aFirst.iInfoName.Compare( aSecond.iInfoName );
    }

// ---------------------------------------------------------------------------
// CFilterField::OrderOfFieldsByName()
// ---------------------------------------------------------------------------
//
TInt CPresenceInfoFilterImp::CFilterField::OrderOfFieldsByName( 
        const TDesC8* aName, 
        const CFilterField& aSecond )
    {
    return aName->Compare( aSecond.iInfoName );
    }

// ---------------------------------------------------------------------------
// CFilterField::ExternalizeL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::CFilterField::ExternalizeL(
        RWriteStream& aStream ) const
    {
    XIMPRBuf8Helper::ExternalizeL( iInfoName, aStream );
    
    aStream.WriteInt32L( iFieldNames->Count() );

    for ( TInt i = 0; i < iFieldNames->Count(); i++ )
        {
        TPtrC8 p = iFieldNames->MdcaPoint( i );
        aStream.WriteInt32L( p.Length() );
        aStream.WriteL( p );
        }
    }

// ---------------------------------------------------------------------------
// CFilterField::InternalizeL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::CFilterField::InternalizeL(
        RReadStream& aStream ) 
    {
    XIMPRBuf8Helper::InternalizeL( iInfoName, aStream );

    TInt count = aStream.ReadInt32L();
    for ( TInt i = 0; i < count; i++ )
        {
        TInt length = aStream.ReadInt32L();
        HBufC8* buf = HBufC8::NewLC( length );

        TPtr8 des = TPtr8( buf->Des() );
        aStream.ReadL( des, length );
        iFieldNames->InsertIsqL( *buf );

        CleanupStack::PopAndDestroy( buf );
        }
    }

// ---------------------------------------------------------------------------
// CFilterField::MergeL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::CFilterField::MergeL( CFilterField& aOtherField ) 
    {
    // merge algorithm:
    // add all field names from given filter field, ignoring duplicates 
    
    for ( TInt i = 0; i < iFieldNames->Count(); i++ )
        {
        TInt pos = 0;
        TInt ret = iFieldNames->FindIsq( aOtherField.iFieldNames->MdcaPoint( i ), pos );

        if ( 0 != ret )
            {
            iFieldNames->InsertIsqL( aOtherField.iFieldNames->MdcaPoint( i ) );
            }
        }
    }
    
// ---------------------------------------------------------------------------
// CFilterField::operator==
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::CFilterField::operator==( const CFilterField& aSource )
    {
    if( iInfoName.Compare( aSource.iInfoName ) != 0 )
        {
        return EFalse;
        }
    TInt count = iFieldNames->Count();
    if( count != aSource.iFieldNames->Count() )
        {
        return EFalse;
        }
        
    for ( TInt i = 0; i < count; i++ )
        {
        if( iFieldNames->MdcaPoint( i ).Compare( aSource.iFieldNames->MdcaPoint( i ) ) != 0 )
            {
            return EFalse;
            }
        }
    return ETrue;
    }    

// ---------------------------------------------------------------------------
// CFilterField::operator!=
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::CFilterField::operator!=( const CFilterField& aSource )
    {
    return !( *this == aSource );
    }    

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::NewLC()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* CPresenceInfoFilterImp::NewLC()
    {
    CPresenceInfoFilterImp* self = new( ELeave ) CPresenceInfoFilterImp;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::NewL()
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* CPresenceInfoFilterImp::NewL()
    {
    CPresenceInfoFilterImp* self = NewLC();
    CleanupStack::Pop( self );
    return self;
    }
// ---------------------------------------------------------------------------
// CPresenceInfoImp::NewFromStreamLC()
// ---------------------------------------------------------------------------
//
XIMPIMP_IMPLEMENT_DATAOBJ_NEWFROMSTREAM( CPresenceInfoFilterImp )

// ---------------------------------------------------------------------------
// Implement supported interface access.
// ---------------------------------------------------------------------------
//
XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( CPresenceInfoFilterImp, 
                                     MPresenceInfoFilter )
XIMPIMP_IF_BASE_GET_INTERFACE_END()


XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( CPresenceInfoFilterImp, 
                                           MPresenceInfoFilter )
XIMPIMP_IF_BASE_GET_INTERFACE_END()

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::EqualsContent()
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::EqualsContent( 
    const CXIMPApiDataObjBase& aOtherInstance ) const
    {
    const CPresenceInfoFilterImp* tmp =
        TXIMPGetImpClassOrPanic< const CPresenceInfoFilterImp >::From( aOtherInstance.Base() );

    TInt count = iPersonFilter->Count();
    if( count != tmp->iPersonFilter->Count() )
        {
        return EFalse;
        }
        
    for ( TInt i = 0; i < count; i++ )
        {
        if( iPersonFilter->MdcaPoint( i ).Compare( tmp->iPersonFilter->MdcaPoint( i ) ) != 0 )
            {
            return EFalse;
            }
        }
        
    count = iServiceFilter.Count();
    if( count != tmp->iServiceFilter.Count() )
        {
        return EFalse;
        }
        
    for( TInt a = 0; a < count; ++a )
        {
        if( iServiceFilter[ a ] != tmp->iServiceFilter[ a ] )
            {
            return EFalse;
            }
        }
        
    count = iDeviceFilter.Count();
    if( count != tmp->iDeviceFilter.Count() )
        {
        return EFalse;
        }
        
    for( TInt a = 0; a < count; ++a )
        {
        if( iDeviceFilter[ a ] != tmp->iDeviceFilter[ a ] )
            {
            return EFalse;
            }
        }
    return ETrue;        
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::CPresenceInfoFilterImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp::CPresenceInfoFilterImp()
    {
    }


// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::~CPresenceInfoFilterImp()
// ---------------------------------------------------------------------------
//
CPresenceInfoFilterImp::~CPresenceInfoFilterImp()
    {
    delete iPersonFilter;
    iServiceFilter.ResetAndDestroy();
    iDeviceFilter.ResetAndDestroy();
    }



// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::ConstructL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::ConstructL()
    {
    iPersonFilter = new ( ELeave ) CDesC8ArraySeg( KFilterGranularity );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::ExternalizeArrayL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::ExternalizeArrayL( 
        const CDesC8ArraySeg* aArray, RWriteStream& aStream ) const
    {
    aStream.WriteInt32L( aArray->Count() );

    for ( TInt i = 0; i < aArray->Count(); i++ )
        {
        TPtrC8 p = aArray->MdcaPoint( i );
        aStream.WriteInt32L( p.Length() );
        aStream.WriteL( p );
        }
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::ExternalizeFieldSetL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::ExternalizeFieldSetL(
        const RXIMPObjOwningPtrArray<CFilterField>& aFieldSet,
        RWriteStream& aStream
        ) const
    {
    TInt count = aFieldSet.Count();

    aStream.WriteInt32L( count );
    for ( TInt i = 0; i < count; i++ )
        {
        CFilterField* field = aFieldSet[ i ];
        field->ExternalizeL( aStream );
        }
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::InternalizeFieldSetL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::InternalizeFieldSetL(
        RXIMPObjOwningPtrArray<CFilterField>& aFieldSet,
        RReadStream& aStream
        )
    {
    TInt count = aStream.ReadInt32L();

    for ( TInt i = 0; i < count; i++ )
        {
        CFilterField* field = CFilterField::NewLC();
        field->InternalizeL( aStream );
        aFieldSet.AppendL( field );
        CleanupStack::Pop( field );
        }
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::ExternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFilterImp::ExternalizeL( RWriteStream& aStream ) const
    {
    ExternalizeArrayL( iPersonFilter, aStream );
    ExternalizeFieldSetL( iServiceFilter, aStream );
    ExternalizeFieldSetL( iDeviceFilter, aStream );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::InternalizeArrayL()
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::InternalizeArrayL( 
        CDesC8ArraySeg* aArray, RReadStream& aStream )
    {
    TInt count = aStream.ReadInt32L();
    for ( TInt i = 0; i < count; i++ )
        {
        TInt length = aStream.ReadInt32L();

        HBufC8* buf = HBufC8::NewLC( length );

        TPtr8 des = TPtr8( buf->Des() );
        aStream.ReadL( des, length );
        aArray->InsertIsqL( des );

        CleanupStack::PopAndDestroy( buf );
        }
    }


// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::InternalizeL()
// ---------------------------------------------------------------------------
//
EXPORT_C void CPresenceInfoFilterImp::InternalizeL( RReadStream& aStream )
    {
    InternalizeArrayL( iPersonFilter, aStream );
    InternalizeFieldSetL( iServiceFilter, aStream );
    InternalizeFieldSetL( iDeviceFilter, aStream );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::ContainsPersonFilter
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::ContainsPersonFilter(
        CDesC8ArraySeg* aA,
        CDesC8ArraySeg* aCompare 
        )
    {
    TInt count = aCompare->Count();
    if ( count >= aA->Count() )
        {
        // impossible to be contained
        return EFalse;
        }

    for ( TInt i = 0; i < count; i++ )
        {
        // all person filter strings in aCompare MUST be found in this class
        TInt pos = 0;
        if( 0 != aA->FindIsq( aCompare->MdcaPoint( i ), pos ) )
            {
            // element not contained within "this", so whole filter not contained
            return EFalse;
            }
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::ContainsFilterFields
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::ContainsFilterFields(
        RXIMPObjOwningPtrArray<CFilterField>& aA,
        RXIMPObjOwningPtrArray<CFilterField>& aCompare 
        )
    {
    // check all filter fields
    TInt count = aCompare.Count();
    if ( count >= aA.Count() )
        {
        // impossible for "aCompare" to be contained, has more fields
        return EFalse;
        }

    for( TInt i = 0; i < count; i++ )
        {
        // all filters in aCompare MUST be found in this class.
        // first check for the named filter field class
        TInt pos = 0;
        pos = aA.FindInOrder( 
                aCompare[ i ]->InfoName(), 
                CFilterField::OrderOfFieldsByName );

        if ( KErrNotFound == pos )
            {
            // element not contained within "aA", so whole filter not contained
            return EFalse;
            }

        // now the subfields in the filter field class
        TInt countFields = aCompare[ i ]->iFieldNames->MdcaCount();
        if ( countFields >= aA[ pos ]->iFieldNames->MdcaCount() )
            {
            // impossible to be contained
            return EFalse;
            }

        for ( TInt j = 0; j < countFields; j++ )
            {
            TInt dummyPos = 0;
            if ( aA[ pos ]->iFieldNames->FindIsq( aCompare[ i ]->iFieldNames->MdcaPoint( j ), dummyPos ) )
                {
                // field not contained within "aA", so whole
                // filter not contained
                return EFalse;
                }
            }
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::Contains
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPresenceInfoFilterImp::Contains( CPresenceInfoFilterImp& aCompare )
    {
    // walk through all filters in aCompare,
    // if a filter is NOT found in this class --> not contained
    // if all filters are found in this class --> contained

    // person filter
    if ( ! ContainsPersonFilter( iPersonFilter, aCompare.iPersonFilter ) )
        {
        return EFalse;
        }

    // service filter
    if ( ! ContainsFilterFields( iServiceFilter, aCompare.iServiceFilter ) )
        {
        return EFalse;
        }

    // device filter
    if ( ! ContainsFilterFields( iDeviceFilter, aCompare.iDeviceFilter ) )
        {
        return EFalse;
        }

    return ETrue;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::operator==
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPresenceInfoFilterImp::operator==( CPresenceInfoFilterImp& aSource )
    {
    return EqualsContent( aSource );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::operator!=
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPresenceInfoFilterImp::operator!=( CPresenceInfoFilterImp& aSource )
    {
    return !EqualsContent( aSource );
    }


// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::AcceptPersonFilterL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::AcceptPersonFilterL( const TDesC8& aFieldName )
    {
    if ( !aFieldName.Compare( NPresenceInfo::NFieldType::KAcceptAll  ) )
        {
        iPersonFilter->InsertIsqL( NPresenceInfo::NFieldType::KStatusMessage );
        iPersonFilter->InsertIsqL( NPresenceInfo::NFieldType::KAvatar );
        iPersonFilter->InsertIsqL( NPresenceInfo::NFieldType::KAvailability );
        }
    else
        {
    iPersonFilter->InsertIsqL( aFieldName );
    }
    }
    
// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::AcceptFilterL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::AcceptFilterL( 
            RXIMPObjOwningPtrArray< CFilterField >& aFieldSet,
            const TDesC8& aInfoName,
            const TDesC8& aFieldName )
    {
    TLinearOrder< CPresenceInfoFilterImp::CFilterField > order( 
            CPresenceInfoFilterImp::CFilterField::OrderOfFields );

    CFilterField* field = CFilterField::NewL( aInfoName );
    CleanupStack::PushL( field );
    field->iFieldNames->InsertIsqL( aFieldName );
    aFieldSet.InsertInOrderL( field, order );
    CleanupStack::Pop( field );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::AcceptServiceFilterL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::AcceptServiceFilterL( 
            const TDesC8& aServiceName,
            const TDesC8& aFieldName )
    {
    AcceptFilterL( iServiceFilter, aServiceName, aFieldName );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::AcceptDeviceFilterL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::AcceptDeviceFilterL( 
            const TDesC8& aDeviceName,
            const TDesC8& aFieldName )
    {
    AcceptFilterL( iDeviceFilter, aDeviceName, aFieldName );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::MatchPersonFilter
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::MatchPersonFilter( 
        const TDesC8& aFieldName ) const
    {
    TBool found = EFalse;
    for ( TInt i = 0; i < iPersonFilter->Count() && ! found; i++ )
        {
        if ( KErrNotFound != ( iPersonFilter->MdcaPoint( i ).Match( aFieldName ) ) )
            {
            // found it
            return ETrue;
            }
        }

    return EFalse;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::MatchFilterL
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::MatchFilterL(
        const RXIMPObjOwningPtrArray< CFilterField >& aFieldSet, 
        const TDesC8& aInfoName,
        const TDesC8& aFieldName ) const
    {
    CFilterField* candidate = CFilterField::NewL( aInfoName );
    CleanupStack::PushL( candidate );

    TBool foundMatch = EFalse;

    // walk through all fields. slow algo! use a hash to index the field set.
    for ( TInt j = 0; j < aFieldSet.Count(); j++ )
        {
        // now, go through all fields in service filter which match 
        // the candidate
        CFilterField* field = aFieldSet[ j ];

        if ( KErrNotFound != aInfoName.Match( field->iInfoName ) )
            {
            // field name matches, so try to find the types

            for ( TInt i = 0; i < field->iFieldNames->Count(); i++ )
                {
                TBool found = ( KErrNotFound != aFieldName.Match( field->iFieldNames->MdcaPoint( i ) ) )
                    ? ETrue : EFalse;
                foundMatch |= found;
                }
            }
        }
    CleanupStack::PopAndDestroy( candidate );

    return foundMatch;
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::MatchServiceFilter
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::MatchServiceFilterL( 
        const TDesC8& aServiceName,
        const TDesC8& aFieldName ) const
    {
    return MatchFilterL( iServiceFilter, aServiceName, aFieldName );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::MatchDeviceFilterL
// ---------------------------------------------------------------------------
//
TBool CPresenceInfoFilterImp::MatchDeviceFilterL(
        const TDesC8& aDeviceName,
        const TDesC8& aFieldName ) const
    {
    return MatchFilterL( iDeviceFilter, aDeviceName, aFieldName );
    }

// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::UnionL
// ---------------------------------------------------------------------------
//
EXPORT_C CPresenceInfoFilterImp* CPresenceInfoFilterImp::UnionL( 
        CPresenceInfoFilterImp& aFirst,
        CPresenceInfoFilterImp& aSecond )
    {
    CPresenceInfoFilterImp* cloned = TXIMPObjectCloner< CPresenceInfoFilterImp >::CloneLC( aFirst );

    // union of the person filters - must not add duplicates
    for ( TInt i = 0; i < aSecond.iPersonFilter->Count(); i++ )
        {
        TInt pos = 0;
        TInt ret = cloned->iPersonFilter->FindIsq( aSecond.iPersonFilter->MdcaPoint( i ), pos );

        if ( 0 != ret )
            {
            cloned->iPersonFilter->InsertIsqL( aSecond.iPersonFilter->MdcaPoint( i ) );
            }
        }

    // union of service fields
    aFirst.UnionOfFieldsL( aSecond.iServiceFilter, cloned->iServiceFilter );

    // union of device fields
    aFirst.UnionOfFieldsL( aSecond.iDeviceFilter, cloned->iDeviceFilter );

    CleanupStack::Pop( cloned );
    return cloned;
    }
    
// ---------------------------------------------------------------------------
// CPresenceInfoFilterImp::UnionOfFieldsL
// ---------------------------------------------------------------------------
//
void CPresenceInfoFilterImp::UnionOfFieldsL( 
            RXIMPObjOwningPtrArray< CFilterField>& aSource, 
            RXIMPObjOwningPtrArray< CFilterField>& aTarget )
    {
    TLinearOrder< CPresenceInfoFilterImp::CFilterField > order( 
            CPresenceInfoFilterImp::CFilterField::OrderOfFields );

    for ( TInt i = 0; i < aSource.Count(); i++ )
        {
        CFilterField* field = aSource[ i ];

        // if the field name exists in the target, merge the fields
        TInt pos = aTarget.FindInOrder( field, order );
        if ( pos == KErrNotFound )
            {
            // not found, insert the non-found one as a copy
            CFilterField* newField = TXIMPObjectCloner< CFilterField >::CloneLC( *field );
            aTarget.InsertInOrder( newField, order );
            CleanupStack::Pop( newField );
            }
        else
            {
            // found one, merge them to the cloned one
            aTarget[ pos ]->MergeL( *field );
            }
        }
    }

// End of file
