

#include "thirdpluginfortest.h"
#include "thirdpluginviewfortest.h"
#include <cpsettingformentryitemdataimpl.h>

ThirdPluginForTest::ThirdPluginForTest()
{
}

ThirdPluginForTest::~ThirdPluginForTest()
{
}
QList<CpSettingFormItemData*> ThirdPluginForTest::createSettingFormItemData(CpItemDataHelper &itemDataHelper) const
{
    return QList<CpSettingFormItemData*>() 
            << new CpSettingFormEntryItemDataImpl<ThirdPluginViewForTest>(
               itemDataHelper,
               tr("The plugin for test"), 
               tr("TestPlugin."));
}

CpBaseSettingView *ThirdPluginForTest::createSettingView(const QVariant &hint) const
{
    return 0;
}

Q_EXPORT_PLUGIN2(ThirdPluginForTest, ThirdPluginForTest);
