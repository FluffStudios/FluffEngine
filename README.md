![Luminos Logo][Logo]

# Luminos Engine

The Luminos Engine is a game engine written in C++ on top of the modern OpenGL pipeline.  Access to low level system objects is written in a robust, yet safe way that is beneficial to both novice and professional developers.

### Creating Your Application

In order to create your new application, you need to link the proper version of the engine static library and include the headers in your application.  You can include a section of the library at a time by calling `<section/section.h>`, such as `<gfx\gfx.h>`.  To include the entire library's headers:
```cpp
#define INCLUDE_LUMINOS_UTILS
#include <luminos.h>
```

If you are using MSVC, you can either use the built-in linking property panel to add libraries or you can use:
```cpp
#ifdef _MSV_VER
    #pragma comment(lib, "luminos-core.lib")
#endif
```
Note that this will not work when using GCC.

### Stand-Alone Utilities Library

[Logo]: https://raw.githubusercontent.com/nickclark2016/LuminosEngine/master/luminos-core/logo.png