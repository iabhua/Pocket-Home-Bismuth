#include "FocusingListPage.h"

FocusingListPage::FocusingListPage() : PageComponent("FocusingListPage") 
{ 
    pageList.setItemsPerPage()
}

/*
 * Gets the index of the selected list item.
 */
int FocusingListPage::getSelectedIndex() const
{
    return selectedIndex;
}

/*
 * Sets the selected list index.
 */
void FocusingListPage::setSelectedIndex(const int index)
{
    if(index >= 0 && index < getListSize())
    {
        selectedIndex = index;
        updateList(TransitionAnimator::toDestination, focusDuration);
    }
    
}

/*
 * If a list item is currently selected, it will be de-selected and the
 * component will be updated.
 */
void FocusingListPage::deselect() 
{
    if(selectedIndex >= 0)
    {
        selectedIndex = -1;
        updateList(TransitionAnimator::toDestination, focusDuration);
    }
}

/*
 * Refreshes displayed list content.
 */
void FocusingListPage::updateList(TransitionAnimator::Transition transition,
        unsigned int duration)
{
    refreshList(transition, duration);
}

/*
 * Handles list item selection.
 */
void FocusingListPage::pageButtonClicked(Button* button)
{
    ListItem* listButton = dynamic_cast<ListItem*>(button);
    if(listButton != nullptr)
    {
        if(listButton->getIndex() != getSelectedIndex())
        {
            setSelectedIndex(listButton->getIndex());
        }
        else
        {
            deselect();
        }
    }
}

/*
 * When a list item is selected, override the back button to de-select
 * the list item instead of closing the page.
 */
bool FocusingListPage::overrideBackButton()
{
    if(getSelectedIndex() < 0)
    {
        return false;
    }
    deselect();
    return true;
}

FocusingListPage::ListItem::ListItem() { }

/*
 * Gets the layout used by this list item.
 */
LayoutManager::Layout FocusingListPage::ListItem::getLayout() const
{ 
    return buttonLayout.getLayout();
}

/*
 * Sets the layout used by this list item.  All components in the layout
 * will be added to the list item as child components.
 */
void FocusingListPage::ListItem::setLayout(LayoutManager::Layout layout,
        const TransitionAnimator::Transition transition,
        const unsigned int duration)) 
{ 
    buttonLayout.transitionLayout(layout, this, transition, duration);
}


        
/*
 * Gets the current list index assigned to this list item.
 */
int FocusingListPage::ListItem::getIndex() const
{
    return index;
}

/**
 * Sets the list index value stored by this list item.
 */
void FocusingListPage::ListItem::setIndex(const int newIndex)
{
    index = newIndex;
}

/*
 * Draws a border around the list item.
 */
void FocusingListPage::ListItem::paint(Graphics &g) 
{ 
    g.setColour(findColour(Label::ColourIds::textColourId));
    g.drawRoundedRectangle(0, 0, getWidth(), getHeight(), 1, borderWidth);
}

/*
 * Reapply the list item's layout when it is resized. 
 */
void FocusingListPage::ListItem::resized()
{
    buttonLayout.layoutComponents(getLocalBounds());
}

FocusingListPage::FocusingList::FocusingList() 
{
    setItemsPerPage(defaultItemsPerPage);
    setYPaddingFraction(defaultPaddingFraction);
}
      
/*
* Reloads list content, running updateListItem for each visible
* list item.
*/
void FocusingListPage::FocusingList::updateList(
        TransitionAnimator::Transition transition,
        unsigned int duration);
{
    refreshListContent(transition, duration);
}

/*
 * Reads the number of list items from the parent FocusingListPage
 */
unsigned int FocusingListPage::FocusingList::getListSize() 
{ 
    FocusingListPage* parentPage 
            = dynamic_cast<FocusingListPage*>(getParentComponent());
    if(parentPage != nullptr)
    {
        return parentPage->getListSize();
    }
    return 0;
}

/*
 * Updates a list item, loading and applying its layout from the parent
 * FocusingListPage.
 */
Component* FocusingListPage::FocusingList::updateListItem(Component* listItem,
        unsigned int index) 
{
    ListItem * pageButton;
    if(pageButton == nullptr)
    {
        pageButton = new ListItem();
    }
    else
    {
        pageButton = static_cast<ListItem*>(listItem)
    }
    
    
    FocusingListPage* parentPage 
            = static_cast<FocusingListPage*>(getParentComponent());
    
    LayoutManager::Layout buttonLayout = pageButton->getLayout();
    
    if(index == parentPage->getSelectedIndex())
    {
        updateSelectedItemLayout
        layout = controlLayout;
        
    }
    
    
}