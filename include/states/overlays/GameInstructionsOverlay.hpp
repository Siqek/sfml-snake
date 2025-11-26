#ifndef GAMEINSTRUCTIONSOVERLAY_HPP
#define GAMEINSTRUCTIONSOVERLAY_HPP

#include "states/overlays/Overlay.hpp"

class GameInstructionsOverlay
    : public Overlay
{
public:
    GameInstructionsOverlay(sf::Vector2f windowSize, const sf::Font& font);
    ~GameInstructionsOverlay() = default;

    void OnWindowResize(const sf::Vector2f& windowSize) override;

    void Render(sf::RenderTarget& target) override;

private:
    void UpdateUIScaling(sf::Vector2f windowSize);

    struct KeyPrompt {
        sf::Vector2f Position;
        sf::Vector2f LabelOrigin;
        std::string Label;

        KeyPrompt(const char* label)
            : Position(0.f, 0.f), LabelOrigin(0.f, 0.f), Label(label) {}
    };

    struct ArrowPrompt {
        float Rotation;
        sf::Vector2f Position;

        ArrowPrompt(float rotation, sf::Vector2f position = sf::Vector2f(0.f, 0.f))
            : Rotation(rotation), Position(position) {}
    };

    struct KeyPromptRenderer {
        sf::RectangleShape Shape;
        sf::Text Label;

        void Render(sf::RenderTarget& target, const KeyPrompt& keyPrompt)
        {
            Shape.setPosition(keyPrompt.Position);
            Label.setPosition(keyPrompt.Position);
            Label.setOrigin(keyPrompt.LabelOrigin);
            Label.setString(keyPrompt.Label);
            target.draw(Shape);
            target.draw(Label);
        }
    };

    struct ArrowPromptRenderer {
        sf::RectangleShape Shape;
        sf::CircleShape Arrow;

        void Render(sf::RenderTarget& target, const ArrowPrompt& arrowPrompt)
        {
            Shape.setPosition(arrowPrompt.Position);
            Arrow.setPosition(arrowPrompt.Position);
            Arrow.setRotation(arrowPrompt.Rotation);
            target.draw(Shape);
            target.draw(Arrow);
        }
    };

    KeyPromptRenderer KeyPromptRendererInstance;
    ArrowPromptRenderer ArrowPromptRendererInstance;

    std::array<KeyPrompt, 4> KeyPrompts;
    std::array<ArrowPrompt, 4> ArrowPrompts;

    sf::Text OrText;
    sf::Text PressAnyKeyText;
};

#endif // GAMEINSTRUCTIONSOVERLAY_HPP
