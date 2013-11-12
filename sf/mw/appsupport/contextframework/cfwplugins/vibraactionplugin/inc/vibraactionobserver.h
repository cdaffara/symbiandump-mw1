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
* Description:  CVibraActionObserver class declaration.
*
*/



#ifndef M_VIBRAACTIONOBSERVER_H
#define M_VIBRAACTIONOBSERVER_H

/**
* Vibra action observer interface.
*
* @lib -
* @since S60 4.0
*/
NONSHARABLE_CLASS( MVibraActionObserver )
    {
    public:
    
        /**
        * Notifies that started vibra action is completed.
        * 
        * @since S60 4.0
        * @param None.
        * @return None.
        */
        virtual void VibraActionCompletedL() = 0;
    
    protected:
    
        // Deny destruction through this class
        ~MVibraActionObserver(){};
    };
    
#endif
