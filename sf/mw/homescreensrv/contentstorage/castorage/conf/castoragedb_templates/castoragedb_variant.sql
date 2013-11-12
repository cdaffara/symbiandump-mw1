SELECT "------------------------------------" AS " ";
SELECT "castoragedb_variant - BEGIN" AS " ";

{%- macro stripPath(IconPath) %}
    {%- set cont = true -%}
    {%- set Icon = '' -%}
    {%- for c in IconPath|reverse|list -%}
        {%- if c == '\\' or c == '/' -%}
            {%- set cont = false -%}
        {%- endif %}
        {%- if cont %}
            {%- set Icon = c ~ Icon %}
        {%- endif %}
        {%- if loop.last -%}
            {%- if Icon == "dummy" %}
            {% else -%}
                {{ "z:\\private\\20022F35\\customsvg\\" ~ Icon }}
            {%- endif -%}
        {%- endif %}
    {%- endfor %}
{%- endmacro %}

{%- macro processURLs(URLs) %}
    {%- for i in range(feat_tree.CaStorageDbSetting[URLs]._value|length) -%}
        {%- set URLTitle = feat_tree.CaStorageDbSetting[URLs].URLTitle._value[i] -%}
        {%- set URLShortTitle = feat_tree.CaStorageDbSetting[URLs].URLShortTitle._value[i] or '' -%}
        {%- set URL = feat_tree.CaStorageDbSetting[URLs].URL._value[i] or '' -%}
        {%- set URLIcon = feat_tree.CaStorageDbSetting[URLs].URLIcon.localPath._value[i] or '' -%}
        {%- set IconSkinId = feat_tree.CaStorageDbSetting[URLs].IconSkinId._value[i] or '' -%}
        {%- if URLIcon != "" -%}
            {%- set URLIcon = stripPath(URLIcon) -%}
        {%- endif %}
        INSERT INTO URL ( URL_TITLE, URL_SHORT_TITLE, URL_DEST, ICON_FILENAME, ICON_SKIN_ID ) 
        VALUES ( '{{URLTitle}}', "{{URLShortTitle}}", "{{URL}}", "{{URLIcon}}", "{{IconSkinId}}" );
    {%- endfor %}
{%- endmacro %}

{% macro processCollections(Collections) -%}
    {%- for col in range(feat_tree.CaStorageDbSetting[Collections]._value|length) -%}
        {%- set Name = feat_tree.CaStorageDbSetting[Collections].Name._value[col] or '' -%}
        {%- set ShortName = feat_tree.CaStorageDbSetting[Collections].ShortName._value[col] or '' -%}
        {%- set TitleName = feat_tree.CaStorageDbSetting[Collections].TitleName._value[col] or '' -%}
        {%- set GroupName = feat_tree.CaStorageDbSetting[Collections].GroupName._value[col] or '' -%}
        {%- set Locked = feat_tree.CaStorageDbSetting[Collections].Locked._value[col] or false -%}
        {%- set LocalizeNames = false -%}
        {%- if Collections == "Collections" -%}
            {%- set LocalizeNames = feat_tree.CaStorageDbSetting[Collections].LocalizeNames._value[col] or false -%}
            {%- set TranslationFile = feat_tree.CaStorageDbSetting[Collections].TranslationFile._value[col] or '' -%}
            {%- if TranslationFile != "" -%}
                {%- set TranslationFile = TranslationFile~'_' -%}
            {%- endif -%}
        {%- endif -%}
        {%- set IconFileName = feat_tree.CaStorageDbSetting[Collections].Icon.localPath._value[col] or '' -%}
        {%- set IconSkinId = feat_tree.CaStorageDbSetting[Collections].IconSkinId._value[col] or '' -%}
        {%- if IconFileName != "" -%}
            {%- set IconFileName = stripPath(IconFileName) -%}
        {%- endif -%}
        {%- if ShortName == "" -%}
            {%- set ShortName = Name -%}
        {%- endif -%}
        {%- if GroupName == "" -%}
            {%- set GroupName = Name -%}
        {%- endif -%}        
    INSERT INTO COLLECTION (COLLECTION_NAME, COL_APP_GROUP_NAME, FLAGS{%- if TitleName -%},  COL_TITLE_NAME{%- endif %} {% if LocalizeNames -%}, TRANSLATION_FILE{%- endif %}, COL_SHORT_NAME, ICON_FILENAME, ICON_SKIN_ID) 
    VALUES ( '{{Name}}', '{{GroupName}}', {% if Locked -%} 0 {% else %} 4 {%- endif %}{%- if TitleName -%}, "{{TitleName}}"{%- endif %}{% if LocalizeNames -%}, "{{TranslationFile}}"{%- endif %}, "{{ShortName}}", "{{IconFileName}}", "{{IconSkinId}}");
    {% endfor %}
{%- endmacro %}

{% macro processItems(Items) -%}
    {%- for index in range(feat_tree.CaStorageDbSetting[Items]._value|length) -%}
        {%- set Type = feat_tree.CaStorageDbSetting[Items].Type._value[index] or '' -%}
        {%- set Application = feat_tree.CaStorageDbSetting[Items].Application._value[index] or '' -%}
        {%- set URLTitle = feat_tree.CaStorageDbSetting[Items].URL._value[index] or '' -%}
        {%- set HomescreenWidget = feat_tree.CaStorageDbSetting[Items].HomescreenWidget._value[index] or '' -%}
        
        {%- if Items == "Items" %}
            {%- set Collection = feat_tree.CaStorageDbSetting[Items].ColName._value[index] or '' -%}
            {%- set URLs = 'URLs' -%}
        {%- else %} {# Items == "OperatorCollectionItems" #}
            {%- set Collection = feat_tree.CaStorageDbSetting.OperatorCollections.Name._value[0] %}
            {%- set URLs = 'OperatorURLs' -%}
        {%- endif %}
        
        {% if Type == "1" -%} INSERT INTO ITEM_TO_COLLECTION (ITEM_TYPE, ITEM_NAME, ITEM_DEST, COLLECTION_NAME) 
          VALUES ("application", '', {{Application|hex2dec}},  '{{Collection}}' );{%- endif %}
          
        {% if Type == "2" -%} INSERT INTO WIDGET_TO_COLLECTION (ITEM_TYPE, ITEM_NAME, URI, COLLECTION_NAME ) 
          VALUES ("widget", '{{HomescreenWidget}}', "{{HomescreenWidget}}", '{{Collection}}' ); {%- endif %}
          
        {% if Type == "3" -%}
          {% for i in range(feat_tree.CaStorageDbSetting[URLs]._value|length) if URLTitle == feat_tree.CaStorageDbSetting[URLs].URLTitle._value[i] -%}
            INSERT INTO ITEM_TO_COLLECTION (ITEM_TYPE, ITEM_NAME, COLLECTION_NAME)
            VALUES ( "url", '{{URLTitle}}', '{{Collection}}' );
          {% endfor %}
        {%- endif %}
        
    {%- endfor %}
{%- endmacro %}

{{ processCollections('OperatorCollections') }}

{{ processURLs('OperatorURLs') }}

{{ processItems('OperatorCollectionItems') }}

{{ processCollections('Collections') }}

{{ processURLs('URLs') }}

{{ processItems('Items') }}

SELECT "castoragedb_variant - END" AS " ";
SELECT "------------------------------------" AS " ";

