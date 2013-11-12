/*
 * BookmarkResults.cpp
 *
 *  Created on: Aug 13, 2010
 *      Author: mmoretti
 */

#include <QString>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>
#include <QVariant>

#include "BookmarkResults.h"
#include "BookmarkFav.h"

BookmarkResults::BookmarkResults(QSqlQuery *query/*, QWidget *parent*/) : 
    /*QObject(parent), */m_query(query)
{
//    setObjectName("bookmarkResults");
}

BookmarkFav *BookmarkResults::nextBookmark()
{
    if (!m_query->isActive())
        return NULL;
    if (!m_query->next()) {
        m_query->clear();
        return NULL;
    }
    bool dummy;
    return
        new BookmarkFav(m_query->value(0).toInt(&dummy),
            m_query->value(1).toString(),
            m_query->value(2).toString(),
            m_query->value(3).toInt(&dummy));
}

BookmarkResults::~BookmarkResults()
{
    delete m_query;
}

bool BookmarkResults::isMoreBookmarks()
{
	//return (m_query->isValid());
	return (m_query->isActive());
}

//void BookmarkResults::registerMetaType()
//{
//    qRegisterMetaType<BookmarkResults>("BookmarkResults");
//}
//
