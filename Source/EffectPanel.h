#pragma once

#include "EffectBlock.h"
#include "Effects/BaseEffect.h"
#include "Effects/EffectRegistry.h"
#include "PluginProcessor.h"
#include "ThemeManager.h"
#include <JuceHeader.h>

// ─────────────────────────────────────────────────────────────────────────────
// ChainStatusBar: 체인 전환 상태 (예열중 / 준비됨 / 설치됨)를 표시합니다.
// ─────────────────────────────────────────────────────────────────────────────
class ChainStatusBar : public juce::Component, private juce::Timer {
public:
  ChainStatusBar(BleuAudioProcessor &p, ThemeManager &tm)
      : processor(p), themeManager(tm) {
    startTimerHz(24);
  }

  ~ChainStatusBar() override { stopTimer(); }

  void timerCallback() override {
    auto s = processor.getTransitionState();
    if (s != lastState) {
      lastState = s;
      pulse = 0.0f;
      repaint();
    }
    if (s == BleuAudioProcessor::TransitionState::Preparing) {
      pulse = std::fmod(pulse + 0.08f, juce::MathConstants<float>::twoPi);
      repaint();
    }
    if (s == BleuAudioProcessor::TransitionState::Idle && justInstalled > 0) {
      justInstalled--;
      repaint();
    }
    if (lastState != s &&
        lastState == BleuAudioProcessor::TransitionState::Crossover &&
        s == BleuAudioProcessor::TransitionState::Idle)
      justInstalled = 48;
  }

  void paint(juce::Graphics &g) override {
    auto colors = themeManager.getColors();
    auto area = getLocalBounds().toFloat();
    g.setColour(colors.textMain.withAlpha(0.08f));
    g.fillRoundedRectangle(area, area.getHeight() * 0.5f);

    juce::Colour dotCol;
    juce::String label;

    switch (lastState) {
    case BleuAudioProcessor::TransitionState::Preparing:
      dotCol =
          juce::Colour(0xFFFFAA33).withAlpha(0.6f + 0.4f * std::sin(pulse));
      label = "WARMING";
      break;
    case BleuAudioProcessor::TransitionState::Crossover:
      dotCol = juce::Colour(0xFF33CCFF);
      label = "READY";
      break;
    default:
      if (justInstalled > 0) {
        float alpha = juce::jmap((float)justInstalled, 0.0f, 48.0f, 0.0f, 1.0f);
        dotCol = juce::Colour(0xFF44FF88).withAlpha(alpha);
        label = "INSTALLED";
      } else {
        dotCol = colors.textMain.withAlpha(0.25f);
        label = "IDLE";
      }
      break;
    }

    float dotR = area.getHeight() * 0.28f;
    float dotX = area.getX() + 14.0f;
    float dotY = area.getCentreY();
    g.setColour(dotCol);
    g.fillEllipse(dotX - dotR, dotY - dotR, dotR * 2.0f, dotR * 2.0f);

    g.setColour(dotCol.withMultipliedAlpha(0.5f));
    g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold")));
    g.drawText(label, area.withTrimmedLeft(28.0f),
               juce::Justification::centredLeft);
  }

private:
  BleuAudioProcessor &processor;
  ThemeManager &themeManager;
  BleuAudioProcessor::TransitionState lastState =
      BleuAudioProcessor::TransitionState::Idle;
  float pulse = 0.0f;
  int justInstalled = 0;
};

// ------------------------------------------------------------------------------
// AddButton: "+" 모양의 커스텀 버튼
// ------------------------------------------------------------------------------
class AddButton : public juce::Button {
public:
  AddButton(ThemeManager &tm) : juce::Button("Add"), themeManager(tm) {}

  void paintButton(juce::Graphics &g, bool isMouseOver,
                   bool isButtonDown) override {
    auto colors = themeManager.getColors();
    auto area = getLocalBounds().toFloat().reduced(2.0f);

    float alpha = isButtonDown ? 0.8f : (isMouseOver ? 0.4f : 0.2f);
    g.setColour(colors.primaryPoint.withAlpha(alpha));
    g.fillEllipse(area);

    g.setColour(colors.primaryPoint.withAlpha(0.6f));
    g.drawEllipse(area, 1.5f);

    g.setColour(colors.textMain);
    float thickness = 2.0f;
    float size = area.getWidth() * 0.4f;
    auto center = area.getCentre();
    g.drawLine(center.x - size / 2, center.y, center.x + size / 2, center.y,
               thickness);
    g.drawLine(center.x, center.y - size / 2, center.x, center.y + size / 2,
               thickness);
  }

private:
  ThemeManager &themeManager;
};

// ------------------------------------------------------------------------------
// ParameterPanel: 이펙터의 파라미터를 조절하는 슬라이더들을 생성합니다.
// ------------------------------------------------------------------------------
class ParameterPanel : public juce::Component {
public:
  ParameterPanel(ThemeManager &tm) : themeManager(tm) {}

  void setEffect(BaseEffect *e) {
    currentEffect = e;
    sliders.clear();
    labels.clear();

    if (!currentEffect)
      return;

    auto params = currentEffect->getParameterList();
    for (auto &p : params) {
      auto *s = new juce::Slider();
      s->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
      s->setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
      s->setRange(0.0, 1.0);
      s->setValue(currentEffect->getParameter(p));
      s->onValueChange = [this, p, s] {
        if (currentEffect)
          currentEffect->setParameter(p, (float)s->getValue());
      };
      addAndMakeVisible(s);
      sliders.add(s);

      auto *l = new juce::Label("", p);
      l->setJustificationType(juce::Justification::centred);
      l->setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
      l->setColour(juce::Label::textColourId,
                   themeManager.getColors().textMain);
      addAndMakeVisible(l);
      labels.add(l);
    }
    resized();
  }

  BaseEffect *getCurrentEffect() const { return currentEffect; }

  void paint(juce::Graphics &g) override {
    auto colors = themeManager.getColors();
    auto area = getLocalBounds().toFloat();

    ThemeManager::drawGlassPanel(g, area, colors, 15.0f);

    if (currentEffect != nullptr) {
      g.setColour(colors.textHeader);
      g.setFont(juce::Font(juce::FontOptions(14.0f).withStyle("Bold")));
      g.drawText(currentEffect->getName().toUpperCase() + " PARAMETERS",
                 getLocalBounds().removeFromTop(30),
                 juce::Justification::centred);
    }
  }

  void resized() override {
    auto area = getLocalBounds().reduced(20);
    area.removeFromTop(40);

    int count = sliders.size();
    if (count == 0)
      return;

    int itemsPerRow = std::min(count, 4);
    int itemW = area.getWidth() / itemsPerRow;
    int itemH = 100;

    for (int i = 0; i < count; ++i) {
      int row = i / itemsPerRow;
      int col = i % itemsPerRow;
      auto itemArea = juce::Rectangle<int>(
          area.getX() + col * itemW, area.getY() + row * itemH, itemW, itemH);

      sliders[i]->setBounds(itemArea.withHeight(70));
      labels[i]->setBounds(itemArea.removeFromBottom(25));
    }
  }

private:
  ThemeManager &themeManager;
  BaseEffect *currentEffect = nullptr;
  juce::OwnedArray<juce::Slider> sliders;
  juce::OwnedArray<juce::Label> labels;
};

// ------------------------------------------------------------------------------
// EffectPanel: 이펙트 체인과 파라미터 에디터를 통합 관리합니다.
// ------------------------------------------------------------------------------
class EffectPanel : public juce::Component {
public:
  EffectPanel(BleuAudioProcessor &p, ThemeManager &tm)
      : processor(p), themeManager(tm), animatorUpdater(this) {
    addAndMakeVisible(viewport);
    viewport.setViewedComponent(&chainView, false);
    viewport.setScrollBarsShown(false, false);

    addAndMakeVisible(statusBar);
    refreshBlocks();

    addAndMakeVisible(paramPanel);
    paramPanel.setVisible(false);
  }

  void paint(juce::Graphics &g) override {
    auto colors = themeManager.getColors();
    auto area = getLocalBounds();
    g.fillAll(juce::Colours::transparentBlack);

    auto bottomBar = area.removeFromBottom(40);

    g.setFont(juce::Font(juce::FontOptions(13.0f).withStyle("Bold")));
    g.setColour(colors.primaryPoint);
    g.drawText("BLEU SIGNAL CHAIN", bottomBar, juce::Justification::centred);
  }

  void resized() override {
    auto area = getLocalBounds();
    // Bottom label row
    auto bottomBar = area.removeFromBottom(40);
    auto statusArea = bottomBar.removeFromRight(120).reduced(8, 11);
    statusBar.setBounds(statusArea);

    int totalHeight = area.getHeight();
    int chainH = totalHeight / 3;
    viewport.setBounds(area.removeFromTop(chainH));

    paramPanel.setBounds(0, area.getBottom() - (int)currentParamHeight,
                         area.getWidth(), (int)currentParamHeight);

    updateChainViewSize();
    layoutBlocks();
  }

  void refreshBlocks() {
    std::map<BaseEffect *, juce::Point<int>> oldPositions;
    for (auto *b : blocks)
      oldPositions[b->getEffect()] = b->getPosition();

    blocks.clear();
    const auto &activeEffects = processor.getTargetEffects();

    int i = 0;
    int effectCount = (int)activeEffects.size();
    for (auto effect : activeEffects) {
      auto *block = new EffectBlock(BaseEffect::Ptr(effect), themeManager, i,
                                    animatorUpdater);
      setupBlockCallbacks(block);
      chainView.addAndMakeVisible(block);

      if (oldPositions.count(effect)) {
        block->setTopLeftPosition(oldPositions[effect]);
      } else {
        if (i == effectCount - 1 && effectCount > 1) {
          block->skipScaleAnimation();
          block->setTopLeftPosition(chainView.getWidth(),
                                    viewport.getHeight() / 2 - 45);
        }
      }

      blocks.add(block);
      i++;
    }

    updateTailAddButton();
    layoutBlocks(false);
    resized();
  }

private:
  BleuAudioProcessor &processor;
  ThemeManager &themeManager;
  juce::VBlankAnimatorUpdater animatorUpdater;

  juce::Viewport viewport;
  ParameterPanel paramPanel{themeManager};
  ChainStatusBar statusBar{processor, themeManager};

  AddButton tailAddButton{themeManager};

  std::optional<juce::Animator> paramPanelAnimator;
  std::optional<juce::Animator> insertAlphaAnimator;

  float currentParamHeight = 0.0f;
  int insertIndex = -1;
  float currentInsertAlpha = 0.0f;

  enum class PanelState { Idle, Spreading, Dragging };
  PanelState state = PanelState::Idle;
  int dragSourceIndex = -1, dragTargetIndex = -1;

  juce::OwnedArray<EffectBlock> blocks;

  class SignalChainView : public juce::Component {
  public:
    SignalChainView(EffectPanel &p, ThemeManager &tm)
        : owner(p), themeManager(tm) {
      setMouseClickGrabsKeyboardFocus(false);
      setRepaintsOnMouseActivity(true);
    }

    void paint(juce::Graphics &g) override {
      auto colors = themeManager.getColors();
      auto centerY = getHeight() / 2.0f;
      auto w = (float)getWidth();

      float xStart = w * 0.1f;
      float xEnd = w * 0.9f;

      g.setColour(colors.primaryPoint.withAlpha(0.2f));
      g.drawLine(xStart, centerY, xEnd, centerY, 3.0f);

      if (owner.state == PanelState::Spreading && owner.insertIndex != -1) {
        auto &blocks = owner.blocks;
        if (owner.insertIndex > 0 && owner.insertIndex < blocks.size()) {
          float x1 = (float)blocks[owner.insertIndex - 1]->getRight();
          float x2 = (float)blocks[owner.insertIndex]->getX();
          float midX = (x1 + x2) / 2.0f;
          float alpha = owner.currentInsertAlpha;

          g.setColour(colors.primaryPoint.withAlpha(alpha));
          g.drawEllipse(midX - 10, centerY - 10, 20, 20, 1.5f);
          g.drawText("+", midX - 10, centerY - 10, 20, 20,
                     juce::Justification::centred);
        }
      }
    }

    void mouseMove(const juce::MouseEvent &e) override {
      if (owner.state == PanelState::Dragging)
        return;

      int newInsertIdx = -1;
      auto &blocks = owner.blocks;
      for (int i = 0; i < (int)blocks.size() - 1; ++i) {
        float midX = (blocks[i]->getRight() + blocks[i + 1]->getX()) / 2.0f;
        if (std::abs(e.position.x - midX) < 40 &&
            std::abs(e.position.y - getHeight() / 2.0f) < 50) {
          newInsertIdx = i + 1;
          break;
        }
      }

      if (newInsertIdx != owner.insertIndex) {
        owner.insertIndex = newInsertIdx;
        if (newInsertIdx != -1) {
          owner.setState(PanelState::Spreading);
          owner.animateInsertAlpha(1.0f);
        } else {
          owner.animateInsertAlpha(0.0f);
        }
      }
    }

    void mouseExit(const juce::MouseEvent &) override {
      if (owner.state == PanelState::Spreading)
        owner.animateInsertAlpha(0.0f);
    }

    void mouseDown(const juce::MouseEvent &e) override {
      if (owner.state == PanelState::Spreading && owner.insertIndex != -1)
        owner.showEffectSelectionMenu(owner.insertIndex);
      else
        owner.deselectAll();
    }

  private:
    EffectPanel &owner;
    ThemeManager &themeManager;
  };

  SignalChainView chainView{*this, themeManager};

  void updateTailAddButton() {
    if (processor.getTargetEffects().size() < 12) {
      chainView.addAndMakeVisible(tailAddButton);
      tailAddButton.setVisible(true);
      tailAddButton.onClick = [this] {
        showEffectSelectionMenu((int)processor.getTargetEffects().size());
      };
    } else {
      tailAddButton.setVisible(false);
    }
  }

  void updateChainViewSize() {
    int blockCount = blocks.size();
    int totalWidth = blockCount * (90 + 30) + 200;
    chainView.setSize(std::max(getWidth(), totalWidth), viewport.getHeight());
  }

  void layoutBlocks(bool immediate = false) {
    int blockWidth = 90;
    int blockHeight = 90;
    int spacing = 30;
    int centerY = viewport.getHeight() / 2;

    int blockCount = blocks.size();
    int totalWidth = blockCount * (blockWidth + spacing) - spacing;
    int startX = (chainView.getWidth() - std::max(0, totalWidth)) / 2;

    for (int i = 0; i < blockCount; ++i) {
      if (state == PanelState::Dragging && i == dragSourceIndex)
        continue;
      int visualIdx = i;
      if (state == PanelState::Dragging) {
        if (i >= dragTargetIndex && i < dragSourceIndex)
          visualIdx = i + 1;
        else if (i <= dragTargetIndex && i > dragSourceIndex)
          visualIdx = i - 1;
      }
      int targetX = startX + visualIdx * (blockWidth + spacing);
      int targetY = centerY - blockHeight / 2;

      blocks[i]->setSize(blockWidth, blockHeight);

      if (state == PanelState::Spreading && insertIndex != -1) {
        int shift = (int)(25 * currentInsertAlpha);
        if (i >= insertIndex)
          targetX += shift;
        else
          targetX -= shift;
      }

      if (immediate)
        blocks[i]->setTopLeftPosition(targetX, targetY);
      else
        blocks[i]->setTargetPosition(targetX, targetY);
    }
    if (tailAddButton.isVisible()) {
      int tailX = (blockCount == 0)
                      ? (chainView.getWidth() / 2 - 15)
                      : (startX + blockCount * (blockWidth + spacing) + 10);
      tailAddButton.setBounds(tailX, centerY - 15, 30, 30);
    }
  }

  void setupBlockCallbacks(EffectBlock *block) {
    block->onSelect = [this](EffectBlock *b) {
      for (auto *other : blocks)
        other->setSelected(false);
      b->setSelected(true);
      showParameterPanel(b->getEffect());
    };
    block->onMoveStart = [this]() {
      setState(PanelState::Dragging);
      animateInsertAlpha(0.0f);
    };
    block->onMoving = [this](int srcIdx, juce::Point<int> centre) {
      dragSourceIndex = srcIdx;
      int blockWidth = 90;
      int spacing = 30;
      int blockCount = blocks.size();
      int totalWidth = blockCount * (blockWidth + spacing) - spacing;
      int startX = (chainView.getWidth() - totalWidth) / 2;
      int nearestIdx =
          juce::jlimit(0, blockCount - 1,
                       (centre.x - startX + (blockWidth + spacing) / 2) /
                           (blockWidth + spacing));
      if (nearestIdx != dragTargetIndex) {
        dragTargetIndex = nearestIdx;
        layoutBlocks();
      }
      chainView.repaint();
    };
    block->onMove = [this](int srcIdx, juce::Point<int> centre) {
      if (dragSourceIndex != -1 && dragTargetIndex != -1 &&
          dragSourceIndex != dragTargetIndex)
        processor.moveEffect(dragSourceIndex, dragTargetIndex);
      dragSourceIndex = -1;
      dragTargetIndex = -1;
      setState(PanelState::Idle);
      refreshBlocks();
    };
    block->onRemove = [this](int idx) {
      const auto &activeEffects = processor.getTargetEffects();
      if (idx >= 0 && idx < activeEffects.size()) {
        auto effectToRemove = activeEffects[idx];
        if (paramPanel.getCurrentEffect() == effectToRemove.get())
          deselectAll();
      }
      processor.removeEffect(idx);
      refreshBlocks();
    };
  }

  void showParameterPanel(BaseEffect *effect) {
    paramPanel.setEffect(effect);
    paramPanel.setVisible(true);
    if (paramPanelAnimator)
      animatorUpdater.removeAnimator(*paramPanelAnimator);
    int targetH = (getHeight() - 40) * 2 / 3;
    paramPanelAnimator =
        juce::ValueAnimatorBuilder()
            .withDurationMs(400)
            .withEasing(juce::Easings::createEaseOut())
            .withValueChangedCallback([this, targetH](float t) {
              currentParamHeight = targetH * t;
              resized();
            })
            .build();
    animatorUpdater.addAnimator(*paramPanelAnimator);
    paramPanelAnimator->start();
  }

  void deselectAll() {
    for (auto *b : blocks)
      b->setSelected(false);
    hideParameterPanel();
    paramPanel.setEffect(nullptr);
  }

  void hideParameterPanel() {
    if (paramPanelAnimator)
      animatorUpdater.removeAnimator(*paramPanelAnimator);
    int startH = (int)currentParamHeight;
    paramPanelAnimator = juce::ValueAnimatorBuilder()
                             .withDurationMs(300)
                             .withEasing(juce::Easings::createEaseIn())
                             .withValueChangedCallback([this, startH](float t) {
                               currentParamHeight = startH * (1.0f - t);
                               resized();
                               if (currentParamHeight < 0.1f)
                                 paramPanel.setVisible(false);
                             })
                             .build();
    animatorUpdater.addAnimator(*paramPanelAnimator);
    paramPanelAnimator->start();
  }

  void showEffectSelectionMenu(int index) {
    juce::PopupMenu m;
    auto effects = EffectRegistry::getInstance().getAvailableEffects();
    for (int i = 0; i < effects.size(); ++i)
      m.addItem(i + 1, effects[i]);
    m.showMenuAsync(juce::PopupMenu::Options(),
                    [this, index, effects](int result) {
                      if (result > 0 && result <= effects.size()) {
                        processor.addEffect(effects[result - 1], index);
                      }
                      refreshBlocks();
                    });
  }

  void setState(PanelState newState) {
    if (state == newState)
      return;

    auto getPriority = [](PanelState s) {
      switch (s) {
      case PanelState::Dragging:
        return 2;
      case PanelState::Spreading:
        return 1;
      default:
        return 0;
      }
    };

    if (getPriority(newState) >= getPriority(state) ||
        newState == PanelState::Idle)
      state = newState;
  }

  void animateInsertAlpha(float target) {
    if (insertAlphaAnimator)
      animatorUpdater.removeAnimator(*insertAlphaAnimator);
    insertAlphaAnimator =
        juce::ValueAnimatorBuilder()
            .withDurationMs(200)
            .withValueChangedCallback(
                [this, startAlpha = currentInsertAlpha, target](float t) {
                  currentInsertAlpha = startAlpha + (target - startAlpha) * t;
                  layoutBlocks();
                  chainView.repaint();
                  if (target < 0.01f && currentInsertAlpha < 0.01f &&
                      state == PanelState::Spreading)
                    setState(PanelState::Idle);
                })
            .build();
    animatorUpdater.addAnimator(*insertAlphaAnimator);
    insertAlphaAnimator->start();
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectPanel)
};
