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
* Description:
*
*/

#include "tsservicesprovider.h"
#include "tsservice.h"
// -----------------------------------------------------------------------------
/**
 * Symbian two phase constructor. Create and initialize services provider instance
 * @param aConfig - services provider configurator
 * @return address to initialized services provider instance 
 */
CTsServiceProvider* CTsServiceProvider::NewL( 
                                       MTsResourceManager& aResources,
                                       const CTsServiceProviderConfig& aConfig )
    {
    CTsServiceProvider* self = new (ELeave)CTsServiceProvider();
    self->Construct( aResources, aConfig );
    return self;
    }

// -----------------------------------------------------------------------------
/**
 * Constructor
 */
CTsServiceProvider::CTsServiceProvider()
    {
    //No implementation required
    }

// -----------------------------------------------------------------------------
/**
 * Function initialzie services provider. Using configurator load serivces
 * @param aConfig - serivces provider configurator
 * 
 */
void CTsServiceProvider::Construct( MTsResourceManager& aResources, 
                                    const CTsServiceProviderConfig& aConfig )
    {
    CTsService* srvPtr(0);
    for( TInt iter(0); iter < aConfig.Count(); ++iter ) 
        {
        TRAP_IGNORE( srvPtr = CTsService::NewLC(aResources, aConfig.LoadL( iter ) );
                     iServices.AppendL( srvPtr );
                     CleanupStack::Pop( srvPtr ); )
        }
    }

// -----------------------------------------------------------------------------
/**
 * Destructor. Release allocated resouces
 */
CTsServiceProvider::~CTsServiceProvider()
    {
    iServices.ResetAndDestroy();
    }

// -----------------------------------------------------------------------------
/**
 * Operator enable access to initialized service
 * @param aOffset - index of requested service
 * @return reference to MTsModel interface
 */
MTsModel& CTsServiceProvider::operator[](TInt aOffset) const
    {
    return *( iServices[ aOffset ] );
    }

// -----------------------------------------------------------------------------
/**
 * @return number of available services
 */
TInt CTsServiceProvider::Count() const
    {
    return iServices.Count();
    }
