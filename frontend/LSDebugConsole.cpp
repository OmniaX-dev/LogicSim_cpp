#include "LSDebugConsole.hpp"
#include "GameData.hpp"
#include "LSColors.hpp"

namespace ls
{
	LSDebugConsole::LSDebugConsole(void)
	{
		create();
	}

	void LSDebugConsole::create(void)
	{
		m_linesOnScreen = 20;
		m_textFiledHeight = 30;
		m_currentScroll = 0;
		m_cursorPosition = 0;
		m_currentCommand = "";
		m_cursorWidth = 2;
		m_cursorDelay = 20;
		m_cursorCurrent = 0;
		m_cursorState = false;
		m_cursor2ndColor = Color::Blue;
		m_currCmdHistory = -1;
		m_borderWidth = 1;
		m_anim = eLSDebugConsoleAnim::Open;
		m_ready = false;
		setAnimationLength(20);
		useGfxLayer(false);
		enableRender(false);
		setTypeName("LSDebugConsole");
		m_textCharHeight = 18;
		float height = (m_textCharHeight * m_linesOnScreen) + m_textFiledHeight, margin = 25;
		m_fullY = LSRuntimeData::instance().ls_rtdata.screenSize.y - height;
		setBounds(margin, m_fullY + height, ls_rtdata.screenSize.x - (margin * 2), height);
		setValid(true);
		setVisible(true);
		if (ONSPool::instance().hasNamespace("console"))
			return;
		setMemberNamespace("console");
		addMember("title", eNamespaceMemberType::String);
		addMember("linesOnScreen", eNamespaceMemberType::Integer);
		addMember("textFieldHeight", eNamespaceMemberType::Integer);
		addMember("cursorWidth", eNamespaceMemberType::Integer);
		addMember("cursorDelay", eNamespaceMemberType::Integer);
		addMember("textSize", eNamespaceMemberType::Integer);
		addMember("textColor", eNamespaceMemberType::Color);
	}

	void LSDebugConsole::refreshSize(void)
	{
		float height = (m_textCharHeight * m_linesOnScreen) + m_textFiledHeight, margin = 25;
		m_fullY = LSRuntimeData::instance().ls_rtdata.getScreenSize().y - height;
		setBounds(margin, m_fullY + height, ls_rtdata.getScreenSize().x - (margin * 2), height);
	}
	
	String LSDebugConsole::getMember(String name, eNamespaceMemberType& outType)
	{
		if (!hasMember(name)) return NS_MEMBER_NOT_FOUND;
		/*if (name == "title")
		{
			outType = eNamespaceMemberType::String;
			return getTitle();
		}*/
		if (name == "linesOnScreen")
		{
			outType = eNamespaceMemberType::Integer;
			return StringBuilder(m_linesOnScreen).get();
		}
		if (name == "textFieldHeight")
		{
			outType = eNamespaceMemberType::Integer;
			return StringBuilder(m_textFiledHeight).get();
		}
		if (name == "cursorWidth")
		{
			outType = eNamespaceMemberType::Integer;
			return StringBuilder(m_cursorWidth).get();
		}
		if (name == "cursorDelay")
		{
			outType = eNamespaceMemberType::Integer;
			return StringBuilder(m_cursorDelay).get();
		}
		/*if (name == "textSize")
		{
			outType = eNamespaceMemberType::Integer;
			return StringBuilder(getTheme().textCharHeight).get();
		}
		if (name == "textColor")
		{
			outType = eNamespaceMemberType::Color;
			return getTheme().textColor.toString();
		}*/
		outType = eNamespaceMemberType::Null;
		return NS_MEMBER_NOT_FOUND;
	}

	bool LSDebugConsole::setMember(String name, String value)
	{
		if (!hasMember(name)) return false;
		/*if (name == "title")
		{
			setTitle(value);
			return true;
		}*/
		if (name == "linesOnScreen" && value.isNumeric())
		{
			m_linesOnScreen = value.toInt();
			return true;
		}
		if (name == "textFieldHeight" && value.isNumeric())
		{
			m_textFiledHeight = value.toInt();
			return true;
		}
		if (name == "cursorWidth" && value.isNumeric())
		{
			m_cursorWidth = value.toInt();
			return true;
		}
		if (name == "cursorDelay" && value.isNumeric())
		{
			m_cursorDelay = value.toInt();
			return true;
		}
		/*if (name == "textSize" && value.isNumeric())
		{
			getTheme().textCharHeight = value.toInt();
			return true;
		}
		if (name == "textColor")
			return getTheme().textColor.setMember("full", value);*/
		return false;
	}

	void LSDebugConsole::onRender(LSRuntimeData& rtdata)
	{
		if (isInvalid()) return;

		if (!is_Visible() && !isAnimating()) return;

		geometryLayer().addOutlinedQuad(getBounds(), LSColors::DebugConsoleBackground, LSColors::DebugConsoleBorder, m_borderWidth);

		BitmapFont& f = textLayer().get().getBitmapFont();
		uint8 cw = f.getStringBounds("A", m_textCharHeight).x;
		uint16 cs = 0;
		if (m_currentCommand.trim().length() > 0)
			cs = f.getStringBounds(m_currentCommand, m_textCharHeight).x;
		
		geometryLayer().get().addQuad(FRect(clientX(), clientY(geth() - m_textFiledHeight), getw(), m_borderWidth), LSColors::DebugConsoleBorder);

		float yy = 0;

		for (uint32 i = m_currentScroll; i < m_linesOnScreen + m_currentScroll; i++)
		{
			if (i >= m_buffer.size()) break;
			Prompt& m_prompt = m_prompts[i];
			textLayer().renderString("[", FPoint(clientX(10), clientY(yy)), m_prompt.bracketColor, m_textCharHeight);
			textLayer().renderString(m_prompt.message, FPoint(clientX(10 + cw), clientY(yy)), m_prompt.msgColor, m_textCharHeight);
			textLayer().renderString("]", FPoint(clientX(10 + ((1 + m_prompt.message.length()) * cw)), clientY(yy)), m_prompt.bracketColor, m_textCharHeight);
			textLayer().renderString(m_prompt.indicator, FPoint(clientX(10 + ((2 + m_prompt.message.length()) * cw)), clientY(yy)), m_prompt.indColor, m_textCharHeight);
			textLayer().renderString(m_buffer[i].trim(), FPoint(clientX(10 + ((2 + m_prompt.indicator.length() + m_prompt.message.length()) * cw)), clientY(yy)), m_prompt.commandColor, m_textCharHeight);
			yy += m_textCharHeight;
		}

		textLayer().renderString(">", FPoint(clientX(14), clientY(geth() - m_textFiledHeight + 4)), LSColors::DebugConsoleText, m_textCharHeight);

		if (cs > 0)
			textLayer().renderString(m_currentCommand, FPoint(clientX(30), clientY(geth() - m_textFiledHeight + 4)), LSColors::DebugConsoleText, m_textCharHeight);

		if (m_cursorState)
			geometryLayer().get().addQuad(FRect(clientX(30 + (m_cursorPosition * cw) - 4), clientY(geth() - m_textFiledHeight + 6), m_cursorWidth, m_textFiledHeight - 14), m_cursor2ndColor);
		else
			geometryLayer().get().addQuad(FRect(clientX(30 + (m_cursorPosition * cw) - 4), clientY(geth() - m_textFiledHeight + 6), m_cursorWidth, m_textFiledHeight - 14), Color::Transparent);

		if (m_buffer.size() + 1 <= m_linesOnScreen) return;
		float diff = m_buffer.size() - m_linesOnScreen + 2;
		float sbh = geth() - m_textFiledHeight;
		float div = sbh / diff;
		geometryLayer().get().addQuad(FRect(clientX(getw()) - 7, clientY(), 5, sbh), Color(40, 40, 40, 240));
		geometryLayer().get().addQuad(FRect(clientX(getw()) - 7, clientY(div * m_currentScroll), 5, div), Color(0, 100, 255));
	}

	void LSDebugConsole::show(void)
	{
		if (m__visible) return;
		enableRender(true);
		m__visible = true;
		m_anim = eLSDebugConsoleAnim::Open;
		restartAnimation();
	}

	void LSDebugConsole::hide(void)
	{
		if (!m__visible) return;
		m__visible = false;
		m_anim = eLSDebugConsoleAnim::Close;
		restartAnimation();
	}

	void LSDebugConsole::update(LSRuntimeData& rtdata)
	{
		if (!isAnimating() && !is_Visible()) enableRender(false);
		updateAnimation(rtdata);
		if (isInvalid() || !is_Visible()) return;
		if (m_cursorCurrent++ > m_cursorDelay)
		{
			m_cursorCurrent = 0;
			m_cursorState = !m_cursorState;
		}
	}

	void LSDebugConsole::onAnimationUpdate(LSRuntimeData& rtdata)
	{
		if (isInvalid() || (!is_Visible() && !isAnimating())) return;
		if (m_anim == eLSDebugConsoleAnim::Open)
		{
			suby(geth() / getAnimationLength());
		}
		else if (m_anim == eLSDebugConsoleAnim::Close)
		{
			addy(geth() / getAnimationLength());
		}
	}

	void LSDebugConsole::initialize(void)
	{
		std::vector<Prompt> prompts = m_prompts;
		m_prompts.clear();
		m_buffer.clear();
		m_ready = true;
		for (uint16 i = 0; i < m_preBuffer.size(); i++)
			log(m_preBuffer[i], prompts[i]);
		m_preBuffer.clear();
	}

	void LSDebugConsole::pushLine(String line, Prompt prompt)
	{
		BitmapFont& font = textLayer().get().getBitmapFont();
		uint16 lw = font.getStringBounds(line, m_textCharHeight).x;
		uint16 cw = font.getStringBounds("A", m_textCharHeight).x;
		float rcw = (float)lw / (float)line.length();
		float lx = clientX(10 + ((2 + prompt.indicator.length() + prompt.message.length()) * cw));
		if (lx + lw > getx() + getw())
		{	
			float diff = (lx + lw) - (getx() + getw());
			float s1l = lw - diff;
			uint16 ci = (s1l / rcw);
			m_buffer.push_back(line.substr(0, ci));
			Prompt pr = Prompt::LineBreakPrompt;
			pr.commandColor = prompt.commandColor;
			pr.message = "";
			for (uint8 i = 0; i < prompt.message.length(); i++)
				pr.message = pr.message.add(' ');
			log(line.substr(ci), pr);
		}
		else 
			m_buffer.push_back(line.trim());
	}

	void LSDebugConsole::log(String str, Prompt prompt)
	{
		if (isInvalid()) return;
		if (str.trim() == "") return;
		CommandInterpreter& cmd = LSRuntimeData::instance().command;
		m_prompts.push_back(prompt);
		if (!isReady())
		{
			m_preBuffer.push_back(str);
			return;
		}
		if (str.trim().startsWith("/") && prompt.indicator.trim() == "/")
		{
			//m_buffer.push_back(str.trim().substr(1).trim());
			pushLine(str.trim().substr(1).trim(), prompt);
			eCommandResult ecr = cmd.run(str.trim());
			if (cmd.outputLength() > 0)
			{
				for (uint16 i = 0; i < cmd.outputLength(); i++)
					log(cmd.output()[i], cmd.outputPrompts()[i]);
			}
		}
		else
			pushLine(str.trim(), prompt);
			//m_buffer.push_back(str.trim());
		if (m_buffer.size() > m_linesOnScreen)
			m_currentScroll = (m_buffer.size() - m_linesOnScreen) + 1;
	}

	void LSDebugConsole::error(String err)
	{
		log(err, Prompt::ErrorPrompt);
	}

	void LSDebugConsole::print(String msg)
	{
		log(msg, Prompt::LogPrompt);
	}

	void LSDebugConsole::system(String msg)
	{
		log(msg, Prompt::SystemPrompt);
	}

	void LSDebugConsole::command(String com)
	{
		log(com, Prompt::CommandPrompt);
	}

	void LSDebugConsole::lineBreak(String line)
	{
		log(line, Prompt::LineBreakPrompt);
	}

	void LSDebugConsole::mouseScrolled(LSEvent& evt, LSRuntimeData& rtdata)
	{
		if (isInvalid() || !is_Visible()) return;
		int8 d = evt.mouseWheelScroll.delta;
		if (d == -1 && m_buffer.size() >= m_linesOnScreen)
		{
			m_currentScroll++;
			m_currentScroll = CAP(m_currentScroll, (m_buffer.size() - m_linesOnScreen) + 1);
		}
		else if (d == 1 && m_buffer.size() >= m_linesOnScreen && m_currentScroll > 0)
		{
			m_currentScroll--;
			m_currentScroll = ZERO(m_currentScroll);
		}
	}
	
	void LSDebugConsole::onEvent(LSEvent& evt, LSRuntimeData& rtdata)
	{
		if (isInvalid() || !is_Visible()) return;
		if (evt.type == sf::Event::TextEntered)
		{
			//Flags::set(FLG_SKIP_NEXT_EVENT_ROUND);
			char c = (char)(evt.text.unicode);
			if (c == 8 && m_currentCommand.length() > 0)
			{
				if (m_cursorPosition == m_currentCommand.length())
				{
					m_currentCommand = m_currentCommand.substr(0, m_currentCommand.length() - 1);
					m_cursorPosition--;
				}
				else if (m_cursorPosition > 0)
				{
					String s1 = m_currentCommand.substr(0, m_cursorPosition - 1);
					String s2 = m_currentCommand.substr(m_cursorPosition);
					m_currentCommand = s1.add(s2);
					m_cursorPosition--;
				}
			}
			else if (c == 13)
			{
				if (m_currentCommand.trim().startsWith("/"))
					command(m_currentCommand);
				else
					print(m_currentCommand);
				m_cmdHistory.insert(m_cmdHistory.begin(), m_currentCommand);
				m_currCmdHistory = -1;
				m_currentCommand = "";
				m_cursorPosition = 0;
			}
			else if (c >= 32 && c <= 126)
			{
				if (m_cursorPosition == m_currentCommand.length())
					m_currentCommand = m_currentCommand.add(c);
				else if (m_cursorPosition >= 0)
				{
					String s1 = m_currentCommand.substr(0, m_cursorPosition);
					String s2 = m_currentCommand.substr(m_cursorPosition);
					m_currentCommand = s1.add(c);
					m_currentCommand = m_currentCommand.add(s2);
				}
				m_cursorCurrent = 0;
				m_cursorState = true;
				m_cursorPosition++;
			}
		}
		else if (evt.type == sf::Event::KeyPressed)
		{
			//Flags::set(FLG_SKIP_NEXT_EVENT_ROUND);
			if (evt.key.code == SF_KEY(Left))
			{
				if (m_cursorPosition > 0)
					m_cursorPosition--;
			}
			else if (evt.key.code == SF_KEY(Right))
			{
				if (m_cursorPosition < m_currentCommand.length())
					m_cursorPosition++;
			}
			else if (evt.key.code == SF_KEY(Up))
			{
				if (m_cmdHistory.size() > 0 && m_currCmdHistory < (signed)m_cmdHistory.size() - 1)
				{
					m_currCmdHistory++;
					m_currentCommand = m_cmdHistory[m_currCmdHistory];
					m_cursorPosition = m_currentCommand.length();
				}
			}
			else if (evt.key.code == SF_KEY(Down))
			{
				if (m_cmdHistory.size() > 0 && m_currCmdHistory > 0)
				{
					m_currCmdHistory--;
					m_currentCommand = m_cmdHistory[m_currCmdHistory];
					m_cursorPosition = m_currentCommand.length();
				}
				else if (m_cmdHistory.size() > 0 && m_currCmdHistory == 0)
				{
					m_currCmdHistory--;
					m_currentCommand = "";
					m_cursorPosition = 0;
				}
			}
		}
	}
}