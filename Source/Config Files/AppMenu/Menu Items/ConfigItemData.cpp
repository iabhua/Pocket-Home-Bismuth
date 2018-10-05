#include "Utils.h"
#include "ConfigItemData.h"

/* Menu item title key. */
static const juce::Identifier titleKey("name");

/* Menu item icon key. */
static const juce::Identifier iconKey("icon");

/* Application launch command key. */
static const juce::Identifier commandKey("command");

/* Terminal application key. */
static const juce::Identifier launchInTermKey("launch in terminal");

/* Application/directory category key. */
static const juce::Identifier categoryKey("categories");

/* Folder item list key. */
static const juce::Identifier folderItemKey("folder items");

ConfigItemData::ConfigItemData(juce::var& jsonData, const int index,
        const juce::Array<int>& folderIndex) :
Localized("ConfigItemData"),
MenuItemData(index, folderIndex),
jsonData(jsonData) { }

/*
 * Creates a copy of this object.
 */
MenuItemData* ConfigItemData::clone() const
{
    juce::var dataCopy = jsonData.clone();
    return new ConfigItemData(dataCopy, getIndex(), getFolderIndex());
}

/*
 * Gets the menu item's displayed title.
 */
juce::String ConfigItemData::getTitle() const
{
    return jsonData.getProperty(titleKey, "");
}

/*
 * Sets the menu item's displayed title.
 */
void ConfigItemData::setTitle(const juce::String& title)
{
    jsonData.getDynamicObject()->setProperty(titleKey, title);
}

/*
 * Gets the name or path use to load the menu item's icon file.
 */
juce::String ConfigItemData::getIconName() const
{
    return jsonData.getProperty(iconKey, "");
}

/*
 * Sets the name or path used to load the menu item's icon file.
 */
void ConfigItemData::setIconName(const juce::String& iconName)
{
    jsonData.getDynamicObject()->setProperty(iconKey, "");
}

/*
 * Gets the application categories connected to this menu item.
 */
juce::StringArray ConfigItemData::getCategories() const
{
    using namespace juce;
    var categoryList = jsonData.getProperty(categoryKey, var());
    StringArray categoryStrings;
    for(var& category : (*categoryList.getArray()))
    {
        categoryStrings.add(category);
    }
    return categoryStrings;
}

/*
 * Sets the application categories connected to this menu item.
 */
void ConfigItemData::setCategories(const juce::StringArray& categories)
{
    jsonData.getDynamicObject()->setProperty(categoryKey, categories);
}

/*
 * Gets the menu item's application launch command.
 */
juce::String ConfigItemData::getCommand() const
{
    return jsonData.getProperty(commandKey, "");
}

/*
 * Sets the menu item's application launch command.
 */
void ConfigItemData::setCommand(const juce::String& newCommand)
{
    jsonData.getDynamicObject()->setProperty(commandKey, newCommand);
}

/*
 * Checks if this menu item launches an application in a new terminal window.
 */
bool ConfigItemData::getLaunchedInTerm() const
{
    return jsonData.getProperty(launchInTermKey, false);
}

/*
 * Sets if this menu item runs its command in a new terminal window. */
void ConfigItemData::setLaunchedInTerm(const bool termLaunch)
{
    jsonData.getDynamicObject()->setProperty(launchInTermKey, termLaunch);
}

/*
 * Deletes this menu item data from its source JSON file.
 */
void ConfigItemData::deleteFromSource()
{
    // TODO: Implement after AppConfigFile redesign.
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void ConfigItemData::updateSource()
{
    // TODO: Implement after AppConfigFile redesign.
}


/*
 * Checks if this menu item can be moved within its menu folder.
 */
bool ConfigItemData::canMoveIndex(const int offset)
{
    // TODO: Implement after AppConfigFile redesign.
    return false;
}


/*
 * Attempts to move this menu item within its menu folder.
 *                moved by the given offset value.
 */
bool ConfigItemData::moveIndex(const int offset)
{
    // TODO: Implement after AppConfigFile redesign.
    return false;
}

/*
 * Gets an appropriate title to use for a deletion confirmation 
 *         window.
 */
juce::String ConfigItemData::getConfirmDeleteTitle() const
{
    return localeText(remove_APP) 
            + getTitle() + localeText(from_favorites);
}

/*
 * Gets appropriate descriptive text for a deletion confirmation 
 *         window.
 */
juce::String ConfigItemData::getConfirmDeleteMessage() const
{
    return localeText(will_remove_link);
}



/*
 * Gets an appropriate title to use for a menu item editor.
 */
juce::String ConfigItemData::getEditorTitle() const
{
    return localeText(edit_app);
}

/*
 * Checks if a data field within this menu item can be edited.
 */
bool ConfigItemData::isEditable(const DataField dataField)
{
    switch(dataField)
    {
        case DataField::categories:
            return folderItemCount() > 0;
        case DataField::command:
        case DataField::termLaunchOption:
            return folderItemCount() == 0;
        case DataField::icon:
        case DataField::title:
            return true;
}

/*
 * Gets the number of menu items in the folder opened by this menu item.
 */
int ConfigItemData::folderItemCount()
{
}


/*
 * Gets a single menu item in the folder this menu item would open.
 */
MenuItemData* ConfigItemData::getFolderItem(int index)
{
}


/*
 * Gets all menu items in the folder this menu item would open.
 */
juce::Array<MenuItemData*> ConfigItemData::getFolderItems()
{
}


