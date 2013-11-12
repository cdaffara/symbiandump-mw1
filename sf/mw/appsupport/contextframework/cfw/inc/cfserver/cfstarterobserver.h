/*
* Copyright (c) 2006-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Initialization phase interface.
*
*/


#ifndef M_CFSTARTEROBSERVER_H
#define M_CFSTARTEROBSERVER_H

#include "cfphasebase.h"
#include "cfstartereventhandler.h"

/**
* Interface for initilize phases.
* Context Framework server will inform when to initialize a phase.
*/
class MCFStarterObserver
    {
    public:
    
        /**
        * Informs initialization phases.
        * 
        * @since S60 4.0
        * @param aPhase Current phase.
        * @return None.
        */
        virtual void InitializePhaseL( CCFPhaseBase::TCFPhaseId aPhaseId ) = 0;
        
        /**
        * Sets event handler to observers.
        * 
        * @since S60 3.2
        * @param aEventHandler Event handler.
        * @return None.
        */
        virtual void SetEventHandler( MCFStarterEventHandler& aEventHandler ) = 0;
    
    protected:
    
        // Deny destruction through this class
        virtual ~MCFStarterObserver(){};
    };

#endif//M_CFSTARTEROBSERVER_H
