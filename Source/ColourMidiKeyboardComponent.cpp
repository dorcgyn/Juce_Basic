#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ColourMidiKeyboardComponent.h"

namespace juce
{

	ColourMidiKeyboardComponent::ColourMidiKeyboardComponent(MidiKeyboardState& state,
		Orientation orientation)
		: MidiKeyboardComponent::MidiKeyboardComponent(state, orientation)
		, groupManager(nullptr)
	{
	}

	void ColourMidiKeyboardComponent::drawWhiteNote(int midiNoteNumber,
		Graphics& g, Rectangle<float> area,
		bool isDown, bool isOver,
		Colour lineColour, Colour textColour)
	{
		if (this->groupManager != nullptr)
		{
			Group* activeGroup = this->groupManager->getFirstActiveGroup(midiNoteNumber);
			if (activeGroup != nullptr)
			{
				customizedDrawWhiteNote(midiNoteNumber, g, area, isDown, isOver, lineColour, textColour, activeGroup->getkeyColour());
				return;
			}
			
		}

		MidiKeyboardComponent::drawWhiteNote(midiNoteNumber, g, area, isDown, isOver, lineColour, textColour);
	}
	
	void ColourMidiKeyboardComponent::drawBlackNote(int midiNoteNumber,
		Graphics& g, Rectangle<float> area,
		bool isDown, bool isOver,
		Colour noteFillColour) 
	{
		MidiKeyboardComponent::drawBlackNote(midiNoteNumber, g, area, isDown, isOver, noteFillColour);
	}

	void ColourMidiKeyboardComponent::setGroupManager(GroupManager* newGroupManager)
	{
		this->groupManager = newGroupManager;
	}

	void ColourMidiKeyboardComponent::customizedDrawWhiteNote(int midiNoteNumber,
		Graphics& g, Rectangle<float> area,
		bool isDown, bool isOver,
		Colour lineColour, Colour textColour, Colour fillColour) 
	{
		auto localKeyWidth = MidiKeyboardComponent::getKeyWidth();
		auto localOrientation = MidiKeyboardComponent::getOrientation();
		auto localRangeEnd = MidiKeyboardComponent::getRangeEnd();

		auto c = fillColour;

		if (isDown)  c = findColour(keyDownOverlayColourId);
		if (isOver)  c = c.overlaidWith(findColour(mouseOverKeyOverlayColourId));

		g.setColour(c);
		g.fillRect(area);

		auto text = getWhiteNoteText(midiNoteNumber);

		if (text.isNotEmpty())
		{
			auto fontHeight = jmin(12.0f, localKeyWidth * 0.9f);

			g.setColour(textColour);
			g.setFont(Font(fontHeight).withHorizontalScale(0.8f));

			switch (localOrientation)
			{
			case horizontalKeyboard:            g.drawText(text, area.withTrimmedLeft(1.0f).withTrimmedBottom(2.0f), Justification::centredBottom, false); break;
			case verticalKeyboardFacingLeft:    g.drawText(text, area.reduced(2.0f), Justification::centredLeft, false); break;
			case verticalKeyboardFacingRight:   g.drawText(text, area.reduced(2.0f), Justification::centredRight, false); break;
			default: break;
			}
		}

		if (!lineColour.isTransparent())
		{
			g.setColour(lineColour);

			switch (localOrientation)
			{
			case horizontalKeyboard:            g.fillRect(area.withWidth(1.0f)); break;
			case verticalKeyboardFacingLeft:    g.fillRect(area.withHeight(1.0f)); break;
			case verticalKeyboardFacingRight:   g.fillRect(area.removeFromBottom(1.0f)); break;
			default: break;
			}

			if (midiNoteNumber == localRangeEnd)
			{
				switch (localOrientation)
				{
				case horizontalKeyboard:            g.fillRect(area.expanded(1.0f, 0).removeFromRight(1.0f)); break;
				case verticalKeyboardFacingLeft:    g.fillRect(area.expanded(0, 1.0f).removeFromBottom(1.0f)); break;
				case verticalKeyboardFacingRight:   g.fillRect(area.expanded(0, 1.0f).removeFromTop(1.0f)); break;
				default: break;
				}
			}
		}
	}
}