You MUST copy these project files (inside of the OS folders) to the root directory of the local clone of the git repository! Otherwise your IDE will NOT be able to find any libraries or source code files.

You should also copy the .gitignore file to the root directory, so that git can properly ignore generated files and things that do not need to be in the git repository.

This way everyone can ignore the projects in their root directory if they are modified by your IDE, so git doesn't try committing them all the time. It also works better with IDEs so you aren't doing ../../.. to go up through a bunch of directories.
