/*
 * defaultBookmarks.xml.h
 *
 *  Created on: Aug 20, 2010
 *      Author: chriwhit
 */

#ifndef DEFAULTBOOKMARKS_XML_H_
#define DEFAULTBOOKMARKS_XML_H_

#include "bookmarksapi.h"

extern const QString defaultBookmarksList = "<!DOCTYPE xbel>\n\
<xbel version='1.0'>\n\
   <bookmark href='http://www.nokia.com'>\n\
       <title>Nokia</title>\n\
           <info>\n\
               <metadata owner='http://nokia.com' >\n\
                   <tags>\n\
                       <tag>nokia</tag>\n\
                       <tag>default</tag>\n\
                   </tags>\n\
               </metadata>\n\
           </info>\n\
       <desc></desc>\n\
   </bookmark>\n\
   <bookmark href='http://www.google.com'>\n\
       <title>Google</title>\n\
           <info>\n\
               <metadata owner='http://nokia.com' >\n\
                   <tags>\n\
                       <tag>google</tag>\n\
                       <tag>nokia</tag> \n\
                       <tag>search</tag>\n\
                       <tag>default</tag>\n\
                   </tags>\n\
               </metadata>\n\
           </info>\n\
       <desc></desc>\n\
   </bookmark>   \n\
   <bookmark href='http://www.yahoo.com'>\n\
       <title>Yahoo</title>\n\
           <info>\n\
               <metadata owner='http://nokia.com' >\n\
                   <tags>\n\
                       <tag>yahoo</tag>\n\
                       <tag>nokia</tag>\n\
                       <tag>search</tag>\n\
                       <tag>default</tag>\n\
                   </tags>\n\
               </metadata>\n\
           </info>\n\
       <desc></desc>\n\
   </bookmark>\n\
   <bookmark href='http://www.gmail.com'>\n\
       <title>Google Mail</title>\n\
           <info>\n\
               <metadata owner='http://nokia.com' >\n\
                   <tags>\n\
                       <tag>google</tag>\n\
                       <tag>email</tag>\n\
                       <tag>nokia</tag>\n\
                       <tag>default</tag>\n\
                   </tags>\n\
               </metadata>\n\
           </info>\n\
       <desc></desc>\n\
   </bookmark>\n\
   <bookmark href='http://www.ovi.com/services/'>\n\
       <title>Nokia Ovi Services</title>\n\
           <info>\n\
               <metadata owner='http://nokia.com' >\n\
                   <tags>\n\
                       <tag>ovi</tag>\n\
                       <tag>nokia</tag>\n\
                       <tag>default</tag>\n\
                   </tags>\n\
               </metadata>\n\
           </info>\n\
       <desc></desc>\n\
   </bookmark>\n\
</xbel>";

#endif /* DEFAULTBOOKMARKS_XML_H_ */
