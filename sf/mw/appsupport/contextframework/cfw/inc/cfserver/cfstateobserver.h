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
* Description:  CF server events observer interface.
*
*/


#ifndef M_CFSTATEOBSERVER_H
#define M_CFSTATEOBSERVER_H


/**
* Interface for observing CF server events.
*/
class MCFStateObserver
    {
    public:
    
        /**
        * UpdatePlugIns.
        * This is called when the CF server detects a change in ECom
        * plugin registrations (adding, removing or upgrading ECOM-plugins).
        * 
        * @since S60 5.0
        * @return None.
        */
        virtual void UpdatePlugInsL() = 0;    
    };

#endif //M_CFSTATEOBSERVER_H
