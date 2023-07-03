#include <iostream>

#ifdef __APPLE__	
#define APPLE_OS
#endif

#ifdef __MINGW32__
#define WINDOWS_OS
#endif

#ifdef __linux__
#define LINUX_OS
#endif

int main()
{
#ifdef APPLE_OS
	std::cout << "Hello from Apple!" << std::endl;
#elif WINDOWS_OS
	std::cout << "Hello from Windows!" << std::endl;
#elif __linux__
	std::cout << "Hello from Linux!" << std::endl;
#endif


	return 0;

}
