/****************************************************************************
**
** Copyright (C) 2008-2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (developer.feedback@nokia.com)
**
** This file is part of the HbCore module of the UI Extensions for Mobile.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at developer.feedback@nokia.com.
**
****************************************************************************/


#include <QPoint>
#include <hbglobal.h>
#include "hbparameterlengthlimiter.h"
#include "hbparameterlengthlimiter_p.h"

/*!
    @stable
    @hbcore
    \class HbParameterLengthLimiter
    \brief HbParameterLengthLimiter is a class that offers support to insert different types of arguments into a QString.

    Supports upto 9 argument inserts per QString.

    Supports different kinds of indicators that are used for marking where arguments are inserted
        - %x for inserting QStrings, where x is number from 1 to 9 indicating the order in which argumentss are positioned.
        - %[y]x for inserting QStrings, x indicates argument inserting order and y is a number indicating the maximum length
                for the argument e.g. %[10]2 would mean that the inserted QStrings maximum length is 10 characters and that the
                limited argument is the second argument to be inserted.
        - %Lx   for inserting numbers, x is used to indicate argument inserting order.

    Example of how to use HbParameterLengthLimiter:
    \snippet{unittest_HbParameterLengthLimiter/unittest_HbParameterLengthLimiter.cpp,1}

*/

/*!
    Constructs a HbParameterLengthLimiter with \a HbParameterLengthLimiter.
    
    \attention Cross-Platform API
    
    \param a HbParameterLengthLimiter that will have arguments inserted
*/
HbParameterLengthLimiter::HbParameterLengthLimiter( const HbParameterLengthLimiter& a )
{
    p = new HbParameterLengthLimiterPrivate();
    p->str = a.p->str;
}

/*!
    Constructs a HbParameterLengthLimiter with \a QString.
    
    \attention Cross-Platform API
    
    \param a QString that will have arguments inserted
*/
HbParameterLengthLimiter::HbParameterLengthLimiter( QString a )
{
    p = new HbParameterLengthLimiterPrivate();
    p->str = a;
}

/*!
    Constructs a HbParameterLengthLimiter with \a char*.
    
    \attention Cross-Platform API
    
    \param a char string that will have arguments inserted
*/
HbParameterLengthLimiter::HbParameterLengthLimiter( const char* a )
{
    p = new HbParameterLengthLimiterPrivate();
    p->str = hbTrId(a);
}

/*!
    Constructs a HbParameterLengthLimiter with \a char* and \a int.
    
    \attention Cross-Platform API
    
    \param a char string that will have arguments inserted
    \param n used to identify which plural form to use
*/
HbParameterLengthLimiter::HbParameterLengthLimiter( const char* a, int n )
{
    p = new HbParameterLengthLimiterPrivate();
    p->str = hbTrId(a, n);
}

/*!
    Conp->structs a HbParameterLengthLimiter without a predefined QString.
    
    \attention Cross-Platform API  
*/
HbParameterLengthLimiter::HbParameterLengthLimiter()
{
    p = new HbParameterLengthLimiterPrivate();
    p->str = "";
}

/*!
    Destructor
*/
HbParameterLengthLimiter::~HbParameterLengthLimiter()
{
    delete p;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldwidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg(qlonglong a,
                                      int fieldwidth,
                                      int base,
                                      const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldwidth,base,tmpChar);
    return *this;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldwidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( qulonglong a,
                                       int fieldwidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldwidth,base,tmpChar);

    return *this;
 }

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldwidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( long a,
                                       int fieldwidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldwidth,base,tmpChar);

    return *this;
 }

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldwidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( ulong a,
                                       int fieldwidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldwidth,base,tmpChar);
    return *this;
 }

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( int a,
                                       int fieldWidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldWidth,base,tmpChar);

    return *this;
 }

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( uint a,
                                       int fieldWidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldWidth,base,tmpChar);
    return *this;
 }

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( short a,
                                       int fieldWidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldWidth,base,tmpChar);
    return *this;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a number that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param base defines the number base
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( ushort a,
                                       int fieldWidth,
                                       int base,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a,fieldWidth,base,tmpChar);
    return *this;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a argument a is formatted according to the specified format and precision
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param fmt defines the format to be used
    \param prec defines the precision to be used
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( double a,
                                       int fieldWidth,
                                       char fmt,
                                       int prec,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a, fieldWidth, fmt, prec, tmpChar);
    return *this;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a character that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( char a,
                                       int fieldWidth,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a, fieldWidth, tmpChar);
    return *this;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a character that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( QChar a,
                                       int fieldWidth,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;
    p->str = p->str.arg(a, fieldWidth, tmpChar);
    return *this;
}

/*!
    Inserts an \a argument to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a string that will be inserted to the QString
    \param fieldWidth specifies the minimum amount of space that a is padded to and filled with the character fillChar
    \param fillChar defines the fill character
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a,
                                       int fieldWidth,
                                       const QChar &fillChar)
{
    QChar tmpChar = fillChar;

    QString result = "";
    int i = 0;

    int signPosition;

    QPoint position(10,0);
    while ( i < p->str.length() ) {
        if ( p->str.at(i) == '%' ) {
            signPosition = i;
            i++;
            if ( i >= p->str.length() ) {
                break;
            }
            if ( p->str.at(i).isDigit() ) {
                //normal QString
                QString number = p->str.at( i );
                if( number.toInt() < position.x() ) {
                    position.setX( number.toInt() );
                    position.setY( signPosition );
                }
            } else if ( p->str.at(i) == '[' ) {
                //limited QString
                i++;
                if( i >= p->str.length() ) {
                    break;
                }

                while( p->str.at(i) != ']' ) {
                    i++;
                    if( i >= p->str.length() ) {
                        break;
                    }
                }

                i++;
                if( i >= p->str.length() ) {
                    break;
                }

                if( p->str.at(i).isDigit() ) {
                    QString number = p->str.at(i);
                    if( number.toInt() < position.x() ) {
                        position.setX( number.toInt() );
                        position.setY( signPosition );
                    }
                }
                else{
                    --i;
                }
            }
        }
        i++;
    }

    i = position.y();
    if ( p->str.at(i) == '%' ) {
        result.append( p->str.at(i) );
        i++;
        if ( p->str.at(i).isDigit() ) {
            //normal QString

            p->str = p->str.arg( a, fieldWidth, tmpChar );
        } else if ( p->str.at(i) == '[' ) {
            //limited QString

            i++;
            if( i >= p->str.length() ) {
                return *this;
            }
            QString limiter = "";
            bool limitedFound = false;
            while (p->str.at(i) != ']'){
                if ( p->str.at(i).isNumber() ) {
                    limitedFound = true;
                    limiter.append( p->str.at(i) );
                    i++; 
                    if ( i >= p->str.length() ) {
                        return *this;
                    }
                }
            }
            i++;
            if ( i >= p->str.length() ) {
                return *this;
            }
            if( limitedFound && p->str.at(i).isNumber() ) {
                i++;
                result = p->str.left( position.y() );
                int length = limiter.toInt();
                QString limitedA = a;

                int o = 0;
                int u = 0;
                if( length < a.length() ) {
                    while ( o <= a.length() ) {
                        // If character is Thai vowel or tone mark, skip it as it does not have length
                        // U+E34 = 3636, U+E3B = 3643, U+E47 = 3655, U+E4D = 3661
                        // or zero wide space 200B = 8203
                        if ( (a.at(o).unicode() < 3636 || a.at(o).unicode() > 3643)
                             && (a.at(o).unicode() < 3655 || a.at(o).unicode() > 3661)
                             && a.at(o).unicode() != 8203 ) {
                            u++;
                        }
                        if ( u == length+1 ) {
                            limitedA = a.left( o) ;
                            // Add ellipsis as a mark that QString is truncated
                            limitedA.append( QChar(0x2026) );
                            break;
                        }
                        o++;
                    }
                }
                result.append( "%1" );
                int cutter = p->str.length() - i ;
                QString tmp = p->str.right(cutter);
                result.append( tmp );
                
                p->str = result.arg( limitedA, fieldWidth, tmpChar );
            }

        }
    }
    return *this;
}

/*!
    Inserts an arguments a1 and a2 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2 )
{
    this->arg(a1).arg(a2);
    return *this;
}

/*!
    Inserts an arguments a1, a2 and a3 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2,
                                       const QString &a3)
{
    this->arg(a1, a2).arg(a3);
    return *this;
}

/*!
    Inserts an arguments a1, a2, a3 and a4 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
    \param a4 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2,
                                       const QString &a3,
                                       const QString &a4 )
{
    this->arg(a1, a2, a3).arg(a4);
    return *this;
}

/*!
    Inserts an arguments a1, a2, a3, a4 and a5 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
    \param a4 string that will be inserted to the QString
    \param a5 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2,
                                       const QString &a3,
                                       const QString &a4,
                                       const QString &a5 )
{
    this->arg(a1, a2, a3, a4 ).arg(a5);
    return *this;
}

/*!
    Inserts an arguments a1, a2, a3, a4, a5 and a6 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
    \param a4 string that will be inserted to the QString
    \param a5 string that will be inserted to the QString
    \param a6 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2,
                                       const QString &a3,
                                       const QString &a4,
                                       const QString &a5,
                                       const QString &a6 )
{
    this->arg(a1, a2, a3, a4, a5).arg(a6);
    return *this;
}

/*!
    Inserts an arguments a1, a2, a3, a4, a5, a6 and a7 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
    \param a4 string that will be inserted to the QString
    \param a5 string that will be inserted to the QString
    \param a6 string that will be inserted to the QString
    \param a7 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2,
                                       const QString &a3,
                                       const QString &a4,
                                       const QString &a5,
                                       const QString &a6,
                                       const QString &a7 )
{
    this->arg(a1, a2, a3, a4, a5, a6).arg(a7);
    return *this;
}

/*!
    Inserts an arguments a1, a2, a3, a4, a5, a6, a7 and a8 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
    \param a4 string that will be inserted to the QString
    \param a5 string that will be inserted to the QString
    \param a6 string that will be inserted to the QString
    \param a7 string that will be inserted to the QString
    \param a8 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2,
                                       const QString &a3,
                                       const QString &a4,
                                       const QString &a5,
                                       const QString &a6,
                                       const QString &a7,
                                       const QString &a8 )
{
    this->arg(a1, a2, a3, a4, a5, a6, a7).arg(a8);
    return *this;
}

/*!
    Inserts an arguments a1, a2, a3, a4, a5, a6, a7, a8 and a9 to a HbParameterLengthLimiter QString.
    
    \attention Cross-Platform API
    
    \param a1 string that will be inserted to the QString
    \param a2 string that will be inserted to the QString
    \param a3 string that will be inserted to the QString
    \param a4 string that will be inserted to the QString
    \param a5 string that will be inserted to the QString
    \param a6 string that will be inserted to the QString
    \param a7 string that will be inserted to the QString
    \param a8 string that will be inserted to the QString
    \param a9 string that will be inserted to the QString
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::arg( const QString &a1,
                                       const QString &a2, 
                                       const QString &a3,
                                       const QString &a4, 
                                       const QString &a5, 
                                       const QString &a6,
                                       const QString &a7, 
                                       const QString &a8, 
                                       const QString &a9 )
{
    this->arg(a1, a2, a3, a4, a5, a6, a7, a8).arg(a9);
    return *this;
}

/*!
    Changes the QString that is to be used for inserting arguments to QString.
    
    \attention Cross-Platform API
    
    \param a QString that will be used for inserting arguments
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::operator=( const QString &a )
{
    p->str = a;
    return *this;
}

/*!
    Changes the QString that is to be used for inserting arguments to HbParameterLengthLimiter.
    
    \attention Cross-Platform API
    
    \param a HbParameterLengthLimiter holding the QString that will be used for inserting arguments
*/
HbParameterLengthLimiter& HbParameterLengthLimiter::operator=( const HbParameterLengthLimiter &a )
{
    p->str = a.p->str;
    return *this;
}

/*!
    Returns the current QString.
    
    \attention Cross-Platform API    
*/
HbParameterLengthLimiter::operator QString() const
{
    return p->str;
}



