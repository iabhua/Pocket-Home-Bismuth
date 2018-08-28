#include "GPPTestObject.h"

/*
 * Creates a GPPTestObject containing a new GTestObject.
 */
GPPTestObject::GPPTestObject() : 
GPPObject(GTEST_TYPE_OBJECT)
{
    GObject* object = G_OBJECT(gtest_object_new());
    setGObject(object);
}

/*
 * Creates a GPPTestObject sharing data with another GPPTestObject.
 */
GPPTestObject::GPPTestObject(const GPPTestObject& toCopy) :
GPPObject(toCopy, GTEST_TYPE_OBJECT) { }

/*
 * Creates a GPPTestObject holding a GTestObject. 
 */
GPPTestObject::GPPTestObject(const GTestObject* toAssign) :
GPPObject(G_OBJECT(toAssign), GTEST_TYPE_OBJECT) { }

/*
 * Gets the GTestObject testString value.
 */
juce::String GPPTestObject::getTestString()
{
    gchar** property = getProperty<char*>(GTEST_OBJECT_TEST_STRING);
    juce::String value(*property);
    g_free(property);
    return value;
}

/*
 * Gets the GTestObject testInt value. 
 */
int GPPTestObject::getTestInt()
{
    gint* property = getProperty<gint>(GTEST_OBJECT_TEST_INT);
    int value = *property;
    g_free(property);
    return value;
}

/*
 * Changes the GTestObject testString value, or does nothing if this object
 * is null.
 */
void GPPTestObject::setTestString(juce::String newString)
{
    setProperty<const char*>(GTEST_OBJECT_TEST_STRING, newString.toRawUTF8());
}

/*
 * Changes the GTestObject testInt value, or does nothing if this object is
 * null.
 */
void GPPTestObject::setTestInt(int newInt)
{
    setProperty<int>(GTEST_OBJECT_TEST_INT, newInt);
}
 
GPPTestObject::Listener::Listener()
{
}

/*
 * Connects to property change signals from a GTestObject.
 */
void GPPTestObject::Listener::connectAllSignals(GObject* source)
{
    if(source != nullptr && GTEST_IS_OBJECT(source))
    {
       connectNotifySignal(source, GTEST_OBJECT_TEST_STRING);
       connectNotifySignal(source, GTEST_OBJECT_TEST_INT);
       trackedStrings[source] = juce::String();
       trackedInts[source] = 0; 
    }
}

/*
 * Detects property change signals, and checks that the signals are
 * valid.
 */
void GPPTestObject::Listener::propertyChanged
(GObject* source, juce::String property)
{
    using namespace juce;
    GPPTestObject temp(GTEST_OBJECT(source));
    if(property == GTEST_OBJECT_TEST_STRING)
    {
        String newStr = temp.getTestString();
        jassert(newStr.length() > trackedStrings[source].length());
        trackedStrings[source] = newStr;
    }
    else if(property == GTEST_OBJECT_TEST_INT)
    {
        int newInt = temp.getTestInt();
        jassert(newInt > trackedInts[source]);
        trackedInts[source] = newInt;
    }
    else
    {
        jassertfalse;
    }
}

/*
 * Adds a new listener to this GPPTestObject's property changes.
 */
void GPPTestObject::addListener(Listener& listener)
{
    GObject* testObject = getGObject();
    if(testObject != nullptr)
    {
        listener.connectAllSignals(testObject);
    }
    g_clear_object(&testObject);
}

/*
 * Disconnects a listener from this GPPTestObject's property changes.
 */
void GPPTestObject::removeListener(Listener& listener)
{
    GObject* testObject = getGObject();
    if(testObject != nullptr)
    {
        listener.disconnectSignals(testObject);
    }
    g_clear_object(&testObject);
}
