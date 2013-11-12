/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: casrvplugin.h
 *
 */

#ifndef CA_SRV_PLUGIN_H_
#define CA_SRV_PLUGIN_H_

#include <e32base.h>
#include <ecom/ecom.h>
#include "casrvplugindef.h"

class CCaStorageProxy;
class CCaSrvEngUtils;
namespace Usif{
    class RSoftwareComponentRegistry;
}

struct TPluginParams
    {
    CCaStorageProxy* storageProxy;
    CCaSrvEngUtils* engUtils;
    Usif::RSoftwareComponentRegistry* softwareRegistry;
    };

/**
 * CaSrvPlugin is a class representing ECom plugin.
 * Class has virtual function responsible for executing
 * command. It can also returns array of all implementations.
 * Class is deriving by all ECom plugin implementations.
 *
 * @since S60 5.0
 * @ingroup
 */
NONSHARABLE_CLASS( CCaSrvPlugin ): public CBase
    {
public:
    /**
     * Instantiates an object of this type.
     * @param aUid ECom plugin's UID.
     * @param aPluginParam The plugin's parameters.
     * @return Instance of the CaSrvPlugin class.
     */
    static CCaSrvPlugin* NewL( TUid aUid, TPluginParams* aPluginParam );

    /**
     * Virtual C++ Destructor.
     */
    virtual ~CCaSrvPlugin();

    /**
     * Request a list of all available implementations which
     * satisfy this given interface.
     * @param aImplInfoArray ECom plugin implementations array.
     */
    static void ListAllImplementationsL( RImplInfoPtrArray& aImplInfoArray );

protected:
    /**
     * Default constructor.
     */
    inline CCaSrvPlugin();

private:

    /**
     * Unique instance identifier key.
     */
    TUid iDtor_ID_Key;
    };

#include "casrvplugin.inl"

#endif /* CA_SRV_PLUGIN_H_ */

// End of file
