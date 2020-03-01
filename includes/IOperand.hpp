#ifndef IOPERAND_CLASS_HPP_
#define IOPERAND_CLASS_HPP_

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <limits>
//#include <variant>

// class eOperandType;

namespace AVMOperant
{	
	#ifdef SET_PRECISION
		#define FLOAT_POINT_PRESITION SET_PRECISION
	#else
		#define FLOAT_POINT_PRESITION 2
	#endif

	class OperatorFactory;
	class MxExeption;
	class IOperand;

	enum eOperandType {Int8 = 8, Int16 = 16, Int32 = 32, Float = 33, Double = 65, Null_ = 404};

	std::ostream& operator<<(std::ostream& out, const eOperandType value);
	std::string get_type_name(const eOperandType value);
	//bool operator==(const IOperand &st, const IOperand &dt);

	// union all_supported_types
	// {
	// 	int8_t i8_t;
	// 	int16_t i16_t;
	// 	int32_t i32_t;
	// 	float	f_t;
	// 	double	d_t;
	// };

	class IntSupremeValues
	{
		public:
			IntSupremeValues(void) = delete;
			IntSupremeValues(const eOperandType &e);

			int64_t getMin(void) {return _AVM_INT_MIN;}
			int64_t getMax(void) {return _AVM_INT_MAX;}

		private:
			int64_t	_AVM_INT_MAX;
			int64_t	_AVM_INT_MIN;

	};

	class FloatSupremeValues
	{
		public:
			FloatSupremeValues(void) = delete;
			FloatSupremeValues(const eOperandType &e);

			long double	 getMin(void) {return _AVM_FLOAT_MIN;}
			long double	 getMax(void) {return _AVM_FLOAT_MAX;}

		private:
			long double	_AVM_FLOAT_MAX;
			long double	_AVM_FLOAT_MIN;

	};

	template <typename T>
	bool addingIsOverflow(const T& px, const T& pv, 
						const T& _pmax = std::numeric_limits<T>::max())
	{
		return _pmax - pv < px;
	}

	template <typename T>
	bool subtrackingisUnderflow(const T& px, const T& pv, 
						const T& _pmin = std::numeric_limits<T>::min())
	{
		return _pmin + pv > px;
	}

	template <typename T>
	bool multiplyIsOverflow(const T& px, const T& pv, 
						const T& _pmax = std::numeric_limits<T>::max())
	{
		return _pmax / pv > px;
	}

	template <typename T>
	bool divideIsUnderflow(const T& px, const T& pv, 
						const T& _pmin = std::numeric_limits<T>::min())
	{
		return _pmin * pv > px;
	}

	typedef union 	u_benchFunc
	{
		int32_t		(*benchFunc)(const std::string &, size_t*, int);
		int64_t 	(*benchFuncLong)(const std::string &, size_t *, int);
		float		(*benchFuncFloat)(const std::string &, size_t*);
		double		(*benchFuncDouble)(const std::string &, size_t *);
		long double (*benchFuncLongDouble)(const std::string &, size_t *);
	}	 			t_benchFunc;

	class IOperand
	{
		public:
			virtual int				getPrecision(void) const = 0;
			virtual eOperandType	getType(void) const = 0;

			virtual IOperand const *operator+(IOperand const &rhs) const = 0;
			virtual IOperand const *operator-(IOperand const &rhs) const = 0;
			virtual IOperand const *operator*(IOperand const &rhs) const = 0;
			virtual IOperand const *operator/(IOperand const &rhs) const = 0;
			virtual IOperand const *operator%(IOperand const &rhs) const = 0;
			virtual bool			operator==(IOperand const &rhs) const = 0;

			virtual std::string const toString(void) const = 0;
			
		  virtual ~IOperand(void) {}
	};

	template <class T>
	class TypeOperand_ : public IOperand
	{
		public:
			TypeOperand_(void) = default ;
			TypeOperand_(eOperandType type, T value) : _value(value), _operand_type(type) {}
			TypeOperand_(eOperandType type, const std::string &value, const t_benchFunc &func)
			{
				if (type == Float)
				{
					long double pre_value = func.benchFuncLongDouble(value, nullptr);
					
					if (pre_value > std::numeric_limits<float>::max())
					{
						_value = 0;
						_operand_type = Null_;

						#ifdef DEVELOPER_MODE
							std::cout<<type<<" PURE VALUE: "<<value<<" CONVERCED VALUE: "<<pre_value<<std::endl; 
						#endif

						MxExeption exec;
						exec.setFloatOverFlow();
						throw(exec);
					}
					else if (pre_value < std::numeric_limits<float>::lowest())
					{
						_value = 0;
						_operand_type = Null_;

						#ifdef DEVELOPER_MODE
							std::cout<<type<<" PURE VALUE: "<<value<<" CONVERCED VALUE: "<<pre_value<<std::endl; 
						#endif
						
						MxExeption exec;
						exec.setFloatUnderFlow();
						throw(exec);
					}
					else
					{
						_value = pre_value;
						_operand_type = type;
					}
				}
				else if (type == Double)
				{
					long double pre_value = func.benchFuncLongDouble(value, nullptr); 

					if (pre_value > std::numeric_limits<double>::max())
					{
						_value = 0;
						_operand_type = Null_;

						#ifdef DEVELOPER_MODE
							std::cout<<type<<" PURE VALUE: "<<value<<" CONVERCED VALUE: "<<pre_value<<std::endl; 
						#endif

						MxExeption exec;
						exec.setDoubleOverFlow();
						throw(exec);
					}
					else if (pre_value < std::numeric_limits<double>::lowest())
					{
						_value = 0;
						_operand_type = Null_;

						#ifdef DEVELOPER_MODE
							std::cout<<type<<" PURE VALUE: "<<value<<" CONVERCED VALUE: "<<pre_value<<std::endl; 
						#endif
						
						MxExeption exec;
						exec.setDoubleUnderFlow();
						throw(exec);
					}
					else
					{
						_value = pre_value;
						_operand_type = type;
					}
				}
				else
				{
					IntSupremeValues	limits(type);
					int64_t				pre_value = func.benchFunc(value, nullptr, 10);
					if (pre_value > limits.getMax())
					{
						_value = 0;
						_operand_type = Null_;

						#ifdef DEVELOPER_MODE
							std::cout<<type<<" PURE VALUE: "<<value<<" CONVERCED VALUE:"<<pre_value<<std::endl; 
						#endif

						MxExeption exec;
						exec.setOverflow();
						throw(exec);
					}
					else if (pre_value < limits.getMin())
					{
						_value = 0;
						_operand_type = Null_;

						#ifdef DEVELOPER_MODE
							std::cout<<type<<" PURE VALUE: "<<value<<" CONVERCED VALUE:"<<pre_value<<std::endl; 
						#endif
						
						MxExeption exec;
						exec.setOverflow();
						throw(exec);
					}
					else
					{
						_value = pre_value;
						_operand_type = type;
					}
				}
			}
		
			//TypeOperand_(eOperandType type, std::string const &value, const t_benchFunc &);

			inline eOperandType	getType(void) const	{ return _operand_type; }
			int					getPrecision(void) const;
			T					get_value(void) const { return _value; }

			std::string const  toString(void) const;
			IOperand const		*operator+(IOperand const &rhs) const;
			IOperand const		*operator-(IOperand const &rhs) const;
			IOperand const		*operator*(IOperand const &rhs) const;
			IOperand const		*operator/(IOperand const &rhs) const;
			IOperand const		*operator%(IOperand const &rhs) const;
			bool 				operator==(IOperand const &rhs) const;

			class MxExeption : public std::exception
			{
				public:

				MxExeption(void): _message(nullptr){};
			    MxExeption(std::string error) : _message(new std::string(error)){};

				  virtual const char *what() const throw()
				  {
					  return _message->c_str();
				  }

				  std::string GetErrorMessage(void) const throw()
				  {
					  return _message;
					}

					void setOutofRange()
					{
						_message = &out_of_range;
					}

					void setOverflow()
					{
						_message = &int_overflow;
					}

					void setUnderflow()
					{
						_message = &int_underflow;
					}


					void setOverflow8()
					{
						_message = &int8_overflow;
					}

					void setUnderflow8()
					{
						_message = &int8_underflow;
					}

					void setOverflow16()
					{
						_message = &int16_overflow;
					}

					void setUnderflow16()
					{
						_message = &int16_underflow;
					}
	
					void setOverflow32()
					{
						_message = &int32_overflow;
					}

					void setUnderflow32()
					{
						_message = &int32_underflow;
					}

					void setDividebyZero()
					{
						_message = &divide_by_zero;
					}

					void setModulusbyZero()
					{
						_message = &remainder_by_zero;
					}

					void setFloatOverFlow()
					{
						_message = &float_overflow;
					}

					void setFloatUnderFlow()
					{
						_message = &float_underflow;
					}

					void setDoubleOverFlow()
					{
						_message = &double_overflow;
					}

					void setDoubleUnderFlow()
					{
						_message = &double_underflow;
					}

					void setFloatInvalidOperation()
					{
						_message = &floatInvalidOperation;
					}

				  private:
					std::string const*	_message;

				//inline static const struct errors_description
				//{
						inline static const std::string out_of_range = "Involved IOperand Exception: ___out_of_range";
						inline static const std::string divide_by_zero = "Involved IOperand Exception: ___division_by_zero";
						inline static const std::string remainder_by_zero = "Involved IOperand Exception: ___remainder_divisor_equal_zero";
						inline static const std::string int_overflow = "Involved IOperand Exception: ___int_t_overflow_";
						inline static const std::string int_underflow = "Involved IOperand Exception: ___int_t_underflow_";
						inline static const std::string int32_overflow = "Involved IOperand Exception: ___int32_overflow_";
						inline static const std::string int32_underflow = "Involved IOperand Exception: ___int32_underflow_";
						inline static const std::string int16_overflow = "Involved IOperand Exception: ___int16_overflow_";
						inline static const std::string int16_underflow = "Involved IOperand Exception: ___int16_underflow_";
						inline static const std::string int8_overflow = "Involved IOperand Exception: ___int8_overflow_";
						inline static const std::string int8_underflow = "Involved IOperand Exception: ___int8_underflow_";
						inline static const std::string float_overflow = "Involved IOperand Exception: ___float_overflow_";
						inline static const std::string float_underflow = "Involved IOperand Exception: ___float_underflow_";
						inline static const std::string double_overflow = "Involved IOperand Exception: ___double_overflow_";
						inline static const std::string double_underflow = "Involved IOperand Exception: ___double_underflow_";
						inline static const std::string floatInvalidOperation = "Involved IOperand Exception: ___invalid operands of types 'float' and 'double' to binary 'operator%'_";
				//		errors_description(void) : out_of_range("Involved IOperand Exception: ___out_of_range"),
				//								   divide_by_zero("Involved IOperand Exception: ___divide_by_zero"),
				//								   remainder_by_zero("Involved IOperand Exception: ___remainder_divisor_equal_zero"),
				//								   int_overflow("Involved IOperand Exception: ___int_overflow_"),
				//								   int_underflow("Involved IOperand Exception: ___int_underflow_"){}
				//} Errors;
			};

			~TypeOperand_(void)
			{
				#ifdef DEVELOPER_MODE
					std::cout<<"___TypeOperand Destrutor is called"<<std::endl; 
				#endif
			}

		  private:
			T				_value;
			eOperandType	_operand_type;
			eOperandType	_regent_type(const eOperandType foo, const eOperandType doo) const;
	};

	class OperatorFactory
	{
		public:
				IOperand const *createOperand(eOperandType type, std::string const & value ) const;
		private:
				IOperand const *createInt8(std::string const & value) const;
				IOperand const *createInt16( std::string const & value ) const;
				IOperand const *createInt32( std::string const & value ) const;
				IOperand const *createFloat( std::string const & value ) const;
				IOperand const *createDouble( std::string const & value ) const;


				class IOperantExeption : public std::exception
				{
					private:
						std::string _message;

					public:
						IOperantExeption(std::string error);
				};
	};
}

#endif // IOPERAND_CLASS_HPP_