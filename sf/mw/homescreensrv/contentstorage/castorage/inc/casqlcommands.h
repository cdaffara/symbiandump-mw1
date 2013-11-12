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
 * Description:
 *
 */

#ifndef C_CASQL_H
#define C_CASQL_H

//insert Data

_LIT( KSQLInsertToEntry, "INSERT INTO CA_ENTRY \
(EN_TEXT, EN_DESCRIPTION, EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, \
EN_CREATION_TIME, EN_UID, EN_FLAGS ) \
VALUES ( :EN_TEXT, :EN_DESCRIPTION, :EN_ROLE, :EN_TYPE_NAME, :EN_ICON_ID, \
:EN_CREATION_TIME, :EN_UID, :EN_FLAGS )" );

_LIT( KSQLInsertToEntryWithoutIcon, "INSERT INTO CA_ENTRY \
(EN_TEXT, EN_DESCRIPTION, EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, \
EN_CREATION_TIME, EN_UID, EN_FLAGS ) \
VALUES ( :EN_TEXT, :EN_DESCRIPTION, :EN_ROLE, :EN_TYPE_NAME, \
NULL, :EN_CREATION_TIME, :EN_UID, :EN_FLAGS )" );

_LIT( KSQLInsertToEntryUniqueUid, "INSERT INTO CA_ENTRY \
(EN_TEXT, EN_DESCRIPTION, EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, \
EN_CREATION_TIME, EN_FLAGS ) \
VALUES ( :EN_TEXT, :EN_DESCRIPTION, :EN_ROLE, :EN_TYPE_NAME, \
:EN_ICON_ID, :EN_CREATION_TIME, :EN_FLAGS )" );

_LIT( KSQLInsertToEntryUniqueUidWithoutIcon, "INSERT INTO CA_ENTRY \
(EN_TEXT, EN_DESCRIPTION, EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, \
EN_CREATION_TIME, EN_FLAGS ) \
VALUES ( :EN_TEXT, :EN_DESCRIPTION, :EN_ROLE, :EN_TYPE_NAME, \
NULL, :EN_CREATION_TIME, :EN_FLAGS )" );

_LIT( KSQLInsertToGroup, "INSERT INTO CA_GROUP_ENTRY \
(GE_GROUP_ID, GE_ENTRY_ID, GE_POSITION) \
VALUES ( :GE_GROUP_ID, :GE_ENTRY_ID, :GE_POSITION )" );

_LIT( KSQLInsertToIcon, "INSERT INTO CA_ICON \
(IC_FILENAME,IC_SKIN_ID,IC_APP_ID) \
VALUES ( :IC_FILENAME, :IC_SKIN_ID, :IC_APP_ID )" );

_LIT( KSQLInsertToLaunch, "INSERT INTO CA_LAUNCH \
(LA_ENTRY_ID, LA_LAUNCH_TIME) \
VALUES ( :LA_ENTRY_ID, :LA_LAUNCH_TIME )" );

_LIT( KSQLInsertToAttribute, "REPLACE INTO CA_ATTRIBUTE \
(AT_ENTRY_ID, AT_NAME,AT_VALUE) \
VALUES ( :AT_ENTRY_ID, :AT_NAME, :AT_VALUE )" );

_LIT( KSQLAddLocalization, "INSERT INTO CA_LOCALIZATION_TEXT \
(LT_TABLE_NAME, LT_ATTRIBUTE_NAME, LT_STRING_ID, LT_QMFILENAME, LT_ROW_ID) \
VALUES ( :LT_TABLE, :LT_ATTRIBUTE, :LT_STRING, :LT_QMFILENAME, :LT_ROW_ID )" );

//update Data
_LIT( KSQLUpdateEntry, "UPDATE CA_ENTRY SET " );
_LIT( KSQLUpdateGroup, "UPDATE CA_GROUP_ENTRY SET " );
_LIT( KSQLUpdateIcon, "UPDATE CA_ICON SET " );
_LIT( KSQLUpdateEntryUsedFlag, "UPDATE CA_ENTRY SET \
EN_FLAGS = EN_FLAGS | :EN_FLAGS WHERE ENTRY_ID = :LA_ENTRY_ID");
_LIT( KSQLLocalizeTextEntry, "UPDATE CA_ENTRY SET \
EN_TEXT = :LT_STRING WHERE ENTRY_ID = :LT_ROW_ID"); // WHERE ENTRY_ID must be changed if an attribute
_LIT( KSQLLocalizeDescriptionEntry, "UPDATE CA_ENTRY SET \
EN_DESCRIPTION = :LT_STRING WHERE ENTRY_ID = :LT_ROW_ID"); // WHERE ENTRY_ID must be changed if an attribute
_LIT( KSQLLocalizeShortNameAttribute, "UPDATE CA_ATTRIBUTE SET \
AT_VALUE = :LT_STRING WHERE AT_ENTRY_ID = :LT_ROW_ID AND AT_NAME = \"short_name\""); 
_LIT( KSQLLocalizeTitleNameAttribute, "UPDATE CA_ATTRIBUTE SET \
AT_VALUE = :LT_STRING WHERE AT_ENTRY_ID = :LT_ROW_ID AND AT_NAME = \"title_name\""); 

_LIT( KSQLUpdatePositionInGroup, "UPDATE CA_GROUP_ENTRY SET \
GE_POSITION = :GE_POSITION WHERE \
GE_ENTRY_ID = :GE_ENTRY_ID AND GE_GROUP_ID = :GE_GROUP_ID" );

/*
_LIT( KSQLLocalizeEntry, "UPDATE CA_ENTRY SET \
:LT_ATTRIBUTE = :LT_STRING WHERE ENTRY_ID = :LT_ROW_ID"); // WHERE ENTRY_ID must be changed if an attribute
*/
//update Entry
_LIT( KSQLUpdateEntryText, "EN_TEXT = :EN_TEXT " );
_LIT( KSQLUpdateEntryDescription, "EN_DESCRIPTION = :EN_DESCRIPTION " );
_LIT( KSQLUpdateEntryRole, "EN_ROLE = :EN_ROLE " );
_LIT( KSQLUpdateEntryTypeName, "EN_TYPE_NAME = :EN_TYPE_NAME " );
_LIT( KSQLUpdateEntryIdIcon, "EN_ICON_ID = :EN_ICON_ID " );
_LIT( KSQLUpdateEntryIdIconNull, "EN_ICON_ID = NULL " );
_LIT( KSQLUpdateEntryCreationTime, "EN_CREATION_TIME = :EN_CREATION_TIME " );
_LIT( KSQLUpdateEntryUid, "EN_UID = :EN_UID " );
_LIT( KSQLUpdateEntryUidNULL, "EN_UID = NULL " );
_LIT( KSQLUpdateEntryFlags, "EN_FLAGS = :EN_FLAGS " );
_LIT( KSQLUpdateWhere, "WHERE ENTRY_ID = :ENTRY_ID " );

// Null Icon params 
_LIT( KSQLEmptyIconFileName, "(IC_FILENAME IS NULL OR IC_FILENAME = '')" );
_LIT( KSQLEmptyIconSkinId, "(IC_SKIN_ID IS NULL OR IC_SKIN_ID = '')" );
_LIT( KSQLEmptyIconAppId, "(IC_APP_ID IS NULL OR IC_APP_ID = '')" );

// Update Icon
_LIT( KSQLUpdateIconId, "ICON_ID = :ICON_ID " );
_LIT( KSQLUpdateIconFileName, "IC_FILENAME = :IC_FILENAME " );
_LIT( KSQLUpdateIconSkinId, "IC_SKIN_ID = :IC_SKIN_ID " );
_LIT( KSQLUpdateIconAppId, "IC_APP_ID = :IC_APP_ID " );
_LIT( KSQLUpdateIconWhere, "WHERE ICON_ID = :ICON_ID" );

// Update property
_LIT( KSQLUpdateProperty, "UPDATE CA_DB_PROPERTIES \
SET DB_VALUE = :DB_VALUE WHERE DB_PROPERTY = :DB_PROPERTY ");

// Update localization table entry

_LIT( KSQLUpdateLocalization, "UPDATE CA_LOCALIZATION_TEXT SET \
 LT_STRING_ID = :LT_STRING , LT_QMFILENAME = :LT_QMFILENAME \
 WHERE LT_TABLE_NAME = :LT_TABLE AND LT_ATTRIBUTE_NAME = :LT_ATTRIBUTE \
 AND LT_ROW_ID = :LT_ROW_ID" );

//Delete Data
_LIT( KSQLDeleteLaunch, "DELETE FROM CA_LAUNCH \
WHERE LA_ENTRY_ID = :ENTRY_ID" );
_LIT( KSQLDeleteOldFromLaunch, "DELETE FROM CA_LAUNCH \
WHERE LA_LAUNCH_TIME < :LA_LAUNCH_TIME" );
_LIT( KSQLDeleteEntryFromGroup, "DELETE FROM CA_GROUP_ENTRY \
WHERE GE_ENTRY_ID = :ENTRY_ID" );
_LIT( KSQLDeleteGroupFromGroup, "DELETE FROM CA_GROUP_ENTRY \
WHERE GE_GROUP_ID = :ENTRY_ID" );
_LIT( KSQLDeleteAttribute, "DELETE FROM CA_ATTRIBUTE \
WHERE AT_ENTRY_ID = :ENTRY_ID" );
_LIT( KSQLDeleteEntry, "DELETE FROM CA_ENTRY \
WHERE ENTRY_ID = :ENTRY_ID" );
_LIT( KSQLDeleteIcon, "DELETE FROM CA_ICON WHERE ICON_ID = :ICON_ID" );
_LIT( KSQLDeleteIconWhereIconId, "DELETE FROM CA_ICON WHERE ICON_ID = " );

_LIT( KSQLDeleteLocalizationTextWhereEntryId, "DELETE FROM CA_LOCALIZATION_TEXT WHERE LT_ROW_ID = :LT_ROW_ID" );

//Get Data

_LIT( KSQLGetLocalizations, "SELECT LOC_TEXT_ID, LT_TABLE_NAME, \
LT_ATTRIBUTE_NAME, LT_STRING_ID, LT_QMFILENAME, LT_ROW_ID \
FROM CA_LOCALIZATION_TEXT ORDER BY LT_QMFILENAME" );

_LIT( KSQLGetLocalization, "SELECT LOC_TEXT_ID, LT_TABLE_NAME, \
LT_ATTRIBUTE_NAME, LT_STRING_ID, LT_QMFILENAME, LT_ROW_ID \
FROM CA_LOCALIZATION_TEXT WHERE LT_TABLE_NAME = :LT_TABLE \
AND LT_ATTRIBUTE_NAME = :LT_ATTRIBUTE AND LT_ROW_ID = :LT_ROW_ID" );

//Get all entries from group by Parent_Id
// %S string of where for flags;
_LIT( KSQLGetListByEntryIds, "SELECT ENTRY_ID, EN_TEXT, EN_DESCRIPTION, \
EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, EN_CREATION_TIME, EN_UID, EN_FLAGS, \
IC_FILENAME,IC_SKIN_ID,IC_APP_ID \
FROM CA_ENTRY LEFT JOIN CA_ICON ON EN_ICON_ID = ICON_ID \
WHERE ENTRY_ID IN ( %S )" );

_LIT( KSQLGetList1, "SELECT ENTRY_ID, EN_TEXT, EN_DESCRIPTION, EN_ROLE, \
EN_TYPE_NAME, EN_ICON_ID, EN_CREATION_TIME, EN_UID, EN_FLAGS, IC_FILENAME, \
IC_SKIN_ID, IC_APP_ID FROM CA_ENTRY " );

_LIT( KSQLGetList2, "LEFT JOIN CA_ICON ON EN_ICON_ID = ICON_ID " );


_LIT( KSQLGetListByParentId1, "SELECT ENTRY_ID, EN_TEXT, EN_DESCRIPTION, \
EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, EN_CREATION_TIME, EN_UID, EN_FLAGS, \
IC_FILENAME, IC_SKIN_ID, IC_APP_ID, \
GE_GROUP_ID, GE_ENTRY_ID, GE_POSITION FROM CA_ENTRY " );

_LIT( KSQLGetListByParentId2, "LEFT JOIN CA_ICON ON EN_ICON_ID = ICON_ID LEFT JOIN CA_GROUP_ENTRY ON \
GE_ENTRY_ID = ENTRY_ID WHERE GE_GROUP_ID = :GE_GROUP_ID %S " );

_LIT( KSQLGetListByChildId, "LEFT JOIN CA_ICON ON EN_ICON_ID = ICON_ID LEFT JOIN CA_GROUP_ENTRY ON \
GE_GROUP_ID = ENTRY_ID WHERE GE_ENTRY_ID = :GE_ENTRY_ID %S " );

//Get all attributes by entry id
//:Entry_Id_List is a list of Integers seperated by commas.
_LIT( KSQLGetAttributesByEntryId, "SELECT \
ATTRIBUTE_ID, AT_ENTRY_ID, AT_NAME, AT_VALUE FROM CA_ATTRIBUTE \
WHERE AT_ENTRY_ID IN ( %S )" );

//Select most used
// %S - the input table to work on.
_LIT( KSQLGetListByLaunchDataPart1,"SELECT ENTRY_ID, EN_TEXT, \
EN_DESCRIPTION, EN_ROLE, EN_TYPE_NAME, EN_ICON_ID, EN_CREATION_TIME, \
EN_UID, EN_FLAGS, IC_FILENAME, IC_SKIN_ID, IC_APP_ID FROM (");

// %S - the table to match e.g KSQLGetMostUsed or KSQLGetLastUsed
// The column USAGE_DATA is used to order the items
_LIT( KSQLGetListByLaunchDataPart2,") LEFT JOIN(");
_LIT( KSQLGetListByLaunchDataPart3,") ON ENTRY_ID = LA_ENTRY_ID \
ORDER BY USAGE_DATA");

_LIT( KSQLGetParentIds,"SELECT DISTINCT GE_GROUP_ID FROM CA_GROUP_ENTRY \
WHERE GE_ENTRY_ID IN ( %S )" );

_LIT( KSQLNotINIds," AND GE_GROUP_ID NOT IN( %S )" );

_LIT( KSQLGetIconIdWhere,"SELECT ICON_ID AS Data FROM CA_ICON WHERE ");

_LIT( KSQLGetEntryIdFromEntryTable,"SELECT ENTRY_ID AS Data FROM CA_ENTRY \
WHERE ENTRY_ID = :ENTRY_ID");

_LIT( KSQLGETIconIdFromEntryTable, "SELECT EN_ICON_ID AS Data FROM CA_ENTRY \
WHERE ENTRY_ID = :ENTRY_ID");

_LIT( KSQLGetIconDataByIdIcon,"SELECT ICON_ID, IC_FILENAME, IC_SKIN_ID, IC_APP_ID \
FROM CA_ICON WHERE ICON_ID = :EN_ICON_ID");

_LIT( KSQLGetIconsItems,"SELECT ENTRY_ID, EN_TEXT, EN_DESCRIPTION, EN_ROLE, \
EN_TYPE_NAME, EN_ICON_ID, EN_CREATION_TIME, EN_UID, EN_FLAGS, IC_FILENAME, \
IC_SKIN_ID, IC_APP_ID FROM \
CA_ENTRY LEFT JOIN CA_ICON ON EN_ICON_ID = ICON_ID \
WHERE EN_ICON_ID = :EN_ICON_ID");

_LIT( KSQLGetMostUsed,"SELECT LA_ENTRY_ID, COUNT(*) AS USAGE_DATA FROM \
CA_LAUNCH GROUP BY LA_ENTRY_ID" );

_LIT( KSQLGetLastUsed,"SELECT LA_ENTRY_ID, MAX(LA_LAUNCH_TIME) \
AS USAGE_DATA FROM CA_LAUNCH GROUP BY LA_ENTRY_ID" );

_LIT( KSQLGetProperty, "SELECT DB_VALUE FROM CA_DB_PROPERTIES WHERE DB_PROPERTY = :DB_PROPERTY ");


//Organize statements

_LIT( KSQLOrganizeGetEntryIdCountFromGroup,"SELECT COUNT(GE_ENTRY_ID) \
AS Data FROM CA_GROUP_ENTRY \
WHERE GE_GROUP_ID = :GE_GROUP_ID AND GE_ENTRY_ID IN ( %S ) " );

//Statement to append the entry to the first available position
_LIT( KSQLOrganizeInsertToGroupBasic,"INSERT INTO CA_GROUP_ENTRY \
(GE_GROUP_ID,GE_ENTRY_ID,GE_POSITION) \
VALUES ( :GE_GROUP_ID, :GE_ENTRY_ID, \
( SELECT GE_POSITION FROM CA_GROUP_ENTRY \
WHERE GE_ENTRY_ID = :Entry_Id_Before ) ) ");

_LIT( KSQLOrganizeInsertToGroupPrepend,"INSERT INTO CA_GROUP_ENTRY \
(GE_GROUP_ID,GE_ENTRY_ID,GE_POSITION) \
VALUES ( :GE_GROUP_ID, :GE_ENTRY_ID, 1 ) "); 

//Statement to append the entry at end of table
_LIT( KSQLOrganizeAppendToGroup,"INSERT INTO CA_GROUP_ENTRY \
(GE_GROUP_ID, GE_ENTRY_ID, GE_POSITION) \
VALUES ( :GE_GROUP_ID, :GE_ENTRY_ID, \
(SELECT MAX(DATA) FROM ( SELECT MAX(GE_POSITION)+ 1 AS DATA FROM \
CA_GROUP_ENTRY WHERE \
GE_GROUP_ID = :GE_GROUP_ID UNION SELECT 1 AS DATA FROM \
CA_GROUP_ENTRY ) ) )");

_LIT( KSQLOrganizeRemoveEntriesFromGroup,"DELETE FROM CA_GROUP_ENTRY \
WHERE GE_ENTRY_ID IN (%S) AND GE_GROUP_ID = :GE_GROUP_ID");

//Get Data
_LIT( KSQLGetLastInsertId, "SELECT last_insert_rowid() " );

_LIT( KSQLGetEntryFlags, " EN_FLAGS = :EN_FLAGS " );
_LIT( KSQLGetEntryTypeName, " EN_TYPE_NAME = :EN_TYPE_NAME " );
_LIT( KSQLGetAttributeName, " AT_NAME = :AT_NAME " );
_LIT( KSQLGetAttributeValue, " AT_VALUE = :AT_VALUE " );
_LIT( KSQLGetEntryRole, " :EN_ROLE | EN_ROLE == :EN_ROLE  " );
_LIT( KSQLGetEntryUid, " EN_UID = :EN_UID " );
_LIT( KSQLGetEntryFlagsOff, " :Flags_off & (~EN_FLAGS) == :Flags_off " );
_LIT( KSQLGetEntryFlagsOn, " :Flags_on & EN_FLAGS == :Flags_on " );
_LIT( KSQLGetEntryTypeNames, " EN_TYPE_NAME IN (%S) " );
_LIT( KSQLGetEntrySortOrder, " ORDER BY :Sort_Type " );
_LIT( KWhere, " WHERE " );
_LIT( KDesc, " DESC " );
_LIT( KAnd, " AND " );
_LIT( KLimit, " LIMIT ");
_LIT( KSingleQuotes, "\'" );
_LIT( KDot, "." );
_LIT( KEqual, " = ");
_LIT( KEmpty, "" );
_LIT( KGroupBy, " GROUP BY " );
_LIT( KAt, " at" );
_LIT( KLeftJoinCaAttrubute1, " LEFT JOIN CA_ATTRIBUTE as at" );
_LIT( KLeftJoinCaAttrubute2, " ON ENTRY_ID = at" );
_LIT( KLeftJoinCaAttrubute3, ".AT_ENTRY_ID " );

_LIT( KSQLSortOrderNameAsc, " ORDER BY EN_TEXT ASC" );
_LIT( KSQLSortOrderNameDesc, " ORDER BY EN_TEXT DESC" );
_LIT( KSQLSortOrderCreatedTimestampAsc, " ORDER BY EN_CREATION_TIME ASC" );
_LIT( KSQLSortOrderCreatedTimestampDesc, " ORDER BY EN_CREATION_TIME DESC" );
_LIT( KSQLSortOrderIdAsc, " ORDER BY ENTRY_ID ASC" );
_LIT( KSQLSortOrderIdDesc, " ORDER BY ENTRY_ID DESC" );
_LIT( KSQLSortOrderDefault, " ORDER BY GE_GROUP_ID, GE_POSITION" );
_LIT( KSQLSortOrderDefaultDesc, " ORDER BY GE_GROUP_ID, GE_POSITION DESC" );

// additional data
_LIT( KSQLEntryId, ":ENTRY_ID" );
_LIT( KSQLEnText, ":EN_TEXT" );
_LIT( KSQLEnDescription, ":EN_DESCRIPTION" );
_LIT( KSQLEnRole, ":EN_ROLE" );
_LIT( KSQLEnTypeName, ":EN_TYPE_NAME" );
_LIT( KSQLEnIdIcon, ":EN_ICON_ID" );
_LIT( KSQLEnCreationTime, ":EN_CREATION_TIME" );
_LIT( KSQLEnUid, ":EN_UID" );
_LIT( KSQLEnFlags, ":EN_FLAGS" );

_LIT( KSQLGroupEntryId, ":GROUP_ENTRY_ID" );
_LIT( KSQLGEIdGroup, ":GE_GROUP_ID" );
_LIT( KSQLGEEntryId, ":GE_ENTRY_ID" );
_LIT( KSQLGEPosition, ":GE_POSITION" );

_LIT( KSQLIconId, ":ICON_ID" );
_LIT( KSQLIcFileName, ":IC_FILENAME" );
_LIT( KSQLIcSkinId, ":IC_SKIN_ID" );
_LIT( KSQLIcAppId, ":IC_APP_ID" );

_LIT( KSQLLaunchId, ":LAUNCH_ID" );
_LIT( KSQLLaLaunchTime, ":LA_LAUNCH_TIME" );
_LIT( KSQLLaLaunchEntryId, ":LA_ENTRY_ID" );

_LIT( KSQLAttributeId, ":ATTRIBUTE_ID" );
_LIT( KSQLAttrEntryID, ":AT_ENTRY_ID" );
_LIT( KSQLAttrName, ":AT_NAME" );
_LIT( KSQLAttrValue, ":AT_VALUE" );

_LIT( KSQLDbProperty, ":DB_PROPERTY");
_LIT( KSQLDbValue, ":DB_VALUE");

_LIT( KSQLFlagsOn, ":Flags_on" );
_LIT( KSQLFlagsOff, ":Flags_off" );
_LIT( KSQLSortType, ":Sort_Type" );
_LIT( KSQLEntryIdBefore, ":Entry_Id_Before" );
_LIT( KSQLShiftThreshold, ":Shift_Threshold" );

_LIT( KSQLLocTable, ":LT_TABLE" );
_LIT( KSQLLocAttribute, ":LT_ATTRIBUTE" );
_LIT( KSQLLocString, ":LT_STRING" );
_LIT( KSQLLocQmFilename, ":LT_QMFILENAME" );
_LIT( KSQLLocRowId, ":LT_ROW_ID" );

// columns
_LIT( KColumnSingleData, "Data" );

_LIT( KColumnEntryId, "ENTRY_ID" );
_LIT( KColumnEnText, "EN_TEXT" );
_LIT( KColumnEnDescription, "EN_DESCRIPTION" );
_LIT( KColumnEnRole, "EN_ROLE" );
_LIT( KColumnEnTypeName, "EN_TYPE_NAME" );
_LIT( KColumnEnIdIcon, "EN_ICON_ID" );
_LIT( KColumnEnCreationTime, "EN_CREATION_TIME" );
_LIT( KColumnEnUid, "EN_UID" );
_LIT( KColumnEnFlags, "EN_FLAGS" );

_LIT( KShortName, "short_name");
_LIT( KTitleName, "title_name");

_LIT( KColumnGroupEntryId, "GROUP_ENTRY_ID" );
_LIT( KColumnGEIdGroup, "GE_GROUP_ID" );
_LIT( KColumnGEPosition, "GE_POSITION" );

_LIT( KColumnIconId, "ICON_ID" );
_LIT( KColumnIcFileName, "IC_FILENAME" );
_LIT( KColumnIcSkinId, "IC_SKIN_ID" );
_LIT( KColumnIcAppId, "IC_APP_ID" );

_LIT( KColumnLaunchId, "LAUNCH_ID" );
_LIT( KColumnLaLaunchTime, "LA_LAUNCH_TIME" );

_LIT( KColumnAttributeId, "ATTRIBUTE_ID" );
_LIT( KColumnAttrEntryID, "AT_ENTRY_ID" );
_LIT( KColumnAttrName, "AT_NAME" );
_LIT( KColumnAttrValue, "AT_VALUE" );

_LIT( KColumnLocTextId, "LOC_TEXT_ID" );
_LIT( KColumnLocTableName, "LT_TABLE_NAME" );
_LIT( KColumnLocAttrName,    "LT_ATTRIBUTE_NAME" );
_LIT( KColumnLocStringId,   "LT_STRING_ID" );
_LIT( KColumnQmFile,     "LT_QMFILENAME" );

_LIT( KColumnLocRowId,   "LT_ROW_ID" );

_LIT( KColumnDbProperty, "DB_PROPERTY" );
_LIT( KColumnDbValue, "DB_VALUE" );

//transaction data
_LIT( KSqlStatementBegin, "BEGIN");
_LIT( KSqlStatementCommit, "COMMIT");
_LIT( KSqlStatementRollback, "ROLLBACK");

#endif //C_CPSSQL_H
