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
* Description:  Context interface
*
*/

#ifndef MXIMPPSCCONTEXT_H
#define MXIMPPSCCONTEXT_H

class CPresenceInfoFilterImp;
class CXIMPIdentityImp;

/**     
 * Context interface.
 *
 * @since S60 v3.2
 */
class MXIMPPscContext
    {

public: // Enumerations

    enum TFilterStoreType
        {
        EOwnPresenceFilter,
        EPresentityPresenceFilter,
        EPresentityGroupMemberPresenceFilter,
        EGrantPresenceToEveryoneFilter,
        EGrantPresenceToPresentityFilter,
        EGrantPresenceToGroupMemberFilter
        };
    
public: // Interface

    /*
     * Access to current presence interest. 
     * @param Type of filter to access.
     * @param aIdentity Identity to map filter. Default NULL
     * @return Presence interest. NULL if not defined.
     */
    virtual CPresenceInfoFilterImp* PresenceInfoFilter( TFilterStoreType aTypeOfFilter, 
                                                            const CXIMPIdentityImp* aIdentity = NULL ) const = 0;

protected: // Destruction
    
    /**
     * Object cannot be deleted through this interface
     */
    virtual ~MXIMPPscContext(){}
    };

/* ======================================================================== */



#endif // MXIMPPSCCONTEXT_H
