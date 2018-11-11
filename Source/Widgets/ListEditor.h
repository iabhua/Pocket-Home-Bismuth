#pragma once
#include "LayoutManager.h"
#include "DrawableImageButton.h"
#include "JuceHeader.h"

/**
 * @file ListEditor.h
 * 
 * @brief A UI component used for editing a list of strings.
 * 
 * The user can add new strings, edit existing ones by double clicking them,
 * and delete list items.  The edited list of strings can then be obtained with
 * getListItems().
 */

class ListEditor : public juce::Component, private juce::Button::Listener,
private juce::Label::Listener, private juce::ListBoxModel
{
public:
    /**
     * @param initialList  Sets the initial contents of the list.
     */
    ListEditor(juce::StringArray initialList);

    virtual ~ListEditor() { }

    enum ColourIds
    {
        backgroundColourId = 0x1900100,
        listItemColourId = 0x1900101,
        selectedListItemColourId = 0x1900102,
        textColourId = 0x1900103
    };

    /**
     * @return the number of rows in the list
     */
    virtual int getNumRows() override;

    /**
     * @return all list row strings.
     */
    juce::StringArray getListItems() const;

    /**
     * Replace the existing item list entries with new ones.
     * 
     * @param newItems  The replacement list of strings.
     */
    void setListItems(juce::StringArray newItems);

    /**
     * Calls updateColours whenever component color values are changed. 
     */
    void colourChanged() override;

private:
    /**
     * Sets the colors of child components to match ListEditor colors
     */
    void updateColours();

    /**
     * Receives notifications when ListItemComponent text is changed.
     * These changes are then copied back to the appropriate string in the list
     * items.
     * 
     * @param source  The label component for one of the list rows.
     */
    virtual void labelTextChanged(juce::Label *source) override;

    /**
     * Clicking a listBoxItem selects it.  This should not be called directly.
     * 
     * @param row
     * 
     * @param mouseEvent
     */
    virtual void listBoxItemClicked
    (int row, const juce::MouseEvent& mouseEvent) override;

    /**
     * Double clicking a listBoxItem makes it editable.  This should not be
     * called directly.
     * 
     * @param row
     * 
     * @param mouseEvent
     */
    virtual void listBoxItemDoubleClicked
    (int row, const juce::MouseEvent & mouseEvent) override;

    /**
     * Pressing the delete key removes the selected row.  This should not be
     * called directly.
     * 
     * @param lastRowSelected
     */
    virtual void deleteKeyPressed(int lastRowSelected) override;

    /**
     * Removes a string from the list, and updates the underlying ListBox.
     * 
     * @param rowNumber
     */
    void removeRow(int rowNumber);

    /**
     * Custom ListBox item component class, basically just a label with a 
     * delete button.
     **/
    class ListItemComponent : public juce::Label
    {
    public:
        /**
         * @param text  The initial list item text.
         * 
         * @param owner The ListEditor containing this list item.
         */
        ListItemComponent(juce::String text, ListEditor * owner);
        
        virtual ~ListItemComponent() { }

        /**
         * @param id  This should be set to String(rowIndex) by the
         *             ListEditor.
         */
        void setButtonComponentID(juce::String id);

        /**
         * Sets button colour, used by the ListEditor to apply
         * its color scheme to all list items.
         * 
         * @param colour
         */
        void setButtonColour(juce::Colour colour);

        /**
         * Update the font and delete button to fit new bounds.
         */
        void resized() override;

    private:
        //the list item's delete button
        DrawableImageButton delBtn;
    };

    /**
     * Creates or recycles a list component to fit a list row.
     * 
     * @param rowNumber                 Sets which list item the component will
     *                                   represent.
     * 
     * @param isRowSelected             True iff the list row is selected.
     * 
     * @param existingComponentToUpdate Either an old list component to update,
     *                                   or nullptr if a new component should be
     *                                   created.
     * 
     * @return                          The created/updated list component.
     */
    virtual juce::Component* refreshComponentForRow(
            int rowNumber,
            bool isRowSelected,
            juce::Component * existingComponentToUpdate) override;

    /**
     * Handles the add and delete item buttons.
     * 
     * @param buttonClicked
     */
    void buttonClicked(juce::Button* buttonClicked) override;

    /**
     * Implemented as an empty function, as the list item component
     * handles its own draw operations.
     */
    void paintListBoxItem(int rowNumber, juce::Graphics&g, int width,
            int height, bool rowIsSelected) override { }

    /**
     * Re-applies the layout and adjusts list item height to fit the new
     * bounds.
     */
    void resized() override;

    //Holds all list strings.
    juce::StringArray listItems;

    //Handles the layout of the list and the editor components.
    LayoutManager layoutManager;

    //The component displaying all list items.
    juce::ListBox listContainer;

    //Text field for entering new list item text.
    juce::TextEditor newItemField;

    //Button for adding the contents of newItemField to the list.
    juce::TextButton addItemBtn;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ListEditor)
};