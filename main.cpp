#include "game.h"

#include <exception>  //或者<stdexcept> 具体详见 https://www.runoob.com/cplusplus/cpp-exceptions-handling.html
#include <iostream>

int main()
{
	try
	{
		Game game;
		game.Run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}