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
#include "casqlitestorage.h"
#include "caconsts.h"
#include "casqlcommands.h"
#include "casqlquery.h"
#include "casqlquerycreator.h"
#include "cainnerentry.h"
#include "cainnerquery.h"
#include "caarraycleanup.inl"
#include "calocalizationentry.h"
#include "cainternaltypes.h"
#include "cadef.h"

// ---------------------------------------------------------------------------
// CCASqLiteStorage::CCpStorageEngine()
//
// ---------------------------------------------------------------------------
//
CCaSqLiteStorage::CCaSqLiteStorage()
    {
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::ConstructL()
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ConstructL()
    {
    User::LeaveIfError( iRfs.Connect() );

    User::LeaveIfError( CreatePrivateDirPath( iPrivatePathCDriveDb, KCDrive,
            KDbName ) );

    User::LeaveIfError( CreatePrivateDirPath( iPrivatePathZDriveDb, KZDrive,
            KDbName ) );

    User::LeaveIfError( CreatePrivateDirPath( iPrivatePathCDrive, KCDrive,
            KNullDesC ) );
    
    User::LeaveIfError( CreatePrivateDirPath( iPrivatePathCDriveDbBackup, KCDrive,
    		KDbNameBackup ) );
    

    if( iSqlDb.Open( iPrivatePathCDriveDb, &KSqlDbConfig ) )
        {
        //we could not load data base from C-drive lets try Rom
        LoadDataBaseFromRomL();
        }
    else
        {
        TBuf<KCaMaxAttrNameLen> versionValue;
        DbPropertyL(KCaDbPropVersion, versionValue);
        ASSERT(versionValue.Length()>0);
        if( versionValue.CompareC( KCaDbVersion ) )
            {
            // database loaded from C: is obsolete, load from Z:
            iSqlDb.Close();
            LoadDataBaseFromRomL();
            }
        }
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::NewL()
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::LoadDataBaseFromRomL()
    {
    if( ( BaflUtils::FileExists( iRfs, iPrivatePathCDriveDb ) ) )
        {
        iSqlDb.Close();
        User::LeaveIfError( BaflUtils::DeleteFile( iRfs,
                iPrivatePathCDriveDb ) );
        }

    if( !( BaflUtils::FileExists( iRfs, iPrivatePathZDriveDb ) ) )
        {
        User::Panic( _L("fatal error - castorage.db not exists in ROM"),
                KErrNotFound );
        }
    else
        {
        if( !( BaflUtils::FolderExists( iRfs, iPrivatePathCDrive ) ) )
            {
            User::LeaveIfError( iRfs.CreatePrivatePath( EDriveC ) );
            }
        User::LeaveIfError( BaflUtils::CopyFile( iRfs,
                iPrivatePathZDriveDb, iPrivatePathCDrive ) );
        User::LeaveIfError( iRfs.SetAtt( iPrivatePathCDriveDb,
                KEntryAttNormal, KEntryAttReadOnly ) );
        User::LeaveIfError( iSqlDb.Open( iPrivatePathCDriveDb,
                &KSqlDbConfig ) );
        }
    }

// ---------------------------------------------------------------------------
// 
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::SaveDatabaseL()
    {
    if( ( BaflUtils::FileExists( iRfs, iPrivatePathCDriveDb ) ) )
        {
        iSqlDb.Close();
        User::LeaveIfError( BaflUtils::CopyFile( iRfs,
                iPrivatePathCDriveDb, iPrivatePathCDriveDbBackup ) );
        User::LeaveIfError( iSqlDb.Open( iPrivatePathCDriveDb,
                &KSqlDbConfig ) );
        }
    }

// ---------------------------------------------------------------------------
// 
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::RestoreDatabaseL()
    {
    if( ( BaflUtils::FileExists( iRfs, iPrivatePathCDriveDbBackup ) ) )
        {
        iSqlDb.Close();
        User::LeaveIfError( BaflUtils::CopyFile( iRfs,
        		iPrivatePathCDriveDbBackup, iPrivatePathCDriveDb ) );
        User::LeaveIfError( iSqlDb.Open( iPrivatePathCDriveDb,
                &KSqlDbConfig ) );
        }
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::NewL()
//
// ---------------------------------------------------------------------------
//
CCaSqLiteStorage* CCaSqLiteStorage::NewL()
    {
    CCaSqLiteStorage* self = CCaSqLiteStorage::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::NewLC()
//
// ---------------------------------------------------------------------------
//
CCaSqLiteStorage* CCaSqLiteStorage::NewLC()
    {
    CCaSqLiteStorage* self = new ( ELeave ) CCaSqLiteStorage();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::~CCpStorageEngine()
//
// ---------------------------------------------------------------------------
//
CCaSqLiteStorage::~CCaSqLiteStorage()
    {
    iSqlDb.Close();
    iRfs.Close();
    }

// ---------------------------------------------------------------------------
// CCaSqLiteStorage::CreatePrivateDirPath()
//
// ---------------------------------------------------------------------------
//
TInt CCaSqLiteStorage::CreatePrivateDirPath( TFileName& aPrivatePath,
        const TDesC& aDrive, const TDesC& aDbName )
    {
    TInt error( KErrNone );

#ifdef CONTENT_ARSENAL_STORAGE_UT
    TFileName KPathWithoutDrive( KDoubleSlash );
#else
    TFileName KPathWithoutDrive;
    error = iRfs.PrivatePath( KPathWithoutDrive );
    if( error != KErrNone )
        {
        return error;
        }
#endif

    aPrivatePath.Copy( aDrive );
    aPrivatePath.Append( KPathWithoutDrive );
    aPrivatePath.Append( aDbName );

    return error;
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::GetListL
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::GetEntriesL( const CCaInnerQuery* aQuery,
        RPointerArray<CCaInnerEntry>& aResultContainer )
    {
    //    Fetch entries from
    CCaSqlQuery* sqlGetEntriesQuery = CCaSqlQuery::NewLC( iSqlDb );
    CaSqlQueryCreator::CreateGetEntriesQueryL( aQuery, sqlGetEntriesQuery );
    sqlGetEntriesQuery->PrepareL();
    sqlGetEntriesQuery->BindValuesForGetEntriesL( aQuery );
    sqlGetEntriesQuery->ExecuteL( aResultContainer, CCaSqlQuery::EEntry );
    CleanupStack::PopAndDestroy( sqlGetEntriesQuery );

    //    Populate the entries with their attributes
    if( aResultContainer.Count() > 0 )
        {
        CCaSqlQuery* sqlGetAttributesQuery = CCaSqlQuery::NewLC( iSqlDb );
        CaSqlQueryCreator::CreateGetAttributesQueryL(
                aResultContainer.Count(), sqlGetAttributesQuery );
        sqlGetAttributesQuery->PrepareL();
        sqlGetAttributesQuery->BindEntryIdsL( aResultContainer );
        sqlGetAttributesQuery->ExecuteL( aResultContainer,
                CCaSqlQuery::EAttribute );
        CleanupStack::PopAndDestroy( sqlGetAttributesQuery );
        }
    
    //  set entries if proper order if they were fetched by ids
    if( aQuery->GetIds().Count() > 0 )
        {
        SetEntriesInProperOrderL( aQuery->GetIds(), aResultContainer );
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::LocalizeEntryL( CCaLocalizationEntry& aLocalization )
    {
    CCaSqlQuery* sqlLocalizeEntryQuery = CCaSqlQuery::NewLC( iSqlDb );
    if ( aLocalization.GetAttributeName().Compare( KColumnEnText ) == 0 )
        {
        CaSqlQueryCreator::CreateLocalizationTableQueryL( sqlLocalizeEntryQuery,
            KSQLLocalizeTextEntry );
        }
    else if ( aLocalization.GetAttributeName().Compare(
            KColumnEnDescription ) == 0 )
        {
        CaSqlQueryCreator::CreateLocalizationTableQueryL( sqlLocalizeEntryQuery,
            KSQLLocalizeDescriptionEntry );
        }
    else if ( aLocalization.GetAttributeName().Compare( KShortName ) == 0 )
        {
        CaSqlQueryCreator::CreateLocalizationTableQueryL( sqlLocalizeEntryQuery,
            KSQLLocalizeShortNameAttribute );
        }
    else if ( aLocalization.GetAttributeName().Compare( KTitleName ) == 0 )
        {
        CaSqlQueryCreator::CreateLocalizationTableQueryL( sqlLocalizeEntryQuery,
            KSQLLocalizeTitleNameAttribute );
        }
    sqlLocalizeEntryQuery->PrepareL();
    sqlLocalizeEntryQuery->BindValuesForLocalizeL( aLocalization );
    sqlLocalizeEntryQuery->ExecuteL( );
    CleanupStack::PopAndDestroy( sqlLocalizeEntryQuery );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::AddLocalizationL(
        const CCaLocalizationEntry& aLocalization)
    {
    if (LocalizationEntryPresentL(aLocalization))
        {
        ExecuteLocalizationStatementL(aLocalization, KSQLUpdateLocalization);
        }
    else
        {
        ExecuteLocalizationStatementL(aLocalization, KSQLAddLocalization);
        }
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TBool CCaSqLiteStorage::LocalizationEntryPresentL(
        const CCaLocalizationEntry& aLocalization)
    {
    TBool result(EFalse);
    CCaSqlQuery* sqlQuery = CCaSqlQuery::NewLC( iSqlDb );
    CaSqlQueryCreator::CreateLocalizationTableQueryL(sqlQuery,
            KSQLGetLocalization);
    sqlQuery->PrepareL();
    sqlQuery->BindValuesForGetLocalizationEntryL( aLocalization );
    result = sqlQuery->ExecuteEntryPresentL( );
    CleanupStack::PopAndDestroy( sqlQuery );
    return result;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteLocalizationStatementL(
        const CCaLocalizationEntry& aLocalization, const TDesC& aStatement)
    {
    CCaSqlQuery* sqlQuery = CCaSqlQuery::NewLC( iSqlDb );
    CaSqlQueryCreator::CreateLocalizationTableQueryL(sqlQuery,aStatement );
    sqlQuery->PrepareL();
    sqlQuery->BindValuesForLocalizationEntryL( aLocalization );
    sqlQuery->ExecuteL( );
    CleanupStack::PopAndDestroy( sqlQuery );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::GetLocalizationsL(
        RPointerArray<CCaLocalizationEntry>& aResultContainer )
    {
    CCaSqlQuery* sqlGetLocalizationsQuery = CCaSqlQuery::NewLC( iSqlDb );
    CaSqlQueryCreator::CreateLocalizationTableQueryL(
            sqlGetLocalizationsQuery, KSQLGetLocalizations );
    sqlGetLocalizationsQuery->PrepareL();
    sqlGetLocalizationsQuery->ExecuteLocalizationsL( aResultContainer );
    CleanupStack::PopAndDestroy( sqlGetLocalizationsQuery );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::GetEntriesIdsL
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::GetEntriesIdsL( const CCaInnerQuery* aQuery,
        RArray<TInt>& aResultIdArray )
    {
    CCaSqlQuery* sqlGetEntriesIdsQuery = CCaSqlQuery::NewLC( iSqlDb );
    CaSqlQueryCreator::CreateGetEntriesQueryL( aQuery,
            sqlGetEntriesIdsQuery );
    sqlGetEntriesIdsQuery->PrepareL();
    sqlGetEntriesIdsQuery->BindValuesForGetEntriesL( aQuery );
    sqlGetEntriesIdsQuery->ExecuteL( aResultIdArray,
            CCaSqlQuery::EEntryTable );
    CleanupStack::PopAndDestroy( sqlGetEntriesIdsQuery );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::GetEntriesIdsL
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::GetParentsIdsL( const RArray<TInt>& aEntryIdArray,
        RArray<TInt>& aParentIdArray, TBool aCheckParentsParent )
    {
    CCaSqlQuery* sqlGetParentIdsQuery = CCaSqlQuery::NewLC( iSqlDb );
    CaSqlQueryCreator::CreateGetParentsIdsQueryL( aEntryIdArray,
            sqlGetParentIdsQuery, aParentIdArray );
    sqlGetParentIdsQuery->PrepareL();
    sqlGetParentIdsQuery->BindIdsL( aEntryIdArray, KSQLGEEntryId );
    if( aParentIdArray.Count() > 0 )
        {
        sqlGetParentIdsQuery->BindIdsL( aParentIdArray, KSQLGEIdGroup );
        }
    TInt parentCount = sqlGetParentIdsQuery->ExecuteL( aParentIdArray,
            CCaSqlQuery::EGroupTable );
    CleanupStack::PopAndDestroy( sqlGetParentIdsQuery );
    if( aCheckParentsParent && parentCount > 0 )
        {
        GetParentsIdsL( aParentIdArray, aParentIdArray );
        }
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::AddL
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::AddL( CCaInnerEntry* aEntry, TBool aUpdate)
    {
    RPointerArray<CCaSqlQuery> sqlQueries;
    CleanupResetAndDestroyPushL( sqlQueries );

    CaSqlQueryCreator::CreateAddQueryL( aEntry, sqlQueries, iSqlDb );

    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteAddL( aEntry, sqlQueries ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );

    CleanupStack::PopAndDestroy( &sqlQueries );

    if( aUpdate )
        {
        RemoveFromLocalizationL( aEntry->GetId() );
        }
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::RemoveL
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::RemoveL( const RArray<TInt>& aEntryIds )
    {
    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteRemoveL( aEntryIds ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::OrganizeL( const RArray<TInt>& aEntryIds,
//        TCaOperationParams aParams )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::OrganizeL( const RArray<TInt>& aEntryIds,
        TCaOperationParams aParams )
    {
    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteOrganizeL( aEntryIds, aParams ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::TouchL( const TInt aEntryId )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::TouchL( const TInt aEntryId, TBool aRemovable )
    {
    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteTouchL( aEntryId, aRemovable ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::DbPropertyL
// ( const TDesC& aProperty, TDes& aPropertyValue )
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::DbPropertyL( const TDesC& aProperty,
        TDes& aPropertyValue )
    {
    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteDbPropertyL( aProperty, aPropertyValue ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::SetDBPropertyL
// ( const TDesC& aProperty, const TDesC& aPropertyValue )
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::SetDBPropertyL( const TDesC& aProperty,
        const TDesC& aPropertyValue )
    {
    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteSetDbPropertyL( aProperty, aPropertyValue ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );
    }

// ---------------------------------------------------------------------------
// CCaSqLiteStorage::RemoveFromLocalizationL( const TInt aEntryId )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::RemoveFromLocalizationL( const TInt aEntryId )
    {
    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteRemoveFromLocalizationL( aEntryId ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );
    }

// ---------------------------------------------------------------------------
// CCaSqLiteStorage::CustomSortL
// ( const RArray<TInt>& aEntryIds, const TInt aGroupId )
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::CustomSortL( const RArray<TInt>& aEntryIds,
        const TInt aGroupId )
    {
    RArray<TInt> entryIds;
    CleanupClosePushL( entryIds );
    for( TInt j=0; j<aEntryIds.Count(); j++ )
        {
        entryIds.AppendL(aEntryIds[j]);
        }
    
    RArray<TInt> oldIds;
    CleanupClosePushL( oldIds );
    CCaInnerQuery* innerQuery = CCaInnerQuery::NewLC();
    innerQuery->SetParentId( aGroupId );
    
    GetEntriesIdsL( innerQuery, oldIds );
    
    if( oldIds.Count() != entryIds.Count() )
        {
        for( TInt i=0; i<oldIds.Count(); i++ )
            {
            TInt oldId = oldIds[i];
            if( entryIds.Find( oldId ) == KErrNotFound )
                {
                // instert apps with visible set on false on old positions
                entryIds.InsertL( oldId, i );
                }
            }
        }
    
    const RArray<TInt> constEntryIds( entryIds );
    
    RPointerArray<CCaSqlQuery> sqlQueries;
    CleanupResetAndDestroyPushL( sqlQueries ); 

    CaSqlQueryCreator::CreateCustomSortQueryL(
            constEntryIds, sqlQueries, iSqlDb );

    ExecuteStatementL( KSqlStatementBegin );
    TRAPD( err, ExecuteCustomSortL( constEntryIds, aGroupId, sqlQueries ) );
    if( err )
        {
        ExecuteStatementL( KSqlStatementRollback );
        User::Leave( err );
        }
    ExecuteStatementL( KSqlStatementCommit );

    CleanupStack::PopAndDestroy( &sqlQueries );
    CleanupStack::PopAndDestroy( innerQuery );
    CleanupStack::PopAndDestroy( &oldIds );
    CleanupStack::PopAndDestroy( &entryIds );
    }

// ---------------------------------------------------------------------------
// CCaSqLiteStorage::ExecuteCustomSortL
// ( const RArray<TInt>& aEntryIds, const TInt aGroupId )
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteCustomSortL( const RArray<TInt>& aEntryIds,
        const TInt aGroupId , RPointerArray<CCaSqlQuery>& aSqlQuery )
    {
    for( TInt i = 0; i < aSqlQuery.Count(); i++ )
        {
        TInt newPosition = i + 1;
        TInt entryId = aEntryIds[i];

        aSqlQuery[i]->PrepareL();
        aSqlQuery[i]->BindValuesForCustomSortL( aGroupId, entryId, newPosition );
        aSqlQuery[i]->ExecuteL();
        aSqlQuery[i]->CloseStatement();
        }
    }

// ---------------------------------------------------------------------------
// CCaSqLiteStorage::ExecuteRemoveFromLocalizationL( const TInt aEntryId )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteRemoveFromLocalizationL( const TInt aEntryId )
    {
    CCaSqlQuery* query = CCaSqlQuery::NewLC( iSqlDb );
    query->SetQueryL( KSQLDeleteLocalizationTextWhereEntryId );
    query->PrepareL();
    query->BindValuesForRemoveFromLocalizationL( aEntryId );
    query->ExecuteL();
    CleanupStack::PopAndDestroy( query );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::ExecuteStatementL( const TDesC& aStatement )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteStatementL( const TDesC& aStatement )
    {
    CCaSqlQuery* query = CCaSqlQuery::NewLC( iSqlDb );
    query->SetQueryL( aStatement );
    query->PrepareL();
    query->ExecuteL();
    CleanupStack::PopAndDestroy( query );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::ExecuteAddL(CCaInnerEntry* aEntry,
//         RPointerArray<CCASqlQuery>& sqlQuery)
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteAddL( CCaInnerEntry* aEntry,
        RPointerArray<CCaSqlQuery>& aSqlQuery )
    {
    TBool isAttributeDeleted( EFalse );
    for( TInt i = 0; i < aSqlQuery.Count(); i++ )
        {
        if( aSqlQuery[i]->Type() == CCaSqlQuery::EIconTable )
            {
            //check if icon exists - just update
            CCaSqlQuery* query = CCaSqlQuery::NewLC( iSqlDb );
            CaSqlQueryCreator::CreateFindIconQueryL(aEntry, query);
            query->SetTableType( CCaSqlQuery::EIconTable );
            query->PrepareL();
            query->BindValuesForGetIconL( aEntry );
            TInt idIcon( 0 );
            query->ExecuteL( idIcon );
            CleanupStack::PopAndDestroy( query );

            aSqlQuery[i]->PrepareL();
            aSqlQuery[i]->BindValuesForAddL( aEntry );
            TInt tmpId( 0 );
            TRAPD( err, tmpId = aSqlQuery[i]->ExecuteL() );
            aSqlQuery[i]->CloseStatement();
            if( !idIcon && !aEntry->GetIconId() )
                { // new icon added
                aEntry->SetIconId( tmpId );
                }
            if( err == KSqlErrConstraint )
                {
                // ignore, this means that the icon cannot be removed
                // because some other entry has the same icon.
                }
            else if( err )
                {
                User::Leave( err );
                }

            }
        else if( aSqlQuery[i]->Type() == CCaSqlQuery::EEntryTable )
            {
            aSqlQuery[i]->PrepareL();
            aSqlQuery[i]->BindValuesForAddL( aEntry );
            TInt tmpId = aSqlQuery[i]->ExecuteL();
            aSqlQuery[i]->CloseStatement();
            if( !aEntry->GetId() )
                {
                // new entry added
                aEntry->SetId( tmpId );
                }
            }
        else if( !isAttributeDeleted
                && aSqlQuery[i]->Type() == CCaSqlQuery::EAttributeTable )
            {
            // delete all entry's attributes, just ones
            aSqlQuery[i]->PrepareL();
            aSqlQuery[i]->BindEntryIdL( aEntry->GetId() );
            aSqlQuery[i]->ExecuteL();
            aSqlQuery[i]->CloseStatement();
            isAttributeDeleted = ETrue;
            }
        else if( aSqlQuery[i]->Type()==CCaSqlQuery::EAttributeTable )
            {
            // add new attribute(s) if it's neccesery
            aSqlQuery[i]->PrepareL();
            for( TInt k(0); k < aEntry->GetAttributes().Count(); k++ )
                {
                // if(id_entry, name) exist it'll replaced by new value
                aSqlQuery[i]->BindValuesForAddAttributeL( aEntry,
                        aEntry->GetAttributes()[k] );
                aSqlQuery[i]->ExecuteL();
                aSqlQuery[i]->ResetStatement();
                }
            aSqlQuery[i]->CloseStatement();
            }
        }
    }
// ---------------------------------------------------------------------------
// CCASqLiteStorage::ExecuteRemoveL( const RArray<TInt>& aEntryIds)
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteRemoveL( const RArray<TInt>& aEntryIds )
    {
    for( int i=0; i<aEntryIds.Count(); i++ )
        {
        // check if entry exist in database
        CCaSqlQuery* queryEntry = CCaSqlQuery::NewLC( iSqlDb );
        queryEntry->SetQueryL( KSQLGetEntryIdFromEntryTable );
        queryEntry->PrepareL();
        queryEntry->BindEntryIdL( aEntryIds[i] );
        TInt entryCount( 0 );
        queryEntry->ExecuteL( entryCount );
        CleanupStack::PopAndDestroy( queryEntry );
        if( !entryCount )
            {
            User::Leave( KErrNotFound );
            }

        // SELECT Id_Icon FROM CA_ENTRY WHERE Id = aEntryId;
        CCaSqlQuery* queryIconId = CCaSqlQuery::NewLC( iSqlDb );
        queryIconId->SetQueryL( KSQLGETIconIdFromEntryTable );
        queryIconId->PrepareL();
        queryIconId->BindEntryIdL( aEntryIds[i] );
        TInt iconId( 0 );
        queryIconId->ExecuteL( iconId );
        CleanupStack::PopAndDestroy( queryIconId );

        RPointerArray<CCaSqlQuery> sqlQuery;
        CleanupResetAndDestroyPushL( sqlQuery );
        CaSqlQueryCreator::CreateRemoveQueryL( sqlQuery, iSqlDb );
        for( TInt j = 0; j < sqlQuery.Count(); j++ )
            {
            if( !( sqlQuery[j]->Type() == CCaSqlQuery::EIconTable &&
                    iconId == 0 ) )
                {
                sqlQuery[j]->PrepareL();
                sqlQuery[j]->BindValuesForRemoveL( aEntryIds[i], iconId );
                TRAPD( err, sqlQuery[j]->ExecuteL() );
                sqlQuery[j]->CloseStatement();

                if( sqlQuery[j]->Type() == CCaSqlQuery::EIconTable &&
                        err == KSqlErrConstraint )
                    {
                    // ignore, this means that the icon cannot be removed
                    // because some other entry has the same icon.
                    }
                else if( err )
                    {
                    User::Leave( err );
                    }
                }
            }
        CleanupStack::PopAndDestroy( &sqlQuery );
        }
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::ExecuteOrganizeL(const RArray<TInt>& aEntryIds,
//      TCaOperationParams aParams)
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteOrganizeL( const RArray<TInt>& aEntryIds,
        TCaOperationParams aParams )
    {
    VerifyOrganizeParamsL( aEntryIds, aParams );

    RPointerArray<CCaSqlQuery> sqlQueries;
    CleanupResetAndDestroyPushL( sqlQueries );

    TBool revereseInsertOrder = CaSqlQueryCreator::CreateOrganizeQueryL(
            aEntryIds, aParams, iSqlDb, sqlQueries );

    for( TInt i = 0; i < sqlQueries.Count(); i++ )
        {
        switch( sqlQueries[i]->Type() )
            {
            case CCaSqlQuery::EGroupTableRemove:
                {
                sqlQueries[i]->PrepareL();
                sqlQueries[i]->BindValuesForOrganizeL( aEntryIds, aParams );
                sqlQueries[i]->ExecuteL();
                sqlQueries[i]->CloseStatement();
                }
                break;

            case CCaSqlQuery::EGroupTableInsert:
                {
                sqlQueries[i]->PrepareL();
                for( TInt j = 0; j < aEntryIds.Count(); j++ )
                    {
                    TInt k = (revereseInsertOrder) ?
                        (aEntryIds.Count() - 1) - j : j;
                    sqlQueries[i]->BindValuesForOrganizeL(
                            aEntryIds, aParams, aEntryIds[k] );
                    sqlQueries[i]->ExecuteL();
                    sqlQueries[i]->ResetStatement();
                    }
                sqlQueries[i]->CloseStatement();
                }
                break;
            }
        }

    CleanupStack::PopAndDestroy( &sqlQueries );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::ExecuteTouchL( const TInt aEntryId )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteTouchL( const TInt aEntryId, TBool aRemovable )
    {
    RPointerArray<CCaSqlQuery> sqlQuery;
    CleanupResetAndDestroyPushL( sqlQuery );

    CaSqlQueryCreator::CreateTouchQueryL( sqlQuery, iSqlDb, aRemovable );

    TTime time;
    time.UniversalTime();

    for( TInt i = 0; i < sqlQuery.Count(); i++ )
        {
        sqlQuery[i]->PrepareL();
        sqlQuery[i]->BindValuesForLaunchL( aEntryId, time.Int64() );
        sqlQuery[i]->ExecuteL();
        sqlQuery[i]->CloseStatement();
        }

    CleanupStack::PopAndDestroy( &sqlQuery );
    }

// ---------------------------------------------------------------------------
// ExecuteDbPropertyL( const TDesC& aProperty, TDes& aPropertyValue )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteDbPropertyL( const TDesC& aProperty,
        TDes& aPropertyValue )
    {
    CCaSqlQuery* sqlQuery = CCaSqlQuery::NewLC( iSqlDb );
    sqlQuery->SetQueryL( KSQLGetProperty );
    sqlQuery->PrepareL();
    sqlQuery->BindValuesForPropertyL( aProperty );
    sqlQuery->ExecutePropertyL( aPropertyValue );
    sqlQuery->CloseStatement();
    CleanupStack::PopAndDestroy( sqlQuery );
    }

// ---------------------------------------------------------------------------
// ExecuteSetDbPropertyL( const TDesC& aProperty, const TDesC& aPropertyValue )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::ExecuteSetDbPropertyL( const TDesC& aProperty,
        const TDesC& aPropertyValue )
    {
    CCaSqlQuery* sqlQuery = CCaSqlQuery::NewLC( iSqlDb );
    sqlQuery->SetQueryL( KSQLUpdateProperty );
    sqlQuery->PrepareL();
    sqlQuery->BindValuesForPropertyUpdateL( aProperty, aPropertyValue );
    sqlQuery->ExecuteL();
    sqlQuery->CloseStatement();
    CleanupStack::PopAndDestroy( sqlQuery );
    }


// ---------------------------------------------------------------------------
// CCASqLiteStorage::RemoveOldEntriesFromLaunchTableL( TInt aDays )
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::RemoveOldEntriesFromLaunchTableL( TInt aDays )
    {
    TTime presentTime;
    presentTime.UniversalTime();
    TTime borderTime = presentTime - ( TTimeIntervalDays( aDays ) );

    CCaSqlQuery* sqlQuery = CCaSqlQuery::NewLC( iSqlDb );
    sqlQuery->SetQueryL( KSQLDeleteOldFromLaunch );
    sqlQuery->PrepareL();
    sqlQuery->BindValuesForLaunchL( 0, borderTime.Int64() );
    sqlQuery->ExecuteL();

    CleanupStack::PopAndDestroy( sqlQuery );
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::VerifyOrganizeParamsL( const RArray<TInt>& aEntryIds,
//      TCaOperationParams aParams);
//
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::VerifyOrganizeParamsL( const RArray<TInt>& aEntryIds,
        TCaOperationParams aParams )
    {
    switch( aParams.iOperationType )
        {
        case TCaOperationParams::ERemove:
            {
            CCaSqlQuery * query = CCaSqlQuery::NewLC( iSqlDb );
            CaSqlQueryCreator::PopulateQueryWithIdListL( aEntryIds.Count(),
                    query, KSQLOrganizeGetEntryIdCountFromGroup );

            query->PrepareL();
            query->BindValuesForOrganizeL( aEntryIds, aParams );
            TInt dbEntryCount;
            query->ExecuteL( dbEntryCount );
            query->CloseStatement();
            if( dbEntryCount < aEntryIds.Count() )
                {
                User::Leave( KErrArgument );
                }

            CleanupStack::PopAndDestroy( query );
            break;
            }
        case TCaOperationParams::EInsert:
            {
            CCaSqlQuery * query = CCaSqlQuery::NewLC( iSqlDb );
            CaSqlQueryCreator::PopulateQueryWithIdListL( 1, query,
                    KSQLOrganizeGetEntryIdCountFromGroup );
            RArray<TInt> id;
            CleanupClosePushL( id );
            id.AppendL( aParams.iBeforeEntryId );

            query->PrepareL();
            query->BindValuesForOrganizeL( id, aParams );
            TInt dbEntryCount;
            query->ExecuteL( dbEntryCount );
            query->CloseStatement();
            if( dbEntryCount == 0 )
                {
                User::Leave( KErrArgument );
                }

            CleanupStack::PopAndDestroy( &id );
            CleanupStack::PopAndDestroy( query );
            break;
            }
        case TCaOperationParams::EPrepend:
        case TCaOperationParams::EAppend:
        default:
            {
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CCASqLiteStorage::SetEntriesInProperOrderL
// ---------------------------------------------------------------------------
//
void CCaSqLiteStorage::SetEntriesInProperOrderL(
        const RArray<TInt>& aEntryIds,
        RPointerArray<CCaInnerEntry>& aResultContainer )
    {
    for( TInt i = 0; i < aEntryIds.Count(); i++ )
        {
        for( TInt j = i + 1; j < aResultContainer.Count(); j++ )
            {
            if( aEntryIds[i] == aResultContainer[j]->GetId() )
                {
                CCaInnerEntry* temporary( NULL );
                temporary = aResultContainer[i];
                aResultContainer[i] = aResultContainer[j];
                aResultContainer[j] = temporary;
                break;
                }
            }
        }
    }


