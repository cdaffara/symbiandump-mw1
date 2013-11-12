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
* Description:  MXIMPContextStateEventImp API object implementation.
 *
*/

#ifndef CXIMPPRESENCECONTEXTSTATEEVENTIMP_H
#define CXIMPPRESENCECONTEXTSTATEEVENTIMP_H

#include "ximpapieventbase.h"
#include <ximpcontextstate.h>
#include <ximpcontextstateevent.h>


class RWriteStream;
class RReadStream;
class MXIMPStatus;
class CXIMPStatusImp;
class CXIMPContextStateImp;


/**
 * MXIMPContextStateEventImp API object implementation.
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CXIMPContextStateEventImp ): public CXIMPApiEventBase,
    public MXIMPContextStateEvent
    {
    public:
        /** The class ID. */
        enum { KClassId = XIMPIMP_CLSID_CXIMPCONTEXTSTATEEVENTIMP };

    public:

        /**
         * Exported instantiation method for initializing
         * new event object.
         */
        IMPORT_C static CXIMPContextStateEventImp* NewLC();
        IMPORT_C static CXIMPContextStateEventImp* NewL();


        /**
         * Instantiation method for event automation.
         * Method signature must be exactly this to work
         * with event delivery automation system.
         *
         * Event implementation must be registered to
         * XIMPEventCodec KXIMPEventConstructorTable.
         */
        static CXIMPApiEventBase* NewFromStreamLC( RReadStream& aStream );

        virtual ~CXIMPContextStateEventImp();

        
    private:
        CXIMPContextStateEventImp();
        void ConstructL();
        void ConstructL( RReadStream& aStream );


    public: // From API base interfaces

        /**
         * Implementation of MXIMPBase interface methods
         * @see MXIMPBase
         */
        XIMPIMP_DECLARE_IF_BASE_METHODS


        /**
         * Implementation of MXIMPEventBase interface and
         * CXIMPApiEventBase methods
         *
         * @see MXIMPEventBase
         * @see CXIMPApiEventBase
         */
        XIMPIMP_DECLARE_EVENT_BASE_METHODS


    public: // From MXIMPContextStateEvent

        /**
         * Implementation of MXIMPContextStateEvent interface methods
         * @see MXIMPContextStateEventImp
         */
        const MXIMPStatus* StateChangeReason() const;
        const MXIMPContextState& ContextState() const;
        
        IMPORT_C CXIMPContextStateImp& ContextStateImp();
        IMPORT_C void SetStateChangeReasonImp( CXIMPStatusImp* aStateChangeReason );
        IMPORT_C CXIMPStatusImp* StateChangeReasonImp();
        
        

    private: // data

        /**
         *
         * Own.
         */
        CXIMPContextStateImp* iState;


        /**
         * State change reason.
         * Own.
         */
        CXIMPStatusImp* iStateChangeReason;
    };


#endif // CXIMPPRESENCECONTEXTSTATEEVENTIMP_H
