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
* Description:  Configuration item interface
*
*/

#ifndef MXIMPPSCCONFIGURATIONITEM_H
#define MXIMPPSCCONFIGURATIONITEM_H

//class MXIMPPscContext;

/**
 * Context interface.
 *
 * @since S60 v3.2
 */
class MXIMPConfigurationItem
    {
    
public: // Interface

        
protected: // Destruction
    
    /**
     * Object cannot be deleted through this interface
     */
    virtual ~MXIMPConfigurationItem(){}
    };

/* ======================================================================== */

#endif // MXIMPPSCCONFIGURATIONITEM_H
