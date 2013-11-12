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
* Description:  CSysApConnectionMonitorObserver class definition.
*
*/


#ifndef SYSAPCONNECTIONMONITOROBSERVER_H
#define SYSAPCONNECTIONMONITOROBSERVER_H

// INCLUDES
#include <e32base.h>
#include <rconnmon.h>

// FORWARD DECLARATIONS
class CSysApAppUi;

// CLASS DECLARATION
/**
*  CSysApConnectionMonitorObserver class. This class observer changes in Alarm Server.
*
*  @lib   sysap
*  @since 1.0*  
*/

class CSysApConnectionMonitorObserver : 
	public CActive,
	public MConnectionMonitorObserver
    {
    public: // Constructors and destructor
        
        /**
        * EPOC default constructor.
        */
      	static CSysApConnectionMonitorObserver* NewL( CSysApAppUi& aSysApAppUi );
        
        /**
        * Destructor.
        */
      	virtual ~CSysApConnectionMonitorObserver();

	   	/**
    	* From connmon
    	* @since Series60 2.6
    	* @param CConnMonEventBase
    	*/
    	void EventL( const CConnMonEventBase &aConnMonEvent );
    	
    	/**
    	* From connmon
    	* @since Series60 3.0
    	* @return TInt bearer value
    	*/
    	TInt GetBearerValue( );
    	
    protected:

        // From CActive
        void RunL();
        void DoCancel();

    private:

        /**
        * EPOC default constructor.
        * @param None
        * @return void
        */
        void ConstructL( );
        
        /**
        * Constructor
        * @param CSysApAppUi& aSysApAppUi
        * @return void
        */         
        CSysApConnectionMonitorObserver( CSysApAppUi& aSysApAppUi );
        
        /**
        * C++ default constructor.
        * @param None
        * @return void
        */
        CSysApConnectionMonitorObserver();
        
    
    private:
    
        // By default, prohibit copy constructor
        CSysApConnectionMonitorObserver( const CSysApConnectionMonitorObserver& );
    
        // Prohibit assigment operator
        CSysApConnectionMonitorObserver& operator= ( const CSysApConnectionMonitorObserver& );
    

        

    private:
    
    	CSysApAppUi&  		iSysApAppUi;
        TInt		  		iBearerValue;
        RConnectionMonitor  iConnMon;
        
    };

#endif      // SYSAPCONNECTIONMONITOROBSERVER_H
            
// End of File
