
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void open_new_terminal()
{
    pid_t pid = fork(); // Create a new child process

    if (pid == 0) // In the child process
    {
        // Replace the child process with the terminal emulator
        // This example uses "gnome-terminal", but you can replace it with any terminal you use
        execlp("gnome-terminal", "gnome-terminal", "-e", "bash", (char *)NULL);
        
        // If execlp fails, exit the child process
        perror("execlp failed");
        exit(1);
    }
    else if (pid < 0) // Fork failed
    {
        perror("fork failed");
    }
    else // In the parent process
    {
        // Optionally wait for the child process to finish
        wait(NULL);
    }
}

int check_for_builtins(vars_t *vars)
{
    if (vars->av[0] == NULL)
        return NULL;

    // Handle the "new_terminal" command
    if (strcmp(vars->av[0], "new_terminal") == 0)
    {
        // Call a function to open a new terminal window
        open_new_terminal();
        return (char *)1; // Return some value to indicate the command was handled
    }

    // Check other built-in commands here (e.g., exit, cd, etc.)
    return NULL;
}

