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
* Description:  implementation of CXIMPFeatureManager
*
*/

#ifndef C_CPSCFEATUREMANAGER_H
#define C_CPSCFEATUREMANAGER_H


#include <e32base.h>

class CRepository;

/**
 * ?one_line_short_description
 *
 * ?more_complete_description
 *
 * @lib ?library
 * @since S60 v4.0
 */
class CXIMPFeatureManager : public CBase
    {

public:

    static CXIMPFeatureManager* NewL();


    virtual ~CXIMPFeatureManager();


private:

    CXIMPFeatureManager();
    void ConstructL();


public:

    /**
     * ?description
     *
     * @since S60 ?S60_version
     * @param ?arg1 ?description
     * @param ?arg2 ?description
     * @return ?description
     */
    IMPORT_C HBufC8* GetFrameworkFeaturesL();


private:


private: // data

    /**
     * ?description_of_member
     */
    //?type ?member_name;

    /**
     * Pointer to a cenrep client
     * Own. 
     */
    CRepository* iCenRep;

    /**
     * ?description_of_pointer_member
     * Not own.  *** Write "Not own" if some other class owns this object.
     */
    //?type* ?member_name;

    };


#endif // C_CPSCFEATUREMANAGER_H
