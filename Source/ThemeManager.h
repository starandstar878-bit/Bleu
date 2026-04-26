/*
  ==============================================================================

    Source/ThemeManager.h
    Created: 23 Apr 2026
    Description: 전역 테마 관리자 클래스. Dark Starlight와 Soft Snow-White 테마의
                 색상 및 스타일 정보를 중앙에서 관리합니다.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class ThemeManager
{
public:
    enum class ThemeType
    {
        DarkStarlight,
        SoftSnowWhite
    };

    struct ThemeColors
    {
        juce::Colour backgroundGradientStart;
        juce::Colour backgroundGradientEnd;
        juce::Colour primaryPoint;
        juce::Colour secondaryPoint;
        juce::Colour textHeader;
        juce::Colour textMain;
        juce::Colour glassBackground;
        juce::Colour glassOutline;
    };

    ThemeManager()
    {
        // Default to bright theme as requested
        setTheme(ThemeType::SoftSnowWhite);
    }

    void setTheme(ThemeType type)
    {
        currentType = type;
        if (type == ThemeType::DarkStarlight)
        {
            currentColors = {
                juce::Colour::fromFloatRGBA(0.02f, 0.02f, 0.05f, 1.0f), // Dark Navy
                juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 1.0f),    // Black
                juce::Colour::fromFloatRGBA(1.0f, 0.9f, 0.3f, 1.0f),    // Starlight Yellow
                juce::Colour::fromFloatRGBA(0.4f, 0.4f, 0.6f, 1.0f),    // Muted Blue
                juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 1.0f),    // White
                juce::Colour::fromFloatRGBA(0.8f, 0.8f, 0.8f, 1.0f),    // Light Grey
                juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 0.05f),   // Glass Background
                juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 0.1f)     // Glass Outline
            };
        }
        else
        {
            // Improved SoftSnowWhite (Brighter & More Premium)
            currentColors = {
                juce::Colour::fromFloatRGBA(0.96f, 0.97f, 1.0f, 1.0f),  // Very Light Blue-ish White
                juce::Colour::fromFloatRGBA(0.92f, 0.93f, 0.96f, 1.0f), // Soft Grey-Blue
                juce::Colour::fromFloatRGBA(0.1f, 0.5f, 1.0f, 1.0f),    // More Vibrant Blue
                juce::Colour::fromFloatRGBA(0.9f, 0.3f, 0.5f, 1.0f),    // More Vibrant Pink/Red
                juce::Colour::fromFloatRGBA(0.05f, 0.08f, 0.12f, 1.0f), // Much Deeper Navy Text (Header)
                juce::Colour::fromFloatRGBA(0.15f, 0.2f, 0.25f, 1.0f),  // Much Darker Grey-Blue Text (Main)
                juce::Colour::fromFloatRGBA(1.0f, 1.0f, 1.0f, 0.5f),    // Glass Background (More opaque)
                juce::Colour::fromFloatRGBA(0.0f, 0.0f, 0.0f, 0.15f)    // Glass Outline (More visible)
            };
        }
    }

    const ThemeColors& getColors() const { return currentColors; }
    ThemeType getCurrentTheme() const { return currentType; }

    static void drawGlassPanel(juce::Graphics& g, juce::Rectangle<float> area, const ThemeColors& colors, float cornerSize = 10.0f)
    {
        g.setColour(colors.glassBackground);
        g.fillRoundedRectangle(area, cornerSize);
        g.setColour(colors.glassOutline);
        g.drawRoundedRectangle(area, cornerSize, 1.5f);
    }

private:
    ThemeType currentType;
    ThemeColors currentColors;
};
