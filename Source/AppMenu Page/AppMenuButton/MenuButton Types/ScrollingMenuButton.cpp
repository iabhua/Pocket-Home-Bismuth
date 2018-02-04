#include "ScrollingMenuButton.h"
#include "../../../Utils.h"

ScrollingMenuButton::ScrollingMenuButton(AppMenuItem* menuItem, String name,
        IconThread& iconThread) :
AppMenuButton(menuItem,iconThread, name),
ConfigurableComponent(ComponentConfigFile::appMenuButtonKey)
{
    loadAllConfigProperties();
}

ScrollingMenuButton::~ScrollingMenuButton()
{
}

/**
 * Custom button painting method.
 */
void ScrollingMenuButton::paintButton
(Graphics &g, bool isMouseOverButton, bool isButtonDown)
{
    Rectangle<int> border = getBounds().withPosition(0, 0);
    g.setColour(getToggleState() ? selectedFillColour : fillColour);
    g.setOpacity(getToggleState() ? .8 : .2);
    g.fillRect(border);
    g.setOpacity(1);
    //app icon
    g.drawImageWithin(appIcon, imageBox.getX(), imageBox.getY(),
            imageBox.getWidth(), imageBox.getHeight(),
            RectanglePlacement::centred, false);
    //app title
    g.setColour(textColour);
    g.setFont(titleFont);
    g.drawText(getMenuItem()->getAppName(), textBox, Justification::centredLeft, true);
    g.setColour(Colour(0x4D4D4D));
    g.setOpacity(getToggleState() ? 1.0 : 0.8);
    g.drawRect(border, 2);
}

/**
 * Re-calculates draw values whenever the button is resized
 */
void ScrollingMenuButton::resized()
{
    Rectangle<float> bounds = getLocalBounds().toFloat();
    imageBox = bounds.withWidth(bounds.getHeight());
    imageBox.reduce(2, 2);
    textBox = bounds;
    textBox.setLeft(imageBox.getRight());
    textBox.reduce(4, 4);
    //It looks messy if all the fonts are different sizes, so using a default
    //String for size calculations is preferable even if really long names can 
    //get clipped.
    titleFont = fontResizedToFit(titleFont, "DefaultAppNameStr",
            textBox.toNearestInt());
}

/**
 * Load button colors from configuration files.
 */
void ScrollingMenuButton::applyConfigAssets(Array<String> assetNames,
        Array<Colour> colours)
{
    while (colours.size() < 3)
    {
        colours.add(Colours::transparentBlack);
    }
    textColour = colours[0];
    fillColour = colours[1];
    selectedFillColour = colours[2];
}
