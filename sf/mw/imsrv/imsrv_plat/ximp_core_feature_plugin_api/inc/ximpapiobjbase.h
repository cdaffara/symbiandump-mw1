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
* Description:  Base class for XIMP API implementations.
*
*/

#ifndef XIMPAPIOBJBASE_H
#define XIMPAPIOBJBASE_H

#include <e32base.h>
#include <ximpbase.h>



/**
 * XIMP Framework class implementation UIDs.
 *
 * These do not have to be "real" Symbian UIDs, but such ID's,
 * that they do not overlap with XIMP API interface IDs.
 */
 
// 90xx - ximp related
// 91xx - presence related
// 92xx - instant messaging related

//Data model classes
#define XIMPIMP_CLSID_CXIMPAPIDATAOBJBASE                   9000
#define XIMPIMP_CLSID_CXIMPAPIOBJBASE                       9001
#define XIMPIMP_CLSID_CXIMPFEATUREINFOIMP                   9001
#define XIMPIMP_CLSID_CXIMPIDENTITYIMP                      9003
#define XIMPIMP_CLSID_CXIMPPROTOCOLINFOIMP                  9004
#define XIMPIMP_CLSID_CXIMPSERVICEINFOIMP                   9005
#define XIMPIMP_CLSID_CXIMPSTATUSIMP                        9006
#define XIMPIMP_CLSID_CXIMPOBJECTFACTORYIMP                 9007
#define XIMPIMP_CLSID_CXIMPAPIEVENTBASE                     9008
#define XIMPIMP_CLSID_CXIMPRESTRICTEDOBJECTCOLLECTIONIMP    9009
#define XIMPIMP_CLSID_CXIMPOBJECTCOLLECTIONIMP              9010
#define XIMPIMP_CLSID_CXIMPCONTEXTCLIENTINFOIMP             9011
#define XIMPIMP_CLSID_CXIMPCONTEXTSTATEIMP                  9012
#define XIMPIMP_CLSID_CXIMPDATASUBSCRIPTIONSTATEIMP         9013

// Plugin host environment classes
#define XIMPIMP_CLSID_CXIMPPSCHOST                          9020
#define XIMPIMP_CLSID_CXIMPPSCHOSTMANAGER                   9021

// Management
#define XIMPIMP_CLSID_CXIMPCLIENTIMP                        9030
#define XIMPIMP_CLSID_CXIMPCONTEXTIMP                       9031

// Events
#define XIMPIMP_CLSID_CXIMPPROTOCOLREQUESTCOMPLETEEVENTIMP  9040
#define XIMPIMP_CLSID_CXIMPREQUESTCOMPLETEEVENTIMP          9041
#define XIMPIMP_CLSID_CXIMPCONTEXTSTATEEVENTIMP             9042


/**
 * Declares a GetInterface() and similar methods for
 * MXIMPBase and CXIMPApiObjBase derived implementation.
 *
 * To be used in class header (.h) file.
 */
#define XIMPIMP_DECLARE_IF_BASE_METHODS \
    MXIMPBase& Base(); \
    TAny* GetInterface( TInt32 aInterfaceId, TIfGetOps aOps ); \
    TInt32 GetInterfaceId() const; \
    const TAny* GetInterface( TInt32 aInterfaceId, TIfGetOps aOps ) const; \
    const MXIMPBase& Base() const; \


/**
 * Starts a non-const GetInterface() method
 * block for CXIMPApiObjBase-derived implementation.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN( TheClass, TheImplementedIf ) \
    MXIMPBase& TheClass::Base() \
        { \
        return *this; \
        } \
    TAny* TheClass::GetInterface( TInt32 aInterfaceId, TIfGetOps aOps ) \
        { \
        if( aInterfaceId == CXIMPApiObjBase::KClassId ) \
            { \
            CXIMPApiObjBase* classPtr = this; \
            return classPtr; \
            } \
        if( aInterfaceId == TheClass::KClassId ) \
            { \
            TheClass* classPtr = this; \
            return classPtr; \
            } \
        if( aInterfaceId == TheImplementedIf::KInterfaceId ) \
            { \
            TheImplementedIf* ifPtr = this; \
            return ifPtr; \
            } \


/**
 * Non-const interface return statement for GetInterface() block.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_INTERFACE( AdditionalIf, FromPtr ) \
    if( aInterfaceId == AdditionalIf::KInterfaceId )\
        { \
        AdditionalIf* ifPtr = FromPtr; \
        return ifPtr; \
        } \


/**
 * Non-const class return statement for GetInterface() block.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_CLASS( AdditionalClass, FromPtr ) \
    if( aInterfaceId == AdditionalClass::KClassId )\
        { \
        AdditionalClass* classPtr = FromPtr; \
        return classPtr; \
        } \


/**
 * Starts a const GetInterface() method
 * block for CXIMPApiObjBase-derived implementation.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN( TheClass, TheImplementedIf ) \
    TInt32 TheClass::GetInterfaceId() const \
        { \
        return TheImplementedIf::KInterfaceId; \
        } \
    const MXIMPBase& TheClass::Base() const \
        { \
        return *this; \
        } \
    const TAny* TheClass::GetInterface( TInt32 aInterfaceId, TIfGetOps aOps ) const \
        { \
        if( aInterfaceId == CXIMPApiObjBase::KClassId ) \
            { \
            const CXIMPApiObjBase* classPtr = this; \
            return classPtr; \
            } \
        if( aInterfaceId == TheClass::KClassId ) \
            { \
            const TheClass* classPtr = this; \
            return classPtr; \
            } \
        if( aInterfaceId == TheImplementedIf::KInterfaceId ) \
            { \
            const TheImplementedIf* ifPtr = this; \
            return ifPtr; \
            } \


/**
 * Const interface return statement for GetInterface() block.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_CONST_INTERFACE( AdditionalIf, FromPtr ) \
    if( aInterfaceId == AdditionalIf::KInterfaceId )\
        { \
        const AdditionalIf* ifPtr = FromPtr; \
        return ifPtr; \
        } \


/**
 * Const class return statement for GetInterface() block.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_CONST_CLASS( AdditionalClass, FromPtr ) \
    if( aInterfaceId == AdditionalClass::KClassId )\
        { \
        const AdditionalClass* classPtr = FromPtr; \
        return classPtr; \
        } \



/**
 * Shared definition to end the
 * - XIMPIMP_IF_BASE_GET_INTERFACE_BEGIN and
 * - XIMPIMP_IF_BASE_GET_CONST_INTERFACE_BEGIN
 * statements.
 *
 * To be used in class implementation (.cpp) file.
 */
#define XIMPIMP_IF_BASE_GET_INTERFACE_END()\
    return CXIMPApiObjBase::HandleUnknownGetInterface( aInterfaceId, aOps ); \
    } \



/**
 * Macro for XIMP implementation to suppress warnings
 * from unused formal parameters.
 */
#define XIMP_UNUSED_FORMAL_PARAM(p) (void) p



/**
 * Macro to implement the default statement 
 * from switch-case structure with panic call. 
 *
 * If injected code is compiled under BullseyeCoverage, 
 * uses BullseyeCoverage specific guards, 
 * to exclude injected code from coverage 
 * calcualtion. 
 */
#if _BullseyeCoverage

#define XIMP_DEFAULT_CASE_UNSUPPORTED( aPanicReason )    \
    "BullseyeCoverage save off";                         \
    default:                                             \
        {                                                \
        NXIMPPrivPanic::Panic( aPanicReason );           \
        break;                                           \
        }                                                \
    "BullseyeCoverage restore";                          \

#else

#define XIMP_DEFAULT_CASE_UNSUPPORTED( aPanicReason )    \
    default:                                             \
        {                                                \
        NXIMPPrivPanic::Panic( aPanicReason );           \
        break;                                           \
        }                                                \

#endif //_BullseyeCoverage


/**
 * Base class for all XIMP API implementations.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
class CXIMPApiObjBase : public CBase
    {
public:

    enum { KClassId = XIMPIMP_CLSID_CXIMPAPIOBJBASE };

protected:

    /**
     * All XIMP Framework concrete API object
     * implementations are derived from this class.
     */
    IMPORT_C CXIMPApiObjBase();


public:

    /**
     * Virtual destructor CXIMPApiObjBase().
     */
    IMPORT_C virtual ~CXIMPApiObjBase();




protected: //Protected handlers for derived classes

    /**
     * Handler for unknown interface access cases.
     *
     * All XIMP Framework API object implementations
     * should call this to handle access request for
     * unknown interface type.
     *
     * @param aInterfaceId
     *        Interface identifier identifying the interface
     *        which was tried to retrieve.
     *
     * @param aOps
     *        Options for interface retrieval from client.
     *
     * @return NULL.
     */
    IMPORT_C TAny* HandleUnknownGetInterface(
            TInt32 aInterfaceId,
            MXIMPBase::TIfGetOps aOps ) const;


public:

    /**
     * Access to base virtual interface.
     *
     * @return XIMP base virtual interface.
     */
    virtual MXIMPBase& Base() = 0;


    /**
     * Read-only access to base virtual interface.
     *
     * @return XIMP base virtual interface.
     */
    virtual const MXIMPBase& Base() const = 0;

    };
    
    
    
/**
 * Templated 
 * 
 *
 * @since S60 v3.2
 */
template< class Type >
class TXIMPGetImpClassOrPanic
    {

public:
    /**
     * 
     */
    static inline Type* From( MXIMPBase& aObj )
        {
        TAny* ifPtr = aObj.GetInterface( Type::KClassId, MXIMPBase::EPanicIfUnknown );
        Type* ifTypePtr = static_cast< Type* >( ifPtr );
        return ifTypePtr; 
        }

    static inline Type* From( const MXIMPBase& aObj )
        {
        const TAny* ifPtr = aObj.GetInterface( Type::KClassId, MXIMPBase::EPanicIfUnknown );
        const Type* ifTypePtr = static_cast< const Type* >( ifPtr );
        return ifTypePtr; 
        }

private:   //Prohibited constructor
    TXIMPGetImpClassOrPanic();
    };
    

/**
 * Templated 
 * 
 *
 * @since S60 v3.2
 */
template< class Type >
class TXIMPGetImpClassOrNull
    {

public:
    /**
     * 
     */
    static inline Type* From( MXIMPBase& aObj )
        {
        TAny* ifPtr = aObj.GetInterface( Type::KClassId, MXIMPBase::EReturnNullIfUnknown );
        Type* ifTypePtr = static_cast< Type* >( ifPtr );
        return ifTypePtr; 
        }

    static inline Type* From( const MXIMPBase& aObj )
        {
        const TAny* ifPtr = aObj.GetInterface( Type::KClassId, MXIMPBase::EReturnNullIfUnknown );
        const Type* ifTypePtr = static_cast< const Type* >( ifPtr );
        return ifTypePtr; 
        }

private:   //Prohibited constructor
    TXIMPGetImpClassOrNull();
    };



template< class Type >
class TXIMPEqualsContent
    {
    public:
        /**
         * Call equalcontent for all items in array
         */
        static inline TBool Array( const RPointerArray< Type >& aObjArray, 
                                   const RPointerArray< Type >& aOtherObjArray )
            {
            TInt count = aObjArray.Count();
            if( count != aOtherObjArray.Count() )
                {
                return EFalse;
                }
            for( TInt a = 0; a < count; ++a )
                {
                if( !aObjArray[ a ]->EqualsContent( *aOtherObjArray[ a ] ) )
                    {
                    return EFalse;
                    }
                }
            return ETrue;
            }
    private:   //Prohibited constructor
        TXIMPEqualsContent();
    };

#endif // XIMPAPIOBJBASE_H
