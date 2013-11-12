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
* Description:  Concrete server message wrapper implementation.
 *
*/

#ifndef CXIMPSRVMESSAGEIMP_H
#define CXIMPSRVMESSAGEIMP_H

#include "ximpsrvmessage.h"
#include "ximppanics.h"
#include <e32base.h>

/**
 * Server message wrapper implementation.
 *
 * @lib ximputils.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPSrvMessageImp ) : public CBase,
                                          public MXIMPSrvMessage
    {
public:

    /**
     * Special instantiation method..
     *
     * Verifies that given self pointer ins't yet in use
     * and allocates new instance. Returns NULL pointer if in OOM.
     */
    static CXIMPSrvMessageImp* NewOrNull( const RMessage2& aMessage,
                                          MXIMPSrvMessage*& aSelfPtr,
                                          CXIMPSrvMessageImp*& aSourcePtr );
    ~CXIMPSrvMessageImp();


private:
    CXIMPSrvMessageImp( const RMessage2& aMessage,
                        MXIMPSrvMessage*& aSelfPtr );
                        
    void Init( const RMessage2& aMessage,
                        MXIMPSrvMessage*& aSelfPtr );



public: // Methods from MXIMPSrvMessage

    void Complete(TInt aReason);
    TBool IsCompleted() const;
    TInt Function() const;
    TInt Int( TParamIndex aIndex ) const;
    TInt GetDesLengthL( TParamIndex aIndex ) const;
    void ReadL( TParamIndex aIndex, TDes8& aDes ) const;
    void WriteL( TParamIndex aIndex, const TDesC8& aDes ) const;
    void PanicClientAndLeaveL( NXIMPPanic::TReason aReason ) const;
    void PlaceOwnershipHere( MXIMPSrvMessage*& aNewSelfPtr );

private: // Data


    /**
     * The message.
     */
    RMessage2 iMessage;

    /**
     * Pointer to self pointer.
     */
    MXIMPSrvMessage** iSelfPtr;

    };



#endif      //  CXIMPSRVMESSAGEIMP_H


