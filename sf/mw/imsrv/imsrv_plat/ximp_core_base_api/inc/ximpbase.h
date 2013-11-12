/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Base interface and related helpers for XIMPFw interfaces.
*
*/

#ifndef XIMPBASE_H
#define XIMPBASE_H

#include <e32std.h>
#include <ximpbaseifids.hrh>


/**
 * Base interface for XIMP Framework object interfaces.
 *
 * Base interface for XIMP Framework object interfaces.
 * -# All other XIMP Framework object interfaces inherit
 *    directly from MXIMPBase.
 * -# Future extended XIMP Framework interfaces may inherit
 *    also indirectly from MXIMPBase, by deriving from some current
 *    XIMP Framework interface, and extending the interface
 *    functionality with some new methods.
 * -# Each XIMP Framework object derives only once
 *    from MXIMPBase.
 *
 *
 * MXIMPBase::GetInterface() methods allows client to
 * discover referencies to other interfaces that the object
 * in question supports. MXIMPBase::GetInterfaceId() method
 * lets client to discover the objects primary interface id.
 *
 * @ingroup ximpfwbaseapi
 * @since S60 v3.2
 */
class MXIMPBase
    {
public:

    /** Interface ID for the MXIMPBase. */
    enum { KInterfaceId = XIMP_IF_ID_BASE };


public:

    /**
     * Options for interface resolving.
     *
     * Client gives one of these options for interface resolving,
     * to select which way the client wants to handle possible
     * unknown interface.
     */
    enum TIfGetOps
        {
        /**
         * Requestes the interface resolver to signal unknown
         * interface by returning NULL.
         */
        EReturnNullIfUnknown = 0,

        /**
         * Requestes the interface resolver to signal unknown
         * interface by raising a panic.
         */
        EPanicIfUnknown
        };



protected:

    /**
     * Protected destructor.
     *
     * Concrete objects can't be
     * deleted via base interface.
     */
    virtual ~MXIMPBase() {};



public:


    /**
     * Const interface query.
     *
     * Interface query to access other XIMP Framework
     * interfaces that the object in question supports.
     * Returns const (read-only) interface. If unsupported
     * interface type is tried to access, error is handle as
     * defined by given aOps parameter.
     *
     * NOTE: Client may use method directly, and cast returned
     * TAny pointer to requested interface type. Or client optionally
     * use templated TXIMPGetInterface helper to encapsulate
     * interface query and type cast.
     *
     * @param [in] aInterfaceId
     *        Interface identifier identifying the interface
     *        to retrieve.
     *
     * @param [in] aOps
     *        Options for interface query.
     *
     * @return Pointer to the retrieved interface type.
     *         NULL if the object in question does not support
     *         the requested interface.
     */
    virtual const TAny* GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps ) const = 0;


    /**
     * Non-const interface query.
     *
     * Interface query to access other XIMP Framework
     * interfaces that the object in question supports.
     * Returns modifiable (non-const) interface. If unsupported
     * interface type is tried to access, error is handle as
     * defined by given aOps parameter.
     *
     * NOTE: Client may use method directly, and cast returned
     * TAny pointer to requested interface type. Or client optionally
     * use templated TXIMPGetInterface helper to encapsulate
     * interface query and type cast.
     *
     * @param [in] aInterfaceId
     *        Interface identifier identifying the interface
     *        to retrieve.
     *
     * @param [in] aOps
     *        Options for interface query.
     *
     * @return Pointer to the retrieved interface type.
     *         NULL if the object in question does not support
     *         the requested interface.
     */
    virtual TAny* GetInterface(
                    TInt32 aInterfaceId,
                    TIfGetOps aOps ) = 0;


    /**
     * Interface identifier access.
     *
     * Method to access object interface identifier.
     * Returns always objects "primary" interface id.
     * Primary interface id, is the interface id of the
     * MXIMPBase derived interface that object
     * implements.
     *
     * @return Object interface id.
     */
    virtual TInt32 GetInterfaceId() const = 0;

    };



/**
 * Templated array of pointers.
 *
 * Templated array of pointers, array owns the pointed objects
 * and overwrites the normal RPointerArray::Close()
 * behaviour to destroy objects, before reseting the
 * array contents.
 *
 * Array is used with XIMP Framework interface objects.
 *
 * @ingroup ximpfwbaseapi
 * @since S60 v3.2
 */
template< class T >
class RXIMPObjOwningPtrArray : public RPointerArray<T>
    {
 public:

    /**
     * Default C++ constructor.
     */
    inline RXIMPObjOwningPtrArray();


    /**
     * Overwritten Close().
     *
     * Calls ResetAndDestroy() to delete holded objects
     * before reseting array contents. Thus array can be
     * pushed on the cleanup stack with CleanupClosePushL().
     */
    inline void Close();


    /**
     * @todo
     * R-class destrcutor. This must be removed from interface,
     * since by Symbian OS convention, R-classes do not have destructor.
     */
    inline ~RXIMPObjOwningPtrArray();
    };





/**
 * Request identifier.
 *
 * Request identifier to identify the asynchronous request
 * made by client or protocoll adaptation to the XIMP Framework.
 * Identifier type is also used to identify requests that
 * XIMP Framework issues to Protocol Adaptation.
 *
 * @ingroup ximpfwbaseapi
 * @since S60 v3.2
 */
class TXIMPRequestId
    {
public:
    enum
        {
        /** The maximum size of the request id.*/
        EMaxSize = 16
        };

public:

    /**
     * C++ default constructor.
     *
     * Initializes the request ID with TXIMPRequestId::Null()
     * value.
     */
    inline TXIMPRequestId();


    /**
     * Equality operator.
     *
     * Tests request ID for equality with the specified request ID.
     *
     * @param [in] aRhs
     *        The request ID to be compared with.
     *
     * @return ETrue, if request ID equals to the specified one.
     *         Otherwise EFalse.
     */
    inline TBool operator==( const TXIMPRequestId& aRhs ) const;


    /**
     * Inequality operator.
     *
     * Tests this request ID for inequality with the specified request ID.
     *
     * @param [in] aRhs
     *        The request ID to be compared with.
     *
     * @return ETrue, if request ID inequals to the specified one.
     *         Otherwise EFalse.
     */
    inline TBool operator!=( const TXIMPRequestId& aRhs ) const;


    /**
     * Constructs a Null-valued TXIMPRequestId object.
     *
     * @return Null-valued TXIMPRequestId object.
     */
    static inline TXIMPRequestId Null();


    /**
     * Constructs a TXIMPRequestId object having
     * given integer value.
     *
     * This method is ment for XIMP Framework
     * client and adaptation level test code.
     * In normal circumstances XIMP Framework
     * fills the TXIMPRequestId contents internally.
     *
     * @param aValue Integer value for TXIMPRequestId.
     *
     * @return Integer-valued TXIMPRequestId object.
     */
    static inline TXIMPRequestId Init( TInt aValue );



private: // data

    //OWN: Request id data
    TUint8 iRequestId[ TXIMPRequestId::EMaxSize ];
    };




/**
 * Interface query and type cast helper.
 *
 * Templated interface query and type cast helper.
 * Capable to retrieve const interface from const source object
 * and non-const interface from non-const source object.
 *
 * Const interface usage example:
 * @code
 * const MXIMPBase* obj = GetObjectImplementingTheInterfaceLC();
 *
 * const MXimpFwTheInterface* theInterface =
 *      TXIMPGetInterface< const MXimpFwTheInterface >::From( *obj,
 *                                                          MXIMPBase::EPanicIfUnknown );
 * theInterface->DoSomething();
 * @endcode
 *
 *
 * Non-const interface usage example:
 * @code
 * MXIMPBase* obj = GetObjectImplementingTheInterfaceLC();
 *
 * MXimpFwTheInterface* theInterface =
 *      TXIMPGetInterface< MXimpFwTheInterface >::From( *obj,
 *                                                    MXIMPBase::EPanicIfUnknown );
 * theInterface->DoSomething();
 * @endcode
 *
 * @ingroup ximpfwbaseapi
 * @since S60 v3.2
 */
template< class T >
class TXIMPGetInterface
    {
public:

    /**
     * Interface query and type cast.
     *
     * Encapsulates query of const interface from given
     * const source object. The retrieved interface type is
     * defined with template parameter. KInterfaceId identifier
     * from template parameter, is used to identify
     * the retrieved interface.
     *
     * @param [in] aObj
     *        Source object from where to retrieve interface.
     *
     * @param [in] aOps
     *        Options for interface resolving.
     *        Given options are delivered directly
     *        to MXIMPBase::GetInterface().
     *
     * @return Pointer to the retrieved interface.
     *         NULL if the object in question does not support
     *         the requested interface.
     */
    static inline T* From(
                const MXIMPBase& aObj,
                MXIMPBase::TIfGetOps aOps );



    /**
     * Interface query and type cast.
     *
     * Encapsulates query of interface from given source object.
     * The retrieved interface type is defined with template
     * parameter. KInterfaceId identifier from template parameter,
     * is used to identify the retrieved interface.
     *
     * @param [in] aObj
     *        Source object from where to retrieve interface.
     *
     * @param [in] aOps
     *        Options for interface resolving.
     *        Given options are delivered directly
     *        to MXIMPBase::GetInterface().
     *
     * @return Pointer to the retrieved interface.
     *         NULL if the object in question does not support
     *         the requested interface.
     */
    static inline T* From(
                MXIMPBase& aObj,
                MXIMPBase::TIfGetOps aOps );



private:   //Prohibited constructor
    TXIMPGetInterface();
    };


#include "ximpbase.inl"

#endif // XIMPBASE_H



