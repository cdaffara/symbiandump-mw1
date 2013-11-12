/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/

#ifndef BOOKMARKSCLIENTDEFS_H
#define BOOKMARKSCLIENTDEFS_H

#define BOOKMARKSCLIENT_PRIVATE(Class) \
                 private: \
                 inline Class##Private* priv_func() { return reinterpret_cast<Class##Private*>(priv_ptr); } \
                 inline const Class##Private* priv_func() const{ return static_cast<Class##Private*>(priv_ptr);} \
                 friend class Class##Private; \
                 void* priv_ptr;
#define BOOKMARKSCLIENT_PUBLIC(Class) \
        public: \
                inline Class* pub_func() { return static_cast<Class *>(pub_ptr); } \
                inline const Class* pub_func() const { return static_cast<const Class *>(pub_ptr); } \
        private: \
                friend class Class; \
                void* pub_ptr;

#define BOOKMARKSCLIENT_INITIALIZE(Class) \
                 priv_ptr=new Class##Private(); \
                 Class##Private* priv=priv_func();\
                 priv->pub_ptr = this;
#define BOOKMARKSCLIENT_PUBLICPTR(Class) Class * const pub = pub_func()
#define BOOKMARKSCLIENT_PRIVATEPTR(Class) Class##Private* const priv=priv_func()

#ifdef BUILDING_BOOKMARKS_CLIENT
    #define BOOKMARKSENGINECLIENT_EXPORT Q_DECL_EXPORT
#else
    #define BOOKMARKSENGINECLIENT_EXPORT Q_DECL_IMPORT
#endif 

#endif //BOOKMARKSCLIENTDEFS_H
