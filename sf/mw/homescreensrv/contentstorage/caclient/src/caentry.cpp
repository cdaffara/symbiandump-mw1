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
 * Description: caentry.cpp
 *
 */

#include <QSharedDataPointer>
#include <QDebug>
#include <HbIcon>

#include "caentry.h"
#include "caentry_p.h"
#include "caobjectadapter.h"
#include "caiconcache.h"
#include "caclienttest_global.h"
#include "camenuiconutility.h"


// ======== MEMBER FUNCTIONS ========

/*!
 \class CaEntry
 \brief This abstract class describes particular entry.
 To create instance of CaEntry object, you have to use service's object's
 createEntry() method.
 \example
 \code
 QSharedPointer<CaService> service = CaService::instance();
 // default you create item (ItemEntryRole)
 CaEntry entry;
 // to create CaEntry invoking setText() and setEntryTypeName() methods
 is obligatoried
 entry.setText("Text");
 entry.setEntryTypeName("TypeName");
 CaEntry * resultEntry = service->createEntry(entry);
 ...
 delete resultEntry;
 ...
 // if you want create group, you should use entry role parametr
 CaEntry entryGroup(GroupEntryRole);
 ...
 resultEntry = service->createEntry(entryGroup);
 \endcode
 */

/*!
 \var CaEntryPrivate::m_q
 Points to the CaEntry instance that uses
 this private implementation.
 */

/*!
 \var CaEntryPrivate::mId
 Id.
 */

/*!
 Constructor.
 \param entryRole entry's role.
 */
CaEntry::CaEntry(EntryRole entryRole) :
    m_d(new CaEntryPrivate(this))
{
    m_d->setRole(entryRole);
}

/*!
 Copy constructor.
 \param entry const reference to CaEntry.
 \code
 ...
 CaEntry * copyEntry( *resultEntry );
 \endcode
 */
CaEntry::CaEntry(const CaEntry &entry) :
    m_d(new CaEntryPrivate(this))
{
    *m_d = *(entry.m_d);
}

/*!
 Copy assignment operator.
 \param entry const reference to CaEntry.

 \retval reference to CaEntry.

 \code
 ...
 CaEntry * copyEntry;
 copyEntry = resultEntry;
 \endcode
 */
CaEntry &CaEntry::operator=(const CaEntry &entry)
{
    if (this != &entry) {
        m_d = entry.m_d;
    }
    return *this;
}

/*!
 Destructor.
 */
CaEntry::~CaEntry()
{

}

/*!
 Returns item id.
 \retval item id.

 \code
 ...
 // after create entry, we can get entry's id
 resultEntry = service->createEntry(entry);
 int entryId = resultEntry->id();
 ...
 \endcode
 */
int CaEntry::id() const
{
    return m_d->id();
}

/*!
 Returns item name.
 \retval name of the item.

 \code
 ...
 QString entryText = resultEntry->text();
 ...
 \endcode
 */
QString CaEntry::text() const
{
    return m_d->text();
}

/*!
 Sets localized name of the item.
 \param text new name of the item.
 \param localized set to true if its localized

 \code
 ...
 QString entryText( QString("EntryText") );
 resultEntry->setText( entryText, true);
 ...
 \endcode
 */
void CaEntry::setText(const QString &text, bool localized)
{
    m_d->setText(text, localized);
}

/*!
Returns item description.
\retval description of the item.

\code
...
QString entryDescription = resultEntry->description();
...
\endcode
*/
QString CaEntry::description() const
{
    return m_d->description();
}


/*!
Sets localized description of the item.
\param new description of the item.

\code
...
QString entryDescription( QString("EntryDescription") );
resultEntry->entryDescription(entryDescription);
...
\endcode
*/
void CaEntry::setDescription(const QString &description,
        bool localized)
{
    m_d->setDescription(description, localized);
}

/*!
 Returns copy of icon description of the entry.
 \retval icon Description (CaIconDescription).

 \code
 ...
 CaIconDescription iconDesc;
 iconDesc.setBitmapId(5555);
 iconDesc.setFilename(QString("fileName"));
 iconDesc.setMaskId(5556);
 iconDesc.setSkinMajorId(5557);
 iconDesc.setSkinMinorId(5558);

 resultEntry->setIconDescription( iconDesc );

 CaIconDescription entryIcon;
 entryIcon = resultEntry->iconDescription();
 ...
 \b Output:
 iconDesc == entryIcon

 \endcode
 */
CaIconDescription CaEntry::iconDescription() const
{
    return m_d->iconDescription();
}

/*!
 Sets icon description
 \param iconDescription const reference to CaIconDescription

 \code
 ...
 CaIconDescription iconDesc;
 iconDesc.setBitmapId(5555);
 iconDesc.setFilename(QString("fileName"));
 iconDesc.setMaskId(5556);
 iconDesc.setSkinMajorId(5557);
 iconDesc.setSkinMinorId(5558);

 resultEntry->setIconDescription( iconDesc );
 ...
 \b Output:
 iconDesc == resultEntry->iconDescription();

 \endcode
 */
void CaEntry::setIconDescription(const CaIconDescription &iconDescription)
{
    m_d->setIconDescription(iconDescription);
}

/*!
 Returns items flags.
 \retval flags.

 \code
 ...
 TUint flags = contentarsenal::RemovableEntryFlag
 |contentarsenal::VisibleEntryFlag;
 resultEntry->setFlags( flags );

 EntryFlags entryFlags = resultEntry->flags();

 \b Output:
 flags == entryFlags

 \endcode
 */
EntryFlags CaEntry::flags() const
{
    return m_d->flags();
}

/*!
 Sets flags.
 \param flags entry flags.

 \code
 ...
 TUint flags = contentarsenal::RemovableEntryFlag
 |contentarsenal::VisibleEntryFlag;
 resultEntry->setFlags( flags );
 ...
 \endcode
 */
void CaEntry::setFlags(EntryFlags flags)
{
    m_d->setFlags(flags);
}

/*!
 Returns a name of an entry type.
 \retval name of entry type.

 \code
 CaEntry entry;
 entry.setText( "Text" );
 entry.setEntryTypeName( "TypeName" );
 CaEntry * resultEntry = service->createEntry( entry );

 QString entryTypeName = resultEntry->entryTypeName();
 \b Output:
 entryTypeName == "TypeName"

 \endcode
 */
QString CaEntry::entryTypeName() const
{
    return m_d->entryTypeName();
}

/*!
 Sets name of entry type.
 \param entryTypeName name of entry type (const reference).

 \code
 ...
 resultEntry->setEntryTypeName("EntryTypeName");

 \b Output:
 resultEntry->entryTypeName == "EntryTypeName"

 \endcode
 */
void CaEntry::setEntryTypeName(const QString &entryTypeName)
{
    m_d->setEntryTypeName(entryTypeName);
}

/*!
 Returns item attributes.
 \retval map of attributes indexed by their names.

 \code
 ...
 QString attrName_1( "name_1" );
 QString attrValue_1( "value_1" );
 QString attrName_2( "name_2" );
 QString attrValue_2( "value_2" );
 resultEntry->setAttribute( attrName_1, attrValue_1 );
 resultEntry->setAttribute( attrName_2, attrValue_2 );
 ...
 QMap<QString, QString> attrMap = entryAttrValue = resultEntry->attributes();

 \b Output:
 attrMap[0] == name: name_1, value: value_1
 attrMap[1] == name: name_2, value: value_2

 \endcode
 */
QMap<QString, QString> CaEntry::attributes() const
{
    return m_d->attributes();
}

/*!
 \param name name of an attribute
 \retval value of attribute

 \code
 ...
 QString attrName_1( "name_1" );
 QString attrValue_1( "value_1" );
 QString attrName_2( "name_2" );
 QString attrValue_2( "value_2" );
 resultEntry->setAttribute( attrName_1, attrValue_1 );
 resultEntry->setAttribute( attrName_2, attrValue_2 );
 ...
 QString entryAttrValue = resultEntry->attribute( attrName_1 );

 \b Output:
 entryAttrValue == "attrValue_1"

 \endcode
 */
QString CaEntry::attribute(const QString &name) const
{
    return m_d->attribute(name);
}

/*!
 Sets attribute.
 \param name name of an attribute.
 \param value value of an attribute.

 \code
 ...
 QString attrName_1( "name_1" );
 QString attrValue_1( "value_1" );
 QString attrName_2( "name_2" );
 QString attrValue_2( "value_2" );
 resultEntry->setAttribute( attrName_1, attrValue_1 );
 resultEntry->setAttribute( attrName_2, attrValue_2 );

 \endcode
 */
void CaEntry::setAttribute(const QString &name, const QString &value)
{
    m_d->setAttribute(name, value);
}

/*!
 Removes attribute.
 \param name name of an attribute.

 \code
 ...
 QString attrName_1( "name_1" );
 QString attrValue_1( "value_1" );
 QString attrName_2( "name_2" );
 QString attrValue_2( "value_2" );
 resultEntry->setAttribute( attrName_1, attrValue_1 );
 resultEntry->setAttribute( attrName_2, attrValue_2 );
 resultEntry->removeAttribute( attrName_2 );

 \endcode
 */
void CaEntry::removeAttribute(const QString &name)
{
    m_d->removeAttribute(name);
}

/*!
 Creates an icon.
 \param  size icon size to display
 \retval created icon (HbIcon).

 \code
 ...
 QSize iconSize( 50, 80 );
 HbIcon icon = resultEntry->makeIcon( iconSize );

 \b Output:
 icon.width() == 50
 icon.height() == 80
 icon.size() == iconSize;
 \endcode
 */
HbIcon CaEntry::makeIcon(const QSizeF &size) const
{
    CACLIENTTEST_FUNC_ENTRY("CaEntry::makeIcon");
    HbIcon icon = CaIconCache::cache()->icon(*this, size);
    if (icon.isNull()) {
        icon = m_d->makeIcon(size);
        CaIconCache::cache()->insert(*this, size, icon);
    }
    CACLIENTTEST_FUNC_EXIT("CaEntry::makeIcon");
    return icon;
}

/*!
 Sets entry id.
 \param id item id.
 */
void CaEntry::setId(int id)
{
    m_d->setId(id);
}
/*!
 Gets entry role.
 \retval entry's role.

 \code
 CaEntry entry;
 entry.setText("Text");
 entry.setEntryTypeName("TypeName");
 CaEntry * resultEntry = service->createEntry(entry);

 EntryRole entryRole = resultEntry->role();

 \b Output:
 entryRole == ItemEntryRole

 \endcode

 \code
 CaEntry entryGroup( GroupEntryRole );
 entryGroup.setText("Text");
 entryGroup.setEntryTypeName("TypeName");
 CaEntry * resultGroup = service->createEntry(entryGroup);
 EntryRole groupRole = resultGroup->role();

 \b Output:
 groupRole == GroupEntryRole

 \endcode
 */
EntryRole CaEntry::role() const
{
    return m_d->role();
}


bool CaEntry::isLocalized(LocalizationType localized)  const
{
    return m_d->isLocalized(localized);  
}

/*
 Constructor
 \param entryPublic associated public entry
 */
CaEntryPrivate::CaEntryPrivate(CaEntry *entryPublic) :
    m_q(entryPublic), mId(0), mText(), mDescription(), mIconDescription(),
    mFlags(RemovableEntryFlag|VisibleEntryFlag),mEntryTypeName(),
    mAttributes(), mEntryRole(ItemEntryRole), 
    mTextLocalized(false), mDescriptionLocalized(false)
{
}
/*!
 Copy assignment operator.
 \param entry const reference to CaEntryPrivate.
 \retval reference to CaEntryPrivate.
 */
CaEntryPrivate &CaEntryPrivate::operator=(const CaEntryPrivate &entry)
{
    mId = entry.mId;
    mText = entry.mText;
    mDescription = entry.mDescription;
    mIconDescription = entry.mIconDescription;
    mFlags = entry.mFlags;
    mEntryTypeName = entry.mEntryTypeName;
    mAttributes = entry.mAttributes;
    mEntryRole = entry.mEntryRole;
    mTextLocalized = entry.mTextLocalized;
    mDescriptionLocalized = entry.mDescriptionLocalized;
    return *this;
}

/*!
 Destructor
 */
CaEntryPrivate::~CaEntryPrivate()
{
}

/*!
 \retval item id
 */
int CaEntryPrivate::id() const
{
    return mId;
}

/*!
 \retval name of the item.
 */
QString CaEntryPrivate::text() const
{
    return mText;
}

/*!
 Sets localized name of the item.
 \param text new name of the item.
 */
void CaEntryPrivate::setText(const QString &text, bool localized)
{
    mText = text;
    mTextLocalized = localized;
}

/*!
\retval description of the item.
*/
QString CaEntryPrivate::description() const
{
    return mDescription;
}

/*!
Sets description of the item.
\param text new name of the item.
*/
void CaEntryPrivate::setDescription(const QString &description,
        bool localized)
{
    mDescription = description;
    mDescriptionLocalized = localized;
}

/*!
 \retval icon Description (CaIconDescription).
 */
CaIconDescription CaEntryPrivate::iconDescription() const
{
    return mIconDescription;
}

/*!
 Sets icon description
 \param iconDescription const reference to CaIconDescription
 */
void CaEntryPrivate::setIconDescription(
    const CaIconDescription &iconDescription)
{
    mIconDescription = iconDescription;
}

/*!
 \retval flags
 */
EntryFlags CaEntryPrivate::flags() const
{
    return mFlags;
}

/*!
 Sets flags.
 \param flags entry flags.
 */
void CaEntryPrivate::setFlags(EntryFlags flags)
{
    mFlags = flags;
}

/*!
 \retval name of entry type.
 */
QString CaEntryPrivate::entryTypeName() const
{
    return mEntryTypeName;
}

/*!
 Sets name of entry type.
 \param entryTypeName name of entry type (const reference)
 */
void CaEntryPrivate::setEntryTypeName(const QString &entryTypeName)
{
    mEntryTypeName = entryTypeName;
}

/*!
 \retval map of attributes indexed by their names
 */
QMap<QString, QString> CaEntryPrivate::attributes() const
{
    return mAttributes;
}

/*!
 \param name name of an attribute
 \retval value of attribute
 */
QString CaEntryPrivate::attribute(const QString &name) const
{
    return mAttributes.value(name);
}

/*!
 Sets attribute.
 \param name name of an attribute.
 \param value value of an attribute.
 */
void CaEntryPrivate::setAttribute(const QString &name, const QString &value)
{
    mAttributes.insert(name, value);
}

/*!
 Removes attribute.
 \param name name of an attribute.
 */
void CaEntryPrivate::removeAttribute(const QString &name)
{
    mAttributes.remove(name);
}
/*!
 Creates an icon.
 \param  size icon size to display
 \retval created icon (HbIcon).
 */
HbIcon CaEntryPrivate::makeIcon(const QSizeF &size) const
{
    return CaMenuIconUtility::getEntryIcon(*m_q, size);
}

/*!
 Sets entry id.
 \param id item id.
 */
void CaEntryPrivate::setId(int id)
{
    mId = id;
}
/*!
 Sets entry role.
 \retval entry's role.
 */
EntryRole CaEntryPrivate::role() const
{
    return mEntryRole;
}
/*!
 Gets entry role.
 \param role entry's role.
 */
void CaEntryPrivate::setRole(const EntryRole &role)
{
    mEntryRole = role;
}

bool CaEntryPrivate::isLocalized(LocalizationType localized)  const
{
    if(localized == NameLocalized)
    {
        return mTextLocalized;
    }
    if(localized == DescriptionLocalized)
    {
        return mDescriptionLocalized;
    }
    return false;
    
}


