#include "../JuceLibraryCode/JuceHeader.h"

#include <functional>
#pragma once

class OdinTreeListener : public AudioProcessorValueTreeState::Listener {
public:
  std::function<void(const String &, float)> onValueChange;

  void parameterChanged(const String &parameterID, float newValue) override {
    onValueChange(parameterID, newValue);
  }
};

class OdinTreeListenerNonParam : public ValueTree::Listener {
public:
  std::function<void(ValueTree &, const Identifier &)> onValueChange =
      [](ValueTree &, const Identifier &) {};

  void
  valueTreePropertyChanged(ValueTree & treeWhosePropertyHasChanged,
                           const Identifier &property) override {
    onValueChange(treeWhosePropertyHasChanged, property);
  }

  // need to define these
  void valueTreeChildAdded(ValueTree &parentTree,
                           ValueTree &childWhichHasBeenAdded) {}
  void valueTreeChildRemoved(ValueTree &parentTree,
                             ValueTree &childWhichHasBeenRemoved,
                             int indexFromWhichChildWasRemoved) {}
  void valueTreeChildOrderChanged(ValueTree &parentTreeWhoseChildrenHaveMoved,
                                  int oldIndex, int newIndex) {}
  void valueTreeParentChanged(ValueTree &treeWhoseParentHasChanged) {}
};