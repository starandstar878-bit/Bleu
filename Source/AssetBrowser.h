#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"

/**
    AssetBrowser: JSON 데이터를 기반으로 모델/IR 리스트를 보여주는 프리미엄 브라우저 컴포넌트
*/
class AssetBrowser : public juce::Component,
                     public juce::TextEditor::Listener
{
public:
    struct Item {
        juce::String name;
        juce::String subText; // author or category
        juce::String filename;
        juce::String type;     // optional (Full, DI etc)
    };

    AssetBrowser(ThemeManager& tm) : themeManager(tm)
    {
        addAndMakeVisible(searchBar);
        searchBar.setTextToShowWhenEmpty("Search assets...", juce::Colours::grey.withAlpha(0.5f));
        searchBar.addListener(this);
        searchBar.setColour(juce::TextEditor::backgroundColourId, juce::Colours::transparentBlack);
        searchBar.setColour(juce::TextEditor::outlineColourId, juce::Colours::transparentBlack);
        
        addAndMakeVisible(viewport);
        viewport.setViewedComponent(&listComponent, false);
        viewport.setScrollBarsShown(true, false);
        viewport.getVerticalScrollBar().setColour(juce::ScrollBar::thumbColourId, themeManager.getColors().primaryPoint.withAlpha(0.3f));
        
        setInterceptsMouseClicks(true, true);
    }
    
    void mouseDown(const juce::MouseEvent& e) override
    {
        // Consume click to prevent it from reaching components behind or closing the browser
    }

    void loadAssetsFromJson(const juce::File& jsonFile, const juce::String& arrayKey)
    {
        allItems.clear();
        filteredItems.clear();

        if (!jsonFile.existsAsFile()) return;

        auto json = juce::JSON::parse(jsonFile);
        if (auto* obj = json.getDynamicObject())
        {
            if (auto* array = obj->getProperty(arrayKey).getArray())
            {
                for (auto& v : *array)
                {
                    Item item;
                    item.name = v["name"].toString();
                    item.filename = v["filename"].toString();
                    
                    if (v.hasProperty("author")) item.subText = v["author"].toString();
                    else if (v.hasProperty("category")) item.subText = v["category"].toString();
                    
                    if (v.hasProperty("type")) item.type = v["type"].toString();
                    
                    allItems.push_back(item);
                }
            }
        }

        updateFilter("");
    }

    void updateFilter(const juce::String& searchText)
    {
        filteredItems.clear();
        if (searchText.isEmpty())
        {
            filteredItems = allItems;
        }
        else
        {
            for (auto& item : allItems)
            {
                if (item.name.containsIgnoreCase(searchText) || item.subText.containsIgnoreCase(searchText))
                    filteredItems.push_back(item);
            }
        }

        refreshList();
    }

    void textEditorTextChanged(juce::TextEditor& ed) override
    {
        updateFilter(ed.getText());
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        auto area = getLocalBounds().toFloat();
        
        // Glass background for the entire browser - HIGHER OPACITY
        g.setColour(colors.backgroundGradientStart.withAlpha(0.95f));
        g.fillRoundedRectangle(area, 15.0f);
        
        ThemeManager::drawGlassPanel(g, area, colors, 15.0f);
        
        // Search bar bottom line
        g.setColour(colors.glassOutline.withAlpha(0.4f));
        g.drawHorizontalLine(45, 10, getWidth() - 10);
    }

    void resized() override
    {
        auto area = getLocalBounds();
        searchBar.setBounds(area.removeFromTop(45).reduced(15, 8));
        viewport.setBounds(area.reduced(2, 2)); // Use full remaining area
        
        refreshList();
    }

    std::function<void(const Item&)> onItemClicked;

private:
    class ListComponent : public juce::Component
    {
    public:
        ListComponent(AssetBrowser& b) : owner(b) {}

        void paint(juce::Graphics& g) override
        {
            auto colors = owner.themeManager.getColors();
            auto& items = owner.filteredItems;
            int itemH = 40;

            for (int i = 0; i < (int)items.size(); ++i)
            {
                auto itemArea = juce::Rectangle<int>(0, i * itemH, getWidth(), itemH).reduced(5, 2);
                
                // Hover effect
                if (hoverIndex == i)
                {
                    g.setColour(colors.primaryPoint.withAlpha(0.1f));
                    g.fillRoundedRectangle(itemArea.toFloat(), 5.0f);
                }

                // Selection effect (if needed, can be added)
                
                g.setColour(colors.textMain);
                g.setFont(juce::Font(juce::FontOptions(13.0f).withName("Outfit")));
                g.drawText(items[i].name, itemArea.reduced(10, 0), juce::Justification::centredLeft);

                if (!items[i].subText.isEmpty())
                {
                    g.setColour(colors.textMain.withAlpha(0.4f));
                    g.setFont(juce::Font(juce::FontOptions(10.0f).withName("Outfit")));
                    g.drawText(items[i].subText, itemArea.reduced(10, 0), juce::Justification::centredRight);
                }
                
                if (!items[i].type.isEmpty())
                {
                    auto typeArea = itemArea.withTrimmedLeft(itemArea.getWidth() - 100).withWidth(30).withHeight(14).withCentre(itemArea.getCentre().translated(0, 0));
                    g.setColour(items[i].type == "DI" ? juce::Colours::cyan.withAlpha(0.3f) : colors.primaryPoint.withAlpha(0.3f));
                    g.fillRoundedRectangle(typeArea.toFloat(), 3.0f);
                    g.setColour(juce::Colours::white.withAlpha(0.8f));
                    g.setFont(juce::Font(juce::FontOptions(8.0f).withStyle("Bold")));
                    g.drawText(items[i].type, typeArea, juce::Justification::centred);
                }
            }
        }

        void mouseMove(const juce::MouseEvent& e) override
        {
            int newIdx = e.y / 40;
            if (newIdx >= (int)owner.filteredItems.size()) newIdx = -1;
            
            if (newIdx != hoverIndex)
            {
                hoverIndex = newIdx;
                repaint();
            }
        }

        void mouseExit(const juce::MouseEvent&) override
        {
            hoverIndex = -1;
            repaint();
        }

        void mouseDown(const juce::MouseEvent& e) override
        {
            int idx = e.y / 40;
            if (idx >= 0 && idx < (int)owner.filteredItems.size())
            {
                if (owner.onItemClicked)
                    owner.onItemClicked(owner.filteredItems[idx]);
            }
        }

    private:
        AssetBrowser& owner;
        int hoverIndex = -1;
    };

    void refreshList()
    {
        listComponent.setSize(viewport.getWidth() - 15, (int)filteredItems.size() * 40);
        listComponent.repaint();
    }

    ThemeManager& themeManager;
    juce::TextEditor searchBar;
    juce::Viewport viewport;
    ListComponent listComponent { *this };

    std::vector<Item> allItems;
    std::vector<Item> filteredItems;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AssetBrowser)
};
