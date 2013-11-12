/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of MXIMPContext
*
*/

#ifndef CPRESENCEFEATURESIMP_H
#define CPRESENCEFEATURESIMP_H

#include <presencefeatures.h>
#include "ximpapiobjbase.h"
#include "presenceapiobjbase.h"

class CPresencePublishingImp;
class CPresenceObjectFactoryImp;
class CPresentityGroupsImp;
class CPresenceWatchingImp;
class CPresenceAuthorizationImp;
class MXIMPContext;
class MXIMPContextInternal;


/**
 * MXIMPContext API object implementation.
 *
 * @lib ximpmanager.dll
 * @since S60 v3.2
 */
NONSHARABLE_CLASS( CPresenceFeaturesImp ):  public MPresenceFeatures,
                                            public CXIMPApiObjBase
                                           
    {
    public:
    /** The class ID. */
    enum { KClassId = PRIMP_CLSID_CPRESENCEFEATURESIMP };


public:

    IMPORT_C static CPresenceFeaturesImp* NewL( MXIMPContext* aContext );
    virtual ~CPresenceFeaturesImp();


private:

    CPresenceFeaturesImp();
    void SetCtxL(MXIMPContext* aContext);
    void ConstructL(MXIMPContext* aContext);


public: // From MXIMPBase

    /**
     * Implementation of MXIMPBase interface methods
     * @see MXIMPBase
     */
    XIMPIMP_DECLARE_IF_BASE_METHODS


public: //From MPresenceFeatures

   
    MPresenceObjectFactory& PresenceObjectFactory() const;
    MPresenceWatching& PresenceWatching() const;
    MPresencePublishing& PresencePublishing() const;
    MPresentityGroups& PresentityGroups() const;
    MPresenceAuthorization& PresenceAuthorization() const;

//private: // Helpers

    /**
     * Converts a string to a uid.
     * String must be in Hex and begin with "0x".
     */
    //void ConvertStringToUidL( const TDesC8& aData, TUid& aUid );


private: // data

    /**
     * Context for having a connection context server.
     */
    MXIMPContextInternal* iCtxInternal;
    

    /**
     * Object factory sub interface.
     */
    CPresenceObjectFactoryImp* iPresObjFactory;

    /**
     * Own presence management sub interface.
     */
    CPresencePublishingImp* iPresPub;

    /**
     * Presentity group management sub interface
     */
    CPresentityGroupsImp* iPresGroup;

    /**
     * Presence watching management sub interface
     */
    CPresenceWatchingImp* iPresWatch;
    
    /**
     * Presence authorization management sub interface
     */
    CPresenceAuthorizationImp* iPresAuth;
    
    /**
     * Event pump.
     * Own.
     */
   // CXIMPContextEventPump* iEventPump;
    };


#endif // CPRESENCEFEATURESIMP_H
