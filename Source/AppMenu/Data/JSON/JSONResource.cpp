#define APPMENU_IMPLEMENTATION_ONLY
#include "AppMenu/Data/JSON/ConfigKeys.h"
#include "AppMenu/Data/JSON/JSONResource.h"

/* SharedResource object key */
const juce::Identifier AppMenu::JSONResource::resourceKey 
        = "AppMenu::JSONResource";

/* JSON configuration file name */
static const constexpr char * jsonFilename = "apps.json";

/*
 * Initializes the menu data tree.
 */
AppMenu::JSONResource::JSONResource() :
Config::FileResource(resourceKey, jsonFilename)
{
    ConfigData::Ptr rootItem = new ConfigData();
    rootFolderItem = MenuItem(rootItem);
    loadJSONData(); 
    juce::var rootFolder = initProperty<juce::var>(ConfigKeys::folderItemKey);
    ((ConfigData*) rootItem.get())->initMenuData(rootFolder); 
    entryLoader.initialEntryLoad();
}

/*
 * Destroys all menu data on destruction.
 */
AppMenu::JSONResource::~JSONResource()
{
    rootFolderItem = MenuItem();
}

/*
 * Gets a menu item representing the root folder of the application menu.
 */
AppMenu::MenuItem AppMenu::JSONResource::getRootFolderItem() const
{
    return rootFolderItem;
}

/*
 * Adds a new menu item to the list of menu items.
 */
AppMenu::MenuItem AppMenu::JSONResource::addMenuItem(
        const juce::String& title, 
        const juce::String& icon,
        const juce::String& command,
        const bool launchInTerm,
        const juce::StringArray& categories,
        MenuItem& parentFolder,
        const int index)
{
    if(parentFolder.isNull() || index < 0 
            || index > parentFolder.getMovableChildCount())
    {
        DBG("AppMenu::JSONResource::" << __func__ << ": Failed to insert \""
                << title << "\" into folder \"" << parentFolder.getTitle()
                << "\" at index " << index);
        return MenuItem();
    }
    ItemData::Ptr newData = new ConfigData();
    newData->setTitle(title);
    newData->setIconName(icon);
    newData->setCommand(command);
    newData->setLaunchedInTerm(launchInTerm);
    if(!categories.isEmpty())
    {
        newData->setCategories(categories);
    }
    MenuItem newItem(newData);
    if(!parentFolder.insertChild(newItem, index))
    {
        DBG("AppMenu::JSONResource::" << __func__ << ": Failed to insert \""
                << title << "\" into folder \"" << parentFolder.getTitle()
                << "\" at index " << index);
        return MenuItem();
    }
    newItem.saveChanges();
    if(newItem.isFolder())
    {
        entryLoader.loadFolderEntries(newItem);
    }
    return newItem;
}

/*
 * Copies all menu data back to the JSON configuration file.
 */
void AppMenu::JSONResource::writeDataToJSON()
{
    using juce::Array;
    using juce::var;
    MenuItem rootItem = getRootFolderItem();
    int numItems = rootItem.getMovableChildCount();
    if(numItems > 0)
    {
        Array<var> rootFolder;
        for(int i = 0; i < numItems; i++)
        {
            MenuItem folderItem = rootItem.getFolderItem(i);
            if(!folderItem.isNull())
            {
                rootFolder.add(itemToVar(folderItem));
            }
        }
        updateProperty<Array<var>>(ConfigKeys::folderItemKey, rootFolder);
    }
}

/*
 * Gets all parameters with basic data types tracked by this ConfigFile.
 */
const std::vector<Config::DataKey>& AppMenu::JSONResource::getConfigKeys() const 
{
    return ConfigKeys::allKeys;
}

/*
 * Recursively copies a menu item and all of its child folder items into a 
 * juce::var object.
 */
juce::var AppMenu::JSONResource::itemToVar(const AppMenu::MenuItem& menuItem)
{
    juce::DynamicObject::Ptr itemObject = new juce::DynamicObject();
    itemObject->setProperty(ConfigKeys::titleKey, menuItem.getTitle());
    itemObject->setProperty(ConfigKeys::iconKey, menuItem.getIconName());
    if(menuItem.isFolder())
    {
        int configChildCount = menuItem.getMovableChildCount();
        if(configChildCount > 0)
        {
            juce::Array<juce::var> folderItems;
            for(int i = 0; i < configChildCount; i++)
            {
                MenuItem folderItem = menuItem.getFolderItem(i);
                if(!folderItem.isNull())
                {
                    folderItems.add(itemToVar(folderItem));
                }
            }
            itemObject->setProperty(ConfigKeys::folderItemKey, folderItems);
        }
        juce::StringArray categories = menuItem.getCategories();
        if(!categories.isEmpty())
        {
            itemObject->setProperty(ConfigKeys::categoryKey, categories);
        }
    }
    else
    {
        itemObject->setProperty(ConfigKeys::commandKey, menuItem.getCommand());
        itemObject->setProperty(ConfigKeys::launchInTermKey, 
                menuItem.getLaunchedInTerm());
    }
    return juce::var(itemObject);
}

/*
 * Writes all changes to this menu item back to its data source.
 */
void AppMenu::JSONResource::ConfigData::saveChanges()
{
    JSONWriter jsonWriter;
    jsonWriter.writeChanges();
}

/*
 * Removes this menu item from JSONResource's config file.
 */
void AppMenu::JSONResource::ConfigData::deleteFromSource()
{
    // This should only be called after a call to remove()
    jassert(getIndex() == -1 && getParentFolder() == nullptr);
    saveChanges();
}

/*
 * Creates an empty child menu item.
 */
AppMenu::ConfigData::Ptr 
AppMenu::JSONResource::ConfigData::createChildItem()
{
    return new ConfigData;
}

/*
 * Writes all config-defined menu data back to the JSON file.
 */
void AppMenu::JSONResource::ConfigData::JSONWriter::writeChanges()
{
    auto appJSON = getWriteLockedResource();
    appJSON->writeDataToJSON();
}
