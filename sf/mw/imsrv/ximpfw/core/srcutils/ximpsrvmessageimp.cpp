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


//  INCLUDE FILES
#include "ximpsrvmessageimp.h"
#include <e32base.h>

// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::NewOrNull()
// -----------------------------------------------------------------------------
//
CXIMPSrvMessageImp* CXIMPSrvMessageImp::NewOrNull(
    const RMessage2& aMessage,
    MXIMPSrvMessage*& aSelfPtr,
    CXIMPSrvMessageImp*& aSourcePtr )
    {
    __ASSERT_ALWAYS( !aSelfPtr,
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::EMessageSelfPtrAlreadyOccupied ) );
    if( !aSourcePtr )
        {
        aSourcePtr = new CXIMPSrvMessageImp( aMessage, aSelfPtr ); // CSI: 62 #
                                                                              // Special non-leaving function
        }
    else
        {
        aSourcePtr->Init( aMessage, aSelfPtr );
        }
    return aSourcePtr;
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::~CXIMPSrvMessageImp()
// -----------------------------------------------------------------------------
//
CXIMPSrvMessageImp::~CXIMPSrvMessageImp()
    {
    //If assert below is triggered, it means
    //that owned message is not completed before
    //deleting the wrapper
    __ASSERT_ALWAYS( iMessage.IsNull(),
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::EMessageStillPendingOnDestruction ) );

    if( iSelfPtr )
        {
        iSelfPtr = NULL;
        }
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::CXIMPSrvMessageImp()
// -----------------------------------------------------------------------------
//
CXIMPSrvMessageImp::CXIMPSrvMessageImp( const RMessage2& aMessage,
                                        MXIMPSrvMessage*& aSelfPtr )
    : iMessage( aMessage ),
      iSelfPtr( &aSelfPtr )
    {
    }

// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::Init()
// -----------------------------------------------------------------------------
//
void CXIMPSrvMessageImp::Init( const RMessage2& aMessage,
                                        MXIMPSrvMessage*& aSelfPtr )
    {
    iMessage = aMessage;
    iSelfPtr = &aSelfPtr;
    }

// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::Complete()
// -----------------------------------------------------------------------------
void CXIMPSrvMessageImp::Complete( TInt aReason )
    {
    if( !iMessage.IsNull() )
        {
        iMessage.Complete( aReason );
        }
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::IsCompleted()
// -----------------------------------------------------------------------------
TBool CXIMPSrvMessageImp::IsCompleted() const
    {
    return iMessage.IsNull();
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::Function()
// -----------------------------------------------------------------------------
TInt CXIMPSrvMessageImp::Function() const
    {
    return iMessage.Function();
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::Int()
// -----------------------------------------------------------------------------
TInt CXIMPSrvMessageImp::Int( TParamIndex aIndex ) const
    {
    TInt value = 0;
    switch( aIndex )
        {
        case Ep0:
            {
            value = iMessage.Int0();
            break;
            }

        case Ep1:
            {
            value = iMessage.Int1();
            break;
            }

        case Ep2:
            {
            value = iMessage.Int2();
            break;
            }

        case Ep3:
            {
            value = iMessage.Int3();
            break;
            }
        }

    return value;
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::GetDesLengthL()
// -----------------------------------------------------------------------------
TInt CXIMPSrvMessageImp::GetDesLengthL( TParamIndex aIndex ) const
    {
    return iMessage.GetDesLengthL( aIndex );
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::ReadL()
// -----------------------------------------------------------------------------
void CXIMPSrvMessageImp::ReadL( TParamIndex aIndex, TDes8& aDes ) const
    {
    return iMessage.ReadL( aIndex, aDes );
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::WriteL()
// -----------------------------------------------------------------------------
void CXIMPSrvMessageImp::WriteL( TParamIndex aIndex, const TDesC8& aDes ) const
    {
    return iMessage.WriteL( aIndex, aDes );
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::PanicClientAndLeaveL()
// -----------------------------------------------------------------------------
void CXIMPSrvMessageImp::PanicClientAndLeaveL( NXIMPPanic::TReason aReason ) const
    {
    iMessage.Panic( NXIMPPanic::KCategory, aReason );
    User::Leave( KErrCompletion );
    }


// -----------------------------------------------------------------------------
// CXIMPSrvMessageImp::PlaceOwnershipHere()
// -----------------------------------------------------------------------------
void CXIMPSrvMessageImp::PlaceOwnershipHere( MXIMPSrvMessage*& aNewSelfPtr )
    {
    __ASSERT_ALWAYS( !aNewSelfPtr,
                     User::Panic( NXIMPPrivPanic::KCategory,
                                  NXIMPPrivPanic::EMessageSelfPtrAlreadyOccupied ) );

    //remove ownership from old pointer
    (*iSelfPtr) = NULL;

    //Cache new owning pointer
    iSelfPtr = &aNewSelfPtr;

    //And setup new owning pointer to point self
    aNewSelfPtr = this;
    }


// End of file
