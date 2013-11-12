-- ONLY IF YOU SURE THAT WANT DEFAULT CONTENT


SELECT "castoragedb_variant_test - BEGIN" AS " ";


-- insert collection
INSERT INTO COLLECTION (COLLECTION_NAME, COL_APP_GROUP_NAME, ICON_FILENAME, FLAGS) VALUES ( 'Test Apps', 'Test Apps', "qtg_large_applications_user", 4 );

-- insert example urls
  INSERT INTO ITEM_TO_COLLECTION (ITEM_TYPE, ITEM_NAME, ICON_FILENAME, ITEM_DEST, COLLECTION_NAME) 
  VALUES ("url", 'Facebook', 'c:\data\Images\kqticaicons\facebook.png', "http://www.facebook.com/", 'Test Apps' );
  INSERT INTO ITEM_TO_COLLECTION (ITEM_TYPE, ITEM_NAME, ICON_FILENAME, ITEM_DEST, COLLECTION_NAME, ICON_SKIN_ID) 
  VALUES ("url", 'Google', 'c:\data\Images\kqticaicons\google.png', "http://www.google.com/", 'Test Apps', '4' );

-- insert example widgets
  INSERT INTO WIDGET_TO_COLLECTION (ITEM_TYPE, ITEM_NAME, ICON_FILENAME, ICON_SKIN_ID, PACKAGE_UID, LIBRARY, URI, COLLECTION_NAME) 
  VALUES ("widget", 'Clock', 'Z:\private\20022F35\import\widgetregistry\20022F6C\tclock.png', '4' , '20022f6c', 'Z:\private\20022F35\import\widgetregistry\20022F6C\hsclockwidgetplugin', 'hsclockwidgetplugin', 'Test Apps' ); 


SELECT "castoragedb_variant_test - END" AS " ";

