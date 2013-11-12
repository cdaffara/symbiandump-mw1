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
 * Description: caicondescription.cpp
 *
 */

#include <QDebug>

#include "caicondescription.h"
#include "caicondescription_p.h"


// ======== MEMBER FUNCTIONS ========

/*!
 \class CaIconDescription
 \brief This class describes entry's icon.

 \example
 \code
 QSharedPointer<CaService> service = CaService::instance();
 CaEntry entry;
 entry.setText("Text");
 entry.setEntryTypeName("TypeName");
 CaEntry * resultEntry = service->createEntry(entry);
 ...
 CaIconDescription iconDescription;
 iconDescription.setFileName( "z:/path/iconFileName.extension" );
 iconDescription.setBitmapId( 1234 );
 iconDescription.setMaskId( 1235 );
 iconDescription.setSkinMajorId( 987654 );
 iconDescription.setSkinMinorId( 654987 );

 resultEntry->setIconDescription( iconDescription );

 CaIconDescription entryIconDescription;
 entryIconDescription = resultEntry->iconDescription();

 ASSERT( iconDescription.fileName() == entryIconDescription.fileName() );
 ASSERT( iconDescription.bitmapId() == entryIconDescription.bitmapId() );
 ASSERT( iconDescription.maskId() == entryIconDescription.maskId() );
 ASSERT( iconDescription.skinMajorId() == entryIconDescription.skinMajorId() );
 ASSERT( iconDescription.skinMinorId() == entryIconDescription.skinMinorId() );

 delete resultEntry;
 \endcode

 */

/*!
 \var CaIconDescriptionPrivate::m_q
 Points to the CaEntry instance that uses this private implementation.
 */

/*!
 \var CaIconDescriptionPrivate::mId
 Id.
 */

/*!
 Default constructor.
 */
CaIconDescription::CaIconDescription() :
    m_d(new CaIconDescriptionPrivate(this))
{

}

/*!
 Copy constructor.
 \param const reference to CaIconDescription.
 */
CaIconDescription::CaIconDescription(
    const CaIconDescription &iconDescription) :
    m_d(iconDescription.m_d)
{

}

/*!
 Destructor.
 */
CaIconDescription::~CaIconDescription()
{

}

/*!
 Copy assignment operator.
 \param iconDescription const reference to CaIconDescription.
 \retval reference to CaIconDescription.
 */
CaIconDescription &CaIconDescription::operator=(
    const CaIconDescription &iconDescription)
{
    if (m_d != iconDescription.m_d) {
        m_d = iconDescription.m_d;
    }
    return *this;
}

/*!
 Returns icon id.
 \retval icon id.
 */
int CaIconDescription::id() const
{
    return m_d->id();
}

/*!
 Returns icon file name.
 \retval file name.
 */
QString CaIconDescription::filename() const
{
    return m_d->filename();
}

/*!
 Sets icon file name.
 \param QString with file name.
 */
void CaIconDescription::setFilename(const QString &filename)
{
    m_d->setFileName(filename);
}


/*!
 Returns skin id.
 \retval skin id.
 */
QString CaIconDescription::skinId() const
{
    return m_d->skinId();
}

/*!
 Sets skin id
 \param skin id
 */
void CaIconDescription::setSkinId(const QString &skinId)
{
    m_d->setSkinId(skinId);
}

/*!
 Returns icon application id.
 \retval icon application id.
 */
QString CaIconDescription::applicationId() const
{
    return m_d->applicationId();
}

/*!
 Sets icon application id.
 \param QString with icon application id.
 */
void CaIconDescription::setApplicationId(const QString &applicationId)
{
    m_d->setApplicationId(applicationId);
}

/*!
 Sets icon id
 \param id
 */
void CaIconDescription::setId(int id)
{
    m_d->setId(id);
}

/*!
 Construcor.
 \param iconDescriptionPublic pointer to CaIconDescription.
 */
CaIconDescriptionPrivate::CaIconDescriptionPrivate(
    CaIconDescription *iconDescriptionPublic) :
    m_q(iconDescriptionPublic), mId(0), mFilename(), mSkinId(), 
    mApplicationId()
{
}

/*!
 Destructor.
 */
CaIconDescriptionPrivate::~CaIconDescriptionPrivate()
{
}

/*!
 Returns icon id.
 \retval icon id.
 */
int CaIconDescriptionPrivate::id() const
{
    return mId;
}

/*!
 Returns file name.
 \retval file name.
 */
QString CaIconDescriptionPrivate::filename() const
{
    return mFilename;
}

/*!
 Sets a file name.
 \param fileName file name
 */
void CaIconDescriptionPrivate::setFileName(const QString &fileName)
{
    mFilename = fileName;
}

/*!
 Returns bitmap id.
 \retval bitmap id.
 */
QString CaIconDescriptionPrivate::skinId() const
{
    return mSkinId;
}

/*!
 Sets skin id.
 \param skinId skin id.
 */
void CaIconDescriptionPrivate::setSkinId(const QString &skinId)
{
    mSkinId = skinId;
}

/*!
 Returns icon application id.
 \retval icon application id.
 */
QString CaIconDescriptionPrivate::applicationId() const
{
    return mApplicationId;
}

/*!
 Sets icon application id.
 \param applicationId icon application id.
 */
void CaIconDescriptionPrivate::setApplicationId(const QString &applicationId)
{
    mApplicationId = applicationId;
}


/*!
 Sets icon id.
 \param id icon id.
 */
void CaIconDescriptionPrivate::setId(int id)
{
    mId = id;
}

