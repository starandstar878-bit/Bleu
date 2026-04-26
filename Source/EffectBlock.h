#pragma once

#include <JuceHeader.h>
#include "ThemeManager.h"
#include "Effects/BaseEffect.h"

class EffectBlock : public juce::Component
{
public:
    EffectBlock(BaseEffect::Ptr effectInstance, ThemeManager& tm, int slotIndex, juce::VBlankAnimatorUpdater& updater) 
        : effect(effectInstance), themeManager(tm), index(slotIndex), animatorUpdater(updater) 
    {
        setInterceptsMouseClicks(true, true);
        
        // Default animation: Fade-in & Scale-up
        opacityAnimator = juce::ValueAnimatorBuilder()
            .withDurationMs(400)
            .withEasing(juce::Easings::createEaseOut())
            .withValueChangedCallback([this](float t) {
                currentOpacity = t;
                repaint();
            })
            .build();
        animatorUpdater.addAnimator(*opacityAnimator);
        opacityAnimator->start();

        scaleAnimator = juce::ValueAnimatorBuilder()
            .withDurationMs(400) // Reduced duration for faster response
            .withEasing(juce::Easings::createEaseOut()) // REMOVED SPRING (BOUNCE)
            .withValueChangedCallback([this](float t) {
                currentScale = 0.8f + (1.0f - 0.8f) * t;
                repaint();
            })
            .build();
        animatorUpdater.addAnimator(*scaleAnimator);
        scaleAnimator->start();
    }

    ~EffectBlock() override
    {
        if (posAnimator) animatorUpdater.removeAnimator(*posAnimator);
        if (scaleAnimator) animatorUpdater.removeAnimator(*scaleAnimator);
        if (opacityAnimator) animatorUpdater.removeAnimator(*opacityAnimator);
    }

    void skipScaleAnimation()
    {
        if (scaleAnimator) animatorUpdater.removeAnimator(*scaleAnimator);
        currentScale = 1.0f;
    }

    void setTargetPosition(int x, int y)
    {
        if (posAnimator.has_value() && ! posAnimator->isComplete())
            animatorUpdater.removeAnimator(*posAnimator);

        auto startX = (float)getX();
        auto startY = (float)getY();
        
        posAnimator = juce::ValueAnimatorBuilder()
            .withDurationMs(300)
            .withEasing(juce::Easings::createEaseOut())
            .withValueChangedCallback([this, startX, startY, x, y](float t) {
                float curX = startX + (x - startX) * t;
                float curY = startY + (y - startY) * t;
                setTopLeftPosition((int)curX, (int)curY);
                if (auto* p = getParentComponent()) p->repaint();
            })
            .build();
            
        animatorUpdater.addAnimator(*posAnimator);
        posAnimator->start();
    }

    void paint(juce::Graphics& g) override
    {
        auto colors = themeManager.getColors();
        
        auto area = getLocalBounds().toFloat().reduced(2.0f);
        area = area.withSizeKeepingCentre(area.getWidth() * currentScale, area.getHeight() * currentScale);
        
        bool bypassed = effect ? effect->getBypassed() : false;
        
        float opacity = isDragging ? 1.0f : ((bypassed ? 0.4f : 1.0f) * currentOpacity);
        float bgOpacity = isDragging ? 1.0f : currentOpacity;
        
        if (isDragging)
        {
            auto shadowArea = area.translated(0, 8).withHeight(10).reduced(5, 0);
            g.setColour(juce::Colours::black.withAlpha(0.15f));
            g.fillRoundedRectangle(shadowArea, 5.0f);
        }

        auto bgBase = isHovered ? colors.glassBackground.withAlpha(0.95f) : colors.glassBackground.withAlpha(0.85f);
        g.setColour(bgBase.withAlpha(bgBase.getFloatAlpha() * (isDragging ? 1.0f : (bypassed ? 0.4f : 1.0f)) * bgOpacity));
        g.fillRoundedRectangle(area, 12.0f);
        
        if (isSelected)
        {
            g.setColour(colors.primaryPoint.withAlpha(0.9f * bgOpacity));
            g.drawRoundedRectangle(area, 12.0f, 2.5f);
        }
        else if (isHovered)
        {
            g.setColour(colors.glassOutline.withAlpha(0.6f * bgOpacity));
            g.drawRoundedRectangle(area, 12.0f, 1.5f);
        }
        else
        {
            g.setColour(colors.glassOutline.withAlpha(0.3f * bgOpacity));
            g.drawRoundedRectangle(area, 12.0f, 1.0f);
        }

        if (effect)
        {
            auto iconArea = area.reduced(12.0f);
            iconArea.removeFromBottom(18); 
            
            if (bypassed && !isDragging)
            {
                g.setColour(colors.textMain.withAlpha(0.3f * bgOpacity));
                g.drawLine(area.getX() + 5, area.getBottom() - 5, area.getRight() - 5, area.getY() + 5, 1.5f);
            }

            g.saveState();
            g.setOpacity(opacity);
            effect->drawIcon(g, iconArea, colors.textMain);
            g.restoreState();
            
            g.setColour(colors.textMain.withAlpha(opacity * 0.9f));
            g.setFont(juce::Font(juce::FontOptions(10.5f).withStyle("Bold")));
            g.drawText(effect->getName().toUpperCase(), area.removeFromBottom(22).reduced(5, 0), juce::Justification::centred);

            auto powerArea = juce::Rectangle<float>(area.getRight() - 18, area.getY() + 8, 10, 10);
            g.setColour((bypassed && !isDragging) ? colors.textMain.withAlpha(0.4f * bgOpacity) : colors.primaryPoint.withAlpha(bgOpacity));
            g.drawEllipse(powerArea, 1.5f);
            if (!bypassed || isDragging) g.fillEllipse(powerArea.reduced(2.0f));

            auto removeArea = juce::Rectangle<float>(area.getX() + 8, area.getY() + 8, 14, 14);
            g.setColour(colors.textMain.withAlpha(0.25f * bgOpacity));
            g.drawEllipse(removeArea, 1.0f);
            g.setColour(colors.textMain.withAlpha(isHoveringRemove ? 0.9f : 0.5f * bgOpacity));
            g.drawLine(removeArea.getX() + 4, removeArea.getY() + 4, removeArea.getRight() - 4, removeArea.getBottom() - 4, 1.5f);
            g.drawLine(removeArea.getRight() - 4, removeArea.getY() + 4, removeArea.getX() + 4, removeArea.getBottom() - 4, 1.5f);
            removeBounds = removeArea.toNearestInt();
        }
    }

    void mouseEnter(const juce::MouseEvent&) override { isHovered = true; repaint(); }
    void mouseExit(const juce::MouseEvent&) override { isHovered = false; isHoveringRemove = false; repaint(); }
    void mouseMove(const juce::MouseEvent& e) override
    {
        bool wasHoveringRemove = isHoveringRemove;
        isHoveringRemove = removeBounds.contains(e.getPosition());
        if (wasHoveringRemove != isHoveringRemove) repaint();
    }

    void mouseDown(const juce::MouseEvent& e) override
    {
        if (removeBounds.contains(e.getPosition()))
        {
            if (onRemove) onRemove(index);
            return;
        }

        if (e.mods.isLeftButtonDown())
        {
            if (onSelect) onSelect(this);
            
            dragger.startDraggingComponent(this, e);
            isDragging = true;
            if (onMoveStart) onMoveStart();
            repaint();
        }
    }

    void mouseDrag(const juce::MouseEvent& e) override
    {
        if (isDragging)
        {
            dragger.dragComponent(this, e, nullptr);
            if (onMoving) onMoving(index, getBounds().getCentre());
        }
    }

    void mouseUp(const juce::MouseEvent& e) override
    {
        if (isDragging)
        {
            isDragging = false;
            if (onMove) onMove(index, getBounds().getCentre());
            repaint();
        }
    }

    void mouseDoubleClick(const juce::MouseEvent&) override
    {
        if (effect)
        {
            effect->setBypassed(!effect->getBypassed());
            repaint();
        }
    }

    void setSelected(bool s) { isSelected = s; repaint(); }
    BaseEffect* getEffect() { return effect.get(); }
    BaseEffect::Ptr getEffectPtr() { return effect; }

    std::function<void(EffectBlock*)> onSelect;
    std::function<void()> onMoveStart;
    std::function<void(int, juce::Point<int>)> onMoving;
    std::function<void(int, juce::Point<int>)> onMove;
    std::function<void(int)> onRemove;

private:
    BaseEffect::Ptr effect;
    ThemeManager& themeManager;
    int index;
    juce::VBlankAnimatorUpdater& animatorUpdater;
    
    std::optional<juce::Animator> posAnimator;
    std::optional<juce::Animator> scaleAnimator;
    std::optional<juce::Animator> opacityAnimator;
    
    float currentScale = 0.0f;
    float currentOpacity = 0.0f;
    
    bool isSelected = false;
    bool isHovered = false;
    bool isDragging = false;
    bool isDragOver = false;
    bool isHoveringRemove = false;
    juce::Rectangle<int> removeBounds;
    juce::ComponentDragger dragger;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectBlock)
};
