#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    DragDropTarget - Drag-and-Drop File Handling
    
    Mixin class for components that accept dragged files.
    Used in SamplerTab for sample loading.
*/
class DragDropTarget : public juce::FileDragAndDropTarget
{
public:
    virtual ~DragDropTarget() = default;
    
    virtual bool isInterestedInFileDrag(const juce::StringArray& files) override = 0;
    virtual void filesDropped(const juce::StringArray& files, int x, int y) override = 0;
};
