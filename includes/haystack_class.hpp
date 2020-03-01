#ifndef HAYSTACK_CLASS_HPP_
#define HAYSTACK_CLASS_HPP_

#include <vector>
#include "IOperand.hpp"

#ifdef __cplusplus__
	#include <cstdlib>
#else
	#include <stdlib.h>
#endif

namespace stack_ev
{

	class dy_stack
	{
		public:
			dy_stack(void) : _stack_size(0){};
			~dy_stack(void);
			void	 push(const AVMOperant::IOperand *);
			void	 pop(void);
			void	 assert_(const AVMOperant::IOperand *);
			void	 display_content(void);
			void	 display_top_hip_ascii(void);

			void	add(void);
			void	subtraction(void);
			void	multiply(void);
			void	divide(void);
			void	modulus(void);
			void 	screen_cleaner(void);
			size_t	 size(void);

		  private:
			std::vector<const AVMOperant::IOperand *> _stack;
			size_t					_stack_size;

			class SKException : public std::exception
			{
				public:
					SKException(const std::string& msg);

					~SKException();

					virtual const char* what() const throw ();
				private:
					const std::string m_msg;
			};
			
	};

} // namespace stack_ev

#endif // HAYSTACK_CLASS_HPP_