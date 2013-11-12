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
* Description:  Presence Service Connection plugin factory
 *
*/


#ifndef CPSCPLUGINFACTORY_H
#define CPSCPLUGINFACTORY_H


#include <e32base.h>
#include <ximpbase.h>


class MXIMPFeatureInfo;
class MXIMPProtocolInfo;
class CXIMPFeatureInfoImp;
class CXIMPProtocolPluginBase;


/**
 * Plugin factory and plugin utility.
 * Class is owned by and handled as singleton by CXIMPGlobals.
 *
 * @lib ximpprocessor.dll
 * @since S60 v4.0
 */
class CXIMPPluginFactory : public CBase
    {
    public:

        static CXIMPPluginFactory* NewL();

        ~CXIMPPluginFactory();

    private:

        /**
         * C++ default constructor.
         */
        CXIMPPluginFactory();

        /**
         * Symbian 2nd phase constructor
         */
        void ConstructL();


    public:

        /**
         * Get a list of protocols.
         *
         * Retrieves a list of presence protocols.
         * @return Externalized form of RXIMPObjOwningPtrArray<
         * CXIMPProtocolInfoImp > containing the protocols
         */
        IMPORT_C HBufC8* GetProtocolsL();


        IMPORT_C CXIMPProtocolPluginBase* GetProtocolL( TUid aProtocolUid );


        CXIMPFeatureInfoImp* ParseProtocolInfoLC( const TDesC8& aFeaturesBuffer );


    private:

        void ParseTaggedDataL( const TDesC8& aData,
                               CXIMPFeatureInfoImp* aFeatInfo );


        void ProcessTaggedDataL( const TDesC8& aTag,
                                 const TDesC8& aData,
                                 CXIMPFeatureInfoImp* aFeatInfo );


    private:    // Data

    };

#endif      // CPSCPLUGINFACTORY_H


