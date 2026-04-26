#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"
#include "SpectrumVisualizer.h"
#include "InteractiveEQOverlay.h"
#include "CustomLookAndFeel.h"

//==============================================================================
/** 이미지 스타일의 수직 레벨 메터 컴포넌트 */
class LevelMeter : public juce::Component
{
public:
    LevelMeter(ThemeManager& tm, bool isLeft) : themeManager(tm), isL(isLeft) {}

    void mouseDown(const juce::MouseEvent& e) override {
        peakLevel = 0.0f;
        repaint();
    }


    void paint(juce::Graphics& g) override {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat().reduced(2);
        
        // Background track
        g.setColour(colors.glassBackground.withAlpha(0.2f));
        g.fillRoundedRectangle(area, 3.0f);
        
        // Value fill with gradient (Logarithmic scale)
        float dB = juce::Decibels::gainToDecibels(level);
        float normalized = juce::jmap(dB, -60.0f, 6.0f, 0.0f, 1.0f);
        
        if (normalized > 0.01f) {
            auto fillH = area.getHeight() * juce::jlimit(0.0f, 1.0f, normalized);
            auto fillArea = area.withTrimmedTop(area.getHeight() - fillH);
            
            juce::ColourGradient grad(juce::Colours::red, area.getX(), area.getY(),
                                      juce::Colours::green, area.getX(), area.getBottom(), false);
            grad.addColour(0.5, juce::Colours::yellow);
            
            g.setGradientFill(grad);
            g.fillRoundedRectangle(fillArea, 3.0f);
        }

        // Channel Label
        g.setColour(colors.textMain.withAlpha(0.6f));
        g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold")));
        g.drawText(isL ? "L" : "R", getLocalBounds().removeFromBottom(15), juce::Justification::centred);
        
        // Peak Hold Line
        if (peakLevel > 0.01f) {
            float peakDB = juce::Decibels::gainToDecibels(peakLevel);
            float peakNorm = juce::jmap(peakDB, -60.0f, 6.0f, 0.0f, 1.0f);
            float peakY = area.getBottom() - area.getHeight() * juce::jlimit(0.0f, 1.0f, peakNorm);
            g.setColour(juce::Colours::white.withAlpha(0.8f));
            g.drawHorizontalLine((int)peakY, area.getX(), area.getRight());
            
            peakLevel *= 0.99f; // Slow decay
        }
    }

    void setLevel(float l) { 
        level = l; 
        if (l > peakLevel) peakLevel = l;
        repaint(); 
    }

private:
    ThemeManager& themeManager;
    bool isL;
    float level = 0.0f;
    float peakLevel = 0.0f;
};

//==============================================================================
/** 알약 형태의 오버샘플링 선택기 */
class OversamplingToggleGroup : public juce::Component
{
public:
    OversamplingToggleGroup(ThemeManager& tm) : themeManager(tm) {
        juce::StringArray labels = {"OFF", "2X", "4X", "8X"};
        for (int i = 0; i < labels.size(); ++i) {
            auto* b = buttons.add(new juce::TextButton(labels[i]));
            addAndMakeVisible(b);
            b->setRadioGroupId(123);
            b->setClickingTogglesState(true);
            if (i == 0) b->setToggleState(true, juce::dontSendNotification); // Default OFF
            
            b->setTooltip("Set Global Oversampling factor (Higher = Better quality, Higher CPU)");
            b->onClick = [this, i, labels] {
                if (onFactorChanged) {
                    int factor = 1;
                    if (labels[i] == "2X") factor = 2;
                    else if (labels[i] == "4X") factor = 4;
                    else if (labels[i] == "8X") factor = 8;
                    onFactorChanged(factor);
                }
            };
        }
    }

    std::function<void(int)> onFactorChanged;

    void paint(juce::Graphics& g) override {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();
        ThemeManager::drawGlassPanel(g, area, colors, area.getHeight() / 2.0f);
    }

    void resized() override {
        auto area = getLocalBounds().reduced(4);
        auto w = area.getWidth() / buttons.size();
        for (auto* b : buttons) b->setBounds(area.removeFromLeft(w).reduced(2));
    }

private:
    ThemeManager& themeManager;
    juce::OwnedArray<juce::TextButton> buttons;
};

//==============================================================================
class OutputPanel : public juce::Component
{
public:
    OutputPanel(BleuAudioProcessor& p, ThemeManager& tm) 
        : themeManager(tm), visualizer(p, tm), eqOverlay(p, tm), customLF(tm),
          meterL(tm, true), meterR(tm, false), inMeterL(tm, true), inMeterR(tm, false),
          oversampling(tm)
    {
        addAndMakeVisible(visualizer);
        addAndMakeVisible(eqOverlay);
        addAndMakeVisible(meterL);
        addAndMakeVisible(meterR);
        addAndMakeVisible(inMeterL);
        addAndMakeVisible(inMeterR);
        addAndMakeVisible(oversampling);
        
        oversampling.onFactorChanged = [&p](int factor) {
            p.setGlobalOversamplingFactor(factor);
        };

        setLookAndFeel(&customLF);

        masterVolume.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        masterVolume.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        masterVolume.setRange(-60.0, 12.0, 0.1);
        masterVolume.setValue(0.0);
        masterVolume.onValueChange = [this, &p] { p.setMasterGain((float)masterVolume.getValue()); };
        masterVolume.setTooltip("Final output volume control");
        addAndMakeVisible(masterVolume);

        masterLabel.setText("MASTER VOLUME", juce::dontSendNotification);
        masterLabel.setJustificationType(juce::Justification::centred);
        masterLabel.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
        addAndMakeVisible(masterLabel);

        oversamplingLabel.setText("OVERSAMPLING", juce::dontSendNotification);
        oversamplingLabel.setJustificationType(juce::Justification::centred);
        oversamplingLabel.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
        addAndMakeVisible(oversamplingLabel);

        hdButton.setButtonText("HD MODE: ON");
        hdButton.setClickingTogglesState(true);
        hdButton.setToggleState(p.isHDModeEnabled(), juce::dontSendNotification);
        hdButton.onClick = [this, &p] {
            bool on = hdButton.getToggleState();
            p.setHDMode(on);
            hdButton.setButtonText(on ? "HD MODE: ON" : "HD MODE: OFF");
        };
        hdButton.setTooltip("High-Definition Mode: Adds analog warmth and air");
        addAndMakeVisible(hdButton);

        abButton.setButtonText("A / B");
        abButton.setTooltip("Swap between two independent settings states");
        abButton.onClick = [this, &p] { 
            p.toggleABComparison(); 
            abButton.setButtonText(p.isComparisonStateB() ? "STATE: B" : "STATE: A");
        };
        addAndMakeVisible(abButton);

        phaseButton.setButtonText("Ø");
        phaseButton.setTooltip("Invert Output Phase");
        phaseButton.setClickingTogglesState(true);
        phaseButton.onClick = [this, &p] { p.setPhaseInvert(phaseButton.getToggleState()); };
        addAndMakeVisible(phaseButton);

        widthSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        widthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        widthSlider.setRange(0.0, 2.0, 0.01);
        widthSlider.setValue(1.0);
        widthSlider.setTooltip("Stereo Width: 0=Mono, 1=Normal, 2=Extra Wide");
        widthSlider.onValueChange = [this, &p] { p.setStereoWidth((float)widthSlider.getValue()); };
        addAndMakeVisible(widthSlider);

        widthLabel.setText("WIDTH", juce::dontSendNotification);
        widthLabel.setJustificationType(juce::Justification::centred);
        widthLabel.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold")));
        addAndMakeVisible(widthLabel);

        tiltSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        tiltSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
        tiltSlider.setRange(-1.0, 1.0, 0.01);
        tiltSlider.setValue(0.0);
        tiltSlider.setTooltip("Spectral Tilt: Tilt the frequency balance (Darker <-> Brighter)");
        tiltSlider.onValueChange = [this, &p] { p.setSpectralTilt((float)tiltSlider.getValue()); };
        addAndMakeVisible(tiltSlider);

        tiltLabel.setText("TILT", juce::dontSendNotification);
        tiltLabel.setJustificationType(juce::Justification::centred);
        tiltLabel.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold")));
        addAndMakeVisible(tiltLabel);

        cpuLabel.setJustificationType(juce::Justification::left);
        cpuLabel.setFont(juce::Font(juce::FontOptions(10.0f)));
        addAndMakeVisible(cpuLabel);

        latencyLabel.setJustificationType(juce::Justification::right);
        latencyLabel.setFont(juce::Font(juce::FontOptions(10.0f)));
        addAndMakeVisible(latencyLabel);
    }

    ~OutputPanel() override { setLookAndFeel(nullptr); }

    void paint(juce::Graphics& g) override {
        auto colors = themeManager.getColors();
        
        // Update label colors in case theme changed
        masterLabel.setColour(juce::Label::textColourId, colors.textMain.withAlpha(0.8f));
        oversamplingLabel.setColour(juce::Label::textColourId, colors.textMain.withAlpha(0.8f));

        // Section labels
        g.setColour(colors.textMain.withAlpha(0.8f));
        g.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
        g.drawText("SPECTRUM ANALYZER & MASTER EQ", 25, 10, 300, 30, juce::Justification::left);
    }

    void setLevels(float inL, float inR, float outL, float outR, double latencyMs, float cpu) {
        inMeterL.setLevel(inL);
        inMeterR.setLevel(inR);
        meterL.setLevel(outL);
        meterR.setLevel(outR);
        latencyLabel.setText("Latency: " + juce::String(latencyMs, 1) + " ms", juce::dontSendNotification);
        cpuLabel.setText("CPU: " + juce::String(cpu * 100.0f, 1) + "%", juce::dontSendNotification);
    }

    void resized() override {
        auto area = getLocalBounds().reduced(20);
        
        // 2:1 Ratio Split
        auto topHeight = (area.getHeight() * 2) / 3;
        auto topArea = area.removeFromTop(topHeight);
        
        visualizer.setBounds(topArea);
        eqOverlay.setBounds(topArea);
        
        auto bottomArea = area.removeFromTop(area.getHeight()).reduced(0, 10);
        auto centerW = 120;
        
        // Symmetrical Layout: [IN Meter] [L Meter] [Volume] [Oversampling] [R Meter] [IN Meter]
        inMeterL.setBounds(5, bottomArea.getY() + 10, 10, bottomArea.getHeight() - 20);
        meterL.setBounds(20, bottomArea.getY() + 10, 20, bottomArea.getHeight() - 20);
        
        inMeterR.setBounds(getWidth() - 15, bottomArea.getY() + 10, 10, bottomArea.getHeight() - 20);
        meterR.setBounds(getWidth() - 40, bottomArea.getY() + 10, 20, bottomArea.getHeight() - 20);
        
        auto controlsArea = bottomArea.reduced(50, 0);
        auto volArea = controlsArea.removeFromLeft(centerW);
        masterVolume.setBounds(volArea.withTrimmedBottom(30));
        masterLabel.setBounds(volArea.removeFromBottom(25).translated(0, -5));

        controlsArea.removeFromLeft(20);
        auto overArea = controlsArea.removeFromLeft(200);
        oversampling.setBounds(overArea.withSize(200, 40).withCentre(overArea.getCentre().translated(0, -10)));
        oversamplingLabel.setBounds(overArea.withY(overArea.getCentreY() + 15).withHeight(25));
        
        hdButton.setBounds(controlsArea.removeFromRight(120).withSize(100, 40).withCentre(overArea.getCentre().translated(180, -10)));
        
        auto sideControls = controlsArea.removeFromRight(80);
        abButton.setBounds(sideControls.removeFromTop(30).reduced(2));
        phaseButton.setBounds(sideControls.removeFromTop(30).reduced(2));

        auto widthArea = sideControls.removeFromTop(60);
        widthSlider.setBounds(widthArea.removeFromTop(40));
        widthLabel.setBounds(widthArea);

        auto tiltArea = sideControls.removeFromTop(60);
        tiltSlider.setBounds(tiltArea.removeFromTop(40));
        tiltLabel.setBounds(tiltArea);

        latencyLabel.setBounds(getWidth() - 110, 5, 100, 20);
        cpuLabel.setBounds(10, 5, 80, 20);
    }

private:
    ThemeManager& themeManager;
    CustomLookAndFeel customLF;

    SpectrumVisualizer visualizer;
    InteractiveEQOverlay eqOverlay;
    LevelMeter meterL, meterR; // Separated L and R meters
    LevelMeter inMeterL, inMeterR;
    OversamplingToggleGroup oversampling;

    juce::Slider masterVolume;
    juce::Label masterLabel;
    juce::Label oversamplingLabel;
    juce::TextButton hdButton, abButton, phaseButton;
    juce::Slider widthSlider, tiltSlider;
    juce::Label latencyLabel, widthLabel, tiltLabel, cpuLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputPanel)
};
