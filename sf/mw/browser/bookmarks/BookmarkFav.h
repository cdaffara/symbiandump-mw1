/*
 * BookmarkFav1.h
 *
 *  Created on: Aug 13, 2010
 *      Author: mmoretti
 */

#ifndef BOOKMARKFAV_H_
#define BOOKMARKFAV_H_

#include <QString>
#include <QObject>
#include "bookmarksapi.h"

class BOOKMARKSAPI_EXPORT BookmarkFav : public QObject {

    Q_OBJECT

    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(int sortIndex READ sortIndex WRITE setSortIndex)

public:
    BookmarkFav(int id, QString title, QString url, int sortIndex/*, QString description*/):
            m_id(id),
            m_title(title),
            m_url(url),
            m_sortIndex(sortIndex)/*,
            m_description(description) */{}; 
            //TODO: Support XBEL description tag  
    BookmarkFav() : m_id(-1), m_title(""), m_url(""), m_sortIndex(-1) {};

public slots:
    int id() const { return m_id; }
    QString url() const { return m_url; }
    QString title() const { return m_title; }
    int sortIndex() const { return m_sortIndex; }
    /* QString description() const { return m_description; } */

    void setId(int id) {m_id = id;}
    void setUrl(QString url) { m_url = url; }
    void setTitle(QString title) { m_title = title; }
    void setSortIndex(int sortIndex) { m_sortIndex = sortIndex; }
    /*void setDescription(QString description) { m_description = description; } */

//    static void registerMetaType();

private:
    int m_id;
    QString m_url;
    QString m_title;
    int m_sortIndex;
    /* QString m_description; */

};

// For scripting
//Q_DECLARE_METATYPE(BookmarkFav *)


#endif /* BOOKMARKFAV_H_ */
