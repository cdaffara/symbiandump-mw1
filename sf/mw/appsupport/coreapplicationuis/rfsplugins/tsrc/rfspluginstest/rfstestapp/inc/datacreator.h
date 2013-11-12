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
* Description:  Declaration of CDataCreator class
*
*/


#ifndef DATACREATOR_H
#define DATACREATOR_H

#include <e32base.h>

/**
* Active object which manages test data creation process.
* Starts testdatacreator.exe and waits for it to finish.
*/
class CDataCreator : public CActive
    {

public:

    /**
    * Constructs a CDataCreator object.
    *
    * @param aCallBack Identifies a call-back function to call when loading is
    * complete.
    * @return The new object
    */
    static CDataCreator* NewL(
        const TDesC& aSpecFile,
        const TDesC& aExcludeFile,
        const TDesC& aExcludeTargetFile,
        const TCallBack& aCallBack );

    /**
    * Destructor.
    */
    virtual ~CDataCreator();

private:

    /**
    * First phase constructor.
    *
    * @param aCallBack Identifies a call-back function to call when loading is
    * complete.
    */
    CDataCreator( const TCallBack& aCallBack );

    /**
    * Second phase constructor.
    */
    void ConstructL( const TDesC& aSpecFile, const TDesC& aExcludeFile,
        const TDesC& aExcludeTargetFile );

private: // From CActive

    /**
    * @see CActive
    */
    void DoCancel();

    /**
    * @see CActive
    */
    void RunL();

private: // data

    /** Call-back function to call when action completes. */
    TCallBack iCallBack;

    /** Process responsible of the data operations. */
    RProcess iDataCreator;

    /** Name of the exclude file to use. */
    HBufC* iExcludeFileName;

    /** Name of target exclude file. */
    HBufC* iExcludeTargetFileName;

    /** Internal state of the object. */
    enum TState
        {
        ESetupData,
        ESetupExcludeList
        };
    TState iState;
    };

#endif // DATACREATOR_H
