#include "DataTypes.hpp"
#include <iostream>
#include <sstream>
#include "BitmapFont.hpp"
#include "VertexArray.hpp"
#include "StringTokens.hpp"
#include "GameData.hpp"
#include "Application.hpp"
#include "../../frontend/headers/DataFiles.hpp"
#include "../../frontend/headers/Logic.hpp"
#include "../../frontend/headers/Runtime.hpp"
#include "../../frontend/headers/LSComponents.hpp"

#define __BLOCKING__ST_HAS_NEXT(errorMessage) if (!st.hasNext()) { addOutput(errorMessage, Prompt::ErrorPrompt); return eCommandResult::Error; }

namespace Omnia
{
	Prompt Prompt::ErrorPrompt = Prompt().set("error", "> ", 		Color::DarkRed, 	Color::DarkBlue, 	Color::Red);
	Prompt Prompt::LogPrompt = Prompt().set("log", "> ", 			Color::Blue, 		Color::DarkBlue, 	Color::Aqua);
	Prompt Prompt::SystemPrompt = Prompt().set("system", "> ", 		Color::Blue, 		Color::DarkBlue, 	Color(0, 100, 255));
	Prompt Prompt::CommandPrompt = Prompt().set("#", " /", 			Color::DarkGreen,	Color::DarkRed, 	Color::Gold);
	Prompt Prompt::CmdOutPrompt = Prompt().set("-", "  > ", 		Color::DarkGray, 	Color::Transparent, Color(0, 100, 255));
	Prompt Prompt::InvisibleTextPrompt = Prompt().set("-", "  > ", 	Color::DarkGray, 	Color::Transparent, Color::Transparent);
	Prompt Prompt::LineBreakPrompt = Prompt().set(" ", "  ", 		Color::Transparent, Color::Transparent, Color::Transparent, Color::Transparent);

	CommandInterpreter::CommandInterpreter(void) {  }
	
	//TODO: add sub-commands
	eCommandResult CommandInterpreter::run(String command, bool flushOut)
	{
		GameData& gdata = GameData::instance();
		if (flushOut)
			flushOutput();
		command = command.trim();
		StringTokens st = command.splitExc('|', true);
		String arg = st.next();
//---------------------------------------------------------------------------------------------------
        /***  SET & GET COMMANDS  ***/
		if (arg == "/set" || arg == "/get")
		{
            bool set = arg == "/set";
			if (set)
			{
				__BLOCKING__ST_HAS_NEXT("No options nor parameters specified. Use /set -help to get information");
			}
			else
			{
				__BLOCKING__ST_HAS_NEXT("No options nor parameters specified. Use /get -help to get information");
			}
			while (st.hasNext())
			{
				arg = st.next();
				if (arg.contains("."))
				{
					String ns = arg.substr(0, arg.indexOf(".")).trim();
					String me = arg.substr(arg.indexOf(".") + 1).trim();
					if (!ONSPool::instance().hasNamespace(ns))
					{
						addOutput(StringBuilder("Namespace not found: ").add(ns).get(), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
					Object& obj = ONSPool::instance().getByNamespae(ns);
					if (!obj.hasMember(me))
					{
						addOutput(StringBuilder("Member not found: ").add(me).get(), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
					if (set)
					{
						__BLOCKING__ST_HAS_NEXT("Value missing");
						if (!obj.setMember(me, st.next()))
						{
							addOutput(StringBuilder("Member could not be set.").get(), Prompt::ErrorPrompt);
							return eCommandResult::Error;
						}
						addOutput("Member set correctly.", Prompt::CmdOutPrompt);
						continue;
					}
					else
					{
						eNamespaceMemberType t;
						String v = obj.getMember(me, t);
						if (v == NS_MEMBER_NOT_FOUND)
						{
							addOutput(StringBuilder("Member not found: ").add(me).get(), Prompt::ErrorPrompt);
							return eCommandResult::Error;
						}
						addOutput(StringBuilder(ns).add(".").add(me).add(" = ").add(v).get(), Prompt::CmdOutPrompt);
						continue;
					}
				}
    //---------------------------------------------------------------------------------------------------
                /***  /set & /get OPTIONS  ***/
				else if (arg.startsWith("-"))
				{
					arg = arg.substr(1).trim();
					if (arg == "help")
					{
						if (set)
							addOutput("Sets any parameter present in one of the namespaces.", Prompt::CmdOutPrompt);
						else
							addOutput("Gets any parameter present in one of the namespaces.", Prompt::CmdOutPrompt);

						addOutput("To see all available namespaces use /list namespaces.", Prompt::CmdOutPrompt);
						addOutput("To see all members of a namespace use /list members <namespace>.", Prompt::CmdOutPrompt);
						
						if (set)
							addOutput("Usage: /set <namespace>.<param> <data>", Prompt::CmdOutPrompt);
						else
							addOutput("Usage: /get <namespace>.<param> <data>", Prompt::CmdOutPrompt);
					}
					else
					{
						addOutput(String("Invalid option: -").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
					continue;
				}
				else
				{
					addOutput(StringBuilder("Unknown option: ").add(arg).get(), Prompt::ErrorPrompt);
					return eCommandResult::Error;
				}
			}
		}
//---------------------------------------------------------------------------------------------------
        /***  HELP COMMAND  ***/
		else if (arg == "/help")
		{
			addOutput(String("/set            ->  Set parameters of the engine"), Prompt::CmdOutPrompt);
			addOutput(String("/get            ->  Get parameters of the engine"), Prompt::CmdOutPrompt);
			addOutput(String("/help           ->  Get a list of all commands"), Prompt::CmdOutPrompt);
			addOutput(String("/list           ->  List Data"), Prompt::CmdOutPrompt);
			addOutput(String("SPACE"), Prompt::InvisibleTextPrompt);
			addOutput(String("/solve          ->  Solve any logical expression"), Prompt::CmdOutPrompt);
			addOutput(String("/add_component  ->  Add components to the main board"), Prompt::CmdOutPrompt);
			addOutput(String("/test_component ->  Runs a logic test on any component"), Prompt::CmdOutPrompt);
			addOutput(String("SPACE"), Prompt::InvisibleTextPrompt);
			addOutput(String("/<command> -help to get information about any command"), Prompt::CmdOutPrompt);
			addOutput(String("SPACE"), Prompt::InvisibleTextPrompt);
			addOutput(String("Press <F11>    to toggle debug console."), Prompt::CmdOutPrompt);
		}
//---------------------------------------------------------------------------------------------------
        /***  LIST COMMAND  ***/
		else if (arg == "/list")
		{
			__BLOCKING__ST_HAS_NEXT("No options nor parameters specified. Use /list -help to get information");
			bool mem = false, tab = false;
			while (st.hasNext())
			{
				arg = st.next();
				if (arg == "onsp")
				{
					std::vector<Object*> objs = ONSPool::instance().list();
					for (auto& o : objs)
					{
						if (o != nullptr)
							addOutput(StringBuilder(o->getObjectHeader()).get(), Prompt::CmdOutPrompt);
					}
				}
				else if (arg == "fonts")
				{
					for (auto& f : gdata.application->fonts().list())
						addOutput(StringBuilder(f.getName()).add(" - ID = ").add(f.getID()).get(), Prompt::CmdOutPrompt);
				}
                else if (arg == "namespaces")
                {
					std::vector<Object*> objs = ONSPool::instance().list();
					for (auto& o : objs)
					{
						if (o != nullptr)
						{
							if (mem)
							{
								Prompt pr = Prompt::CmdOutPrompt;
								pr.commandColor = Color::DarkGreen;
								addOutput(StringBuilder(o->getMemberNamespace()).get(), pr);
								run(StringBuilder("/list -tab members ").add(o->getMemberNamespace()).get(), false);
							}
							else
							{
								addOutput(StringBuilder(o->getMemberNamespace()).get(), Prompt::CmdOutPrompt);
							}
							
						}
					}
                }
                else if (arg == "members")
                {
					if (!st.hasNext())
					{
						addOutput("Namespace name missing.", Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
                    arg = st.next();
                    if (ONSPool::instance().hasNamespace(arg.trim()))
                    {
						Object& obj = ONSPool::instance().getByNamespae(arg.trim());
						for (auto& m : obj.getMemberList())
						{
							if (tab)
							{
								Prompt pr = Prompt::CmdOutPrompt;
								pr.indicator = "   | ";
								addOutput(StringBuilder("|  ").add(m.name).get(), pr);
							}
							else
								addOutput(m.name, Prompt::CmdOutPrompt);
						}
                    }
					else
					{
						addOutput(String("Unknown namespace: ").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
                }
    //---------------------------------------------------------------------------------------------------
                /***  /list OPTIONS  ***/
				else if (arg.startsWith("-"))
				{
					arg = arg.substr(1).trim();
					if (arg == "help")
					{
						addOutput("Available parameters are: ", Prompt::CmdOutPrompt);
						addOutput("fonts                ->   List Bitmap fonts loaded", Prompt::CmdOutPrompt);
                        addOutput("namespaces           ->   List all namespaces", Prompt::CmdOutPrompt);
                        addOutput("members <namespace>  ->   List members of a namespace", Prompt::CmdOutPrompt);
						addOutput("Usage: /list -option -option <param>", Prompt::CmdOutPrompt);
						addOutput("SPACE", Prompt::InvisibleTextPrompt);
						addOutput("Available options are:", Prompt::CmdOutPrompt);
						addOutput("-help   Shows information abount the command", Prompt::CmdOutPrompt);
						addOutput("-m      whene used with <namespaces> parameter, list all members of all namespaces", Prompt::CmdOutPrompt);
					}
					else if (arg == "m")
						mem = true;
					else if (arg == "tab")
						tab = true;
					else
					{
						addOutput(String("Invalid option: -").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
				}
				else
				{
					addOutput(StringBuilder("Unknown parameter: ").add(arg).get(), Prompt::ErrorPrompt);
					return eCommandResult::Error;
				}
			}
		}




//----------------------------------------<<<  LS++  >>>---------------------------------------------
		/***  TEST_COMPONENT COMMAND  ***/
		else if (arg == "/test_component")
		{
			bool o_sig = false;
			while (st.hasNext())
			{
				arg = st.next();
				if (arg.startsWith("-"))
				{
					arg = arg.substr(1).trim();
					if (arg == "help")
					{
						addOutput("Runs a logic test on the specified component.", Prompt::CmdOutPrompt);
						addOutput("Usage: /test_component <COMPONENT_ID>", Prompt::CmdOutPrompt);
						addOutput("SPACE", Prompt::InvisibleTextPrompt);
						addOutput("Available options are:", Prompt::CmdOutPrompt);
						addOutput("-help   Shows information abount the command", Prompt::CmdOutPrompt);
						addOutput("-sig    Runs the test usign the current signal of the component's pins, without running the function", Prompt::CmdOutPrompt);
					}
					else if (arg == "sig")
					{
						o_sig = true;
					}
					else
					{
						addOutput(String("Invalid option: -").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
				}
				else
				{
					if (!arg.isNumeric())
					{
						addOutput(String("Invalid component_id: ").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
					ls::LSChip& chip = ls_mainBoard.getChipByID(arg.toInt());
					if (chip.isInvalid())
					{
						addOutput(String("Invalid component."), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
					Prompt pr = Prompt::CmdOutPrompt;
					for (auto& p : chip.getPinList())
					{
						pr.commandColor = Color::Gold;
						addOutput(StringBuilder("Pin: ").add(p.label).get(), pr);
						pr.commandColor = Prompt::CmdOutPrompt.commandColor;
						addOutput(StringBuilder("|   Function: ").add(p.function).get(), pr);
						pr.commandColor = Color::Blue;
						String type = "";
						if (p.type == ls::eLSCPinType::Ground) type = "Ground";
						else if (p.type == ls::eLSCPinType::Power) type = "Power";
						else if (p.type == ls::eLSCPinType::Output) type = "Output";
						else if (p.type == ls::eLSCPinType::Input) type = "Input";
						addOutput(StringBuilder("|   Type: ").add(type).get(), pr);
						if (o_sig)
						{
							pr.commandColor = p.signal ? Color::Green : Color::Red;
							addOutput(StringBuilder("|   Status: ").add(p.signal ? "High" : "Low").get(), pr);
						}
						else
						{
							bool s = p.func->run();
							pr.commandColor = p.func->run() ? Color::Green : Color::Red;
							addOutput(StringBuilder("|   Status: ").add(s ? "High" : "Low").get(), pr);
						}
					}
				}
			}
		}
        /***  SOLVE COMMAND  ***/
		else if (arg == "/solve")
		{
			while (st.hasNext())
			{
				arg = st.next();
				if (arg.startsWith("-"))
				{
					arg = arg.substr(1).trim();
					if (arg == "help")
					{
						addOutput("Solves any logical expression: ex: /solve | 1 & (0 + 1) |", Prompt::CmdOutPrompt);
						addOutput("  Where '1' is true, '0' is false, '&' is and, '+' is or", Prompt::CmdOutPrompt);
						addOutput("Usage: /solve | <expression> |", Prompt::CmdOutPrompt);
						addOutput("SPACE", Prompt::InvisibleTextPrompt);
						addOutput("Available options are:", Prompt::CmdOutPrompt);
						addOutput("-help   Shows information abount the command", Prompt::CmdOutPrompt);
					}
					else
					{
						addOutput(String("Invalid option: -").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
				}
				else
				{
					bool res = ls::LSCLogicEvaluator::eval(arg.trim());
					addOutput((res ? "true" : "false"), Prompt::CmdOutPrompt);
				}
			}
		}
//---------------------------------------------------------------------------------------------------
        /***  ADD_COMPONENT COMMAND  ***/
		else if (arg == "/add_component")
		{
			FPoint pos(-1, -1);
			String name = "";
			int16 count = 1;
			while (st.hasNext())
			{
				arg = st.next();
				if (arg.startsWith("-"))
				{
					arg = arg.substr(1).trim();
					if (arg == "help")
					{
						addOutput("Adds a component to the main board. ex: /add_component 74LS04 x=100 y=200 count=10", Prompt::CmdOutPrompt);
						addOutput("Usage: /add_component <NAME> [x=<X_POSITION>] [y=<Y_POSITION>] [count=<QUANTITY<]", Prompt::CmdOutPrompt);
						addOutput("SPACE", Prompt::InvisibleTextPrompt);
						addOutput("Available options are:", Prompt::CmdOutPrompt);
						addOutput("-help   Shows information abount the command", Prompt::CmdOutPrompt);
					}
					else
					{
						addOutput(String("Invalid option: -").add(arg), Prompt::ErrorPrompt);
						return eCommandResult::Error;
					}
				}
				else
				{
					if (arg.toLowerCase().startsWith("x="))
					{
						arg = arg.substr(2).trim();
						pos.x = (float)arg.toInt();
					}
					else if (arg.toLowerCase().startsWith("y="))
					{
						arg = arg.substr(2).trim();
						pos.y = (float)arg.toInt();
					}
					else if (arg.toLowerCase().startsWith("count="))
					{
						arg = arg.substr(6).trim();
						count = arg.toInt();
					}
					else
					{
						name = arg;
					}
				}
			}
			if (name.trim() != "")
			{
				if (count < 1) count = 1;
				bool err = false;
				for (uint16 i = 0; i < count; i++)
				{
					ls::LSChip& chip = ls_mainBoard.addComponent(name, pos);
					if (chip.isInvalid())
					{
						addOutput("Error: Invalid component name.", Prompt::ErrorPrompt);
						err = true;
						break;
					}
				}
				if (!err)
					addOutput(StringBuilder("Added ").add(count).add((count > 1 ? " copies" : " copy of the")).add("<").add(name).add("> chip.").get(), Prompt::CmdOutPrompt);
			}
		}
//---------------------------------------------------------------------------------------------------
        else
        {
            addOutput(String("Unknown command: ").add(arg), Prompt::ErrorPrompt);
            return eCommandResult::Error;
        }
		return eCommandResult::Ok;
	}
}