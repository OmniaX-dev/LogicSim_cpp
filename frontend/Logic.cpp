#include "Logic.hpp"
#include <bits/stdc++.h>
#include "LSComponents.hpp"
#include "Runtime.hpp"

namespace ls
{
	bool LSCLogicEvaluator::eval(String exp)
	{
		exp = exp.replaceAll("!1", "0");
		exp = exp.replaceAll("!0", "1");
		exp = exp.trim();

		std::stack<bool> values;
		std::stack<eLSCLogicOp> ops;
		char c = 0;
		for (unsigned int i = 0; i < exp.length(); i++)
		{
			c = exp[i];
			if (c == ' ' || c == '#') continue;
			else if (c == '1' || c == '0') values.push(c == '1');
			else if (c == ')')
			{
				while(!ops.empty() && ops.top() != eLSCLogicOp::O_PARENTH)
				{
					bool val2 = values.top();
					values.pop();
					bool val1 = values.top();
					values.pop();
					eLSCLogicOp op = ops.top();
					ops.pop();
					values.push(applyOp(val1, val2, op));
				}
				if(!ops.empty()) ops.pop();
				if (!ops.empty())
				{
					if (ops.top() == eLSCLogicOp::NOT)
					{
						ops.pop();
						bool tmp = values.top();
						values.pop();
						values.push(!tmp);
					}
				}
			}
			else ops.push(parseOp(c));
		}
		while(!ops.empty())
		{
			bool val2 = values.top();
			values.pop();
			bool val1 = values.top();
			values.pop();
			eLSCLogicOp op = ops.top();
			ops.pop();
			values.push(applyOp(val1, val2, op));
		}
		return values.top();
	}

	bool LSCLogicEvaluator::applyOp(bool a, bool b, eLSCLogicOp op)
	{
		switch(op)
		{
			case eLSCLogicOp::AND: return (a && b);
			case eLSCLogicOp::OR: return (a || b);
			case eLSCLogicOp::NAND: return !(a && b);
			case eLSCLogicOp::NOR: return !(a || b);
			case eLSCLogicOp::XOR: return (a != b);
			case eLSCLogicOp::XNOR: return !(a != b);
			default: return false;
		}
	}

	eLSCLogicOp LSCLogicEvaluator::parseOp(char op)
	{
		if (op == '&') return eLSCLogicOp::AND;
		if (op == '+') return eLSCLogicOp::OR;
		if (op == '^') return eLSCLogicOp::XOR;
		if (op == '%') return eLSCLogicOp::NAND;
		if (op == '*') return eLSCLogicOp::NOR;
		if (op == '>') return eLSCLogicOp::XNOR;
		if (op == '!') return eLSCLogicOp::NOT;
		if (op == '(') return eLSCLogicOp::O_PARENTH;
		if (op == ')') return eLSCLogicOp::C_PARENTH;
		return eLSCLogicOp::NONE;
	}


//--------------------------------------------------------------------------------------------------------


	LSLogicFunction::LSLogicFunction(LSChip& chip, String func)
	{
		this->chip = &chip;
		function = func.toLowerCase().trim();
	}

	bool LSLogicFunction::compile(void)
	{
		exec.clear();
		int16 i = 0;
		String tmp1 = "", tmp2 = "";
		String nf = "";
		for (uint16 j = 0; j < function.length(); j++)
			if (function[j] != ' ') nf = nf.add(function[j]);
		function = nf;
		for (auto& p : chip->m_pins)
		{
			while ((i = function.indexOf(p.label.toLowerCase())) >= 0)
			{
				tmp1 = function.substr(0, i);
				tmp2 = function.substr(i + p.label.length());
				function = StringBuilder(tmp1).add(Utils::zeroPad("#", p.label.length(), '#')).add(tmp2).get();
				exec[i] = &(p.signal);
			}
		}
		return true;
	}

	bool LSLogicFunction::run(void)
	{
		for (auto& step : exec)
		{
			if (*(step.second)) function.put(step.first, '1');
			else function.put(step.first, '0');
		}
		return LSCLogicEvaluator::eval(function);
	}
}