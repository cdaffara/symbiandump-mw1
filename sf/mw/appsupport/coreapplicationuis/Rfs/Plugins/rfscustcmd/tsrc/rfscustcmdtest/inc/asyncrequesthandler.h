/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
 * Description:
 *
 */

#ifndef ASYNCREQUESTHANDLER_H
#define ASYNCREQUESTHANDLER_H

// SYSTEM INCLUDE FILES
#include <e32base.h>

// DATA TYPES
_LIT( KPanicCat, "ASYNCREQUESTHANDLER" );

/** Panic codes */
enum TPanicCodes
    {
    ENone,
    ERequestPending
    };

// CLASS DECLARATION

/**
 * A template class for handling asynchronous requests.
 *
 * @lib None.
 * @since S60 TB9.2
 */
template <class T>
NONSHARABLE_CLASS( CAsyncRequestHandler ): public CActive
    {
public:
    
    // TYPE DEFINTIONS
    
    /** HandleIssueRequest callback */
    typedef void ( T::*HandleIssueRequest )( TRequestStatus& );
    
    /** HandleRunL callback */
    typedef void ( T::*HandleRunL )( TInt );
    
    /** HandleRunError callback */
    typedef TInt ( T::*HandleRunError )( TInt );
    
    /** HandleDoCancel callback */
    typedef void ( T::*HandleDoCancel )();
    
    // DATA TYPES
    
    /** Request type */
    enum TAsyncRequestType
        {
        ERequestOneShot,
        ERequestContinuous
        };
    
public:

    /**
     * Symbian two phased constructor.
     * 
     * @since S60 TB9.2
     * @param None.
     * @return CAsyncRequestHandler*
     */
    static CAsyncRequestHandler* NewL( T& aPtr,
        HandleIssueRequest aHandleIssueRequest,
        HandleRunL aHandleRunL,
        HandleRunError aHandleRunError,
        HandleDoCancel aHandleDoCancel,
        TAsyncRequestType aType = ERequestContinuous )
        {
        CAsyncRequestHandler* self = CAsyncRequestHandler::NewLC( aPtr,
            aHandleIssueRequest,
            aHandleRunL,
            aHandleRunError,
            aHandleDoCancel,
            aType );
        CleanupStack::Pop( self );
        return self;
        }
    
    /**
     * Symbian two phased constructor.
     * Instance is left in the cleanup stack.
     * 
     * @since S60 TB9.2
     * @param None.
     * @return CAsyncRequestHandler*
     */
    static CAsyncRequestHandler* NewLC( T& aPtr,
        HandleIssueRequest aHandleIssueRequest,
        HandleRunL aHandleRunL,
        HandleRunError aHandleRunError,
        HandleDoCancel aHandleDoCancel,
        TAsyncRequestType aType = ERequestContinuous )
        {
        CAsyncRequestHandler* self = new CAsyncRequestHandler( aPtr,
            aHandleIssueRequest,
            aHandleRunL,
            aHandleRunError,
            aHandleDoCancel,
            aType );
        CleanupStack::PushL( self );
        return self;
        }

    /**
     * C++ destructor.
     */
    virtual ~CAsyncRequestHandler()
        {
        Cancel();
        }
    
public: // New methods
    
    /**
     * Issues a new request.
     * Panic will occur if there already is a request pending.
     * 
     * @since TB9.2
     * @param None.
     * @return None.
     */
    void IssueRequest()
        {
        __ASSERT_DEBUG( !IsActive(),
            User::Panic( KPanicCat, ERequestPending ) );
        
        // Call the HandleIssueRequest from the template class and set active
        ( iPtr.*iHandleIssueRequest )( iStatus );
        SetActive();
        }

protected: // From base classes
    
    // @see CActive
    void RunL()
        {
        // Check result and issue request again
        TInt status = iStatus.Int();
        if( iType == ERequestContinuous )
            {
            IssueRequest();
            }
        
        // Call the HandleRunL from the template class
        ( iPtr.*iHandleRunL )( status );
        }

    // @see CActive
    TInt RunError( TInt aError )
        {
        // Call the HandleRunError from the template class
        TInt err = ( iPtr.*iHandleRunError )( aError );
        return err;
        }

    // @see CActive
    void DoCancel()
        {
        // Call the HandleDoCancel from the template class
        ( iPtr.*iHandleDoCancel )();
        }

private:

    CAsyncRequestHandler( T& aPtr,
        HandleIssueRequest aHandleIssueRequest,
        HandleRunL aHandleRunL,
        HandleRunError aHandleRunError,
        HandleDoCancel aHandleDoCancel,
        TAsyncRequestType aType = ERequestContinuous ):
        CActive( CActive::EPriorityStandard ),
        iPtr( aPtr ),
        iHandleIssueRequest( aHandleIssueRequest ),
        iHandleRunL( aHandleRunL ),
        iHandleRunError( aHandleRunError ),
        iHandleDoCancel( aHandleDoCancel ),
        iType( aType )
        {
        CActiveScheduler::Add( this );
        }

private: // Data
    
    /** Pointer to the template class */
    T& iPtr;
    
    /** HandleIssueRequest function pointer */
    HandleIssueRequest iHandleIssueRequest;
    
    /** HandleRunL function pointer */
    HandleRunL iHandleRunL;
    
    /** HandleRunError function pointer */
    HandleRunError iHandleRunError;
    
    /** HandleDoCancel function pointer */
    HandleDoCancel iHandleDoCancel;
    
    /** Request type */
    TAsyncRequestType iType;
    };

#endif // ASYNCREQUESTHANDLER_H
