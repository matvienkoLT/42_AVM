#include "parser_class.hpp"

namespace parser
{
	t_phrase Input::_instruction[] = {{"push", "value"}, {"pop", ""}, {"dump", ""},
										{"assert", "value"}, {"add", ""}, {"sub", ""},
										{"mul", ""}, {"div", ""}, {"mod", ""}, {"print", ""},
										{"exit", ""}, {"clear", ""}};

	const std::string Input::_typel[] =		{"int8", "int16", "int32", "float", "double"};

	Input::Input(stack_ev::dy_stack &hip) :_exit_mode(0),  _pair_validation(PAIR_VALIDATION), _stack(hip)
	{
		//! Temp storage
		std::string current_instruction;
		CommandHelper helper(current_instruction);

		try
		{
			_store.reserve(COMMAND_STORE_LIMIT);
		}
		catch (std::length_error &ba)
		{
			throw(prsRexeption("Involved Parser Exection: ___bad_alloc"));
		}
		catch (std::bad_alloc &ba)
		{
			throw(prsRexeption("Involved Parser Exection: ___bad_alloc"));
		}
		//! Read line, one by one
		for (;;)
		{
			//a = getche();
			if (!helper.loop_listener())
				continue;

			//std::cin.putback(a);
  			//printf("\nYou typed: %c\n", a);
			//std::cout<<a<<std::endl;
			//! Get text line from input stream
			//std::getline(std::cin, current_instruction);
			//std::cout<<current_instruction<<std::endl;

			//! Specific symbol for stop reading
			if (current_instruction == Constants.STOP_READING_STDIN)
				break;
			else if (current_instruction == Constants.STOP_PARSING_INSTR)
				_exit_mode = 1; // Further, all instruction will be ignored
			else if (!_exit_mode)
				parser_of_rules(current_instruction); // Otherwise, start parsing
		
			if (_store.size() == COMMAND_STORE_LIMIT)
			{
				//try
				{
					lexer();
				}
			//	catch(...)
			//	{
			//		std::cerr << "called excetion(exeption.assert)" << '\n';
			//		break;
			//	}
			}
			current_instruction = "";
		}
	}

	Input::Input(std::string file_name, stack_ev::dy_stack &hip) : _exit_mode(0), _pair_validation(PAIR_VALIDATION), _stack(hip)
	{
		//! Temp storage
		std::string		current_instruction;

		//! Open present file
		std::ifstream	ifs(file_name);

		_store.reserve(COMMAND_STORE_LIMIT);

		//! Not existing or corrupted, file
		if (!ifs)
			std::cout<<file_name + ": " + std::strerror(errno)<<std::endl; // Display error
		else
		{
			while (std::getline(ifs, current_instruction))
			{
				parser_of_rules(current_instruction);

				if (_store.size() == COMMAND_STORE_LIMIT)
				{
					try
					{
						lexer();
					}
					catch (...)
					{
						std::cerr << "called excetion(exeption.assert)" << '\n';
						break;
					}
				}
			}
		}
	}

	//! Copy constructor
	Input::Input(const Input &prototype) : _exit_mode(prototype._exit_mode), _store(prototype._store),
	_pair_validation(prototype._pair_validation)
	{
		*this = prototype;
	}

	//! Copy assignment
	Input &Input::operator=(const Input &prototype)
	{
		if (this != &prototype)
		{
			_pair_validation = prototype._pair_validation;
			_exit_mode = prototype._exit_mode;
			_store = prototype._store;
		}
		return *this;
	}

	//! Move constructor
	Input::Input(Input &&prototype) noexcept :
		_exit_mode(0), _pair_validation(nullptr)
	{
		_exit_mode = std::move(prototype._exit_mode);
		_pair_validation = std::move(prototype._pair_validation);
		_store = std::move(prototype._store);

		prototype._exit_mode = 0;
		prototype._pair_validation = std::regex(nullptr);
		prototype._store.clear();
	}

	//! Move assignment
	Input &Input::operator=(Input &&prototype) noexcept
	{
		if (this != &prototype)
		{
			_exit_mode = std::move(prototype._exit_mode);
			_pair_validation = std::move(prototype._pair_validation);
			_store = std::move(prototype._store);

			prototype._exit_mode = 0;
			prototype._pair_validation = std::regex(nullptr);
			prototype._store.clear();
		}
		return *this;
	}

	//! Output overloading
	std::ostream &operator<<(std::ostream & os, const Input &obj)
	{
		for (auto &v : obj._store)
		{
			os <<"["<<v.instruction <<"]"<<" "<<"["<<v.value<<"]"<<"\n";
		}
		return os;
	}

	void Input::parser_of_rules(std::string &current_instruction)
	{
		//! Temp storage
		t_phrase temp_token_place;

		//! Empty iterator : using for checking data existing in token
		std::regex_token_iterator<std::string::iterator> end;

		//! Tokenize input istruction
		std::regex_token_iterator<std::string::iterator> current_token(current_instruction.begin(),
																	   current_instruction.end(), _pair_validation);
																	   
		if (current_token != end)
		{
			//! Take Instruction
			/*
				example: pop, add, push
			*/

			if (((std::string)*current_token).at(0) != SYMBOL_COMMENT)
			{
				temp_token_place.instruction = *current_token++; // Increment to next token
			}
			else // or just print and ignore
			{
				return;
			}

			 //! Check if next token exist
			if (current_token != end)
			{
				if (((std::string)*current_token).at(0) != SYMBOL_COMMENT)
				{
					temp_token_place.value = *current_token++; // Increment to next token
				}
				else	// or just print and ignore
				{
					current_token++;
				}
				
			}
			//! Check if [value] is last token.
			if (current_token == end)
				_store.push_back(temp_token_place); // ADD to instruction store
			else
			{
				//! Check if next token is comment.
				if (((std::string)*current_token).at(0) != SYMBOL_COMMENT)
					std::cout << current_instruction << ": error: Undefined command: command is ignored." << std::endl;
				else
				{
					_store.push_back(temp_token_place); // ADD to instruction store
				}
				
			}
		}
	}

	/*! Declaration above the current file
		const std::string Input::_typel[] = {"int8", "int16", "int32", "float", "double"}*/

	static AVMOperant::eOperandType def_Operandtype(const std::string &type_humf)
	{
		if (type_humf == "int8")
			return AVMOperant::Int8;
		else if (type_humf == "int16")
			return AVMOperant::Int16;
		else if (type_humf == "int32")
			return AVMOperant::Int32;
		else if (type_humf == "float")
			return AVMOperant::Float;
		else if (type_humf == "double")
			return AVMOperant::Double;
		else
			return AVMOperant::Null_;
	}

	//! Submodule for lexer()

	static t_type *def_type(const std::string *types, const size_t &types_size, const std::string &instruction)
	{
		size_t		i;
		std::regex	def_type("^\\w+");
		std::regex def_value("\\(-?\\d+((\\.|\\,)(\\d+)?)?\\)");
		std::regex content_without_brakets("\\((.*?)\\)");
			std::string temp;
		std::smatch token;
		std::smatch token_value;
		std::smatch pure_value;
		t_type		*storage_preserve = nullptr;

		if (std::regex_search(instruction, token, def_type))
		{
			for (i = 0; i<types_size; ++i)
			{
				if(types[i] == token[0])
				{
					storage_preserve = new t_type();
					std::regex_search(instruction, token_value, def_value);
					std::string temp = token_value[0];
					std::regex_match(temp, pure_value, content_without_brakets);
					storage_preserve->name = types[i];
					storage_preserve->value = pure_value[1];
					storage_preserve->type = def_Operandtype(storage_preserve->name);
					if (!storage_preserve->name.empty() && !storage_preserve->value.empty())
						return storage_preserve;
					else
					{
						delete storage_preserve;
						return nullptr;
					}
				}
			}
		}
		return (nullptr);
	}
		//! The designation is checking
	
	void Input::lexer()
	{
		int64_t	iprt_inst; // Current index in array of preconceived available instruction . See: definition of parser::_instruction[]
		int64_t q_avlbl_inst = sizeof(_instruction) / sizeof(_instruction[0]); // Quantity of available _instruction;
		t_type	*ptrType = nullptr; // Ptr to type of value;

		//! We go in storage and take every phrase and check it.
		for (auto &v : _store)
		{
			//! Try to find relevant instruction.
			for (iprt_inst = 0; iprt_inst < q_avlbl_inst; ++iprt_inst)
			{
				if (_instruction[iprt_inst].instruction == v.instruction)
				{
					//! Check type of value for relevant instruction
					if (!_instruction[iprt_inst].value.empty()  && !v.value.empty())
					{
						if (!(ptrType = def_type(&_typel[0], sizeof(_typel) / sizeof(_typel[0]), v.value)))
						{
							if (v.instruction == "assert")
								throw "Failed to start instruction: " + v.instruction + " with parameters: " + v.value + '\n';
							else
							{
								std::cout << "Failed to start instruction: "<<v.instruction<<" with parameters: "<<v.value<<std::endl;
								break;
							}
						}
						else
						{
							Executant option(_stack, v.instruction, ptrType);
							delete(ptrType);
							break;
						}
					}
					else if ((!_instruction[iprt_inst].value.empty() && v.value.empty()) || (_instruction[iprt_inst].value.empty() && !v.value.empty()))
					{
						if (v.instruction == "assert")
							throw "Failed to start instruction: " + v.instruction + " without any parameter: " + v.value + '\n';
						else
						{
							std::cout<<v.instruction<<" "<<v.value<<"error: undefined argument" <<std::endl;
							break;
						}
					}
					else
					{
						Executant option(_stack, v.instruction, nullptr);
						break;
					}
				}
			}
			if (iprt_inst == q_avlbl_inst)
			{
				std::cout << "Command error: "<<v.instruction<<" is being ignored"<<std::endl;
				iprt_inst = 0;
			}
		}
		_store.clear();
	}

	Input::Executant::Executant(stack_ev::dy_stack &stack, std::string &command, t_type *value)
	{
		if (value)
		{
			AVMOperant::IOperand const *zz = factory.createOperand(value->type, value->value);

			if (!zz)
				return;
			if (command == Constants.PARSER_COMMAND_PUSH)
			{
				try
				{
					stack.push(zz);
					return;
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << '\n';
				}
			}
			else if (command == Constants.PARSER_COMMAND_ASSERT)
			{
				try
				{
					stack.assert_(zz);
				}
				catch(const std::exception& e)
				{
					std::cout << e.what() << '\n';
				}
			}
		}

		if (command == Constants.PARSER_COMMAND_POP)	stack.pop();
		else if (command == Constants.PARSER_COMMAND_DUMP)
		{
			try
			{
				stack.display_content();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (command == Constants.PARSER_COMMAND_ADD)
		{
			try
			{
				stack.add();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
		else if (command == Constants.PARSER_COMMAND_SUB)
		{
			try
			{
				stack.subtraction();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (command == Constants.PARSER_COMMAND_MUL)
		{
			try
			{
				stack.multiply();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (command == Constants.PARSER_COMMAND_DIV)
		{
			try
			{
				stack.divide();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
		else if (command == Constants.PARSER_COMMAND_MOD)
		{
			try
			{
				stack.modulus();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
		}
		else if (command == Constants.PARSER_COMMAND_PRINT)
		{
			try
			{
				stack.display_top_hip_ascii();
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
			}
			
		}
		else if (command == Constants.PARSER_COMMAND_CLEAR) stack.screen_cleaner();
	}
}