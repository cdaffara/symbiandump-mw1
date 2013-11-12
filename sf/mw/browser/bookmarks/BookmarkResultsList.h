/*
 * BookmarkResultsList.h
 *
 *  Created on: Aug 13, 2010
 *      Author: mmoretti
 */

#ifndef BOOKMARKRESULTSLIST_H_
#define BOOKMARKRESULTSLIST_H_

#include <QList>

template <class T>
class BookmarkResultsList : public QList<T>
{
  /*
public:
    inline QQueue() {}
    inline ~QQueue() {}
    inline void enqueue(const T &t) { QList<T>::append(t); }
    inline T dequeue() { return QList<T>::takeFirst(); }
    inline T &head() { return QList<T>::first(); }
    inline const T &head() const { return QList<T>::first(); }
*/
};

#endif /* BOOKMARKRESULTSLIST_H_ */
