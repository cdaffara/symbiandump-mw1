#ifndef	SECONDPLUGINVIEWFORTEST_H 
#define	SECONDPLUGINVIEWFORTEST_H

#include <cpbasesettingview.h>
#include <QGraphicsItem>

class HbDataFormModelItem;
class CpSettingFormItemData;

class SecondPluginViewForTest : public CpBaseSettingView
{
    Q_OBJECT
public:
    explicit SecondPluginViewForTest(QGraphicsItem *parent = 0);
    virtual ~SecondPluginViewForTest();
public:
    void testClose();
private slots:   
    void sliderValueChanged(int value);
    
private:
    HbDataFormModelItem   *mGroupItem;
    CpSettingFormItemData *mSliderItem;
};
#endif
