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
* Description:  Presence Service Connection globals holder
 *
*/

#ifndef CPSCGLOBALS_H
#define CPSCGLOBALS_H

#include <e32base.h>

class CXIMPPluginFactory;
class CXIMPFeatureManager;
class CXIMPHostManager;
class MXIMPOperationFactory;
class MXIMPGlobalsObserver;

/**
 * Singleton for XIMP Framework client IF library.
 *
 * @lib ximpprocessor.dll
 * @since S60 v4.0
 */
class CXIMPGlobals : public CBase
    {
    private:

        /** Private nested structure to hold global objects */
        struct SGlobals;

    public:  // singleton access

        /**
         * Singleton access method.
         *
         * Gets pointer to Psc Globals singleton.
         *
         */
        IMPORT_C static CXIMPGlobals* Instance();

        IMPORT_C static void InstallL( MXIMPOperationFactory* aOperationFactory, MXIMPGlobalsObserver* aGlobalsObserver );

        IMPORT_C static void UninstallD();

    private:

        /**
         * Two-phased constructor.
         * Leaves created object on the CleanupStack.
         */
        static CXIMPGlobals* NewLC( MXIMPOperationFactory* aOperationFactory,
               MXIMPGlobalsObserver* aGlobalsObserver );

        
        /**
         * C++ default constructor.
         */
        CXIMPGlobals();

        
        /**
         * Private destructor.
         *
         * Singleton holder is reference counted.
         * It is automaticly destroyed when last
         * holded object or reference is destroyed.
         */
        ~CXIMPGlobals();

        /**
         * Symbian 2nd phase constructor
         */
        void ConstructL( MXIMPOperationFactory* aOperationFactory,
                MXIMPGlobalsObserver* aGlobalsObserver );

    public:

        IMPORT_C CXIMPPluginFactory* PluginFactory();
        IMPORT_C CXIMPFeatureManager* FeatureManager();
        IMPORT_C CXIMPHostManager* HostManager();       
        IMPORT_C MXIMPOperationFactory* OperationFactory();

        /**
         * Host manager informs the globals that there are no more hosts.
         * This is signalled to the server. The point is to keep the
         * server alive for the right time.
         */
        IMPORT_C void NoMoreHosts();

        // IMPORT_C CPscVariant* PscVariant);

    private:    // Data

        /**
         * Object holder having pointers to global objects.
         * Both holder and pointed objects owned.
         */
        SGlobals* iGlobals;

    };

#endif      // CPSCGLOBALS_H


