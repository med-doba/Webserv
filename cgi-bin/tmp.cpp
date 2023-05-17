#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Read the content length from the environment variables
    char* contentLengthStr = getenv("CONTENT_LENGTH");
    if (contentLengthStr == NULL) {
        std::cout << "Content-type: text/plain\r\n\r\n";
        std::cout << "Error: Content length not specified.";
        return 0;
    }

    // Convert the content length to an integer
    int contentLength = atoi(contentLengthStr);

    // Allocate a buffer to store the POST data
    char* postData = new char[contentLength + 1];

    // Read the POST data from stdin
    if (read(0, postData, contentLength) != contentLength) {
        std::cout << "Content-type: text/plain\r\n\r\n";
        std::cout << "Error: Failed to read POST data.";
        delete[] postData;
        return 0;
    }

    // Null-terminate the POST data
    postData[contentLength] = '\0';

    // Get the file extension from the environment variables
    char* scriptName = getenv("SCRIPT_NAME");
    char* extension = strrchr(scriptName, '.');
    if (extension == NULL) {
        std::cout << "Content-type: text/plain\r\n\r\n";
        std::cout << "Error: Invalid script name.";
        delete[] postData;
        return 0;
    }

    // Determine the interpreter based on the file extension
    const char* interpreter = NULL;
    if (strcmp(extension, ".py") == 0) {
        interpreter = "/usr/bin/python";
    } else if (strcmp(extension, ".pl") == 0) {
        interpreter = "/usr/bin/perl";
    } else {
        std::cout << "Content-type: text/plain\r\n\r\n";
        std::cout << "Error: Unsupported file extension.";
        delete[] postData;
        return 0;
    }

    // Create a child process
    pid_t pid = fork();
    if (pid == -1) {
        std::cout << "Content-type: text/plain\r\n\r\n";
        std::cout << "Error: Failed to fork.";
        delete[] postData;
        return 0;
    }

    if (pid == 0) {
        // Child process
        // Set the environment variable for the POST data
        setenv("CONTENT_LENGTH", contentLengthStr, 1);
        setenv("REQUEST_METHOD", "POST", 1);

        // Execute the CGI script using the appropriate interpreter
        execl(interpreter, interpreter, scriptName, NULL);

        // If execl returns, it means an error occurred
        std::cout << "Content-type: text/plain\r\n\r\n";
        std::cout << "Error: Failed to execute CGI script.";
        delete[] postData;
        return 0;
    } else {
        // Parent process
        // Wait for the child process to finish
        int status;
        waitpid(pid, &status, 0);
    }

    // Clean up
    delete[] postData;

    return 0;
}
