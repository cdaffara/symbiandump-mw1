
/*
 * Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 * This component and the accompanying materials are made available
 * under the terms of "Eclipse Public License v1.0"
 * which accompanies this distribution, and is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 * Initial Contributors:
 * Nokia Corporation - initial contribution.
 *
 * Contributors:
 * Description : ATCmdplugin class declaration.
 *
 */



#ifndef __ATCMDPLUGINECOM_H
#define __ATCMDPLUGINECOM_H

// INCLUDES
#include <atextpluginbase.h>
#include <w32std.h>
#include <hwrmlight.h>
#include "matcmdtimer.h"
#include "atcmd.h"

// FORWARD DECLARATIONS
class CCoeEnv;

// CLASS DECLARATION

/**
*  CATCmdPluginEcom
*  -
*/
class CATCmdPluginEcom : public CATExtPluginBase, public MATCmdTimer, public MHWRMLightObserver
  	{
  	public: // Constructors, destructor
        /**
        * Two-phased constructor.
        */
        static CATCmdPluginEcom* NewL();
        
        /**
        * C++ Destructor.
        */
        ~CATCmdPluginEcom();
        
        /**
         * From MHWRMLightObserver
         */
        void LightStatusChanged (TInt aTarget, CHWRMLight::TLightStatus aStatus);
        
        void HandleCommandCancel();
        
        void ReportConnectionName( const TDesC8& aName );
        
        
        TInt NextReplyPartLength();

        /**
         * Gets the next part of reply initially set by HandleCommandComplete().
         * Length of aNextReply must be equal or less than KDefaultCmdBufLength.
         *
         * @since S60 5.0
         * @param aNextReply Next reply
         * @return Symbian error code on error, KErrNone otherwise
         */
        TInt GetNextPartOfReply( RBuf8& aNextReply ) ;

        /**
         * Receives unsolicited results. Cancelling of the pending request is done
         * by ReceiveUnsolicitedResultCancel(). The implementation in the extension
         * plugin should be asynchronous.
         *
         * @since S60 5.0
         * @return None
         */
        void ReceiveUnsolicitedResult();

        /**
         * Cancels a pending ReceiveUnsolicitedResult request.
         *
         * @since S60 5.0
         * @return None
         */
        void ReceiveUnsolicitedResultCancel();

        /**
         * Reports NVRAM status change to the plugins.
         *
         * @since S60 5.0
         * @param aNvram New NVRAM status. Each call of this function is a result
         *               of DUN extracting the form notified by
         *               CATExtCommonBase::SendNvramStatusChange(). Each of the
         *               settings from SendNvramStatusChange() is separated to
         *               one call of ReportNvramStatusChange().
         * @return None
         */
        void ReportNvramStatusChange( const TDesC8& aNvram );

        /**
         * Reports about external handle command error condition.
         * This is for cases when for example DUN decided the reply contained an
         * error condition but the plugin is still handling the command internally.
         * Example: in command line "AT+TEST;ATDT1234" was given. "AT+TEST" returns
         * "OK" and "ATDT" returns "CONNECT". Because "OK" and "CONNECT" are
         * different reply types the condition is "ERROR" and DUN ends processing.
         * This solution keeps the pointer to the last AT command handling plugin
         * inside ATEXT and calls this function there to report the error.
         * It is to be noted that HandleCommandCancel() is not sufficient to stop
         * the processing as the command handling has already finished.
         *
         * @since S60 5.0
         * @return None
         */
        void ReportExternalHandleCommandError();


        
        enum TPluginStatus
            {
            ECmdCkpd,
            ECmdCbklt,
            EKeepAlive
            };
            
        enum TLightStatus
            {
            ELightUninitialized = -1,
            ELightOff = 0,
            ELightOnWithDuration,
            ELightOn
            };

    private: // From base classes

        // @see CCFActionPlugIn
        void HandleCommand( const TDesC8& aCmd, RBuf8& aReply, TBool aReplyNeeded );

        // @see CCFActionPlugIn
        TBool IsCommandSupported( const TDesC8& aCmd );

    private: // New implementation

        /**
        * AT commands handling
        * @param aCmd
        * @param aReply
        */
        void HandleCommandL( const TDesC8& aCmd );                          


    private: // CATCmdTimer
		/**
        * CATCmdTimer
        */
	    class CATCmdTimer
	        : public CTimer
		    {
	        public: // Constructor, destructor
		        /**
                * Constructor
                */
		        CATCmdTimer( MATCmdTimer& aObserver, TInt aRelatedCommand );

		        /**
                * 2nd phase constructor
                */
		        void ConstructL();

	        public: // from CActive
		        /**
                * Timer callback
                */
		        virtual void RunL();
		        
		    private: // data
		        /** */
		        MATCmdTimer& iObserver;
		        TInt iRelatedCommand;
		    };
    
    private: // from MATCmdTimer
        void TimerExpiredL( TInt aRelatedCommand );
        
    private:
        void LoadResourcesL();

        void HandleCkpdCommandL( const TDesC8& aCmd );
        
        void HandleCbkltCommandL( const TDesC8& aCmd );
        void HandleCtsaCommandL( const TDesC8& aCmd );

        void ParseParametersL( const TDesC8& aCmd, 
                               TInt& aStrokeTime, 
                               TInt& aStrokeDelay );

        void SendKeyL();

        void SendNextKeyL();

        void SimulateRawEventL( TKeyEvent& aKey, TEventCode aEvent  );
        
        void EnableBackLight();
        
        void DisableBackLight();
        
    private:
		/**
        * @param aCmd
        */
        //void ATCmdPNDRecv( CATCommand& aCmd );

    private: // Constructors
        /**
        * C++ Constructor
        */
        CATCmdPluginEcom();

        /**
        * Symbian 2nd phase constructor
        */
        void ConstructL();

    private: // Data
        /** Owns: */
        CArrayFix<TATCkpdCmd>* iCkpdCmdArray;

        /** Owns: */
        CATCmdTimer* iKpdTimer;
        
        CATCmdTimer* iBkltTimer;
        
        CATCmdTimer* iKeepAliveTimer;
        
        /** Owns: */
        CArrayPtr<CATCmdTranslation>* iTranslations;

        TEventCode iLastEvent;
        
        /** Owns: */
        CHWRMLight* iLight;
        
        RWsSession iWsSession;
        
        TPluginStatus iCmdReceived;
        
        TLightStatus iLightStatus;
        
        TInt iDuration;
        
        RBuf8* iReplyBuf;
    };

#endif // __ATCMDPLUGINECOM_H

// End of File
