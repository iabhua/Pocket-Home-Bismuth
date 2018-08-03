/**
 * @file XWindowInterface.h
 * 
 * @brief Interacts with the X Window System to find and manipulate windows.
 * 
 * This module searches for windows by window title, application name, or 
 * process id.  Once found, it can focus these windows, and raise them above
 * other windows on the display.
 * 
 * Most of the code in this module was adapted from xdotool, an extremely useful
 * utility for manipulating windows and automatically triggering mouse and 
 * keyboard events.  
 * 
 * @see https://github.com/jordansissel/xdotool
 */

#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "JuceHeader.h"

class XWindowInterface
{
public:
   /**
    * Create a new interface to the X11 display manager.
    * 
    * @param displayName  The name of the display to access.  If null, the
    *                     default display set by the $DISPLAY environment
    *                     variable will be used.
    */
    XWindowInterface(const char* displayName = nullptr);
        
    ~XWindowInterface();
 
    /**
     * Gets the XLib window object that represents the pocket-home application
     * window.
     *
     * @return  The main pocket-home window used by this application.
     */
    Window getPocketHomeWindow() const;    
    
    /**
     * Gets the title of a window.
     * 
     * @param window   A valid window identifier.
     * 
     * @return   The title of the window, or the empty string if the window
     *           is invalid or has no title. 
     */
    juce::String getWindowName(const Window window) const;
    
    /**
     * Gets the class of a window.
     * 
     * @param window   A valid window identifier.
     * 
     * @return   The class of the window, or the empty string if the window
     *           is invalid or has no class. 
     */
    juce::String getWindowClass(const Window window) const;
    
    /**
     * Gets the className of a window.
     * 
     * @param window   A valid window identifier.
     * 
     * @return   The className of the window, or the empty string if the window
     *           is invalid or has no className. 
     */
    juce::String getWindowClassName(const Window window) const;

    /**
     * Gets the id of the process that created a window.
     * 
     * @param window             Identifies the window being checked.
     * 
     * @return  The process id of the process that created the window.
     *
     */
    int getWindowPID(const Window window) const;

    /**
     * Performs a breadth-first search of the entire window tree, returning
     * windows that fit some match function.
     *
     * @param verifyMatch             An arbitrary matching function that will
     *                                be used to determine which windows are 
     *                                matching.  When called passing in a Window
     *                                parameter, iff verifyMatch returns true, 
     *                                the window will be added to the array 
     *                                returned by getMatchingWindows.
     *
     * @param stopAtFirstMatchDepth   If true, once a matching window is found,
     *                                the search will not check any windows
     *                                that are deeper in the window tree.  If
     *                                false, the search will continue to search
     *                                further down on the window tree.
     *
     * @return                        All windows selected by the verifyMatch
     *                                function.
     */
    juce::Array<Window> getMatchingWindows(
                    const std::function<bool(const Window)> verifyMatch,
                    const bool stopAtFirstMatchDepth = true) const;

    /**
     * Activates a window.  This will switch the active desktop to the one 
     * containing this window, bring the window to the front, and set it
     * as the focused window.
     *
     * @param window   The window to activate.
     */
    void activateWindow(const Window window) const;
    
    /**
     * Finds the current selected desktop index.
     *
     * @return   The index of the current active desktop, or -1 if the system
     *           does not support multiple desktops
     */
    int getDesktopIndex() const;

    /**
     * Sets the current active desktop index.  This will do nothing if the new
     * index is the same as the current index, the index is not a valid desktop 
     * index, or the system does not support multiple desktops.
     *
     * @param desktopIndex  The index of the desktop to set as active.
     */
    void setDesktopIndex(const int desktopIndex) const;

    /**
     * Gets the index of the desktop that contains a specific window.
     *
     * @param window   The window to check.
     *
     * @return   The index of the desktop containing the window, or -1 if
     *           the window is invalid or the system does not support multiple
     *           desktops.
     */
    int getWindowDesktop(const Window window) const;
    
private:
 
    //holds arbitrary window property data.
    struct WindowProperty
    {
        unsigned char* data = nullptr;
        Atom type = 0;
        int size = 0;
        unsigned long numItems = 0;
    
        WindowProperty() { }
    
        WindowProperty(const WindowProperty& rhs);
    
        ~WindowProperty();
    
        WindowProperty& operator=(const WindowProperty& rhs);
    };

    /**
     * Gets an arbitrary window property.
     *
     * @param window    The window to check for the given property.
     * 
     * @param property  The value used by XLib to describe the requested 
     *                  property.
     * 
     * @return  A structure containing the requested property, or one 
     *          containing no data if the property was not found.
     */
    WindowProperty getWindowProperty
    (const Window window, const Atom property) const;

    /**
     * Checks if a particular property is supported by the window manager.
     * 
     * @param property  The name of a property set by the window manager or 
     *                  pager.
     * 
     * @return  True iff this property is supported by the current window 
     *          manager or pager.
     */
    bool xPropertySupported(const char* property) const;

   //XLib display object used to connect to the X Window system.
    Display* display = nullptr; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(XWindowInterface)
};