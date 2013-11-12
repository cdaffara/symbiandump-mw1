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
* Description:  Declaration of CRfsTestAppUi class
*
*/


#ifndef RFSTESTAPPUI_H
#define RFSTESTAPPUI_H

#include <aknappui.h>
#include <aknprogressdialog.h>

class CDataCreator;
class CRfsTestView;
class CAknPopupList;
class CAknSinglePopupMenuStyleListBox;
/**
*  AppUi class for RfsTestApp application.
*
*  @since S60 S60 5.1
*/
class CRfsTestAppUi : public CAknAppUi, public MProgressDialogCallback
    {

public:

    /**
    * First phase constructor.
    */
    CRfsTestAppUi();

    /**
    * Second phase constructor.
    */
    void ConstructL();

    /**
    * Destructor.
    */
    virtual ~CRfsTestAppUi();

    /**
    * Call-back function called when data creation has completed.
    */
    void DataCreated();

private: // From CEikAppUi

    /**
    * @see CEikAppUi
    */
    void HandleCommandL( TInt aCommand );

private: // From MProgressDialogCallback

    /**
    * @see MProgressDialogCallback.
    */
    void DialogDismissedL( TInt aButtonId );

private:

    /**
    * Query test files from user and setup data for a test case.
    */
    void SetupTestL( const TDesC& aSpec, const TDesC& aExclude, 
        const TDesC& aExcludeTarget );
    
    /**
    * List all exclude lists from C and Z.
    */
    void ListListsL();

    /**
    * Finish showing progress bar and delete the progress bar object.
    */
    void ClearProgressBar();
    
    /**
    * Creates list box.
    */
    void CreateListBoxL();
    
    /**
    * Cleans list box.
    */
    void CleanListBox();
    
    /**
    * Collects exclude files from specified drive.
    */
    void ListsFilesOnDriveL( TPtr aBuf, TChar aDrive );

private: // data

    /** Main view of the application. Not NULL. Own. */
    CRfsTestView* iView;

    /** Manages data creation process. May be NULL. Own. */
    CDataCreator* iDataCreator;

    /** Progress bar. May be NULL. Own. */
    CAknProgressDialog* iProgress;

    /** List model. Own. */
	CDesCArray* iListModel;

    /** Listbox showing list of exclude files in system. Own. */
	CAknSinglePopupMenuStyleListBox* iListBox;

    /** List popup. Own. */
	CAknPopupList* iPopup;

    /** File server session. Own. */
	RFs iFs;

    /** System drive character. */
	TChar iSystemDrive;
    };

#endif // RFSTESTAPPUI_H
