#define APPMENU_IMPLEMENTATION_ONLY
#include <map>
#include "AppMenu/MenuData/ConfigFile.h"
#include "AppMenu/Control/InputHandler.h"

/*
 * Initializes the InputHandler, setting it to handle the menu component's input
 * events.
 */
 AppMenu::InputHandler::InputHandler
(MenuComponent* const menuComponent, Controller* const controller) :
     menuComponent(menuComponent), controller(controller)
{
    jassert(menuComponent != nullptr);
    jassert(controller != nullptr);
    menuComponent->addKeyListener(this);
    menuComponent->addMouseListener(this, true);
    ConfigFile appConfig;
    menuComponent->openFolder(appConfig.getRootFolderItem());
}

/*
 * Unsubscribes the InputHandler from menu input events before destruction.
 */ 
AppMenu::InputHandler::~InputHandler()
{
    menuComponent->removeKeyListener(this);
    menuComponent->removeMouseListener(this);
}

/*
 * Handles clicks to menu item buttons.
 */
void AppMenu::InputHandler::menuItemClicked
(const MenuButton* clickedButton, const bool rightClicked)
{
    MenuItem buttonItem = clickedButton->getMenuItem();
    FolderComponent* parentFolder = static_cast<FolderComponent*>
        (clickedButton->getParentComponent());
    parentFolder->setSelectedIndex(buttonItem.getIndex());
    if(rightClicked)
    {
        controller->showContextMenu(buttonItem);
    }
    else
    {
        controller->activateMenuItem(buttonItem);
    }
}

/*
 * Handles clicks to menu folders.
 */
void AppMenu::InputHandler::folderClicked(const FolderComponent* clickedFolder, 
const int closestIndex, const bool rightClicked)
{
    MenuItem folderItem = clickedFolder->getFolderMenuItem();
    while(folderItem != menuComponent->getActiveFolder()
            && menuComponent->openFolderCount() > 1)
    {
        menuComponent->closeActiveFolder();
    }
    if(rightClicked)
    {
        controller->showContextMenu(folderItem, closestIndex);
    }
}

/*
 * Handles clicks elsewhere on the menu component.
 */
void AppMenu::InputHandler::menuClicked(const bool rightClicked)
{
    if(rightClicked)
    {
        controller->showContextMenu();
    }
} 

/*
 * Gets the menu component tracked by this InputHandler.
 */
AppMenu::MenuComponent* AppMenu::InputHandler::getMenuComponent()
{
    return menuComponent;
}

/*
 * Gets the menu controller used by this InputHandler.
 */
AppMenu::Controller* AppMenu::InputHandler::getController()
{
    return controller;
}

/*
 * Convert generic mouse events into calls to InputHandler clicked methods.
 */
void AppMenu::InputHandler::mouseDown(const juce::MouseEvent& event) 
{
    // TODO: Don't just assume ctrl+click is equivalent to right click, define
    //       it in the input settings.
    bool rightClicked = event.mods.isRightButtonDown()
        || event.mods.isPopupMenu()
        || event.mods.isCtrlDown();
    if((MenuComponent*) event.eventComponent == menuComponent)
    {
        menuClicked(rightClicked);
        return;
    }
    MenuButton* clickedButton = dynamic_cast<MenuButton*>(event.eventComponent);
    if(clickedButton != nullptr)
    {
        menuItemClicked(clickedButton, rightClicked);
        return;
    }
    FolderComponent* clickedFolder = dynamic_cast<FolderComponent*>
        (event.eventComponent);
    if(clickedFolder != nullptr)
    {
        juce::Point<int> clickPos = event.getPosition();
        int closestIndex = clickedFolder->closestIndex(clickPos.x, clickPos.y);
        folderClicked(clickedFolder, closestIndex, rightClicked);
    }
}

/*
 * Convert generic key events into calls to the InputHandler's protected 
 * keyPress method.
 */
bool AppMenu::InputHandler::keyPressed(const juce::KeyPress& keyPress,
juce::Component* sourceComponent) 
{
    // TODO: don't hardcode in key bindings, read them from input settings.
    using juce::KeyPress;
    
    static const std::map<int, KeyType> keyMap =
    {
        {KeyPress::upKey,     KeyType::Up},
        {KeyPress::downKey,   KeyType::Down},
        {KeyPress::leftKey,   KeyType::Left},
        {KeyPress::rightKey,  KeyType::Right},
        {KeyPress::returnKey, KeyType::Select},
        {KeyPress::escapeKey, KeyType::Cancel},
        {KeyPress::tabKey,    KeyType::Tab},
        {KeyPress::createFromDescription("ctrl+e").getKeyCode(), KeyType::Edit}
    };
    if(keyMap.count(keyPress.getKeyCode()))
    {
        return keyPressed(keyMap.at(keyPress.getKeyCode()));
    }
    return false;
}

/*
 * Scans desktop entries for updates whenever window focus is gained.
 */
void AppMenu::InputHandler::windowFocusGained() 
{
    // TODO: Implement desktop entry data updates
}

/*
 * Hides the loading spinner and stops waiting for applications to launch when
 * window focus is lost.
 */
void AppMenu::InputHandler::windowFocusLost() 
{
    controller->setLoadingState(false);
}