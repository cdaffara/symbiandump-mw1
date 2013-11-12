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
* Description:  A callback interface for the plug-ins running in the SysAp process.
*
*/

#ifndef SYSAPCALLBACK_H
#define SYSAPCALLBACK_H

#include <e32cmn.h>

class RAknKeylock2;
class CHWRMLight;

/**
 *  An interface for controlling System Application and querying information from it.
 *
 *  This interface is intended for plug-ins that run in the System Application process.
 *  Plug-ins can execute commands to be performed by the System Application and also query
 *  information from the System Application.
 *
 *  @lib None
 *  @since S60 3.1
 */
class MSysapCallback
    {
    public:
        
        /**
        * Sysap command identifiers.
        *
        * @see MSysapCallback::ExecCommandL
        */
        enum TSysapCommand
            {
            EResetKeyguardState, ///< reset the internal keyguard flags of the SysAp
            ECancelPowermenu, ///< hide power menu if it is visible
            
            /** 
            * Updates the light state if necessary.
            *
            * The key event code due to which update takes place is passed 
            * as a parameter, so that SysAp can perform key event specific light handling.
            * Note that calling this method does not necessarily trigger any light related actions,
            * if there is no key specific light handling implemented in SysAp.
            */
            EUpdateLights 
            };
        
        /**
        * Executes a command in SysAp.
        *
        * @since S60 3.1
        * @param[in] aCommand   command identifier
        * @leave a system wide error code
        *
        * @see MSysapCallback::TSysapCommand
        */
        virtual void ExecCommandL( TSysapCommand aCommand ) = 0;
        
        /**
        * Executes a command in SysAp.
        *
        * @since S60 3.1
        * @param aCommand   command identifier
        * @param aParam     command specific parameter data
        * @leave a system wide error code
        *
        * @see MSysapCallback::TSysapCommand
        */
        virtual void ExecCommandL( TSysapCommand aCommand, const TDesC8& aParam ) = 0;
      
        /**
        * Sysap query identifiers.
        *
        * @see MSysapCallback::ExecQueryL
        */
        enum TSysapQuery
            {
            EGetHwrmLight, ///< get handle to HWRM client session. Returns CHWRMLight*.
            EGetKeylock ///< get handle to keylock client session. Returns RAknKeylock2*.
            };    
        
        /**
        * Queries information from SysAp.
        *
        * @since S60 3.1
        * @param[in]  aQuery    query identifier
        * @param[out] aReturn   query specific return data 
        * @leave a system wide error code
        *
        * @see MSysapCallback::TSysapQuery
        */
        virtual void ExecQueryL( TSysapQuery aQuery, TDes8& aReturn ) = 0;
        
        /**
        * Queries information from SysAp.
        *
        * @since S60 3.1
        * @param[in]  aQuery    query identifier
        * @param[out] aReturn   query specific return data 
        * @param[in]  aParam    query specific parameter data
        * @leave a system wide error code
        *
        * @see MSysapCallback::TSysapQuery
        */
        virtual void ExecQueryL( TSysapQuery aQuery, TDes8& aReturn, const TDesC8& aParam ) = 0;
    };


/**
*   Package buffer type definition for getting pointer to HWRM light session instance.
*   This buffer is passed with query EGetHwrm.
*   @see MSysapCallback::EGetHwrm
*/    
typedef TPckgBuf<CHWRMLight*> THwrmLightBuf;

/**
*   Package buffer type definition for getting pointer to keylock session instance.
*   This buffer is passed with query EGetKeylock.
*   @see MSysapCallback::EGetKeylock
*/    
typedef TPckgBuf<RAknKeylock2*> TKeyLockBuf;

/**
* Package buffer type definition for light update command.
* Possible value is a TKeyCode enumeration defined in e32keys.h (or redefinition somewhere else).
* @see MSysapCallback::EUpdateLights 
*/
typedef TPckgBuf<TUint> TUpdateLightsBuf;


#endif // SYSAPCALLBACK_H


