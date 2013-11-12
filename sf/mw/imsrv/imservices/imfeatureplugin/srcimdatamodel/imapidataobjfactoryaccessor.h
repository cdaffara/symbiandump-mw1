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
* Description:  Codec to pack and unpack data objects.
*
*/

#ifndef IMAPIDATAOBJFACTORYACCESSOR_H
#define IMAPIDATAOBJFACTORYACCESSOR_H

#include "apidataobjfactoryaccessor.h"

class MEventCodec;
class CImEventCodec;
class MApiDataObjFactory;
class CImApiDataObjFactory;
class MApiDataObjFactoryAccessor;
/**
 * Class to access the different object factories in the datamodel
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CImApiDataObjFactoryAccessor ) : public MApiDataObjFactoryAccessor
    {
public:
    IMPORT_C static CImApiDataObjFactoryAccessor* NewL();
    
    MApiDataObjFactory& ApiDataObjFactory();
    
    MEventCodec& EventCodec();
    
    ~CImApiDataObjFactoryAccessor();

private:

    CImApiDataObjFactoryAccessor();
    void ConstructL();
    
//data
private:
    // Own
    CImEventCodec* iImEventCodec;
    
    // Own
    CImApiDataObjFactory* iImApiDataObjFactory;

    };

#endif // IMAPIDATAOBJFACTORYACCESSOR_H






