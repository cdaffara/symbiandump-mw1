/*
 * TagResults.cpp
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

#include "TagResults.h"


TagResults::TagResults(QSqlQuery *query, QWidget *parent) : QObject(parent), m_query(query)
{
}

QString *TagResults::nextTag()
{
    if (!hasMoreTags())
        return NULL;
    if (!m_query->next()) {
        m_query->clear();
        return NULL;
    }
    return new QString(m_query->value(0).toString());
}

bool TagResults::hasMoreTags() {
    return m_query->isActive();
}

TagResults::~TagResults()
{
    delete m_query;
}
