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
//

#include "tzuserdata.h"
#include <tz.h>
#include <f32file.h>
#include <e32property.h>
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <tzusernames.h>
#include <tzuserdefineddata.h>
#endif
#include "tzidinternal.h"

_LIT(KUserDatabaseName, "c:\\private\\1020383e\\SQLite__tzuserdata.db");
_LIT(KTzPrivatePath, "c:\\private\\1020383e\\");

// Transaction constants 
_LIT(KBeginTransaction,"BEGIN TRANSACTION;");
_LIT(KCommitTransaction,"COMMIT;");
_LIT(KRollbackTransaction,"ROLLBACK;");

// Tz Rules Table Parameters
_LIT(KTzRulesTblTzIdParam, ":tzId");
_LIT(KTzRulesTblRuleTypeParam, ":ruleType");
_LIT(KTzRulesTblDSTStartTimeParam, ":dstStartTime");
_LIT(KTzRulesTblDSTStartRefParam, ":dstStartRef"); 
_LIT(KTzRulesTblDSTEndTimeParam, ":dstEndTime");
_LIT(KTzRulesTblDSTEndRefParam, ":dstEndRef");
_LIT(KTzRulesTblOldOffsetParam, ":oldOffset");
_LIT(KTzRulesTblNewOffsetParam, ":newOffset");
_LIT(KTzRulesTblMonthParam, ":month");
_LIT(KTzRulesTblDayParam, ":day");
_LIT(KTzRulesTblWeekParam, ":week");
_LIT(KTzRulesTblTimeRefParam, ":timeRef");
_LIT(KTzRulesTblMinsParam, ":mins");

// Tz Rules Table Index
_LIT(KTzRulesTblRuleTypeIndex, "ruleType");
_LIT(KTzRulesTblDSTStartTimeIndex, "dstStartTime");
_LIT(KTzRulesTblDSTStartRefIndex, "dstStartRef"); 
_LIT(KTzRulesTblDSTEndTimeIndex, "dstEndTime");
_LIT(KTzRulesTblDSTEndRefIndex, "dstEndRef");
_LIT(KTzRulesTblOldOffsetIndex, "oldOffset");
_LIT(KTzRulesTblNewOffsetIndex, "newOffset");
_LIT(KTzRulesTblMonthIndex, "month");
_LIT(KTzRulesTblDayIndex, "day");
_LIT(KTzRulesTblWeekIndex, "week");
_LIT(KTzRulesTblTimeRefIndex, "timeRef");
_LIT(KTzRulesTblMinsIndex, "mins");

// Tz Names Table Parameters
_LIT(KTzNamesTblTzIdParam, ":tzId");
_LIT(KTzNamesTblStdNameParam, ":stdName");
_LIT(KTzNamesTblShortStdNameParam, ":shortStdName");
_LIT(KTzNamesTblDSTNameParam, ":dstName");
_LIT(KTzNamesTblShortDSTNameParam, ":shortDstName");
_LIT(KTzNamesTblCityNameParam, ":cityName");
_LIT(KTzNamesTblRegionNameParam, ":regionName");

// Tz Names Table Index
_LIT(KTzNamesTblTzIdIndex, "tzId");
_LIT(KTzNamesTblStdNameIndex, "stdName");
_LIT(KTzNamesTblShortStdNameIndex, "shortStdName");
_LIT(KTzNamesTblDSTNameIndex, "dstName");
_LIT(KTzNamesTblShortDSTNameIndex, "shortDstName");
_LIT(KTzNamesTblCityNameIndex, "cityName");
_LIT(KTzNamesTblRegionNameIndex, "regionName");

// Identifier Table Index
_LIT(KNextTzIdentifierIndex, "nextTzId");

// InitialStdOffset Table Param
_LIT(KTzInitialStdOffsetTblTzIdParam, ":tzId");
_LIT(KTzInitialStdOffsetTblStdInitialOffsetParam, ":initialStdOffset");

// InitialStdOffset Table Index
_LIT(KTzInitialStdOffsetTblStdInitialOffsetIndex, "initialStdOffset");

// Create statements
_LIT8 (KTzRulesCreateStmnt, "CREATE TABLE rules (tzId INTEGER NOT NULL, ruleType \
							 INTEGER, dstStartTime INT64 NOT NULL, dstStartRef INTEGER, \
							 dstEndTime INT64 NOT NULL, dstEndRef INTEGER, oldOffset \
							 INTEGER NOT NULL, newOffset INTEGER NOT NULL, month INTEGER, \
							 day INTEGER, week INTEGER, timeRef INTEGER, mins INTEGER NOT NULL)");
							 								
_LIT8 (KTzNamesCreateStmnt, "CREATE TABLE names ( tzId INTEGER NOT NULL PRIMARY \
							 KEY, stdName CHAR(255), shortStdName CHAR(10), dstName \
							 CHAR(255), shortDstName CHAR(10), cityName CHAR(255), \
							 regionName CHAR(255))");
							
_LIT8 (KTzIdentifierCreateStmnt, "CREATE TABLE  identifier ( nextTzId INTEGER NOT NULL )");

_LIT8 (KTzInitialStdOffsetCreateStmnt, "CREATE TABLE initialStdOffset (tzId INTEGER NOT NULL, initialStdOffset INTEGER)");

// Insert statements
_LIT8(KInsertRulesStmnt, "INSERT INTO rules (tzId, ruleType, dstStartTime, dstStartRef, \
						  dstEndTime, dstEndRef, oldOffset, newOffset, month, day, \
						  week, timeRef, mins) VALUES (:tzId, :ruleType, :dstStartTime,\
						  :dstStartRef, :dstEndTime, :dstEndRef, :oldOffset, :newOffset,\
						  :month, :day, :week, :timeRef, :mins)");
						  
_LIT8(KInsertNamesStmnt, "INSERT INTO names(tzId, stdName, shortStdName, dstName, shortDstName,\
						  cityName, regionName) VALUES(:tzId, :stdName, :shortStdName, :dstName,\
						  :shortDstName, :cityName, :regionName)");
						  
_LIT(KInsertIdentifierStmnt, "INSERT INTO identifier (nextTzId) VALUES (%d)");

_LIT8(KInsertInitialStdOffsetStmnt, "INSERT INTO initialStdOffset (tzId, initialStdOffset) \
									 VALUES (:tzId, :initialStdOffset)");

//Select statements
_LIT8(KSelectNextTzIdStmnt, "SELECT nextTzId FROM identifier");

_LIT(KSelectRulesStmnt, "SELECT * FROM rules WHERE tzId = %d");

_LIT(KSelectRulesCountStmnt, "SELECT COUNT(tzId) FROM rules WHERE tzId = %d");

_LIT(KSelectNamesCountStmnt, "SELECT COUNT(tzId) FROM names WHERE tzId = %d");

_LIT(KSelectTzIdCountFromNamesStmnt, "SELECT COUNT(tzId) FROM names WHERE tzId = %d");

_LIT(KSelectNamesStmnt, "SELECT * FROM names WHERE tzId = %d");

_LIT(KSelectTzIdFromNamesStmnt, "SELECT tzId FROM names");

_LIT(KSelectInitialStdOffsetStmnt, "SELECT initialStdOffset FROM initialStdOffset WHERE tzId = %d");

//Update statements
_LIT (KUpdateIdentifierStmnt, "UPDATE identifier SET nextTzId = :nextTzId");

//Delete statements
_LIT(KDeleteRulesStmnt, "DELETE FROM rules WHERE tzId = %d");

_LIT(KDeleteNamesStmnt, "DELETE FROM names WHERE tzId = %d");

_LIT(KDeleteInitialStdOffset, "DELETE FROM initialStdOffset WHERE tzId = %d");

const TInt KSqlStringLength = 100;
const TUint KOutOfBoundsUserId = 0x5000;

CTzUserDataDb* CTzUserDataDb::NewL()
	{
	CTzUserDataDb* self = new(ELeave) CTzUserDataDb();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

CTzUserDataDb::CTzUserDataDb()
	{
	}

CTzUserDataDb::~CTzUserDataDb()
	{
	// We don't own the pointers in this array
	iChangeObservers.Close();
	iDatabase.Close();
	}

void CTzUserDataDb::ConstructL()
	{
	OpenL();	
	}

void CTzUserDataDb::Close()
	{
	iDatabase.Close();
	}
		
void CTzUserDataDb::OpenL()
	{
	RFs fs;
	CleanupClosePushL(fs);
	
	// Open the database
	TInt ret = iDatabase.Open(KUserDatabaseName());
	// If the database could not be opened because 
	// the file was not found in the path provided, create the directory
	if(ret == KErrNotFound || ret == KErrPathNotFound)
		{
		User::LeaveIfError(fs.Connect());
		ret = fs.MkDir(KTzPrivatePath);
		// If there are no errors while creating path
		// or the path already exists, create the database
		if(ret == KErrNone || ret == KErrAlreadyExists)
			{
			CreateL();	
			}
		}
	else
		{
		User::LeaveIfError(ret);
		}
	CleanupStack::PopAndDestroy(&fs);
	}
	
void CTzUserDataDb::CreateL()
	{	
	User::LeaveIfError(iDatabase.Create(KUserDatabaseName()));
    iDatabase.Close();    
    CreateSchemaL();
   	}
	
void CTzUserDataDb::CreateSchemaL()
	{
	RSqlStatement stmt;
	CleanupClosePushL(stmt);
	
	User::LeaveIfError(iDatabase.Open(KUserDatabaseName()));
	User::LeaveIfError(stmt.Prepare(iDatabase, KTzRulesCreateStmnt));
	User::LeaveIfError(stmt.Exec());
	stmt.Close();
	
	User::LeaveIfError(stmt.Prepare(iDatabase, KTzNamesCreateStmnt));
	User::LeaveIfError(stmt.Exec());
	stmt.Close();

	User::LeaveIfError(stmt.Prepare(iDatabase, KTzIdentifierCreateStmnt));
	User::LeaveIfError(stmt.Exec());
	stmt.Close();
		
	TBuf<KSqlStringLength> sqlId;
	sqlId.Format(KInsertIdentifierStmnt, KUserTzIdMin);
	User::LeaveIfError(stmt.Prepare(iDatabase, sqlId));
	User::LeaveIfError(stmt.Exec());
	stmt.Close();
	
	User::LeaveIfError(stmt.Prepare(iDatabase, KTzInitialStdOffsetCreateStmnt));
	User::LeaveIfError(stmt.Exec());
	stmt.Close();
	
	CleanupStack::PopAndDestroy(&stmt);
	}
	
TInt CTzUserDataDb::CreateTzL(const CTzRules& aRules, const CTzUserNames& aNames)
	{
	if(iBackupInProgress || iRestoreInProgress)
		{
		User::Leave(KErrLocked);
		}

	RSqlStatement stmt;
	CleanupClosePushL(stmt);
			
	BeginTransactionL();
	
	TInt err = stmt.Prepare(iDatabase, KSelectNextTzIdStmnt);
	User::LeaveIfError(err);
	TInt colIndex = stmt.ColumnIndex(KNextTzIdentifierIndex);
	User::LeaveIfError(colIndex);
	while((err = stmt.Next()) == KSqlAtRow)
		{
		iNextCandidateTzId = stmt.ColumnInt64(colIndex);	
		}
	
	stmt.Close();
	
	// Check if the next available user ID is valid
	if(iNextCandidateTzId == KOutOfBoundsUserId || IsIdInDbL(iNextCandidateTzId))
		{
		iNextCandidateTzId = FindUnusedTzIdL();
		}
		
	// If all the user IDs have been used, leave with KErrOverflow	
	if(iNextCandidateTzId == KOutOfBoundsUserId)
		{
		User::Leave(KErrOverflow);		
		}
				
	TUint retId = iNextCandidateTzId;
	TInt index;	
	TInt count = aRules.Count();
	for(TInt i=0; i<count; i++)
		{
		TTzRule& rule = const_cast<CTzRules&>(aRules)[i];
			
		// Insert a row in the rules
		User::LeaveIfError(stmt.Prepare(iDatabase, KInsertRulesStmnt()));
		
		index = stmt.ParameterIndex(KTzRulesTblTzIdParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt64(index, iNextCandidateTzId));
		index = stmt.ParameterIndex(KTzRulesTblRuleTypeParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iDayRule));
		index = stmt.ParameterIndex(KTzRulesTblDSTStartTimeParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt64(index, rule.iFrom.iTime.Int64()));
		index = stmt.ParameterIndex(KTzRulesTblDSTStartRefParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iFrom.iTimeReference));
		index = stmt.ParameterIndex(KTzRulesTblDSTEndTimeParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt64(index, rule.iTo.iTime.Int64()));		
		index = stmt.ParameterIndex(KTzRulesTblDSTEndRefParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iTo.iTimeReference));
		index = stmt.ParameterIndex(KTzRulesTblOldOffsetParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iOldLocalTimeOffset));
		index = stmt.ParameterIndex(KTzRulesTblNewOffsetParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iNewLocalTimeOffset));
		index = stmt.ParameterIndex(KTzRulesTblMonthParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iMonth));
		index = stmt.ParameterIndex(KTzRulesTblDayParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iDayOfMonth));
		index = stmt.ParameterIndex(KTzRulesTblWeekParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iDayOfWeek));
		index = stmt.ParameterIndex(KTzRulesTblTimeRefParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iTimeReference));
		index = stmt.ParameterIndex(KTzRulesTblMinsParam);
		User::LeaveIfError(index);
		User::LeaveIfError(stmt.BindInt(index, rule.iTimeOfChange));
	
		TInt ret = stmt.Exec();
		stmt.Close();
		
		if(ret < KErrNone)
			{
			User::Leave(ret);
			}			
		}
		
	// Insert a row in the names
	User::LeaveIfError(stmt.Prepare(iDatabase, KInsertNamesStmnt()));
	index = stmt.ParameterIndex(KTzNamesTblTzIdParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindInt64(index, iNextCandidateTzId));
	index = stmt.ParameterIndex(KTzNamesTblStdNameParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindText(index, aNames.StandardName()));
	index = stmt.ParameterIndex(KTzNamesTblShortStdNameParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindText(index, aNames.ShortStandardName()));
	index = stmt.ParameterIndex(KTzNamesTblDSTNameParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindText(index, aNames.DaylightSaveName()));
	index = stmt.ParameterIndex(KTzNamesTblShortDSTNameParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindText(index, aNames.ShortDaylightSaveName()));
	index = stmt.ParameterIndex(KTzNamesTblCityNameParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindText(index, aNames.CityName()));
	index = stmt.ParameterIndex(KTzNamesTblRegionNameParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindText(index, aNames.RegionName()));
	TInt ret = stmt.Exec();
	stmt.Close();
	
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}

	User::LeaveIfError(stmt.Prepare(iDatabase, KInsertInitialStdOffsetStmnt));
	index = stmt.ParameterIndex(KTzInitialStdOffsetTblTzIdParam);
	User::LeaveIfError(index);
	User::LeaveIfError(stmt.BindInt(index, iNextCandidateTzId));
	index = stmt.ParameterIndex(KTzInitialStdOffsetTblStdInitialOffsetParam);
	User::LeaveIfError(index);
	TInt initialOffset = aRules.InitialStdTimeOffset();
	User::LeaveIfError(stmt.BindInt(index, initialOffset));
	ret = stmt.Exec();
	stmt.Close();
	
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}
	
	// Update the next available user ID
	if(iNextCandidateTzId >= KUserTzIdMax)
		{
		iNextCandidateTzId = FindUnusedTzIdL();
		}
	else
		{
		iNextCandidateTzId++;	
		}
	
	//Insert a row with the new available value of tz id in TZ IdentifierTable
	User::LeaveIfError(stmt.Prepare(iDatabase, KUpdateIdentifierStmnt));
	User::LeaveIfError(stmt.BindInt64(0, iNextCandidateTzId));
	ret = stmt.Exec();
	stmt.Close();
	
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}
	
	CommitTransactionL();
	NotifyTzRulesChange(retId, ETzUserDataCreated);
	NotifyTzNamesChange(retId, ETzUserDataCreated);

	CleanupStack::PopAndDestroy(&stmt);
	return retId;	
	}
	
void CTzUserDataDb::ReadTzRulesL(CTzRules& aRules, TUint aTzId)
	{
	if(iBackupInProgress || iRestoreInProgress)
		{
		User::Leave(KErrLocked);
		}
		
	TSqlScalarFullSelectQuery fullSelectQuery(iDatabase);

	RSqlStatement stmt;
	CleanupClosePushL(stmt);
	
	TTzRule tzRule;	

	TBuf<KSqlStringLength> sqlRules;
	sqlRules.Format(KSelectRulesCountStmnt, aTzId);
	TInt count = fullSelectQuery.SelectIntL(sqlRules);
	if(count == 0)
		{
		User::Leave(KErrNotFound);
		}

	sqlRules.Format(KSelectRulesStmnt, aTzId);
	TInt err = stmt.Prepare(iDatabase, sqlRules);
	User::LeaveIfError(err);
	
	TInt ruleTypeColIdx = stmt.ColumnIndex(KTzRulesTblRuleTypeIndex);
	User::LeaveIfError(ruleTypeColIdx);
	TInt startTimeColIdx = stmt.ColumnIndex(KTzRulesTblDSTStartTimeIndex);
	User::LeaveIfError(startTimeColIdx);
	TInt startRefColIdx = stmt.ColumnIndex(KTzRulesTblDSTStartRefIndex);
	User::LeaveIfError(startRefColIdx);
	TInt endTimeColIdx = stmt.ColumnIndex(KTzRulesTblDSTEndTimeIndex);
	User::LeaveIfError(endTimeColIdx);
	TInt endRefColIdx = stmt.ColumnIndex(KTzRulesTblDSTEndRefIndex);
	User::LeaveIfError(endRefColIdx);
	TInt oldOffsetColIdx = stmt.ColumnIndex(KTzRulesTblOldOffsetIndex);
	User::LeaveIfError(oldOffsetColIdx);
	TInt newOffsetColIdx = stmt.ColumnIndex(KTzRulesTblNewOffsetIndex);		
	User::LeaveIfError(newOffsetColIdx);
	TInt monthColIdx = stmt.ColumnIndex(KTzRulesTblMonthIndex);
	User::LeaveIfError(monthColIdx);
	TInt dayColIdx = stmt.ColumnIndex(KTzRulesTblDayIndex);
	User::LeaveIfError(dayColIdx);
	TInt weekColIdx = stmt.ColumnIndex(KTzRulesTblWeekIndex);	
	User::LeaveIfError(weekColIdx);
	TInt timeRefColIdx = stmt.ColumnIndex(KTzRulesTblTimeRefIndex);
	User::LeaveIfError(timeRefColIdx);
	TInt minsColIdx = stmt.ColumnIndex(KTzRulesTblMinsIndex);
	User::LeaveIfError(minsColIdx);
	
	while((err = stmt.Next()) == KSqlAtRow)
		{			
		tzRule.iDayRule = static_cast<TTzRuleDay>(stmt.ColumnInt(ruleTypeColIdx));
		
		tzRule.iFrom.iTime = stmt.ColumnInt64(startTimeColIdx);
			
		tzRule.iFrom.iTimeReference = static_cast<TTzTimeReference>(stmt.ColumnInt(startRefColIdx));
		
		tzRule.iTo.iTime = stmt.ColumnInt64(endTimeColIdx); 
			
		tzRule.iTo.iTimeReference = static_cast<TTzTimeReference>(stmt.ColumnInt(endRefColIdx));
		
		tzRule.iOldLocalTimeOffset = stmt.ColumnInt(oldOffsetColIdx);
		
		tzRule.iNewLocalTimeOffset = stmt.ColumnInt(newOffsetColIdx);
		
		tzRule.iMonth = static_cast<TMonth>(stmt.ColumnInt(monthColIdx));
		
		tzRule.iDayOfMonth = stmt.ColumnInt(dayColIdx);
		
		tzRule.iDayOfWeek = stmt.ColumnInt(weekColIdx);
		
		tzRule.iTimeReference = static_cast<TTzTimeReference>(stmt.ColumnInt(timeRefColIdx));
			
		tzRule.iTimeOfChange = stmt.ColumnInt(minsColIdx);
		
		aRules.AddRuleL(tzRule);	
		}
	stmt.Close();
	
	sqlRules.Format(KSelectInitialStdOffsetStmnt, aTzId);
	err = stmt.Prepare(iDatabase, sqlRules);
	User::LeaveIfError(err);
	TInt stdInitialOffsetIdx = stmt.ColumnIndex(KTzInitialStdOffsetTblStdInitialOffsetIndex);
	User::LeaveIfError(stdInitialOffsetIdx);	
	
	// Fetch the row.
	User::LeaveIfError(stmt.Next());

	TInt stdInitialOffset = stmt.ColumnInt(stdInitialOffsetIdx);
	aRules.SetInitialStdTimeOffset(stdInitialOffset);

	// This must be the only row.
	__ASSERT_ALWAYS(stmt.Next() == KSqlAtEnd, RTz::Panic(RTz::EPanicBadSchema));

	CleanupStack::PopAndDestroy(&stmt);	
	}

CTzUserNames* CTzUserDataDb::ReadTzNamesL(TUint aTzId)
	{
	if(iBackupInProgress || iRestoreInProgress)
		{
		User::Leave(KErrLocked);
		}

	RSqlStatement stmt;
	CleanupClosePushL(stmt);	
	
	TBuf<KSqlStringLength> sqlNames;	
	TSqlScalarFullSelectQuery fullSelectQuery(iDatabase);
	sqlNames.Format(KSelectNamesCountStmnt, aTzId);
	TInt count = fullSelectQuery.SelectIntL(sqlNames);
	
	if(count == 0)
		{
		User::Leave(KErrNotFound);
		}
	
	sqlNames.Format(KSelectNamesStmnt, aTzId);
	TInt err = stmt.Prepare(iDatabase, sqlNames);
	User::LeaveIfError(err);
		
	TInt stdNameColIdx = stmt.ColumnIndex(KTzNamesTblStdNameIndex);
	User::LeaveIfError(stdNameColIdx);

	TInt shortStdNameColIdx = stmt.ColumnIndex(KTzNamesTblShortStdNameIndex);
	User::LeaveIfError(shortStdNameColIdx);
		
	TInt dstNameColIdx = stmt.ColumnIndex(KTzNamesTblDSTNameIndex);
	User::LeaveIfError(dstNameColIdx);
		
	TInt shortDstNameColIdx = stmt.ColumnIndex(KTzNamesTblShortDSTNameIndex);
	User::LeaveIfError(shortDstNameColIdx);
		
	TInt cityNameColIdx = stmt.ColumnIndex(KTzNamesTblCityNameIndex);
	User::LeaveIfError(cityNameColIdx);
	
   	TInt regionNameColIdx = stmt.ColumnIndex(KTzNamesTblRegionNameIndex);
	User::LeaveIfError(regionNameColIdx);
	
   	CTzUserNames* userNames = NULL;
   	
	// Fetch the row.
   	User::LeaveIfError(stmt.Next());

	TPtrC standardName;
	standardName.Set(stmt.ColumnTextL(stdNameColIdx));	

	TPtrC shortStandardName;
	shortStandardName.Set(stmt.ColumnTextL(shortStdNameColIdx));

	TPtrC daylightName;
	daylightName.Set(stmt.ColumnTextL(dstNameColIdx));

	TPtrC shortDaylightName; 
	shortDaylightName.Set(stmt.ColumnTextL(shortDstNameColIdx));

	TPtrC cityName;
	cityName.Set(stmt.ColumnTextL(cityNameColIdx));

	TPtrC regionName;
	regionName.Set(stmt.ColumnTextL(regionNameColIdx));

	userNames = CTzUserNames::NewL(standardName, shortStandardName,
		daylightName, shortDaylightName, cityName, regionName);

	// This must be the only row.
	__ASSERT_ALWAYS(stmt.Next() == KSqlAtEnd, RTz::Panic(RTz::EPanicBadSchema));

	CleanupStack::PopAndDestroy(&stmt);

	return userNames;
	}
	
void CTzUserDataDb::ReadTzIdsL(RArray<TUint32>& aTzIds)
	{
	if(iBackupInProgress || iRestoreInProgress )
		{
		User::Leave(KErrLocked);
		}
	
	RSqlStatement stmt;
	CleanupClosePushL(stmt);

	TBuf<KSqlStringLength> sqlRule;
	sqlRule.Format(KSelectTzIdFromNamesStmnt);
	TInt err = stmt.Prepare(iDatabase, sqlRule);
	User::LeaveIfError(err);
		
	while((err = stmt.Next()) == KSqlAtRow)
		{
		TInt col = stmt.ColumnIndex(KTzNamesTblTzIdIndex);
		User::LeaveIfError(col);
		TUint id = stmt.ColumnInt64(col);
		aTzIds.AppendL(id);
		}

	CleanupStack::PopAndDestroy(&stmt);
	}
	
void CTzUserDataDb::UpdateTzL(TUint aTzId, const CTzRules& aTzRules, const CTzUserNames& aTzNames)
	{	
	if(iBackupInProgress || iRestoreInProgress)
		{
		User::Leave(KErrLocked);
		} 
		
	RSqlStatement stmt;
	CleanupClosePushL(stmt);
	
	BeginTransactionL();	
					
	TBuf<KSqlStringLength> sqlRules;
	sqlRules.Format(KDeleteRulesStmnt, aTzId);
	TInt ret = stmt.Prepare(iDatabase, sqlRules);
	if(ret == KErrNone)
		{
		ret = stmt.Exec();
		if(ret < KErrNone)
			{
			User::Leave(ret);
			}
		}
	stmt.Close();
	
	const TInt count = aTzRules.Count();
	for(TInt i=0; i<count; i++)
		{
		TTzRule& rule = const_cast<CTzRules&>(aTzRules)[i];

		User::LeaveIfError(stmt.Prepare(iDatabase, KInsertRulesStmnt));

		TInt paramIdx = stmt.ParameterIndex(KTzRulesTblTzIdParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt64(paramIdx, aTzId));

		paramIdx = stmt.ParameterIndex(KTzRulesTblRuleTypeParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iDayRule));

		paramIdx = stmt.ParameterIndex(KTzRulesTblDSTStartTimeParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt64(paramIdx, rule.iFrom.iTime.Int64()));

		paramIdx = stmt.ParameterIndex(KTzRulesTblDSTStartRefParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iFrom.iTimeReference));

		paramIdx = stmt.ParameterIndex(KTzRulesTblDSTEndTimeParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt64(paramIdx, rule.iTo.iTime.Int64()));

		paramIdx = stmt.ParameterIndex(KTzRulesTblDSTEndRefParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iTo.iTimeReference));

		paramIdx = stmt.ParameterIndex(KTzRulesTblOldOffsetParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iOldLocalTimeOffset));

		paramIdx = stmt.ParameterIndex(KTzRulesTblNewOffsetParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iNewLocalTimeOffset));

		paramIdx = stmt.ParameterIndex(KTzRulesTblMonthParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iMonth));

		paramIdx = stmt.ParameterIndex(KTzRulesTblDayParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iDayOfMonth));

		paramIdx = stmt.ParameterIndex(KTzRulesTblWeekParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iDayOfWeek));

		paramIdx = stmt.ParameterIndex(KTzRulesTblTimeRefParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iTimeReference));

		paramIdx = stmt.ParameterIndex(KTzRulesTblMinsParam);
		User::LeaveIfError(paramIdx);
		User::LeaveIfError(stmt.BindInt(paramIdx, rule.iTimeOfChange));

		ret = stmt.Exec();
		stmt.Close();
		if(ret < KErrNone)
			{
			User::Leave(ret);
			}
		}
	
	TBuf<KSqlStringLength> sqlInitialOffset;
	sqlInitialOffset.Format(KDeleteInitialStdOffset, aTzId);
	ret = stmt.Prepare(iDatabase, sqlInitialOffset);
	if(ret == KErrNone)
		{
		ret = stmt.Exec();	
		stmt.Close();
		if(ret < KErrNone)
			{
			User::Leave(ret);
			}
		}

	User::LeaveIfError(stmt.Prepare(iDatabase, KInsertInitialStdOffsetStmnt));

	TInt paramIdx = stmt.ParameterIndex(KTzInitialStdOffsetTblTzIdParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindInt64(paramIdx, aTzId));

	TInt initialOffset = aTzRules.InitialStdTimeOffset();

	paramIdx = stmt.ParameterIndex(KTzInitialStdOffsetTblStdInitialOffsetParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindInt64(paramIdx, initialOffset));

	ret = stmt.Exec();
	stmt.Close();
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}
	
	TBuf<KSqlStringLength> sqlNames; 
	sqlNames.Format(KDeleteNamesStmnt, aTzId);
	ret = stmt.Prepare(iDatabase, sqlNames);
	if(ret == KErrNone)
		{
		ret = stmt.Exec();	
		if(ret < KErrNone)
			{
			User::Leave(ret);
			}
		}
	stmt.Close();
	
	// Insert a row in the names
	User::LeaveIfError(stmt.Prepare(iDatabase, KInsertNamesStmnt));

	paramIdx = stmt.ParameterIndex(KTzNamesTblTzIdParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindInt64(paramIdx, aTzId));

	paramIdx = stmt.ParameterIndex(KTzNamesTblStdNameParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindText(paramIdx, aTzNames.StandardName()));

	paramIdx = stmt.ParameterIndex(KTzNamesTblShortStdNameParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindText(paramIdx, aTzNames.ShortStandardName()));

	paramIdx = stmt.ParameterIndex(KTzNamesTblDSTNameParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindText(paramIdx, aTzNames.DaylightSaveName()));

	paramIdx = stmt.ParameterIndex(KTzNamesTblShortDSTNameParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindText(paramIdx, aTzNames.ShortDaylightSaveName()));

	paramIdx = stmt.ParameterIndex(KTzNamesTblCityNameParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindText(paramIdx, aTzNames.CityName()));

	paramIdx = stmt.ParameterIndex(KTzNamesTblRegionNameParam);
	User::LeaveIfError(paramIdx);
	User::LeaveIfError(stmt.BindText(paramIdx, aTzNames.RegionName()));

	ret = stmt.Exec();
	stmt.Close();
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}
	
	CommitTransactionL();

	NotifyTzRulesChange(aTzId, ETzUserDataUpdated);
	NotifyTzNamesChange(aTzId, ETzUserDataUpdated);
	
	CleanupStack::PopAndDestroy(&stmt);	
	}
    
/**
Delete the time zone rule and name from the database that matches aTzId
*/
void CTzUserDataDb::DeleteTzL(TUint aTzId)
	{
	if(iBackupInProgress || iRestoreInProgress)
		{
		User::Leave(KErrLocked);
		}
		
	RSqlStatement stmt;
	CleanupClosePushL(stmt);
	
	BeginTransactionL();
	
	TInt ret;		
	TBuf<KSqlStringLength> sqlRules;
	sqlRules.Format(KDeleteRulesStmnt, aTzId);
	User::LeaveIfError(stmt.Prepare(iDatabase, sqlRules));
	ret = stmt.Exec();
	stmt.Close();
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}
	
	TBuf<KSqlStringLength> sqlInitialOffset;
	sqlInitialOffset.Format(KDeleteInitialStdOffset, aTzId);
	ret = stmt.Prepare(iDatabase, sqlInitialOffset);
	if(ret == KErrNone)
		{
		ret = stmt.Exec();	
		stmt.Close();
		if(ret < KErrNone)
			{
			User::Leave(ret);
			}
		}
		
	TBuf<KSqlStringLength> sqlNames;
	sqlNames.Format(KDeleteNamesStmnt, aTzId);
	User::LeaveIfError(stmt.Prepare(iDatabase, sqlNames));
	ret = stmt.Exec();
	stmt.Close();
		
	if(ret < KErrNone)
		{
		User::Leave(ret);
		}
	
	CommitTransactionL();

	NotifyTzRulesChange(aTzId, ETzUserDataDeleted);
	NotifyTzNamesChange(aTzId, ETzUserDataDeleted);	
	
	CleanupStack::PopAndDestroy(&stmt);
	}
	
void CTzUserDataDb::CleanupDatabaseRollback(TAny *aDatabase)
	{
	ASSERT(aDatabase);
	CTzUserDataDb* db = static_cast<CTzUserDataDb*>(aDatabase);
	db->RollbackTransaction();
	}
	
/**
Begin a new transation
*/
void CTzUserDataDb::BeginTransactionL()
	{
	User::LeaveIfError(iDatabase.Exec(KBeginTransaction));
	CleanupStack::PushL(TCleanupItem(CleanupDatabaseRollback, this));
	}
	
/**
Commit the changes made since the start of a transaction
*/
void CTzUserDataDb::CommitTransactionL()
	{
	User::LeaveIfError(iDatabase.Exec(KCommitTransaction));
	CleanupStack::Pop(); //CleanupDatabaseRollback
	}
	
/**
Rollback changes since the start of the transaction
*/
void CTzUserDataDb::RollbackTransaction()
	{
	iDatabase.Exec(KRollbackTransaction);
	}
	
// Notification operations.
void CTzUserDataDb::NotifyTzRulesChange(TUint aTzId, TTzUserDataChangeOp aChangeOp)
	{
	TTzUserDataChange change;
	change.iTzId = aTzId;
	change.iOperation = aChangeOp;
		
	TInt j = 0;
	TInt jEnd = iChangeObservers.Count();
	while (j < jEnd)
		{
		iChangeObservers[j]->NotifyUserTzRulesChange(change);
		++j;
		}
	
	//Publish ETzRulesChange property to the subscribed clients
	NTzUpdate::TTzRulesChange rulesChange;
	rulesChange.iUTCTimeOfRulesChange.UniversalTime();
	TPckgBuf<NTzUpdate::TTzRulesChange> bufRules(rulesChange);
	RProperty::Set(NTzUpdate::KPropertyCategory, NTzUpdate::ETzRulesChange, bufRules);
	}
    
void CTzUserDataDb::NotifyTzNamesChange(TUint aTzId, TTzUserDataChangeOp aChangeOp)
	{
	TTzUserDataChange change;
	change.iTzId = aTzId;
	change.iOperation = aChangeOp;
	
	TInt j = 0;
	TInt jEnd = iChangeObservers.Count();
	while (j < jEnd)
		{
		iChangeObservers[j]->NotifyUserTzNamesChange(change);
		++j;
		}
	
	//Publish ETzNamesChange property to the subscribed clients
	NTzUpdate::TTzNamesChange namesChange;
	namesChange.iUTCTimeOfNamesChange.UniversalTime();
	TPckgBuf<NTzUpdate::TTzNamesChange> bufNames(namesChange);
	RProperty::Set(NTzUpdate::KPropertyCategory, NTzUpdate::ETzNamesChange, bufNames);	
	}

 // Backup and restore operations
void CTzUserDataDb::BackupBeginningL()
	{
	Close();
	iBackupInProgress = ETrue;
	}

void CTzUserDataDb::BackupCompletedL()
	{
	OpenL();
	iBackupInProgress = EFalse;
	}

void CTzUserDataDb::RestoreBeginningL()
	{
	Close();
	iRestoreInProgress = ETrue;
	}

void CTzUserDataDb::RestoreCompletedL()
	{
	OpenL();		
	iRestoreInProgress = EFalse;
	}

void CTzUserDataDb::AddObserverL(MTzUserDataChangeObserver* aChangeObs)
	{
	User::LeaveIfError(iChangeObservers.Append(aChangeObs));
	}

void CTzUserDataDb::RemoveObserver(MTzUserDataChangeObserver* aChangeObs)
	{
	TInt j = 0;
	TInt jEnd = iChangeObservers.Count();
	while (j < jEnd)
		{
		if (iChangeObservers[j] == aChangeObs)
			{
			iChangeObservers.Remove(j);
			break;
			}
		++j;
		}
	}
/**
Checks if aTzId exists in the user timezone database
*/
TBool CTzUserDataDb::IsIdInDbL(TUint aTzId)
	{
	TBuf<KSqlStringLength> sqlNames;
	TSqlScalarFullSelectQuery fullSelectQuery(iDatabase);
	sqlNames.Format(KSelectTzIdCountFromNamesStmnt, aTzId);
	
	TInt count = fullSelectQuery.SelectIntL(sqlNames);
	if(count == 0)
		{
		return EFalse;
		}		
	return ETrue;
	}
/**
Find the next unused id in the database. If all ids have been used
KOutOfBoundsUserId will be returned
*/
TUint32 CTzUserDataDb::FindUnusedTzIdL()
	{
	RArray<TUint32> ids;
	CleanupClosePushL(ids);
	ReadTzIdsL(ids);
	const TInt count = ids.Count();
	TUint foundId = KOutOfBoundsUserId;
	if(count > 0)
		{
		ids.SortSigned();
	
		for(TInt i=0; i<count-1; i++)
			{
			if(ids[i]+1 != ids[i+1])
				{
				foundId = ids[i]+1;
				break;
				}
			}
		}
	CleanupStack::PopAndDestroy(&ids);
	return foundId;
	}
