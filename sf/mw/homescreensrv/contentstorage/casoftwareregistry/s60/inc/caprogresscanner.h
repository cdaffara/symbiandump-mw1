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
 * Description:  ?Description
 *
 */

#ifndef CAPROGRESSCANER_H
#define CAPROGRESSCANER_H

#include <e32base.h>
#include <usif/sif/sifnotification.h>
#include <usif/usifcommon.h>
#include <usif/sif/sifcommon.h>
#include <usif/scr/scr.h>
#include "cauninstallobserver.h"

using namespace Usif;


/**
 *  Uninstall Progress notifier.
 *
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaProgresScanner ) : public CBase, public MSifOperationsHandler
    {

public:
    /**
     * Creates an instance of CCaProgresScanner.
     */
IMPORT_C static CCaProgresScanner* NewL( IUninstallObserver* aObserver );

    /**
     * Destructor.
     */
    virtual ~CCaProgresScanner();

private:

    /**
     * Constructor.
     */
    CCaProgresScanner( IUninstallObserver* aObserver );

    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL( );

    void UpdateProgressL( const CSifOperationProgressData& aProgressData );
    void EndOperationL( TInt aError );
    void SubscribeL( TUint aKey );
    void CancelSubscribeL();

public:
    /**
     * From MSifOperationsHandler
     */
    void StartOperationHandler(
        TUint aKey,
        const CSifOperationStartData& aStartData );

    /**
     * From MSifOperationsHandler
     */
    void EndOperationHandler( const CSifOperationEndData& aEndData );

    /**
     * From MSifOperationsHandler
     */
    void ProgressOperationHandler(
        const CSifOperationProgressData& aProgressData );

private:

    /**
     * USIF notifier
     * Own.
     */
    CSifOperationsNotifier* iNotifier;

    /**
     * Reference to Software component registry. Not own.
     */
    RSoftwareComponentRegistry iSoftwareRegistry;

    TUint iKey;

    TComponentId iComponentId;
    
    const IUninstallObserver* iObserver;
    };


#endif /* CAPROGRESSCANER_H */

// End of File


