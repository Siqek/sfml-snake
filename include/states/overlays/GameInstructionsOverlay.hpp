#ifndef GAMEINSTRUCTIONSOVERLAY_HPP
#define GAMEINSTRUCTIONSOVERLAY_HPP

#include "states/overlays/Overlay.hpp"

#include "render/RenderContext.hpp"

class GameInstructionsOverlay final
    : public Overlay
{
public:
    GameInstructionsOverlay(sf::Vector2f windowSize, const sf::Font& font);
    ~GameInstructionsOverlay() = default;

    void Render(sf::RenderTarget& target) override;

    void FillContext(RenderContext& context) override;

    void OnWindowResize(const sf::Event::SizeEvent& size) override;

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

        void Prepare(const KeyPrompt& keyPrompt)
        {
            Shape.setPosition(keyPrompt.Position);
            Label.setPosition(keyPrompt.Position);
            Label.setOrigin(keyPrompt.LabelOrigin);
            Label.setString(keyPrompt.Label);
        }

        void Render(sf::RenderTarget& target, const KeyPrompt& keyPrompt)
        {
            Prepare(keyPrompt);

            target.draw(Shape);
            target.draw(Label);
        }

        void FillContext(RenderContext& context, const KeyPrompt& keyPrompt)
        {
            Prepare(keyPrompt);

            context.Drawables.emplace_back(Shape);
            context.Drawables.emplace_back(Label);
        }
    };

    struct ArrowPromptRenderer {
        sf::RectangleShape Shape;
        sf::CircleShape Arrow;

        void Prepare(const ArrowPrompt& arrowPrompt)
        {
            Shape.setPosition(arrowPrompt.Position);
            Arrow.setPosition(arrowPrompt.Position);
            Arrow.setRotation(arrowPrompt.Rotation);
        }

        void Render(sf::RenderTarget& target, const ArrowPrompt& arrowPrompt)
        {
            Prepare(arrowPrompt);

            target.draw(Shape);
            target.draw(Arrow);
        }

        void FillContext(RenderContext& context, const ArrowPrompt& arrowPrompt)
        {
            Prepare(arrowPrompt);

            context.Drawables.emplace_back(Shape);
            context.Drawables.emplace_back(Arrow);
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
