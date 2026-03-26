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
    using EState = mgui::ButtonTypes::EState;

    using Callback = std::function<void()>;

    struct Option {
        std::string Id;
        std::string Label;
        T Value;
    };

    ArrowSelector(const std::vector<Option>& options, const sf::Font& font, size_t initialOptionIndex = 0);
    ~ArrowSelector() = default;

    void SetSize(sf::Vector2f size);
    void SetPosition(sf::Vector2f position);
    void SetOrigin(sf::Vector2f origin);
    void SetOutlineThickness(float thickness);

    void SetFillColor(EState state, sf::Color color);
    void SetAccentColor(EState state, sf::Color color);

    void SetActiveOption(const std::string& optionId);

    void SetOnOptionChangedCallback(Callback callback);

    sf::Vector2f GetSize() const { return Size; };

    const Option& GetActiveOption() const { return Options.at(ActiveOptionIndex); }

    T GetActiveValue() const { return GetActiveOption().Value; }
    const T& GetActiveValueRef() const { return GetActiveOption().Value; }

    void Render(sf::RenderTarget& target);

    void OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton);

    void OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton);

    void OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove);

private:
    void NextOption();
    void PrevOption();

    void UpdateOptionLabelString();

    void UpdateOptionLabelOrigin();
    void UpdateOrigins();

    const std::vector<Option> Options;

    Callback OnOptionChangedCallback;

    mgui::Button LeftArrow;
    mgui::Button RightArrow;

    sf::Text OptionLabel;
    sf::RectangleShape OptionBox;

    size_t ActiveOptionIndex;

    sf::Vector2f Size;
    sf::Vector2f Origin;
};

template<typename T>
ArrowSelector<T>::ArrowSelector(const std::vector<Option>& options, const sf::Font& font, size_t initialOptionIndex)
    : Options(options), ActiveOptionIndex(initialOptionIndex)
{
    assert(!Options.empty());
    assert(ActiveOptionIndex < Options.size());

    if (ActiveOptionIndex >= Options.size())
    {
        ActiveOptionIndex = 0;
    }

    LeftArrow.SetFont(font);
    RightArrow.SetFont(font);

    OptionLabel.setFont(font);

    LeftArrow.SetText("<");
    RightArrow.SetText(">");

    LeftArrow.SetOnReleaseCallback([this]{
        PrevOption();
    });

    RightArrow.SetOnReleaseCallback([this]{
        NextOption();
    });

    UpdateOptionLabelString();
}

template<typename T>
void ArrowSelector<T>::SetSize(sf::Vector2f size)
{
    Size = size;

    const float width = size.x;
    const float height = size.y;

    unsigned characterSize = static_cast<unsigned>(height / 2.f);

    LeftArrow.SetCharacterSize(characterSize);
    RightArrow.SetCharacterSize(characterSize);

    OptionLabel.setCharacterSize(characterSize);

    LeftArrow.SetSize(sf::Vector2f(height, height));
    RightArrow.SetSize(sf::Vector2f(height, height));

    OptionBox.setSize(sf::Vector2f(width - 2.f * height, height));

    UpdateOrigins();
}

template<typename T>
void ArrowSelector<T>::SetPosition(sf::Vector2f position)
{
    LeftArrow.SetPosition(position);
    RightArrow.SetPosition(position);

    OptionBox.setPosition(position);
    OptionLabel.setPosition(position);
}

template<typename T>
void ArrowSelector<T>::SetOrigin(sf::Vector2f origin)
{
    Origin = origin;
    UpdateOrigins();
}

template<typename T>
void ArrowSelector<T>::SetOutlineThickness(float thickness)
{
    LeftArrow.SetOutlineThickness(thickness);
    RightArrow.SetOutlineThickness(thickness);

    OptionBox.setOutlineThickness(thickness);
}

template<typename T>
void ArrowSelector<T>::SetFillColor(EState state, sf::Color color)
{
    LeftArrow.SetFillColor(state, color);
    RightArrow.SetFillColor(state, color);

    if (state == EState::Idle)
    {
        OptionBox.setFillColor(color);
    }
}

template<typename T>
void ArrowSelector<T>::SetAccentColor(EState state, sf::Color color)
{
    LeftArrow.SetAccentColor(state, color);
    RightArrow.SetAccentColor(state, color);

    if (state == EState::Idle)
    {
        OptionLabel.setFillColor(color);
        OptionBox.setOutlineColor(color);
    }
}

template<typename T>
void ArrowSelector<T>::SetActiveOption(const std::string& optionId)
{
    for (size_t i = 0; i < Options.size(); ++i)
    {
        if (Options[i].Id == optionId)
        {
            ActiveOptionIndex = i;
            UpdateOptionLabelString();
            break;
        }
    }
}

template<typename T>
void ArrowSelector<T>::SetOnOptionChangedCallback(Callback callback)
{
    OnOptionChangedCallback = std::move(callback);
}

template<typename T>
void ArrowSelector<T>::Render(sf::RenderTarget& target)
{
    target.draw(OptionBox);
    target.draw(OptionLabel);

    LeftArrow.Render(target);
    RightArrow.Render(target);
}

template<typename T>
void ArrowSelector<T>::OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton)
{
    LeftArrow.OnMouseButtonPressed(mouseButton);
    RightArrow.OnMouseButtonPressed(mouseButton);
}

template<typename T>
void ArrowSelector<T>::OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton)
{
    LeftArrow.OnMouseButtonReleased(mouseButton);
    RightArrow.OnMouseButtonReleased(mouseButton);
}

template<typename T>
void ArrowSelector<T>::OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove)
{
    LeftArrow.OnMouseMoved(mouseMove);
    RightArrow.OnMouseMoved(mouseMove);
}

template<typename T>
void ArrowSelector<T>::NextOption()
{
    if (ActiveOptionIndex >= Options.size() - 1)
    {
        return;
    }

    ActiveOptionIndex++;

    if (OnOptionChangedCallback)
    {
        OnOptionChangedCallback();
    }

    UpdateOptionLabelString();
}

template<typename T>
void ArrowSelector<T>::PrevOption()
{
    if (ActiveOptionIndex <= 0)
    {
        return;
    }

    ActiveOptionIndex--;

    if (OnOptionChangedCallback)
    {
        OnOptionChangedCallback();
    }

    UpdateOptionLabelString();
}

template<typename T>
void ArrowSelector<T>::UpdateOptionLabelString()
{
    OptionLabel.setString(GetActiveOption().Label);
    UpdateOptionLabelOrigin();
}

template<typename T>
void ArrowSelector<T>::UpdateOptionLabelOrigin()
{
    const auto tlb = OptionLabel.getLocalBounds();
    OptionLabel.setOrigin(Origin - Size / 2.f + sf::Vector2f(tlb.left + tlb.width / 2.f, tlb.top + tlb.height / 2.f));
}

template<typename T>
void ArrowSelector<T>::UpdateOrigins()
{
    LeftArrow.SetOrigin(Origin);
    RightArrow.SetOrigin(Origin - sf::Vector2f(Size.x - Size.y, 0.f));

    OptionBox.setOrigin(Origin - sf::Vector2f(Size.y, 0.f));
    UpdateOptionLabelOrigin();
}

} // namespace mgui

#endif // MGUI_ARROWSELECTOR_HPP
