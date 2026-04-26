/*
  ==============================================================================

    Source/CustomLookAndFeel.h
    Created: 23 Apr 2026
    Description: 글래스모피즘 스타일의 커스텀 LookAndFeel 클래스. 
                 로터리 슬라이더 및 UI 요소의 디자인을 관리합니다.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel(ThemeManager& tm) : themeManager(tm) 
    {
        auto colors = themeManager.getColors();
        setColour(juce::Slider::textBoxTextColourId, colors.textMain);
        setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
        setColour(juce::Slider::textBoxBackgroundColourId, colors.glassBackground.withAlpha(0.3f));
        
        setColour(juce::Label::textColourId, colors.textMain);
        
        setColour(juce::TextButton::buttonColourId, colors.glassBackground.withAlpha(0.6f));
        setColour(juce::TextButton::buttonOnColourId, colors.primaryPoint.withAlpha(0.8f));
        setColour(juce::TextButton::textColourOffId, colors.textMain);
        setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    }

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                          float sliderPos, const float rotaryStartAngle,
                          const float rotaryEndAngle, juce::Slider& slider) override
    {
        auto colors = themeManager.getColors();
        auto outline = colors.glassOutline;
        auto fill = colors.primaryPoint;

        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = 4.0f;
        auto arcRadius = radius - lineW * 2.0f;

        // 1. Background Track (Subtle glass outline)
        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
        g.setColour(colors.glassOutline.withAlpha(0.1f));
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        // 2. Value Arc (Current Theme Point Color) with Glow
        if (sliderPos > 0.0f)
        {
            juce::Path valueArc;
            valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f, rotaryStartAngle, toAngle, true);
            
            // Outer Glow
            g.setColour(fill.withAlpha(0.2f * sliderPos));
            g.strokePath(valueArc, juce::PathStrokeType(lineW * 2.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
            
            // Main Line
            g.setColour(fill);
            g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        }

        // 3. Center Dial (Glassy Look) with shadow
        auto dialRadius = arcRadius - 8.0f;
        auto dialArea = juce::Rectangle<float>(width, height).withSize(dialRadius * 2, dialRadius * 2).withCentre(bounds.getCentre());
        
        // Shadow for depth
        g.setColour(juce::Colours::black.withAlpha(0.15f));
        g.fillEllipse(dialArea.translated(0, 2));

        juce::ColourGradient dialGrad(colors.glassBackground.withAlpha(0.4f), dialArea.getCentreX(), dialArea.getCentreY(),
                                      colors.glassBackground.withAlpha(0.1f), dialArea.getCentreX(), dialArea.getBottom(), true);
        g.setGradientFill(dialGrad);
        g.fillEllipse(dialArea);
        
        g.setColour(colors.glassOutline.withAlpha(0.4f));
        g.drawEllipse(dialArea, 1.2f);

        // 4. Indicator Dot (Clean and sharp)
        float dotRadius = dialRadius - 6.0f;
        juce::Point<float> dotPos(bounds.getCentreX() + dotRadius * std::cos(toAngle - juce::MathConstants<float>::halfPi),
                                  bounds.getCentreY() + dotRadius * std::sin(toAngle - juce::MathConstants<float>::halfPi));

        // Glow behind dot
        g.setColour(fill.withAlpha(0.6f * sliderPos));
        g.fillEllipse(juce::Rectangle<float>(10, 10).withCentre(dotPos));
        
        g.setColour(colors.textMain);
        g.fillEllipse(juce::Rectangle<float>(6, 6).withCentre(dotPos));
    }

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override
    {
        auto colors = themeManager.getColors();
        auto area = button.getLocalBounds().toFloat();
        
        float alpha = isButtonDown ? 0.9f : (isMouseOverButton ? 0.7f : 0.4f);
        
        if (button.getToggleState())
            g.setColour(colors.primaryPoint.withAlpha(alpha));
        else
            g.setColour(colors.glassBackground.withAlpha(alpha));
            
        g.fillRoundedRectangle(area, 4.0f);
        
        g.setColour(colors.glassOutline.withAlpha(0.2f));
        g.drawRoundedRectangle(area, 4.0f, 1.0f);
    }

    void drawButtonText (juce::Graphics& g, juce::TextButton& button, bool isMouseOverButton, bool isButtonDown) override
    {
        auto colors = themeManager.getColors();
        g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
        
        if (button.getToggleState())
            g.setColour(juce::Colours::white);
        else
            g.setColour(colors.textMain);
            
        g.drawText(button.getButtonText(), button.getLocalBounds(), juce::Justification::centred);
    }

private:
    ThemeManager& themeManager;
};
