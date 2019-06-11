#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "GroupSamplerSound.h"

namespace juce
{

	class ColourMidiKeyboardComponent : public MidiKeyboardComponent
	{
	public:
		ColourMidiKeyboardComponent(MidiKeyboardState& state,
			Orientation orientation);

		/** Draws a white note in the given rectangle.

			isOver indicates whether the mouse is over the key, isDown indicates whether the key is
			currently pressed down.

			When doing this, be sure to note the keyboard's orientation.
		*/
		void drawWhiteNote(int midiNoteNumber,
			Graphics& g, Rectangle<float> area,
			bool isDown, bool isOver,
			Colour lineColour, Colour textColour) override;

		/** Draws a black note in the given rectangle.

			isOver indicates whether the mouse is over the key, isDown indicates whether the key is
			currently pressed down.

			When doing this, be sure to note the keyboard's orientation.
		*/
		void drawBlackNote(int midiNoteNumber,
			Graphics& g, Rectangle<float> area,
			bool isDown, bool isOver,
			Colour noteFillColour) override;

		void setGroupManager(GroupManager* newGroupManager);

	private:
		GroupManager* groupManager;
		void customizedDrawWhiteNote(int midiNoteNumber,
			Graphics& g, Rectangle<float> area,
			bool isDown, bool isOver,
			Colour lineColour, Colour textColour, Colour fillColour);

	};
}