/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MImConversationEvent API object implementation.
 *
*/

#ifndef CIMCONVERSATIONEVENTIMP_H
#define CIMCONVERSATIONEVENTIMP_H

#include  <s32strm.h>
#include "ximpapieventbase.h"
#include <imconversationevent.h>
//#include "ximpdatasubscriptionstateimp.h"
#include "imapiobjbase.h"


class RReadStream;
class CXIMPIdentityImp;
class CImConversationInfoImp;
class CXIMPDataSubscriptionStateImp;


/**
 * MImConversationEvent API object implementation.
 *
 * @lib Imdatamodel.dll
 *
 */
NONSHARABLE_CLASS( CImConversationEventImp ): public CXIMPApiEventBase,public MImConversationEvent
    {
public:
    /** The class ID. */
    enum { KClassId = IMIMP_CLSID_CIMCONVERSATIONEVENTIMP };

public:
    /**
     * Exported instantiation method for initializing
     * new event object.
     */
    IMPORT_C static CImConversationEventImp* NewLC(
            CImConversationInfoImp* aConvinfo
            );

    /**
     * Instantiation method for event automation.
     * Method signature must be exactly this to work
     * with event delivery automation system.
     *
     * Event implementation must be registered to
     * ImEventCodec KImEventConstructorTable.
     */
    static CXIMPApiEventBase* NewFromStreamLC( RReadStream& aStream );

    virtual ~CImConversationEventImp();

private:
    CImConversationEventImp();

    void ConstructL( CImConversationInfoImp* aConvinfo );
            

    void ConstructL( RReadStream& aStream );

    

    /**
     * Helper for internalize
     */
    void InternalizeL( RReadStream& aStream ) ;
            


public: // From API base interfaces

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
      XIMPIMP_DECLARE_IF_BASE_METHODS

    /**
     * Implementation of MImEventBase interface and
     * CImApiEventBase methods
     *
     * @see MImEventBase
     * @see CImApiEventBase
     */
   	XIMPIMP_DECLARE_EVENT_BASE_METHODS


public: // From MImConversationEvent
	/**
     * 
     * @see MImConversationEvent
     */

     TInt NewTextMessageCount() const;
	 const MImConversationInfo& NewTextMessage( TInt aIndex ) const ;

    

private: // data

	/**
     * Conversation Info
     * Own.
     */
     CImConversationInfoImp* iConvInfo;

    };


#endif // CIMCONVERSATIONEVENTIMP_H
