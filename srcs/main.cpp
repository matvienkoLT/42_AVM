#include <iostream>
#include "parser_class.hpp"
//#include "haystack_class.hpp"
#include "IOperand.hpp"
#include <typeinfo>

int main(int argc, char const *argv[])
{
	stack_ev::dy_stack	stack;

	if (argc < 2)
	{
		{
			parser::Input P(stack);
			P.lexer();
		}
	}
	else if (argc == 2)
	{
		parser::Input P(argv[1], stack);
	}
	else
	{
		//cprint usage;
		return (0);
	}
	return 0;
}
