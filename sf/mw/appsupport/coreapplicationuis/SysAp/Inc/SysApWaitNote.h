/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CSysApWaitNote class definition. The class uses CAknGlobalNote
*                to show a wait note..
*
*/


#ifndef SYSAPWAITNOTE_H
#define SYSAPWAITNOTE_H

// INCLUDES
#include <e32base.h>
#include <AknGlobalNote.h>
#include "SysAp.hrh"

// CONSTANTS


// CLASS DECLARATION
/**
*  CSysApWaitNote
*  
*  @lib   sysap
*  @since 1.2* 
*/

class CSysApWaitNote: public CBase
{
public:

    /**
    * Constructor
    */         
    static CSysApWaitNote* NewL(TBool aCoverDisplaySupported);

    /**
    * Destructor.
    */
    virtual ~CSysApWaitNote();

    /**
    * This method shows the waitnote.
    * @param  aNote          -- note id from sysap.hrh 
    * @param  aNoteStringBuf -- localized text 
    * @return void
    */         
    void ShowNoteL(const TSysApWaitNoteIds aNote, HBufC* aNoteStringBuf);
    
    /**
    * This method cancels the waitnote..
    * @param  None
    * @return void
    */         
    void Cancel();

private:

    /**
    * Constructors
    */         
    CSysApWaitNote(TBool aCoverDisplaySupported);
    void ConstructL();

private:

    CAknGlobalNote*          iWaitNote;
    TInt                     iNoteId;
    TBool                    iActive;
    TBool                    iCoverDisplaySupported;
};

#endif // SYSAPWAITNOTE_H

// End of File
