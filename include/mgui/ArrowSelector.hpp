#ifndef MGUI_ARROWSELECTOR_HPP
#define MGUI_ARROWSELECTOR_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <vector>
#include <string>
#include <cassert>

#include "mgui/Button.hpp"

namespace mgui {

template<typename T>
class ArrowSelector
{
public:
    struct Option {
        std::string id;
        std::string label;
        T value;
    };

    ArrowSelector(const std::vector<Option>& options, const sf::Font& font, size_t initialOptionIndex = 0);
    ~ArrowSelector() = default;

    void setSize(const sf::Vector2f& size);
    void setPosition(const sf::Vector2f& position);
    void setOrigin(const sf::Vector2f& origin);

    void setOutlineThickness(float thickness);

    void setFillColor(mgui::ButtonState state, const sf::Color& color);
    void setAccentColor(mgui::ButtonState state, const sf::Color& color);

    void setActiveOption(const std::string& optionId);

    sf::Vector2f getSize() const { return size; };

    T getActiveValue() const { return options.at(activeOptionIndex).value; }
    const T& getActiveValueRef() const { return options.at(activeOptionIndex).value; }

    bool hasActiveOptionChanged() const { return activeOptionChanged; }

    void update(const sf::RenderWindow& window);
    void render(sf::RenderTarget& target);

private:
    void nextOption();
    void prevOption();

    void updateOptionLabelString();

    const std::vector<Option>& options;

    mgui::Button leftArrow;
    mgui::Button rightArrow;

    sf::Text optionLabel;
    sf::RectangleShape optionBox;

    size_t activeOptionIndex;

    sf::Vector2f size;
    sf::Vector2f origin;

    bool activeOptionChanged;
};

template<typename T>
ArrowSelector<T>::ArrowSelector(const std::vector<Option>& options, const sf::Font& font, size_t initialOptionIndex)
    : options(options), activeOptionIndex(initialOptionIndex), activeOptionChanged(false)
{
    assert(!this->options.empty());
    assert(this->activeOptionIndex < this->options.size());

    if (this->activeOptionIndex >= this->options.size())
        this->activeOptionIndex = 0;

    this->leftArrow.setFont(font);
    this->rightArrow.setFont(font);
    this->optionLabel.setFont(font);

    this->leftArrow.setText("<");
    this->rightArrow.setText(">");
    this->updateOptionLabelString();
}

template<typename T>
void ArrowSelector<T>::setSize(const sf::Vector2f& size)
{
    this->size = size;

    float width = size.x;
    float height = size.y;

    unsigned characterSize = static_cast<unsigned>(height / 2.f);

    this->leftArrow.setCharacterSize(characterSize);
    this->rightArrow.setCharacterSize(characterSize);
    this->optionLabel.setCharacterSize(characterSize);

    this->leftArrow.setSize(sf::Vector2f(height, height));
    this->rightArrow.setSize(sf::Vector2f(height, height));
    this->optionBox.setSize(sf::Vector2f(width - 2.f * height, height));

    this->leftArrow.setOrigin(origin);
    this->rightArrow.setOrigin(origin + sf::Vector2f(-1 * (width - height), 0.f));
    this->optionBox.setOrigin(origin + sf::Vector2f(-1 * height, 0.f));
    auto tlb = this->optionLabel.getLocalBounds();
    this->optionLabel.setOrigin(origin + sf::Vector2f(-1 * width / 2.f , -1 * height / 2.f) + sf::Vector2f(tlb.left + tlb.width / 2.f, tlb.top + tlb.height / 2.f));
}

template<typename T>
void ArrowSelector<T>::setPosition(const sf::Vector2f& position)
{
    this->leftArrow.setPosition(position);
    this->rightArrow.setPosition(position);
    this->optionBox.setPosition(position);
    this->optionLabel.setPosition(position);
}

template<typename T>
void ArrowSelector<T>::setOrigin(const sf::Vector2f& origin)
{
    this->origin = origin;

    float width = this->size.x;
    float height = this->size.y;

    this->leftArrow.setOrigin(origin);
    this->rightArrow.setOrigin(origin + sf::Vector2f(-1 * (width - height), 0.f));
    this->optionBox.setOrigin(origin + sf::Vector2f(-1 * height, 0.f));
    auto tlb = this->optionLabel.getLocalBounds();
    this->optionLabel.setOrigin(origin + sf::Vector2f(-1 * width / 2.f, -1 * height / 2.f) + sf::Vector2f(tlb.left + tlb.width / 2.f, tlb.top + tlb.height / 2.f));
}

template<typename T>
void ArrowSelector<T>::setOutlineThickness(float thickness)
{
    this->leftArrow.setOutlineThickness(thickness);
    this->rightArrow.setOutlineThickness(thickness);
    this->optionBox.setOutlineThickness(thickness);
}

template<typename T>
void ArrowSelector<T>::setFillColor(mgui::ButtonState state, const sf::Color& color)
{
    this->leftArrow.setFillColor(state, color);
    this->rightArrow.setFillColor(state, color);

    if (state == ButtonState::Idle) {
        this->optionBox.setFillColor(color);
    }
}

template<typename T>
void ArrowSelector<T>::setAccentColor(mgui::ButtonState state, const sf::Color& color)
{
    this->leftArrow.setAccentColor(state, color);
    this->rightArrow.setAccentColor(state, color);

    if (state == ButtonState::Idle) {
        this->optionLabel.setFillColor(color);
        this->optionBox.setOutlineColor(color);
    }
}

template<typename T>
void ArrowSelector<T>::setActiveOption(const std::string& optionId)
{
    for (size_t i = 0; i < this->options.size(); ++i)
    {
        if (this->options[i].id == optionId)
        {
            this->activeOptionIndex = i;
            this->updateOptionLabelString();
        }
    }
}

template<typename T>
void ArrowSelector<T>::update(const sf::RenderWindow& window)
{
    this->activeOptionChanged = false;

    this->leftArrow.update(window);
    this->rightArrow.update(window);

    if (this->leftArrow.isReleased())
        this->prevOption();

    if (this->rightArrow.isReleased())
        this->nextOption();
}

template<typename T>
void ArrowSelector<T>::render(sf::RenderTarget& target)
{
    target.draw(this->optionBox);
    target.draw(this->optionLabel);

    this->leftArrow.render(target);
    this->rightArrow.render(target);
}

template<typename T>
void ArrowSelector<T>::nextOption()
{
    if (this->activeOptionIndex >= this->options.size() - 1)
        return;

    this->activeOptionIndex++;
    this->activeOptionChanged = true;
    this->updateOptionLabelString();
}

template<typename T>
void ArrowSelector<T>::prevOption()
{
    if (this->activeOptionIndex <= 0)
        return;

    this->activeOptionIndex--;
    this->activeOptionChanged = true;
    this->updateOptionLabelString();
}

template<typename T>
void ArrowSelector<T>::updateOptionLabelString()
{
    this->optionLabel.setString(this->options.at(this->activeOptionIndex).label);
    auto tlb = this->optionLabel.getLocalBounds();
    this->optionLabel.setOrigin(origin + sf::Vector2f(-1 * size.x / 2.f, -1 * size.y / 2.f) + sf::Vector2f(tlb.left + tlb.width / 2.f, tlb.top + tlb.height / 2.f));
}

} // namespace mgui

#endif // MGUI_ARROWSELECTOR_HPP
