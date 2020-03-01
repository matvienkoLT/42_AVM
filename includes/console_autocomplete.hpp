#ifndef CONSOLE_AUTOCOMPLITE_HPP
#define CONSOLE_AUTOCOMPLITE_HPP

#include <termios.h>
#include <iostream>

class CommandHelper
{
	static const char* kPushKey;
	static const char* kDumpKey;
	static const char* kPOPKey;
	
	static struct termios old, current;

	public:

					CommandHelper(void);
		explicit	CommandHelper(std::string &);

		char getch(void);
		char getche(void);
		int loop_listener();

	private:
		std::string*	_ptr_buffer;

		void termiOSInit(int);
		void resetTermiOS(void);
		char getch_(int);
};

#endif // CONSOLE_AUTOCOMPLITE_HPP