/*
 ============================================================================
 Name		: t_oomtestappApplication.h
 Author	  : 
 Copyright   : Your copyright notice
 Description : Declares main application class.
 ============================================================================
 */

#ifndef __T_OOMTESTAPPAPPLICATION_H__
#define __T_OOMTESTAPPAPPLICATION_H__

// INCLUDES
#include <aknapp.h>

// CLASS DECLARATION

/**
 * Ct_oomtestappApplication application class.
 * Provides factory to create concrete document object.
 * An instance of Ct_oomtestappApplication is the application part of the
 * AVKON application framework for the t_oomtestapp example application.
 */
class Ct_oomtestappApplication : public CAknApplication
	{
public:
	Ct_oomtestappApplication();
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID
	 */
	TUid AppDllUid() const;

	~Ct_oomtestappApplication();
protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates Ct_oomtestappDocument document object. The returned
	 * pointer in not owned by the Ct_oomtestappApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	
private:
	};

#endif // __T_OOMTESTAPPAPPLICATION_H__
// End of File
