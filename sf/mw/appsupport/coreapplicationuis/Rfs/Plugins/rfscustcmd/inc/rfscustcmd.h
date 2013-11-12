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
* Declaration of CRfsCustCmd class.
*
*/

#ifndef RFSCUSTCMD_H
#define RFSCUSTCMD_H

#include <e32base.h>
#include <rfsClient.h>
#include <ssm/ssmadaptationcli.h>
#include <ssm/ssmcustomcommand.h>

/**
 *  Each command is implemented in a class derived from MSsmCustomCommand.
 *  It is also possible to implement multiple similar command in the same
 *  class and differentiate between them based on constructor parameters
 *  from the factory method.
 */
NONSHARABLE_CLASS( CRfsCustCmd ) : public CBase, public MSsmCustomCommand
    {

public:

    /**
    * RFS types.
    */
    enum TRfsCustCmdType
        {
        ERfsCustCmdFirstBoot = 1,
        ERfsCustCmdDeep,
        ERfsCustCmdPreUi,
        ERfsCustCmdPostUi
        };
        
    /**
     * Two-phased constructor
     * @param aRfsType Type of rfs to be executed
     */
	static CRfsCustCmd* NewL( TRfsCustCmdType aRfsType );

    /**
     * Destructor
     */
    virtual ~CRfsCustCmd();

private: // From MSsmCustomCommand

    /**
     * @see MSsmCustomCommand
     */
    TInt Initialize( CSsmCustomCommandEnv* aCmdEnv );

    /**
     * @see MSsmCustomCommand
     */
	void Execute( const TDesC8& aParams, TRequestStatus& aRequest );

    /**
     * @see MSsmCustomCommand
     */
	void ExecuteCancel();

    /**
     * @see MSsmCustomCommand
     */
	void Close();

    /**
     * @see MSsmCustomCommand
     */
	void Release();

private:
    
    /**
     * Constructor
     */
    CRfsCustCmd( TRfsCustCmdType aRfsType );

private: // data

    /**
    * Type of rfs to be executed.
    */
    TRfsCustCmdType iRfsType;
    
    /**
    * RFS Server session.
    */
    RRfsClient iRfs;

    /**
    * Handle to adaptation.
    */
    RSsmStateAdaptation iAdaptation;
    };

#endif // RFSCUSTCMD_H
