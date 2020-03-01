#include "IOperand.hpp"
#include <limits.h>
#include <sstream>
#include <iomanip>

namespace AVMOperant
{

	IntSupremeValues::IntSupremeValues(const eOperandType &e)
	{
		switch (e)
		{
		case Int8:
			_AVM_INT_MAX = INT8_MAX;
			_AVM_INT_MIN = INT8_MIN;
			break;
		case Int16:
			_AVM_INT_MAX = INT16_MAX;
			_AVM_INT_MIN = INT16_MIN;
			break;
		case Int32:
			_AVM_INT_MAX = INT32_MAX;
			_AVM_INT_MIN = INT32_MIN;
			break;
		default:
			_AVM_INT_MAX = INT64_MAX;
			_AVM_INT_MIN = INT64_MIN;
			break;
		}
	}

	FloatSupremeValues::FloatSupremeValues(const eOperandType &e)
	{
		switch (e)
		{
		case Float:
			_AVM_FLOAT_MAX = std::numeric_limits<float>::max();
			_AVM_FLOAT_MIN = std::numeric_limits<float>::lowest();
			break;
		case Double:
			_AVM_FLOAT_MAX = std::numeric_limits<double>::max();
			_AVM_FLOAT_MIN = std::numeric_limits<double>::lowest();
			break;
		default:
			_AVM_FLOAT_MAX = std::numeric_limits<long double>::max();
			_AVM_FLOAT_MIN = std::numeric_limits<long double>::lowest();
			break;
		}
	}

	IOperand const * OperatorFactory::createOperand(eOperandType type, std::string const &value) const
	{
		switch (type)
		{
			case Int8:
				return	createInt8(value);
			case Int16:
				return createInt16(value);
			case Int32:
				return createInt32(value);
			case Float:
				return createFloat(value);
			case Double:
				return createDouble(value);
			default:
				std::cout<<"type error: forbidden command\n";
				break;
		}
		return (nullptr);
	}

	IOperand const *OperatorFactory::createInt8(std::string const & value) const
	{
		//! The Union object to function
		//Note: Impossible to have the object as a private member because method has const behavior
		
		t_benchFunc	m_func;
		IOperand *outObject = nullptr;

		try
		{
			m_func.benchFunc = std::stoi;
			outObject = new TypeOperand_<int8_t>(Int8, value, m_func);
		}
		catch (std::exception &e)
		{
			std::cout<<e.what()<<std::endl;
			if (outObject)
				delete outObject;
			return nullptr;
		}

		if (outObject && outObject->getType() == Null_)
			return nullptr;
		else
			return outObject;
	}

	IOperand const *OperatorFactory::createInt16( std::string const & value ) const
	{
		//!Description above
		t_benchFunc	m_func;
		IOperand*	outObject = nullptr;

		try
		{
			m_func.benchFunc = std::stoi;
			outObject = new TypeOperand_<int16_t>(Int16, value, m_func);
		}
		catch (std::exception &e)
		{
			std::cout<<e.what()<<std::endl;
			if (outObject)
				delete outObject;
			return nullptr; 
		}

		if (outObject && outObject->getType() == Null_)
			return nullptr;
		else
			return outObject;
	}
	
	IOperand const *OperatorFactory::createInt32( std::string const & value ) const
	{
		t_benchFunc	m_func;
		IOperand*	outObject = nullptr;

		try
		{
			m_func.benchFunc = std::stoi;
			outObject = new TypeOperand_<int32_t>(Int32, value, m_func);
		}
		catch(const std::exception& e)
		{
			std::cout<<e.what()<<std::endl;
			if (outObject)
				delete outObject;
			return nullptr; 
		}

		if (outObject && outObject->getType() == Null_)
			return nullptr;
		else
			return outObject;
	}

	IOperand const *OperatorFactory::createDouble( std::string const & value ) const
	{
		t_benchFunc m_func;
		IOperand*	outObject = nullptr;
		
		try
		{
			m_func.benchFuncLongDouble = std::stold;
			outObject = new TypeOperand_<double>(Double, value, m_func);
		}

		catch(const std::exception& e)
		{
			std::cout<<e.what()<<std::endl;
			if (outObject)
				delete outObject;
			return nullptr; 
		}

		if (outObject && outObject->getType() == Null_)
			return nullptr;
		else
			return outObject;
	}

	IOperand const *OperatorFactory::createFloat( std::string const & value ) const
	{
		t_benchFunc	m_func;
		IOperand*	outObject = nullptr;

		try
		{
			m_func.benchFuncLongDouble = std::stold;
			outObject = new TypeOperand_<float>(Float, value, m_func);
		}
		catch(const std::exception& e)
		{
			std::cout<<e.what()<<std::endl;
			if (outObject)
				delete outObject;
			return nullptr; 
		}

		if (outObject && outObject->getType() == Null_)
			return nullptr;
		else
			return outObject;
	}

	// template <class T>
	// const struct TypeOperand_<T>::MxExeption::errors_description TypeOperand_<T>::MxExeption::Errors;

	template <class T>
	std::string const TypeOperand_<T>::toString(void) const
	{
		std::string output;

		if (_operand_type == Float || _operand_type == Double)
		{
			std::stringstream sstream;
			sstream << _value;
			output = sstream.str();
		}
		else
		{
			output = std::to_string(_value);
		}
		return output;
	}

	template <class T>
	IOperand const *TypeOperand_<T>::operator+(IOperand const &rhs) const
	{
		eOperandType	regent;
		OperatorFactory	factory;
		//std::variant<TypeOperand_<int8_t>, TypeOperand_<int32_t>> storage;

		regent = _regent_type(this->getType(), rhs.getType());

		if (rhs.getType() == Int8)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int8_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() - secondOperant)) /* `a + x` would overflow */
			{
				MxExeption exec;
				exec.setOverflow8();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant))		/* `a + x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow8();
				throw(exec);
			}
			auto result = firstOperant + secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int16)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int16_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() - secondOperant)) /* `a + x` would overflow */
			{
				MxExeption exec;
				exec.setOverflow16();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a + x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow16();
				throw(exec);
			}
			auto result = firstOperant + secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int32)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int32_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() - secondOperant)) /* `a + x` would overflow */
			{
				MxExeption exec;
				exec.setOverflow32();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a + x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow32();
				throw(exec);
			}
			auto result = firstOperant + secondOperant;
			return factory.createOperand(regent, std::to_string(result));
				}
		else if (rhs.getType() == Float)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<float> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			FloatSupremeValues limits(regent);


			if ((secondOperant > 0) && (firstOperant > limits.getMax() - secondOperant)) /* `a + x` would overflow */
			{
										std::cout<<'N'<<std::endl;
				MxExeption exec;
				exec.setFloatOverFlow();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a + x` would underflow */
			{
				MxExeption exec;
				exec.setFloatUnderFlow();
				throw(exec);
			}
			auto result = firstOperant + secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Double)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<double> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			FloatSupremeValues limits(regent);
		
			if ((secondOperant > 0) && (firstOperant > limits.getMax() - secondOperant)) /* `a + x` would overflow */
			{
				MxExeption exec;
				exec.setDoubleUnderFlow();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a + x` would underflow */
			{
				MxExeption exec;
				exec.setDoubleUnderFlow();
				throw(exec);
			}

			auto result = firstOperant + secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		return this;
	}

	template <class T>
	IOperand const *TypeOperand_<T>::operator-(IOperand const &rhs) const
	{
		eOperandType regent;
		OperatorFactory factory;

		regent = _regent_type(this->getType(), rhs.getType());

		if (rhs.getType() == Int8)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int8_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a - x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow8();
				throw(exec);
			}

			auto result = firstOperant - secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int16)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int16_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a - x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow16();
				throw(exec);
			}

			auto result = firstOperant - secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int32)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int32_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a - x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow32();
				throw(exec);
			}

			auto result = firstOperant - secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Float)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<float> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			FloatSupremeValues limits(regent);

			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a - x` would underflow */
			{
				MxExeption exec;
				exec.setFloatUnderFlow();
				throw(exec);
			}
			auto result = firstOperant - secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Double)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<double> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			FloatSupremeValues limits(regent);

			if ((secondOperant < 0) && (firstOperant < limits.getMin() - secondOperant)) /* `a - x` would underflow */
			{
				MxExeption exec;
				exec.setDoubleUnderFlow();
				throw(exec);
			}
			auto result = firstOperant - secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		return this;
	}

	template <class T>
	IOperand const *TypeOperand_<T>::operator*(IOperand const &rhs) const
	{
		eOperandType regent;
		OperatorFactory factory;

		regent = _regent_type(this->getType(), rhs.getType());

		if (rhs.getType() == Int8)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int8_t> &>(rhs);

			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() / secondOperant)) /* `a * x` would overflow */
			{
				MxExeption exec;
				exec.setOverflow8();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() / secondOperant)) /* `a * x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow8();
				throw(exec);
			}

			auto result = firstOperant * secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int16)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int16_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() / secondOperant)) /* `a * x` would overflow */
			{
				MxExeption exec;
				exec.setOverflow16();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() / secondOperant)) /* `a * x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow16();
				throw(exec);
			}

			auto result = firstOperant * secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int32)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int32_t> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			IntSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() / secondOperant)) /* `a * x` would overflow */
			{
				MxExeption exec;
				exec.setOverflow32();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() / secondOperant)) /* `a * x` would underflow */
			{
				MxExeption exec;
				exec.setUnderflow32();
				throw(exec);
			}

			auto result = firstOperant * secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Float)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<float> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			FloatSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() / secondOperant)) /* `a * x` would overflow */
			{
				MxExeption exec;
				exec.setFloatOverFlow();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() / secondOperant)) /* `a * x` would underflow */
			{
				MxExeption exec;
				exec.setFloatUnderFlow();
				throw(exec);
			}

			auto result = firstOperant * secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Double)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<double> &>(rhs);
			auto firstOperant = rhs_c.get_value();
			auto secondOperant = this->get_value();
			FloatSupremeValues limits(regent);

			if ((secondOperant > 0) && (firstOperant > limits.getMax() / secondOperant)) /* `a * x` would overflow */
			{
				MxExeption exec;
				exec.setDoubleOverFlow();
				throw(exec);
			}
			if ((secondOperant < 0) && (firstOperant < limits.getMin() / secondOperant)) /* `a * x` would underflow */
			{
				MxExeption exec;
				exec.setDoubleUnderFlow();
				throw(exec);
			}

			auto result = firstOperant * secondOperant;
			return factory.createOperand(regent, std::to_string(result));
		}
		return this;
	}

	template <class T>
	IOperand const *TypeOperand_<T>::operator/(IOperand const &rhs) const
	{
		eOperandType regent;
		OperatorFactory factory;

		regent = _regent_type(this->getType(), rhs.getType());

		if (rhs.getType() == Int8)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int8_t> &>(rhs);
			auto value = rhs_c.get_value();

			if (this->get_value() == 0)
			{
				MxExeption exec;
				exec.setDividebyZero();
				throw(exec);
			} //	throw(MxExeption("Involved IOperand Exception: ___divide_by_zero"));
			auto result = value / this->get_value();
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int16)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int16_t> &>(rhs);
			auto value = rhs_c.get_value();

			if (this->get_value() == 0)
			{
				MxExeption exec;
				exec.setDividebyZero();
				throw(exec);
			} //	throw(MxExeption("Involved IOperand Exception: ___divide_by_zero"));

			auto result = value / this->get_value();
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Int32)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int32_t> &>(rhs);
			auto value = rhs_c.get_value();

			if (this->get_value() == 0)
			{
				MxExeption exec;
				exec.setDividebyZero();
				throw(exec);
			} //	throw(MxExeption("Involved IOperand Exception: ___divide_by_zero"));

			auto result = value / this->get_value();
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Float)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<float> &>(rhs);
			auto value = rhs_c.get_value();

			if (this->get_value() == 0)
			{
				MxExeption exec;
				exec.setDividebyZero();
				throw(exec);
			} //	throw(MxExeption("Involved IOperand Exception: ___divide_by_zero"));

			auto result = value / this->get_value();
			return factory.createOperand(regent, std::to_string(result));
		}
		else if (rhs.getType() == Double)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<double> &>(rhs);
			auto value = rhs_c.get_value();

			if (this->get_value() == 0)
			{
				MxExeption exec;
				exec.setDividebyZero();
				throw(exec);
			} //	throw(MxExeption("Involved IOperand Exception: ___divide_by_zero"));

			auto result = value / this->get_value();
			return factory.createOperand(regent, std::to_string(result));
		}
		return this;
	}

	template <class T>
	IOperand const *TypeOperand_<T>::operator%(IOperand const &rhs) const
	{
		eOperandType regent;
		OperatorFactory factory;

		regent = _regent_type(this->getType(), rhs.getType());

		if (rhs.getType() == Int8)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int8_t> &>(rhs);
			auto mod = rhs_c.get_value();

			if (mod == 0)
			{
				MxExeption exec;
				exec.setModulusbyZero();
				throw(exec);
			}

			auto remainder = !this->get_value() ? 0 : this->get_value() - mod * static_cast<int64_t>(this->get_value() / mod);
			return factory.createOperand(regent, std::to_string(remainder));
		}
		else if (rhs.getType() == Int16)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int16_t> &>(rhs);
			auto mod = rhs_c.get_value();

			if (mod == 0)
			{
				MxExeption exec;
				exec.setModulusbyZero();
				throw(exec);
			}

			auto remainder = !this->get_value() ? 0 : this->get_value() - mod * static_cast<int64_t>(this->get_value() / mod);
			return factory.createOperand(regent, std::to_string(remainder));
		}
		else if (rhs.getType() == Int32)
		{
			auto rhs_c = dynamic_cast<const TypeOperand_<int32_t> &>(rhs);
			auto mod = rhs_c.get_value();
	
			if (mod == 0)
			{
				MxExeption exec;
				exec.setModulusbyZero();
				throw(exec);
			}

			auto remainder = !this->get_value() ? 0 : this->get_value() - mod * static_cast<int64_t>(this->get_value() / mod);
			return factory.createOperand(regent, std::to_string(remainder));
		}
		else if (rhs.getType() == Float)
		{
				MxExeption exec;
				exec.setFloatInvalidOperation();
				throw(exec);
		}
		else if (rhs.getType() == Double)
		{
				MxExeption exec;
				exec.setFloatInvalidOperation();
				throw(exec);
		}
		return this;
	}

	template <class T>
	bool TypeOperand_<T>::operator==(IOperand const &rhs) const
	{
		if (this->getType() == rhs.getType() && this->toString() == rhs.toString())
			return true;
		return false;
	}

	template <class T>
	int		TypeOperand_<T>::getPrecision(void) const
	{  
		switch (_operand_type)
		{
			case Float:
				return 0x4;
			case Double:
				return 0x8;
			default:
				return _operand_type;
		}
	}

	template <class T>
	eOperandType TypeOperand_<T>::_regent_type(const eOperandType foo, const eOperandType doo) const
	{
		eOperandType regent;

		if (foo > doo)
			regent = foo;
		else
			regent = doo;
		return regent;
	}

	OperatorFactory::IOperantExeption::IOperantExeption(std::string error) : _message(error)
	{
		std::cerr<<_message<< ": "<< '\n';
	}

	std::ostream& operator<<(std::ostream& out, const eOperandType value)
	{
		static std::map<eOperandType, std::string> _strings;
		if (_strings.size() == 0)
		{
			#define INSERT_ELEMENT(p) _strings[p] = #p
				INSERT_ELEMENT(Int8);     
				INSERT_ELEMENT(Int16);     
				INSERT_ELEMENT(Int32);             
			#undef INSERT_ELEMENT
		}   
   		return out << _strings[value];
	}

	std::string get_type_name(const eOperandType value)
	{
		static std::map<eOperandType, std::string> _strings;
		if (_strings.size() == 0)
		{
			#define INSERT_ELEMENT(p) _strings[p] = #p
				INSERT_ELEMENT(Int8);     
				INSERT_ELEMENT(Int16);     
				INSERT_ELEMENT(Int32);             
			#undef INSERT_ELEMENT
		}   
   		return _strings[value];
	}
}