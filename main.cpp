#include <exception>  //����<stdexcept> ������� https://www.runoob.com/cplusplus/cpp-exceptions-handling.html
#include <iostream>

#include "game.h"

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