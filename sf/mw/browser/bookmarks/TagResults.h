/*
 * TagResults.h
 *
 *  Created on: Aug 13, 2010
 *      Author: mmoretti
 */

#ifndef TAGRESULTS_H_
#define TAGRESULTS_H_

#include <QString>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QWidget>

#include "bookmarksapi.h"

class BOOKMARKSAPI_EXPORT TagResults : public QObject {

    Q_OBJECT

    Q_PROPERTY(QString* next READ nextTag)
    Q_PROPERTY(bool hasMoreTags READ hasMoreTags)

public slots:
    QString *nextTag();
 
public:
    TagResults(QSqlQuery *query, QWidget *parent = 0);
    ~TagResults();
    
public: 
    bool hasMoreTags();
    
protected:
    QSqlQuery*    m_query;
};

// For scripting
//Q_DECLARE_METATYPE(TagResults *)


#endif /* TAGRESULTS_H_ */
