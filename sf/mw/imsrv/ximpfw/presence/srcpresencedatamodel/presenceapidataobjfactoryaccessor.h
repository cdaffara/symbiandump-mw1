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

#ifndef PRESENCEAPIDATAOBJFACTORYACCESSOR_H
#define PRESENCEAPIDATAOBJFACTORYACCESSOR_H

#include "apidataobjfactoryaccessor.h"

class MEventCodec;
class CPresenceEventCodec;
class MApiDataObjFactory;
class CPresenceApiDataObjFactory;

/**
 * Class to access the different object factories in the datamodel
 *
 * @lib ximpdatamodel.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceApiDataObjFactoryAccessor ) : public MApiDataObjFactoryAccessor
    {
public:
    IMPORT_C static CPresenceApiDataObjFactoryAccessor* NewL();
    
    MApiDataObjFactory& ApiDataObjFactory();
    
    MEventCodec& EventCodec();
    
    ~CPresenceApiDataObjFactoryAccessor();

private:

    CPresenceApiDataObjFactoryAccessor();
    void ConstructL();
    
//data
private:
    // Own
    CPresenceEventCodec* iPresenceEventCodec;
    
    // Own
    CPresenceApiDataObjFactory* iPresenceApiDataObjFactory;

    };

#endif // PRESENCEAPIDATAOBJFACTORYACCESSOR_H






