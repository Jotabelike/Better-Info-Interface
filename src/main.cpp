#include <Geode/Geode.hpp>
#include <Geode/modify/FLAlertLayer.hpp>

using namespace geode::prelude;

class $modify(CleanAlerts, FLAlertLayer) {
    void show() {
        FLAlertLayer::show();
        if (typeid(*this) != typeid(FLAlertLayer)) return;

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // 1. Opacidad reducida a 100 (antes 170)
        auto darkBG = CCLayerColor::create({ 0, 0, 0, 100 });
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
                // Ocultar el fondo de la alerta
                if (auto bgBox = typeinfo_cast<CCScale9Sprite*>(child)) {
                    bgBox->setVisible(false);
                }

                // Encontrar el título
                if (auto label = typeinfo_cast<CCLabelBMFont*>(child)) {
                    if (label->getPositionY() > highestY) {
                        highestY = label->getPositionY();
                        titleNode = label;
                    }
                }
            }

            // 2. Mantener la línea separadora
            if (titleNode) {
                // No movemos el título (o muy poco si es necesario), solo agregamos la línea
                // float newTitleY = titleNode->getPositionY() + 22.f; // COMENTADO: No mover título drásticamente

                auto separator = CCSprite::createWithSpriteFrameName("edit_vLine_001.png");

                if (separator) {
                    separator->setRotation(90.0f);
                    separator->setScaleY(2.5f);
                    separator->setScaleX(0.6f);
                    separator->setOpacity(150);

                    // Posicionamos la línea justo debajo del título original
                    separator->setPosition({ titleNode->getPositionX(), titleNode->getPositionY() - 15.f });

                    this->m_mainLayer->addChild(separator);
                }
            }
        }

        if (this->m_buttonMenu) {
            // 3. Quitada la separación extra de los botones
            // this->m_buttonMenu->setPositionY(this->m_buttonMenu->getPositionY() - 20.f); // ELIMINADO

            CCArrayExt<CCNode*> buttons = this->m_buttonMenu->getChildren();
            for (auto node : buttons) {
                if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(node)) {
                    // Limpieza visual de los botones (sin fondo)
                    auto cleanButton = [](CCNode* img) {
                        if (auto btnSprite = typeinfo_cast<ButtonSprite*>(img)) {
                            CCArrayExt<CCNode*> kids = btnSprite->getChildren();
                            for (auto k : kids) {
                                if (!typeinfo_cast<CCLabelBMFont*>(k)) {
                                    k->setVisible(false); // Ocultar fondo del botón
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