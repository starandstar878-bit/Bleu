#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"
#include "CustomLookAndFeel.h"
#include "IRWaveformRenderer.h"
#include "PluginProcessor.h"
#include "Effects/AmpSimEffect.h"
#include "AssetBrowser.h"

class AmpPanel : public juce::Component,
                 public juce::Slider::Listener,
                 public juce::Timer
{
public:
    AmpPanel(BleuAudioProcessor& p, ThemeManager& tm) 
        : processor(p), themeManager(tm), customLF(tm), irRenderer(tm),
          modelBrowser(tm), cabBrowser(tm)
    {
        findAmpEffect();
        if (ampEffect)
        {
            if (ampEffect->ampProfilePath.isNotEmpty())
                currentModelName = juce::File(ampEffect->ampProfilePath).getFileNameWithoutExtension();
                
            if (ampEffect->irPath.isNotEmpty())
                currentCabName = juce::File(ampEffect->irPath).getFileNameWithoutExtension();
        }

        // 1. Setup Browsers
        addChildComponent(modelBrowser);
        addChildComponent(cabBrowser);
        
        modelBrowser.loadAssetsFromJson(juce::File::getCurrentWorkingDirectory().getChildFile("Models/model_list.json"), "models");
        cabBrowser.loadAssetsFromJson(juce::File::getCurrentWorkingDirectory().getChildFile("Cabinets/cabinet_list.json"), "cabinets");

        modelBrowser.onItemClicked = [this](const AssetBrowser::Item& item) {
            if (ampEffect) {
                auto file = juce::File::getCurrentWorkingDirectory().getChildFile("Models").getChildFile(item.filename);
                bool isFull = (item.type == "Full");
                ampEffect->loadAmpProfile(file, isFull);
                currentModelName = item.name;
                animateBrowser(modelBrowser, false);
                repaint();
            }
        };

        cabBrowser.onItemClicked = [this](const AssetBrowser::Item& item) {
            if (ampEffect) {
                auto file = juce::File::getCurrentWorkingDirectory().getChildFile("Cabinets").getChildFile(item.filename);
                ampEffect->loadIR(file);
                irRenderer.setFile(file);
                currentCabName = item.name;
                animateBrowser(cabBrowser, false);
                repaint();
            }
        };

        // 2. Setup Knobs
        setupKnob(gainSlider,     "GAIN",        "Input Sensitivity");
        setupKnob(bassSlider,     "BASS",        "Low Frequency EQ");
        setupKnob(midSlider,      "MIDDLE",      "Mid Frequency EQ");
        setupKnob(trebleSlider,   "TREBLE",      "High Frequency EQ");
        setupKnob(presenceSlider, "PRESENCE",    "Harmonic Clarity");
        setupKnob(masterSlider,   "MASTER",      "Output Level");
        setupKnob(outputSlider,   "FINAL LEVEL", "Final Output Trim");

        // 3. Setup Switches
        addAndMakeVisible(brightToggle);
        brightToggle.setButtonText("BRIGHT");
        brightToggle.onClick = [this] { 
            if (ampEffect) ampEffect->setParameter("Bright", brightToggle.getToggleState() ? 1.0f : 0.0f); 
        };
        
        auto setupModeBtn = [&](juce::TextButton& b, const juce::String& text, int id) {
            addAndMakeVisible(b);
            b.setButtonText(text);
            b.setRadioGroupId(1001);
            b.setClickingTogglesState(true);
            b.setToggleState(id == 0, juce::dontSendNotification);
            b.setColour(juce::TextButton::buttonOnColourId, themeManager.getColors().primaryPoint.withAlpha(0.6f));
            b.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
            
            b.onClick = [this, id] {
                if (ampEffect) ampEffect->setParameter("Channel", (float)id);
            };
        };

        setupModeBtn(normalButton, "NORMAL", 0);
        setupModeBtn(softButton,   "SOFT",   1);
        setupModeBtn(roughButton,  "ROUGH",  2);
        
        addAndMakeVisible(deepToggle);
        deepToggle.setButtonText("DEEP");
        deepToggle.onClick = [this] {
            if (ampEffect) ampEffect->setParameter("Deep", deepToggle.getToggleState() ? 1.0f : 0.0f);
        };

        addAndMakeVisible(irRenderer);
        updateSliderValues();
        startTimer(100); // Check status every 100ms
    }

    void timerCallback() override
    {
        if (ampEffect)
        {
            auto status = ampEffect->getModelStatus();
            if (lastStatus != status)
            {
                lastStatus = status;
                repaint();
            }

            auto expectedSR = ampEffect->getExpectedSampleRate();
            if (expectedSR > 0 && std::abs(expectedSR - processor.getSampleRate()) > 1.0)
            {
                if (!suggestionActive) { suggestionActive = true; repaint(); }
            }
            else if (suggestionActive)
            {
                suggestionActive = false;
                repaint();
            }
        }
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();

        // Premium Background with metal texture look
        g.setGradientFill(juce::ColourGradient::vertical(colors.backgroundGradientStart, area.getY(), colors.backgroundGradientEnd, area.getBottom()));
        g.fillRoundedRectangle(area, 10.0f);
        
        // Sections
        auto topArea = area.reduced(20).removeFromTop(100);
        auto modelArea = topArea.removeFromLeft(topArea.getWidth() / 2 - 10);
        topArea.removeFromLeft(20);
        auto cabArea = topArea;

        drawAssetBar(g, modelArea.withHeight(45), "AMP PROFILE", currentModelName, isModelBrowserOpen, true);
        drawAssetBar(g, cabArea.withHeight(45), "CAB / IR", currentCabName, isCabBrowserOpen, false);
        
        // Switch Labels
        g.setColour(colors.textMain.withAlpha(0.5f));
        g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold").withName("Outfit")));
        
        auto bottomArea = area.removeFromBottom(150);
        auto switchArea = bottomArea.removeFromLeft(bottomArea.getWidth() * 0.4f);
        
        g.drawText("BRIGHT", switchArea.removeFromLeft(100).withHeight(20), juce::Justification::centred);
        g.drawText("CHANNEL", switchArea.removeFromLeft(100).withHeight(20), juce::Justification::centred);
        g.drawText("DEEP", switchArea.removeFromLeft(100).withHeight(20), juce::Justification::centred);

        // Meter area (Right)
        auto meterArea = bottomArea.removeFromRight(100);
        g.drawText("OUTPUT", meterArea.removeFromTop(20), juce::Justification::centred);
    }

    void resized() override
    {
        auto area = getLocalBounds().reduced(20);
        
        // Top Section
        auto topArea = area.removeFromTop(100);
        auto modelArea = topArea.removeFromLeft(topArea.getWidth() / 2 - 10);
        topArea.removeFromLeft(20);
        auto cabArea = topArea;

        // Browser placement
        modelBrowser.setBounds(modelArea.withHeight(300).translated(0, 50));
        cabBrowser.setBounds(cabArea.withHeight(300).translated(0, 50));

        // Knobs in one row
        area.removeFromTop(30);
        auto knobArea = area.removeFromTop(140);
        auto w = knobArea.getWidth() / 6;

        struct { juce::Slider* slider; juce::Label* desc; } knobs[] = {
            { &gainSlider,     &gainDesc     },
            { &bassSlider,     &bassDesc     },
            { &midSlider,      &midDesc      },
            { &trebleSlider,   &trebleDesc   },
            { &presenceSlider, &presenceDesc },
            { &masterSlider,   &masterDesc   },
        };
        for (auto& k : knobs)
        {
            auto col = knobArea.removeFromLeft(w);
            k.slider->setBounds(col.removeFromTop(115));
            k.desc->setBounds(col.removeFromTop(20));
        }

        // Bottom Area
        auto bottomArea = area.removeFromBottom(120);
        
        // Switches (Left)
        auto switchArea = bottomArea.removeFromLeft(300);
        brightToggle.setBounds(switchArea.removeFromLeft(100).reduced(20, 40));
        
        auto modeArea = switchArea.removeFromLeft(160).reduced(0, 35);
        auto btnW = modeArea.getWidth() / 3;
        normalButton.setBounds(modeArea.removeFromLeft(btnW).reduced(2, 5));
        softButton.setBounds(modeArea.removeFromLeft(btnW).reduced(2, 5));
        roughButton.setBounds(modeArea.removeFromLeft(btnW).reduced(2, 5));
        
        deepToggle.setBounds(switchArea.removeFromLeft(100).reduced(20, 40));

        // Output (Right)
        auto outputArea = bottomArea.removeFromRight(150);
        outputSlider.setBounds(outputArea.removeFromLeft(80));
        irRenderer.setVisible(false); // Hide for now to match image better or find new spot
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        auto area = getLocalBounds().reduced(20);
        auto topArea = area.removeFromTop(180);
        auto modelBarArea = topArea.removeFromLeft(topArea.getWidth() / 2 - 10).withHeight(45);
        topArea.removeFromLeft(20);
        auto cabBarArea = topArea.withHeight(45);

        if (modelBarArea.contains(e.x, e.y))
        {
            animateBrowser(modelBrowser, !isModelBrowserOpen);
            isModelBrowserOpen = !isModelBrowserOpen;
            if (isCabBrowserOpen) { animateBrowser(cabBrowser, false); isCabBrowserOpen = false; }
        }
        else if (cabBarArea.contains(e.x, e.y))
        {
            if (ampEffect && ampEffect->isFullRig) return; // Prevent opening if Full Rig
            
            animateBrowser(cabBrowser, !isCabBrowserOpen);
            isCabBrowserOpen = !isCabBrowserOpen;
            if (isModelBrowserOpen) { animateBrowser(modelBrowser, false); isModelBrowserOpen = false; }
        }
        else if (isModelBrowserOpen || isCabBrowserOpen)
        {
            animateBrowser(modelBrowser, false); isModelBrowserOpen = false;
            animateBrowser(cabBrowser, false); isCabBrowserOpen = false;
        }
    }

    void sliderValueChanged(juce::Slider* slider) override
    {
        if (ampEffect == nullptr) return;
        if (slider == &gainSlider) ampEffect->setParameter("Input Gain", (float)slider->getValue());
        else if (slider == &bassSlider) ampEffect->setParameter("Bass", (float)slider->getValue());
        else if (slider == &midSlider) ampEffect->setParameter("Mid", (float)slider->getValue());
        else if (slider == &trebleSlider) ampEffect->setParameter("Treble", (float)slider->getValue());
        else if (slider == &presenceSlider) ampEffect->setParameter("Presence", (float)slider->getValue());
        else if (slider == &masterSlider) ampEffect->setParameter("Output Gain", (float)slider->getValue());
        else if (slider == &outputSlider) processor.setMasterGain((float)slider->getValue());
    }


private:
    void drawAssetBar(juce::Graphics& g, juce::Rectangle<float> area, const juce::String& label, const juce::String& currentName, bool isOpen, bool isModel)
    {
        auto colors = themeManager.getColors();
        
        g.setColour(colors.textMain.withAlpha(0.4f));
        g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold").withName("Outfit")));
        g.drawText(label, area.removeFromTop(15), juce::Justification::left);
        
        ThemeManager::drawGlassPanel(g, area, colors, 8.0f);

        // Dim if disabled (Full Rig auto-disables IR)
        if (!isModel && ampEffect && ampEffect->isFullRig)
        {
            g.setColour(juce::Colours::black.withAlpha(0.4f));
            g.fillRoundedRectangle(area, 8.0f);
            g.setColour(colors.textMain.withAlpha(0.4f));
            g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold").withName("Outfit")));
            g.drawText("DISABLED (FULL RIG)", area, juce::Justification::centred);
            return;
        }

        if (isOpen) {
            g.setColour(colors.primaryPoint.withAlpha(0.3f));
            g.drawRoundedRectangle(area, 8.0f, 1.5f);
        }
        
        auto contentArea = area.reduced(10, 0);
        
        // Icon (Folder icon placeholder)
        g.setColour(colors.textMain.withAlpha(0.6f));
        g.drawRect(contentArea.removeFromLeft(20).reduced(2, 4), 1.0f);
        
        g.setColour(colors.textMain);
        g.setFont(juce::Font(juce::FontOptions(12.0f).withName("Outfit")));
        juce::String prefix = isModel ? "Amp Profile: " : "IR Loader: ";
        g.drawText(prefix + (currentName.isEmpty() ? "None Selected" : currentName), contentArea, juce::Justification::centredLeft);

        // Status LED for NAM
        if (isModel && ampEffect)
        {
            auto status = ampEffect->getModelStatus();
            juce::Colour ledColor = juce::Colours::grey;
            bool pulse = false;

            if (status == AmpSimEffect::ModelStatus::Loading) { ledColor = juce::Colours::orange; pulse = true; }
            else if (status == AmpSimEffect::ModelStatus::Ready) ledColor = juce::Colours::green;
            
            auto ledArea = area.withTrimmedLeft(area.getWidth() - 35).withWidth(8).withHeight(8).withCentre(area.getCentre().translated(area.getWidth()/2 - 40, 0));
            
            if (pulse) ledColor = ledColor.withAlpha(0.5f + 0.5f * std::sin(juce::Time::getMillisecondCounter() * 0.01f));
            
            g.setColour(ledColor);
            g.fillEllipse(ledArea);
            g.setColour(ledColor.withAlpha(0.3f));
            g.drawEllipse(ledArea.expanded(2), 1.0f);
        }

        // Suggestion Text
        if (isModel && suggestionActive)
        {
            g.setColour(colors.secondaryPoint);
            g.setFont(juce::Font(juce::FontOptions(9.0f).withStyle("Bold").withName("Outfit")));
            juce::String sug = "Optimal SR: " + juce::String(ampEffect->getExpectedSampleRate() / 1000.0, 1) + "kHz";
            g.drawText(sug, area.withTrimmedTop(area.getHeight() - 12), juce::Justification::right);
        }
        
        g.setColour(colors.primaryPoint.withAlpha(0.6f));
        g.drawText(isOpen ? juce::String::fromUTF8("\xe2\x96\xb2") : juce::String::fromUTF8("\xe2\x96\xbc"), area.reduced(10, 0), juce::Justification::centredRight);
    }

    void animateBrowser(AssetBrowser& browser, bool open)
    {
        if (open) browser.toFront(true);
        browser.setVisible(true);
        float startAlpha = browser.getAlpha();
        float targetAlpha = open ? 1.0f : 0.0f;

        if (browserAnimator) animatorUpdater.removeAnimator(*browserAnimator);

        browserAnimator = juce::ValueAnimatorBuilder()
            .withDurationMs(300)
            .withEasing(juce::Easings::createEaseOut())
            .withValueChangedCallback([&browser, open, startAlpha, targetAlpha](float t) {
                float curAlpha = startAlpha + (targetAlpha - startAlpha) * t;
                browser.setAlpha(curAlpha);
                if (!open && curAlpha < 0.01f) browser.setVisible(false);
            })
            .build();
        animatorUpdater.addAnimator(*browserAnimator);
        browserAnimator->start();
    }

    void setupKnob(juce::Slider& s, const juce::String& name, const juce::String& desc)
    {
        s.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
        s.setRange(0.0, 1.0, 0.01);
        s.setLookAndFeel(&customLF);
        s.addListener(this);
        addAndMakeVisible(s);

        // Description label
        auto makeDesc = [&](juce::Label& lbl) {
            lbl.setText(desc, juce::dontSendNotification);
            lbl.setJustificationType(juce::Justification::centred);
            lbl.setFont(juce::Font(juce::FontOptions(9.5f).withName("Outfit")));
            lbl.setColour(juce::Label::textColourId, themeManager.getColors().textMain.withAlpha(0.45f));
            addAndMakeVisible(lbl);
        };

        if (&s == &gainSlider)     makeDesc(gainDesc);
        else if (&s == &bassSlider)     makeDesc(bassDesc);
        else if (&s == &midSlider)      makeDesc(midDesc);
        else if (&s == &trebleSlider)   makeDesc(trebleDesc);
        else if (&s == &presenceSlider) makeDesc(presenceDesc);
        else if (&s == &masterSlider)   makeDesc(masterDesc);
    }

    void findAmpEffect()
    {
        for (auto* e : processor.getActiveEffects())
        {
            if (e->getName() == "Amp Sim")
            {
                ampEffect = dynamic_cast<AmpSimEffect*>(e);
                break;
            }
        }
    }

    void updateSliderValues()
    {
        if (ampEffect == nullptr) return;
        gainSlider.setValue(ampEffect->getParameter("Input Gain"), juce::dontSendNotification);
        bassSlider.setValue(ampEffect->getParameter("Bass"), juce::dontSendNotification);
        midSlider.setValue(ampEffect->getParameter("Mid"), juce::dontSendNotification);
        trebleSlider.setValue(ampEffect->getParameter("Treble"), juce::dontSendNotification);
        presenceSlider.setValue(ampEffect->getParameter("Presence"), juce::dontSendNotification);
        masterSlider.setValue(ampEffect->getParameter("Output Gain"), juce::dontSendNotification);
    }

    BleuAudioProcessor& processor;
    ThemeManager& themeManager;
    CustomLookAndFeel customLF;
    AmpSimEffect* ampEffect = nullptr;

    juce::Slider gainSlider, bassSlider, midSlider, trebleSlider, presenceSlider, masterSlider, outputSlider;
    juce::Label  gainDesc, bassDesc, midDesc, trebleDesc, presenceDesc, masterDesc;
    juce::ToggleButton brightToggle, deepToggle;
    juce::TextButton normalButton, softButton, roughButton;
    
    IRWaveformRenderer irRenderer;
    AssetBrowser modelBrowser, cabBrowser;
    
    juce::VBlankAnimatorUpdater animatorUpdater { this };
    std::optional<juce::Animator> browserAnimator;

    juce::String currentModelName, currentCabName;
    bool isModelBrowserOpen = false, isCabBrowserOpen = false;

    AmpSimEffect::ModelStatus lastStatus = AmpSimEffect::ModelStatus::None;
    bool suggestionActive = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AmpPanel)
};
