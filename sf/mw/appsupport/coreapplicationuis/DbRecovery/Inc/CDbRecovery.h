/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
*     DbRecovey is an independent exe which will check databases and 
*     try to fix them before phone is booted. The state of the database
*     is written in the Shared data.
*
*/


#ifndef CDBRECOVERY_H
#define CDBRECOVERY_H

// INCLUDES

#include <d32dbms.h>
#include <e32property.h>

// MACROS

// If DBRECOVERY_WRITE_LOGS is defined, a log file is written into
// c drive root directory. (currently not used)
/*
#ifdef _DEBUG
#define DBRECOVERY_WRITE_LOGS
#endif
*/

// TRACES

#define TRACE_ADDPREFIX(aText) (TPtrC((const TText *)L"DbRecovery: \"" L##aText L"\""))

#ifdef _DEBUG
#define TRACES(aMsg) RDebug::Print( TRACE_ADDPREFIX(aMsg) )
#define TRACES1(aFormat,aP1) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1) )
#define TRACES2(aFormat,aP1,aP2) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2) )
#define TRACES3(aFormat,aP1,aP2,aP3) RDebug::Print( TRACE_ADDPREFIX(aFormat),(aP1),(aP2),(aP3) )
#else
#define TRACES(aMsg)
#define TRACES1(aFormat,aP1)
#define TRACES2(aFormat,aP1,aP2)
#define TRACES3(aFormat,aP1,aP2,aP3)
#endif


// Represents single database.
/* (currently not used)
struct TSingleDatabase
    {
    TSingleDatabase( const TDesC& aName, const TDesC& aSFId, const TUint32& aKey )
        : iName(aName), iSFId(aSFId), iKey(aKey) {}

    const TDesC& iName;
    const TDesC& iSFId;
    const TUint32 iKey;
    };

/**
* class which does the check and the recovery
*/
class CDbRecovery: public CBase
    {
    public: // Constuctor and destructor
        static CDbRecovery* NewLC();
        ~CDbRecovery();

    public: // New functions
    
        /**
        * Does the contacts database check.
        */    
        void DoContactsDbCheckL();
        
        /** (common db check currently not used ) 
        * Does the database check. Leaves only because OOM.
        */
        //void DoDatabaseCheckL();

        /**
        * Check that system.ini file is OK.
        */
        void CheckSystemIniFileL();

    protected: // Constructor
        CDbRecovery();
        void ConstructL();

    private: // Data
        RProperty iProperty;
	    // RDbs iDbsSession;      // database connection
    };

#endif // CDBRECOVERY_H
            
// End of File
