#ifndef __LOGIC_HPP__
#define __LOGIC_HPP__

#include "LSDefines.hpp"
#include <map>

namespace ls
{
	class LSCLogicEvaluator
	{
		public:
			static bool eval(String exp);

		private:
			static bool applyOp(bool a, bool b, eLSCLogicOp op);
			static eLSCLogicOp parseOp(char op);
	};

	class LSChip;
	class LSLogicFunction
	{
		public:
			LSLogicFunction(LSChip& chip, String func);
			bool compile(void);
			bool run(void);

		public:
			String function;
			LSChip* chip;
			std::map<uint16, bool*> exec;
	};
}

#endif