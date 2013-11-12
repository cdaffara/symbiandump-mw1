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
* Description: satmonitor.h
*
*/

#ifndef CASATMONITOR_H_
#define CASATMONITOR_H_

#include "casatnotifier.h"
#include "casrvplugin.h"

class CCaSatNotifier;
class CCaStorageProxy;

/**
 *  SAT Handler.
 *  @lib casathandler.lib
 *  @since S60 v5.0
 */
NONSHARABLE_CLASS( CCaSatMonitor ):
    public CCaSrvPlugin,
    public MCaSatNotifierCallback
    {
    public:

    /**
     * Two-phased constructor. Leaves on failure.
     * @param aPLuginParam plugin param consist of storage, storage utils.
     * @return The constructed object.
     */
    static CCaSatMonitor* NewL( TPluginParams* aPluginParams );

    /**
     * Destructor.
     * @since S60 v5.0
     * @capability None.
     * @throws None.
     * @panic None.
     */
    virtual ~CCaSatMonitor();

private:

    /**
     * Constructor.
     * @param aCaStorageProxy storage.
     */
    CCaSatMonitor( CCaStorageProxy& aCaStorageProxy );

    /**
     * 2nd phase constructor.
     */
    void ConstructL();

    /**
     * Gets sat's app name.
     * @param aName name of sat app.
     * @return KErrNone, if successful;
     * KErrPermissionDenied, if the caller process doesn't pass the Read Policy;
     * KErrNotFound, if the name has not been defined;
     */
    TInt GetSatName( TDes& aName );

    /**
     * Gets sat's app visibility.
     * @param aVisibility visibility of sat app.
     * @return KErrNone, if successful;
     * KErrPermissionDenied, if the caller process doesn't pass the Read Policy;
     * KErrNotFound, if the name has not been defined;
     */
    TInt GetSatVisibility( TInt& aVisibility );

private:
    // from MCaSatNotifierCallback

    /**
     * @since Series 60 5.0
     * @return void
     */
    void SatChangeL();

private:
    // data

    /**
     * Data storage.
     * Not own.
     */
    CCaStorageProxy& iCaStorageProxy;

    /**
     * Sat notifier for name change.
     */
    CCaSatNotifier* iCaSatNotifierName;

    /**
     * Sat notifier for icon change.
     */
    CCaSatNotifier* iCaSatNotifierIcon;

    /**
     * Sat notifier for visibility change.
     */
    CCaSatNotifier* iCaSatNotifierVis;

    };

#endif /* CASATMONITOR_H_ */
