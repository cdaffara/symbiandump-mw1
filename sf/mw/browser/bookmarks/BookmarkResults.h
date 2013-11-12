/*
 * BookmarkResults.h
 *
 *  Created on: Aug 13, 2010
 *      Author: mmoretti
 */

#ifndef BOOKMARKRESULTS_H_
#define BOOKMARKRESULTS_H_

#include <QObject>

#include "bookmarksapi.h"
class QSqlQuery;
class BookmarkFav;

class BOOKMARKSAPI_EXPORT BookmarkResults : public QObject {

    Q_OBJECT

    Q_PROPERTY(BookmarkFav* next READ nextBookmark)

public slots:
    BookmarkFav *nextBookmark();
    // Javascript hack
    bool isMoreBookmarks();

public:
//    BookmarkResults(QSqlQuery *query = 0, QWidget *parent = 0);
    BookmarkResults(QSqlQuery *query/*, QWidget *parent = 0*/);
    ~BookmarkResults();
//    static void registerMetaType();

protected:
    QSqlQuery*    m_query;

};

// For scripting
//Q_DECLARE_METATYPE(BookmarkResults*)


#endif /* BOOKMARKRESULTS_H_ */
