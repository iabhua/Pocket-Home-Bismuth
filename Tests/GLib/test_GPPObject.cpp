#include "JuceHeader.h"

class StressTest
{
public:
    StressTest(int minThreads,
            int maxThreads,
            

    void addAction(std::function<void()> testAction;);

    void runTest() override;

    
private:
    juce::Array<std::function<void()> testActions;

    class TestThread : public juce::Thread
    {
    public:
        TestThread(StressTest& testSource);

        virtual ~TestThread();
    };


}
