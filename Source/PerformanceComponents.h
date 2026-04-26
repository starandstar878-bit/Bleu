/*
  ==============================================================================

    Source/PerformanceComponents.h
    Created: 24 Apr 2026
    Description: 퍼포먼스 패널에 사용될 커스텀 UI 컴포넌트들 (Stomp, Preset, etc.)

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

// ------------------------------------------------------------------------------
// 1. 둥근 금속 스톰프 스위치
// ------------------------------------------------------------------------------
class StompButton : public juce::Button
{
public:
    StompButton(const juce::String& name, ThemeManager& tm) 
        : juce::Button(name), themeManager(tm) {}

    void paintButton(juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();
        
        // 1. 상단 이펙터 이름
        g.setColour(colors.textMain);
        g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold").withName("Outfit")));
        g.drawText(getName(), area.removeFromTop(15), juce::Justification::centred);
        
        area.removeFromTop(5);
        auto circleSize = std::min(area.getWidth(), area.getHeight()) * 0.7f;
        auto circleArea = juce::Rectangle<float>(circleSize, circleSize).withCentre(area.getCentre());

        // 2. 스위치 베이스 (금속 느낌)
        juce::ColourGradient grad(colors.primaryPoint.withAlpha(0.2f), circleArea.getCentreX(), circleArea.getY(),
                                  colors.glassBackground, circleArea.getCentreX(), circleArea.getBottom(), false);
        g.setGradientFill(grad);
        g.fillEllipse(circleArea);
        g.setColour(colors.glassOutline);
        g.drawEllipse(circleArea, 1.5f);

        // 3. 실제 스위치 노브 (둥근 모양)
        auto knobArea = circleArea.reduced(circleArea.getWidth() * 0.2f);
        if (getToggleState())
        {
            // On 상태: 조명 효과
            g.setColour(colors.primaryPoint);
            g.fillEllipse(knobArea);
            g.setColour(juce::Colours::white.withAlpha(0.4f));
            g.drawEllipse(knobArea.reduced(2), 2.0f);
        }
        else
        {
            // Off 상태: 금속 질감
            g.setColour(colors.textHeader.withAlpha(0.5f));
            g.fillEllipse(knobArea);
        }
        
        if (isButtonDown) knobArea = knobArea.reduced(2);
    }

private:
    ThemeManager& themeManager;
};

// ------------------------------------------------------------------------------
// 2. 직사각형 프리셋/매크로 버튼
// ------------------------------------------------------------------------------
class PresetButton : public juce::TextButton
{
public:
    PresetButton(const juce::String& name, ThemeManager& tm) 
        : juce::TextButton(name), themeManager(tm) 
    {
        setClickingTogglesState(true);
        setRadioGroupId(100); // 전역 프리셋 그룹
    }

    void paintButton(juce::Graphics& g, bool isMouseOver, bool isButtonDown) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat().reduced(2);
        
        bool active = getToggleState();
        
        // 배경 드로잉 (글래스모피즘)
        g.setColour(active ? colors.primaryPoint.withAlpha(0.4f) : colors.glassBackground);
        g.fillRoundedRectangle(area, 4.0f);
        
        g.setColour(active ? colors.primaryPoint : colors.glassOutline);
        g.drawRoundedRectangle(area, 4.0f, 1.5f);

        // 텍스트
        g.setColour(active ? colors.textHeader : colors.textMain.withAlpha(0.8f));
        g.setFont(juce::Font(juce::FontOptions(13.0f).withStyle(active ? "Bold" : "Medium")));
        g.drawText(getButtonText(), area, juce::Justification::centred);
    }

private:
    ThemeManager& themeManager;
};
