/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  MCFPropertyListenerCallBack interface declaration.
*
*/



#ifndef M_CFPROPERTYLISTENERCALLBACK_H
#define M_CFPROPERTYLISTENERCALLBACK_H

/**
* Property listener call back interface.
* 
* @lib -
* @since S60 4.0
*/
class MCFPropertyListenerCallBack
    {
    public:
    
        /**
        * Notifies that property value has changed.
        * 
        * @since S60 4.0
        * @param None
        * @return None
        */
        virtual void HandlePropertyChangedL() = 0;
    
    protected:
    
        // Deny destruction through this class
        virtual ~MCFPropertyListenerCallBack(){};
    };
    
#endif
