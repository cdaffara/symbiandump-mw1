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
* Description:  Implementation of MSearchObjectFactory
 *
*/

#ifndef CSEARCHOBJECTFACTORYIMP_H
#define CSEARCHOBJECTFACTORYIMP_H

#include <searchobjectfactory.h>
#include "ximpapiobjbase.h"
#include "searchapiobjbase.h"


/**
 * MSearchObjectFactory API object implementation.
 *
 * @lib searchdatamodel.dll
 * 
 */
class CSearchObjectFactoryImp: public CXIMPApiObjBase,
							   public MSearchObjectFactory
    {
    public:
    /** The class ID. */
    enum { KClassId = IMP_CLSID_CSEARCHOBJECTFACTORYIMP };


public:

    IMPORT_C static CSearchObjectFactoryImp* NewL();
    ~CSearchObjectFactoryImp();

private:
    CSearchObjectFactoryImp();



public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MSearchObjectFactory
	/**
     * Implementation of  MSearchObjectFactory interface method
     * @see  MSearchObjectFactory
     */
    MSearchElement* NewSearchElementLC();
    /**
     * Implementation of  MSearchObjectFactory interface method
     * @see  MSearchObjectFactory
     */
    MSearchInfo* NewSearchInfoLC();
    /**
     * Implementation of  MSearchObjectFactory interface method
     * @see  MSearchObjectFactory
     */
    MSearchKeyInfo* NewSearchKeyInfoLC();
   
    };


#endif // CSEARCHOBJECTFACTORYIMP_H
