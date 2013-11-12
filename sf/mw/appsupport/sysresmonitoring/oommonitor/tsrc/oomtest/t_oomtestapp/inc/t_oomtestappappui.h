/*
 ============================================================================
 Name		: t_oomtestappAppUi.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares UI class for application.
 ============================================================================
 */

#ifndef __T_OOMTESTAPPAPPUI_h__
#define __T_OOMTESTAPPAPPUI_h__

// INCLUDES
#include <aknappui.h>
#include <oommonitorsession.h>

// FORWARD DECLARATIONS
class Ct_oomtestappAppView;

const TInt KKiloByte = 1024;

// CLASS DECLARATION
/**
 * Ct_oomtestappAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class Ct_oomtestappAppUi : public CAknAppUi
	{
public:
		
	// Constructors and destructor

	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * Ct_oomtestappAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 */
	Ct_oomtestappAppUi();

	/**
	 * ~Ct_oomtestappAppUi.
	 * Virtual Destructor.
	 */
	virtual ~Ct_oomtestappAppUi();

	TInt AllocatedHeap() const;

	TPtrC Priority() const;

private:
	// Functions from base classes

	/**
	 * From CEikAppUi, HandleCommandL.
	 * Takes care of command handling.
	 * @param aCommand Command to be handled.
	 */
	void HandleCommandL(TInt aCommand);

	/**
	 *  HandleStatusPaneSizeChange.
	 *  Called by the framework when the application status pane
	 *  size is changed.
	 */
	void HandleStatusPaneSizeChange();

private:
	// Data

	/**
	 * The application view
	 * Owned by Ct_oomtestappAppUi
	 */
	Ct_oomtestappAppView* iAppView;

    ROomMonitorSession iOomSession;
    
    TAny* iMem;

    ROomMonitorSession::TOomPriority iPriority;

	};

#endif // __T_OOMTESTAPPAPPUI_h__
// End of File
