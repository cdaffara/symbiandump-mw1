#ifndef	THIRDPLUGINVIEWFORTEST_H 
#define	THIRDPLUGINVIEWFORTEST_H

#include <cpbasesettingview.h>
#include <QGraphicsItem>

class HbDataFormModelItem;
class CpSettingFormItemData;

class ThirdPluginViewForTest : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit ThirdPluginViewForTest(QGraphicsItem *parent = 0);
    virtual ~ThirdPluginViewForTest();
public:
    void testClose();
private slots:   
    void sliderValueChanged(int value);
    
private:
    HbDataFormModelItem   *mGroupItem;
    CpSettingFormItemData *mSliderItem;
};
#endif
