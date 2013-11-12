// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// apsiconcaptionoverride.h
//

#ifndef __APSICONCAPTIONOVERRIDE_H__
#define __APSICONCAPTIONOVERRIDE_H__

#include <e32base.h>
#include <e32hashtab.h>


class CRepository;
class CApaIconCaptionOverrideStore;

/**
Exposes access to an application's configuration information to the Apparc Server.

Retrieves the caption, icon information from the store (CApaIconCaptionOverrideStore).

Reads the configuration information with reference to the language downgrade path, with
ELangNone used as the default language, if no better match is found.

@internalComponent
@see CApaIconCaptionOverrideStore
*/
NONSHARABLE_CLASS(TApaIconCaptionOverrideReader)
	{
public:
	TApaIconCaptionOverrideReader(const CApaIconCaptionOverrideStore& aStore, const RArray<TLanguage>& aLanguageDowngradePath);
	const TDesC* ShortCaption() const;
	const TDesC* Caption() const;
	TBool NumIconsSet() const;
	TInt NumIcons() const;
	const TDesC* IconFileName() const;

private:
	const TDesC* GetString(TUint32 aFieldId) const;
	const TInt* GetInt(TUint32 aFieldId) const;
	
private:
	const CApaIconCaptionOverrideStore& iStore;
	const RArray<TLanguage>& iLanguageDowngradePath;
	};

/**
Stores the icon and caption overrides for an application.

@internalComponent
*/
NONSHARABLE_CLASS(CApaIconCaptionOverridesForApp) : public CBase
	{
public:
	static CApaIconCaptionOverridesForApp* NewL();
	~CApaIconCaptionOverridesForApp();
	CApaIconCaptionOverridesForApp();
	void ConstructL();
	
	void LoadFromCenRepL(CRepository* aRepository, TUint32 aAppKey);
	TApaIconCaptionOverrideReader Reader(const RArray<TLanguage>& aLanguageDowngradePath) const;
	
private:
	CApaIconCaptionOverrideStore* iStore;
	};

/**
Stores the icon and caption override data indexed by application UID.
Keeps a map of app UIDs to configuration information for that application.

@internalComponent
*/
NONSHARABLE_CLASS(CApaIconCaptionOverrides) : public CBase
	{
public:
	CApaIconCaptionOverrides();
	~CApaIconCaptionOverrides();

	void LoadFromCenRepL();
	void LoadFromCenRepL(CRepository* aRepository);

	CApaIconCaptionOverridesForApp* OverrideForApp(TUid aAppUid) const;
	void SetOverrideForAppL(TUid aAppUid, CApaIconCaptionOverridesForApp* aOverrides);	// takes ownership aOverrides

private:
	void DeleteOverrides();

private:
	typedef RHashMap<TInt, CApaIconCaptionOverridesForApp*> RAppOverrideMap;
	RAppOverrideMap iMap;
	};
	
/**
Watches for changes in the Central Repositiory and update the overrides when they occur.

@internalComponent   
*/
NONSHARABLE_CLASS(CApaIconCaptionCenrepObserver) : public CActive
	{
public:
	static CApaIconCaptionCenrepObserver* NewL(CApaIconCaptionOverrides& aOverrides);
	CApaIconCaptionCenrepObserver(CApaIconCaptionOverrides& aOverrides);
	~CApaIconCaptionCenrepObserver();
	void ConstructL();
	
	void LoadOverridesL();
	
private:
	void Start();
	// from CActive
	void RunL();
	void DoCancel();
	
private:
	CApaIconCaptionOverrides& iOverrides;
	CRepository* iRepository;	// owned
	};

#endif // __APSICONCAPTIONOVERRIDE_H__
