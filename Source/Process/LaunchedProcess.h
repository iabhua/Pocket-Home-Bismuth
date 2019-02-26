/**
 * @file LaunchedProcess.h
 * 
 * @brief  Handles an application launched within a child process.
 * 
 * Creating a new LaunchedProcess object will immediately start a new process
 * running a command passed in on construction.  The LaunchedProcess object can
 * then be used to raise and focus the application's window, stop the 
 * application, or read the application's output.
 * 
 * Destroying the LaunchedProcess object does not stop the associated child
 * process.
 */
#pragma once
#include "JuceHeader.h"

class LaunchedProcess
{
public:
    /**
     * @brief  Creates a LaunchedProcess object, running an application launch 
     *         command in a new child process.
     * 
     * @param launchCommand  The command used to launch an application.
     */
    LaunchedProcess(juce::String launchCommand);
    
    ~LaunchedProcess() { }
    
    /**
     * @brief  Gets the launch command used to start the process.
     * 
     * @return  The launch command provided when the process was created.
     */
    juce::String getLaunchCommand();
    
    /**
     * @brief  Checks if the launched application is still running.
     * 
     * @return  True if the application process is still running, false if it
     *          has terminated.
     */
    bool isRunning();
    
    /**
     * @brief  Attempts to terminate the application process.
     * 
     * @return  Whether this successfully terminates the process.
     */
    bool kill();
    
    /**
     * @brief  Waits for the application process to end.
     * 
     * @param timeoutMs  Maximum number of milliseconds to wait for the process.
     *                   If this value is negative, it will wait as long as 
     *                   necessary.
     */
    void waitForProcessToFinish(int timeoutMs = -1);
    
    /**
     * @brief  Gets all text output by the process to stdout and stderr.
     * 
     * @return  All process output, or the empty string if reading output fails.
     */
    juce::String getProcessOutput();
    
    /**
     * @brief  If the process is finished, return its exit code.
     * 
     * @return  The exit code, or UINT32_MAX if the process is still running.
     */
    juce::uint32 getExitCode();
    
    /**
     * @brief  Moves the application's windows in front of all other windows and
     *         focuses them.
     */
    void activateWindow();
    
private:
    /* Handles the application's process. */
    juce::ChildProcess childProcess;
    /* The command used to launch the process. */
    juce::String launchCommand;
    /* The application's system process ID. */
    int processId = -1;
    /* Iff true, the process is in a state where valid output cannot be read. */
    bool outputValid = true;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LaunchedProcess)
};
