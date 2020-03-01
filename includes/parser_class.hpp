#ifndef PARSER_CLASS_HPP_
#define PARSER_CLASS_HPP_

#include <iostream>
#include <vector>
#include <regex>
#include <fstream>
#include <exception>
#include <cstdint>
#include "haystack_class.hpp"
#include "IOperand.hpp"
#include "console_autocomplete.hpp"

//! General parser namespace
namespace parser
{

	#ifndef PAIR_VALIDATION
		# define PAIR_VALIDATION "\\S+"
	#endif

	#define SYMBOL_COMMENT		';'
	#define COMMAND_STORE_LIMIT	1

	static struct parser_command
	{
		const char* STOP_READING_STDIN	=	";;";
		const char* STOP_PARSING_INSTR	=	"exit";
		const char* PARSER_COMMAND_PUSH	=	"push";
		const char* PARSER_COMMAND_ASSERT =	"assert";
		const char* PARSER_COMMAND_POP	=	"pop";
		const char *PARSER_COMMAND_PRINT 	= 	"print";
		const char* PARSER_COMMAND_DUMP	=	"dump";
		const char* PARSER_COMMAND_ADD	=	"add";
		const char* PARSER_COMMAND_SUB	=	"sub";
		const char* PARSER_COMMAND_MUL	=	"mul";
		const char* PARSER_COMMAND_DIV	=	"div";
		const char* PARSER_COMMAND_MOD  = 	"mod";
		const char *PARSER_COMMAND_CLEAR 	= 	"clear";
	}								Constants;

	//! Struct for storage of instructions
	typedef struct phase
	{
		std::string 				instruction;
		std::string					value;
	}								t_phrase;

	//! Struct for storage of type of value
	typedef struct	type
	{
		std::string					name;
		std::string					value;
		AVMOperant::eOperandType	type;
	}								t_type;

	//! General class of input stream 
	class Input
	{
		private:
			//! Exit Flag
			/*	exit_mode(0)- Iterative mode
				exit_mode(1) - exit mode
			*/
			int						_exit_mode;
			//! Storage for instruction
			std::vector<t_phrase>	_store;

			//! List of supported instruction: example [push, pop]
			static t_phrase			_instruction[];

			//! List of supported types: example [int8, int16]
			static const	std::string _typel[];

			//! Rule of search instruction
			/*
				example:
					#define PAIR_VALIDATION
			*/
			std::regex				_pair_validation;
			stack_ev::dy_stack		_stack;

		public:
			//! Default constructor
			/*
				Supposed stdin mode
			*/
			Input(void) = delete;
			
			//! Constructor(filename) for file reading
			Input(std::string) = delete;

			//! Default constructor with connected stack
			/*
				Supposed stdin mode
			*/
			Input(stack_ev::dy_stack &);

			//! Constructor(filename) for file reading
			Input(std::string, stack_ev::dy_stack &);

			//! Copy constructor
			Input(const Input &);

			//! Copy assignment
			Input& operator=(const Input&);

			//! Move constructor
			Input(Input&&) noexcept;

			//! Move assignment
			Input& operator=(Input&&) noexcept;

			//! Default desructor
			~Input() = default;

			//! Friendly overloading of outputs operator
			friend std::ostream & operator<<(std::ostream &, const Input &);
			
			//! The designation is creating storage of pair in format ["instruction": "value"]
			void parser_of_rules(std::string &);
			void lexer();

			class Executant
			{
				public:
					Executant(void) = delete;
					Executant(stack_ev::dy_stack &, std::string &, t_type *command);

				private:
					AVMOperant::OperatorFactory	factory;
			};

			class prsRexeption : public std::exception
			{
				private:
					std::string _message;

				public:
					prsRexeption(std::string error) : _message(error)
					{
						std::cerr<<_message<< ": "<< '\n';
					}

					const char *bad_alloc() const throw()
					{
						return "bad_alloc in instruction store caught: ";
					}
			};
	};
}

#endif	// PARSER_CLASS_HPP_