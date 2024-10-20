#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

#include "App.h"

int main(void)
{
    App app;
    const int result = app.run();
    return result;
}
