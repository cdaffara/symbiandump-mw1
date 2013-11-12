/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Event handler for CFW server.
*
*/


#ifndef M_CFSTARTEREVENTHANDLER_H
#define M_CFSTARTEREVENTHANDLER_H

/**
 *  Interface for CFW starter events.
 *
 *  @lib -
 *  @since S60 v3.2
 */
class MCFStarterEventHandler
    {
    public:
        
        /**  CFW starter events */
        enum TCFStarterEvents
            {
            // Context source plugins loaded event
            EContextSourcePluginsLoaded,
            
            // Action plugins loaded event
            EActionPluginsLoaded
            };
        
        /**
         * Handles events.
         * 
         * @since S60 3.2
         * @param aEvent CFW starter event
         * @return None.
         */
        virtual void HandleEvent( TCFStarterEvents aEvent ) = 0;          
    };

#endif // M_CFSTARTEREVENTHANDLER_H
