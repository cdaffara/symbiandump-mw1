/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Declaration of CGSAccProfileList class
*
*/


#ifndef GSACCPROFILELIST_H
#define GSACCPROFILELIST_H

#include <badesca.h> // For CDesCArrayFlat
#include <e32base.h>

class CCoeEnv;

/**
*  CGSAccProfileList reads profile information from profile engine and can
*  map user selections to profile ids and other way round.
*/
class CGSAccProfileList : public CBase
    {

public:

    /**
    * Constructs a CGSAccProfileList object.
    *
    * @return The new object
    */
    static CGSAccProfileList* NewL();

    /**
    * Destructor.
    */
    virtual ~CGSAccProfileList();

public:

    /**
    * Return a list of profile names.
    *
    * @param aCoeEnv Used to construct the profile list if it does not yet
    * exist.
    * @return Profile names in list format.
    */
    const MDesCArray& NameListL( CCoeEnv* aCoeEnv );

    /**
    * Return profile id by index in the array.
    *
    * @param aIndex Index to the profile array.
    * @param aCoeEnv Used to construct the profile list if it does not yet
    * exist.
    * @return Profile id.
    */
    TInt IdByIndexL( const TInt aIndex, CCoeEnv* aCoeEnv );

    /**
    * Return index to profile names array by profile id.
    *
    * @param aProfileId Profile id to find.
    * @param aCoeEnv Used to construct the profile list if it does not yet
    * exist.
    * @return Index to profile array.
    */
    TInt IndexByIdL( const TInt aProfileId, CCoeEnv* aCoeEnv );

    /**
    * Return profile name by profile id.
    *
    * @param aProfileId Profile id to find.
    * @param aCoeEnv Used to construct the profile list if it does not yet
    * exist.
    * @return Profile name.
    */
    TPtrC NameByIdL( const TInt aProfileId, CCoeEnv* aCoeEnv );

private:

    /**
    * First phase constructor.
    */
    CGSAccProfileList();

    /**
    * Check if profile list has not yet been created and create it.
    *
    * @param aCoeEnv Used to construct the profile list if it does not yet
    * exist.
    */
    void LoadProfileListL( CCoeEnv* aCoeEnv );

    /**
    * Check if the index is valid in the profile array and return it or the
    * default if is it not valid.
    *
    * @param aIndex Index to check.
    * @return A valid index.
    */
    TInt ValidateIndex( TInt aIndex ) const;

private: // data

    /** Profile names as a list. Only created if needed. Own. */
    CDesCArrayFlat* iProfileNames;

    /** Profile ids as a list. Same order as the names. */
    RArray<TInt> iProfileIds;

    };

#endif // GSACCPROFILELIST_H
