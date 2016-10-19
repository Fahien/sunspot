#include <GL/glew.h> // Order matters
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#include "config.h"

int main(void)
{
	printf("Test version %d.%d succesfull\n", SST_VERSION_MAJOR, SST_VERSION_MINOR);
	return EXIT_SUCCESS;
}
