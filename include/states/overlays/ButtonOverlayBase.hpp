#ifndef BUTTONOVERLAYBASE_HPP
#define BUTTONOVERLAYBASE_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <string>
#include <array>

#include "states/overlays/Overlay.hpp"

#include "mgui/Button.hpp"

#include "config/Colors.hpp"

template<size_t N>
class ButtonOverlayBase
    : public Overlay
{
public:
    struct ButtonInit {
        std::string id;
        std::string text;

        ButtonInit(const char* id, const char* text)
            : id(id), text(text) {}
    };

    ButtonOverlayBase(const sf::Vector2f& windowSize, const sf::Font& font, const std::array<ButtonInit, N>& buttonInits);
    virtual ~ButtonOverlayBase() = default;

    bool isButtonReleased(const std::string& id) const;

    virtual void update(const sf::RenderWindow& window) override;
    virtual void render(sf::RenderTarget& target) override;

protected:
    virtual void updateUIScaling(sf::Vector2f newWindowSize) override;

private:
    struct OverlayButton {
        std::string id;
        mgui::Button button;
    };

    std::array<OverlayButton, N> buttons;
};

template<size_t N>
ButtonOverlayBase<N>::ButtonOverlayBase(const sf::Vector2f& windowSize, const sf::Font& font, const std::array<ButtonInit, N>& buttonInits)
    : Overlay(windowSize, sf::Color(Colors::Hex::OverlayBackground))
{
    for (size_t i = 0; i < N; ++i)
    {
        this->buttons[i].id = buttonInits[i].id;

        this->buttons[i].button.setFont(font);
        this->buttons[i].button.setText(buttonInits[i].text);

        this->buttons[i].button.setFillColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleBg));
        this->buttons[i].button.setFillColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverBg));
        this->buttons[i].button.setFillColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveBg));

        this->buttons[i].button.setAccentColor(mgui::ButtonState::Idle,   sf::Color(Colors::Hex::ButtonIdleOutline));
        this->buttons[i].button.setAccentColor(mgui::ButtonState::Hover,  sf::Color(Colors::Hex::ButtonHoverOutline));
        this->buttons[i].button.setAccentColor(mgui::ButtonState::Active, sf::Color(Colors::Hex::ButtonActiveOutline));
    }
    this->updateUIScaling(windowSize);
}

template<size_t N>
bool ButtonOverlayBase<N>::isButtonReleased(const std::string& id) const
{
    for (const auto& button : this->buttons)
    {
        if (button.id == id)
            return button.button.isReleased();
    }

    return false;
}

template<size_t N>
void ButtonOverlayBase<N>::update(const sf::RenderWindow& window)
{
    for (auto& button : this->buttons)
        button.button.update(window);
}

template<size_t N>
void ButtonOverlayBase<N>::render(sf::RenderTarget& target)
{
    if (!this->getIsActive())
        return;

    this->renderWindowOverlay(target);

    for (auto& button : this->buttons)
        button.button.render(target);
}

template<size_t N>
void ButtonOverlayBase<N>::updateUIScaling(sf::Vector2f newWindowSize)
{
    const auto size = sf::Vector2f(newWindowSize.x / 3.f, newWindowSize.y / 16.f);
    const auto position = newWindowSize / 2.f;
    const auto positionOffset = sf::Vector2f(0.f, size.y * 1.4f);
    const auto outlineThickness = size.y / 16.f;
    const auto characterSize = static_cast<unsigned>(std::min(size.y / 2.f, size.x / 12.f));

    for (size_t i = 0; i < this->buttons.size(); ++i)
    {
        mgui::Button& button = this->buttons[i].button;

        button.setSize(size);
        button.setPosition(position + static_cast<float>(i) * positionOffset);
        button.setOrigin(size / 2.f);
        button.setOutlineThickness(outlineThickness);
        button.setCharacterSize(characterSize);
    }
}

#endif // BUTTONOVERLAYBASE_HPP
