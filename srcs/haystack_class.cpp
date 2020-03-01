#include "haystack_class.hpp"
#include <exception>


namespace stack_ev
{
	void			dy_stack::push(const AVMOperant::IOperand *obj)
	{
		try
		{
			_stack.push_back(obj);
			++_stack_size;
		}
		catch(const std::bad_alloc& e)
		{
			std::cerr << e.what() << '\n';
			throw ;
		}
	}

	void			dy_stack::assert_(const AVMOperant::IOperand *obj)
	{
		if (_stack_size > 0)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();

			if (operant1)
			{
				if (*obj == *operant1)
				{
					#ifdef DEVELOPER_MODE
						std::cout<< "Equal"<<std::endl;
					#endif
				}
				else
				{
					throw(SKException("Invalid_comparison::_different_type >>> " +
						AVMOperant::get_type_name(operant1->getType()) + "::" + operant1->toString() +
						" == " +
						AVMOperant::get_type_name(obj->getType()) + "::" + obj->toString()));
				}
			}
		}
	}

	void			dy_stack::pop(void)
	{
		if (_stack_size > 0)
		{
			delete (_stack.at(_stack_size - 1));
			_stack.erase(_stack.end() - 1);
			--_stack_size;
		}
		else
			std::cout << "Stack has " << _stack_size << " active operant" << std::endl;
	}

	inline size_t	dy_stack::size(void)
	{
		return _stack_size; 
	}

	void dy_stack::add(void)
	{
		if (_stack_size > 1)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();
			const AVMOperant::IOperand *operant2 = (&_stack.back())[-1];
			
			if (operant1 && operant2)
			{
				
				try
				{
					const AVMOperant::IOperand *result = *operant1 + *operant2;
					pop(); //collapse first operant
					pop(); //collapse second operant
					push(result);
				}
				catch (std::exception &e)
				{
					std::cout<<e.what()<<std::endl;
				}
			}
		}
		else
		{
			std::cout << "Stack has " << _stack_size << " active operands" << std::endl;
		}
		
	}

	void dy_stack::subtraction(void)
	{
		if (_stack_size > 1)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();
			const AVMOperant::IOperand *operant2 = (&_stack.back())[-1];

			if (operant1 && operant2)
			{
				try
				{
					const AVMOperant::IOperand *result = *operant1 - *operant2;
					pop(); //collapse first operant
					pop(); //collapse second operant
					push(result);
				}
				catch (std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Stack has " << _stack_size << " active operands" << std::endl;
		}
	}

	void dy_stack::multiply(void)
	{
		if (_stack_size > 1)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();
			const AVMOperant::IOperand *operant2 = (&_stack.back())[-1];

			if (operant1 && operant2)
			{

				
				try
				{
					const AVMOperant::IOperand *result = (*operant1) * (*operant2);
					pop(); //collapse first operant
					pop(); //collapse second operant
					push(result);
				}
				catch (std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Stack has " << _stack_size << " active operands" << std::endl;
		}
	}

	void dy_stack::divide(void)
	{
		if (_stack_size > 1)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();
			const AVMOperant::IOperand *operant2 = (&_stack.back())[-1];

			if (operant1 && operant2)
			{
				try
				{
					const AVMOperant::IOperand *result = (*operant1) / (*operant2);
					pop(); //collapse first operant
					pop(); //collapse second operant
					push(result);
				}
				catch (std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Stack has " << _stack_size << " active operands" << std::endl;
		}
	}

	void dy_stack::modulus(void)
	{
		if (_stack_size > 1)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();
			const AVMOperant::IOperand *operant2 = (&_stack.back())[-1];

			if (operant1 && operant2)
			{
				try
				{
					const AVMOperant::IOperand *result = (*operant1) % (*operant2);
					pop(); //collapse first operant
					pop(); //collapse second operant
					push(result);
				}
				catch (std::exception &e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
		else
		{
			std::cout << "Stack has " << _stack_size <<" active operant"<<std::endl;
		}
	}

	void			dy_stack::display_content(void)
	{
		// std::for_each(_stack.rbegin(), _stack.rend(),
		// 			  [](auto c) { std::cout << c->toString() << " ";});

		if (_stack.size() > 0)
		{
			for (auto v = _stack.crbegin(); v != _stack.crend(); v++)
				std::cout << (*v)->toString() << " " << std::endl;
		}
		else
		{
			throw(SKException("Nothing to display::stack is empty."));
		}
		
	}

	void dy_stack::display_top_hip_ascii(void)
	{
		if (_stack_size > 0)
		{
			const AVMOperant::IOperand *operant1 = _stack.back();

			if (operant1->getType() == AVMOperant::eOperandType::Int8)
			{
				std::cout<<(char)std::stoi(operant1->toString())<<std::endl;
			}
			else
			{
				throw(SKException("___Value " +  AVMOperant::get_type_name(operant1->getType()) +
				"::" + operant1->toString() + " can't be interpreted to ASCII format"));
			}
		}
		else
		{
			throw(SKException("___Intercepted forbidden command: stack is empty"));
		}
		
	}

	dy_stack::~dy_stack(void)
	{
		for (auto item : _stack)
			delete item;

		_stack.clear();
		_stack_size = 0;

		#ifdef DEVELOPER_MODE
			std::cout << "STACK DESTRUCTOR"<<std::endl;
		#endif
	}

	void			dy_stack::screen_cleaner()
	{
		if (system("CLS")) system("clear");
	}

	dy_stack::SKException::SKException(const std::string& msg) : m_msg(msg)
	{
	}

	dy_stack::SKException::~SKException()
	{
	}

	const char* dy_stack::SKException::what() const throw ()
	{
		return m_msg.c_str();
	}
}