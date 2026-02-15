#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

using namespace geode::prelude;

class $modify(CleanAlerts, FLAlertLayer) {
    void show() {
        FLAlertLayer::show();
        if (typeid(*this) != typeid(FLAlertLayer)) return;
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto darkBG = CCLayerColor::create({ 0, 0, 0, 170 });
        darkBG->setContentSize(winSize);
        darkBG->setZOrder(-1);
        darkBG->ignoreAnchorPointForPosition(false);
        darkBG->setPosition(winSize / 2);
        this->addChild(darkBG);
        CCLabelBMFont* titleNode = nullptr; 
        if (this->m_mainLayer) {

            CCArrayExt<CCNode*> children = this->m_mainLayer->getChildren();
            float highestY = -9999.f;
            for (auto child : children) {
            
                if (auto bgBox = typeinfo_cast<CCScale9Sprite*>(child)) {
                    bgBox->setVisible(false);
                }
 
                if (auto label = typeinfo_cast<CCLabelBMFont*>(child)) {
                    if (label->getPositionY() > highestY) {
                        highestY = label->getPositionY();
                        titleNode = label;
                    }
                }
            }

           
            if (titleNode) {       
                float newTitleY = titleNode->getPositionY() + 22.f;
                titleNode->setPositionY(newTitleY);

                
                auto separator = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");

                if (separator) {
                    separator->setRotation(90.0f);
                    separator->setScaleY(2.5f);
                    separator->setScaleX(0.6f);
                    separator->setOpacity(150);

                    separator->setPosition({ titleNode->getPositionX(), newTitleY - 25.f });

                    this->m_mainLayer->addChild(separator);
                }
            }
        }

       
        if (this->m_buttonMenu) {
            this->m_buttonMenu->setPositionY(this->m_buttonMenu->getPositionY() - 20.f);

            CCArrayExt<CCNode*> buttons = this->m_buttonMenu->getChildren();
            for (auto node : buttons) {
                if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {

                    auto cleanButton = [](CCNode* img) {
                        if (auto btnSprite = typeinfo_cast<ButtonSprite*>(img)) {
                            CCArrayExt<CCNode*> kids = btnSprite->getChildren();
                            for (auto k : kids) {
                                if (!typeinfo_cast<CCLabelBMFont*>(k)) {
                                    k->setVisible(false);
                                }
                            }
                        }
                        };

                    if (auto n = btn->getNormalImage()) cleanButton(n);
                    if (auto s = btn->getSelectedImage()) cleanButton(s);
                }
            }
        }
    }
};