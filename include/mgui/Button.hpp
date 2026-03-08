#ifndef MGUI_BUTTON_HPP
#define MGUI_BUTTON_HPP

namespace mgui
{
    struct ButtonTypes
    {
        enum class EState
        {
            Idle,
            Hovered,
            Pressed
        };
    };

    class Button {
    public:
        using EState = ButtonTypes::EState;

        using Callback = std::function<void()>;

        Button();
        ~Button() = default;

        void SetSize(sf::Vector2f size);
        void SetPosition(sf::Vector2f position);
        void SetOrigin(sf::Vector2f origin);
        void SetOutlineThickness(float thickness);

        void SetText(const std::string& text);
        void SetFont(const sf::Font& font);
        void SetCharacterSize(unsigned size);

        void SetFillColor(EState state, sf::Color color);

        ////////////////////////////////////////////////////////////
        /// \brief Assigns a common color to both the
        ///        button's text and its outline depending on its state.
        ///
        /// \param state The button state for which the accent
        ///              color should be applied.
        /// \param color The color to be used for the text and
        ///              outline in the given state.
        ///
        ////////////////////////////////////////////////////////////
        void SetAccentColor(EState state, sf::Color color);

        void SetOnReleaseCallback(Callback callback);

        sf::Vector2f GetSize() const;
        sf::FloatRect GetTextLocalBounds() const;

        bool IsPressed() const;

        void Render(sf::RenderTarget& target);

        void OnMouseButtonPressed(const sf::Event::MouseButtonEvent& mouseButton);

        void OnMouseButtonReleased(const sf::Event::MouseButtonEvent& mouseButton);

        void OnMouseMoved(const sf::Event::MouseMoveEvent& mouseMove);

    private:
        void SetState(EState newState);

        inline bool IsMouseOnButton(sf::Vector2f mousePosition) const;

        void UpdateTextOrigin();

        void UpdateColor();

        struct ColorScheme
        {
            sf::Color FillColor;
            sf::Color AccentColor;

            ColorScheme()
                : FillColor(0x000000FF), AccentColor(0xFFFFFFFF) {}
        };

        void ApplyColorScheme(const ColorScheme& colorScheme);

        sf::RectangleShape Shape;
        sf::Text Label;

        ColorScheme IdleColor;
        ColorScheme HoveredColor;
        ColorScheme PressedColor;

        EState CurrentState;

        Callback OnReleaseCallback;
    };
}

#endif // MGUI_BUTTON_HPP
