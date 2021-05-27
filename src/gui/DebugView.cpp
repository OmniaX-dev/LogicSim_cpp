#include "DebugView.hpp"
#include "GameData.hpp"
#include "Application.hpp"
#include "DataFile.hpp"
#include "../../frontend/headers/Runtime.hpp"

#ifdef __DEBUG__

namespace Omnia
{
	void RichString::render(GameData& gdata, Layer& layer)
	{
		if (isInvalid() || layer.isInvalid() || ptr == nullptr) return;
		layer.renderString(createString(), getPosition(), color, charHeight);
	}

	void RichString::update(GameData& gdata, BitmapFont& font)
	{
		if (isInvalid() || ptr == nullptr) return;
		UIPoint sb = font.getStringBounds(createString(), charHeight);
		setSize(sb.x, sb.y);
	}

	String RichString::createString(void)
	{
		String tmp = text;
		tmp = tmp.add(": ");
		tmp = tmp.add(ptr->toString());
		return tmp;
	}

	Tooltip::Tooltip(bool atMousePos)
	{
		m_dbgView = &(GameData::instance().debugView);
		m_bgColor = Color(0, 0, 0, 220);
		m_atMousePosition = atMousePos;
		m_visible = true;
		m_padding = I8Rect(15, 2, 5, 2);
		m_key = SF_KEY(LControl);
		m_onKeyPress = true;
		setValid(true);
	}

	void Tooltip::render(GameData& gdata)
	{
		if (!isVisible() || isInvalid()) return;
		FRect b = getBounds();
		m_dbgView->m_boxLayer.get().addQuad(FRect(b.x - 1, b.y - 1, b.w + 2, b.h + 2), Color(255, 255, 255, 130)); //TODO: Add border color
		m_dbgView->m_boxLayer.get().addQuad(b, m_bgColor);
		for (auto& rs : m_lines)
			rs.render(gdata, m_dbgView->m_textLayer);
	} 

	void Tooltip::update(GameData& gdata, FPoint position)
	{
		if (!isVisible() || isInvalid()) return;
		FRect bounds;
		FPoint size;
		float lineSpacing = 5;
		for (auto& rs : m_lines)
		{
			rs.update(gdata, m_dbgView->m_textLayer.get().getBitmapFont());
			rs.setPosition(m_padding.x + position.x, size.y + m_padding.y + position.y);
			if (rs.getw() > size.x)
				size.x = rs.getw();
			size.y += rs.geth() + lineSpacing;
		}
		size.y -= lineSpacing;
		size.x += m_padding.x + m_padding.w;
		size.y += m_padding.y + m_padding.h;
		setSize(size);
		if (position.x != -1 && position.y != -1)
			setPosition(position);
	}
	
	bool Tooltip::removePtr(uint16 index)
	{
		if (index >= m_lines.size())
			return false;
		m_lines.erase(m_lines.begin() + index);
		return true;
	}

	DebugView::DebugView(void)
	{
		create();
	}

	void DebugView::create(void)
	{
		m_nextTooltipYOffset = 0;
		m_lineSpace = 0;
		m_padding = UI16Point(15, 5);
		getTheme().setDefault();
		setName("DBG_V");
		setTypeName("DebugView");
		setTitle("Default Debug View");
		setBounds(100, 100, 400, 200);
		setValid(true);
	}
	
	void DebugView::onRender(GameData& gdata)
	{
		if (isInvalid() || m_pointers.size() == 0 || !isVisible())
			return;
		SubWindow::onRender(gdata);
		recalcBounds();
		setSize(m_textSize.x + (m_padding.x * 2), m_textSize.y + (m_padding.y * 2));
		uint16 i = 0;
		uint32 y = clientY() + m_padding.y - 2; //TODO: Hardcoded -2 due to little offset in calcualtions
		for (auto& ptr : m_pointers)
		{
			String str = ptr.text;
			if (str.trim().length() > 0)
				str = str.add(" ");
			str = str.add(ptr.ptr->toString());
			textLayer().renderString(str, FPoint(getx() + m_padding.x,  y), ptr.color, ptr.charHeight);
			y += ptr.charHeight + m_lineSpace;
			i++;
		}
	}

	void DebugView::onUpdate(GameData& gdata)
	{
		if (isInvalid() || Flags::isunset(FLG_SHOW_DEBUG_VIEW))
			return;
	}
	
	void DebugView::recalcBounds(void)
	{
		if (isInvalid() || m_textLayer.isInvalid() || m_boxLayer.isInvalid()) return;
		String tmp1 = "", tmp2 = "";
		UIPoint size1, size2;
		m_textSize = UIPoint(0, titlebarBounds().h);
		int8 tmp = 0;
		for (auto& ptr : m_pointers)
		{
			tmp1 = ptr.ptr->toString();
			tmp2 = ptr.text;
			tmp = (tmp2.trim().length() == 0 ? 0 : 1);
			size1 = m_textLayer.get().getBitmapFont().getStringBounds(tmp1, ptr.charHeight);
			size2 = m_textLayer.get().getBitmapFont().getStringBounds(tmp2, ptr.charHeight);
			if (m_textSize.x < size1.x + size2.x + tmp)
				m_textSize.x = size1.x + size2.x + tmp;
			m_textSize.y += size1.y + m_lineSpace;
		}
		m_textSize.y -= m_lineSpace;
	}

	uint16 DebugView::attachPtr(Object& variable, String text, Color color, uint16 chHeight, bool asTooltip, bool atMousePos)
	{
		if (isInvalid()) return -1;
		if (asTooltip)
		{
			Tooltip tool(atMousePos);
			tool.addPtr(variable, text, color, chHeight);
			m_tooltips.push_back(tool);
			return (uint16)(m_tooltips.size() - 1);
		}
		PtrHolder ptr;
		ptr.ptr = &variable;
		ptr.text = text;
		ptr.color = color;
		ptr.charHeight = chHeight;
		m_pointers.push_back(ptr);
		return (uint16)(m_pointers.size() - 1);
	}

	bool DebugView::removePtr(uint16 index)
	{
		if (isInvalid()) return false;
		if (index >= m_pointers.size())
			return false;
		m_pointers.erase(m_pointers.begin() + index);
		recalcBounds();
		return true;
	}

	bool DebugView::removeTooltp(uint16 index)
	{
		if (isInvalid()) return false;
		if (index >= m_tooltips.size())
			return false;
		m_tooltips.erase(m_tooltips.begin() + index);
		return true;
	}

	
	DebugConsole::DebugConsole(void)
	{
		create();
	}

	void DebugConsole::create(void)
	{
		//m_linesOnScreen = 16;
		//m_textFiledHeight = 25;
		//m_currentScroll = 0;
		//m_cursorPosition = 0;
		//m_currentCommand = "";
		//m_cursorWidth = 2;
		//m_cursorDelay = 20;
		//m_cursorCurrent = 0;
		//m_cursorState = false;
		//m_cursor2ndColor = Color::Blue;
		//m_currCmdHistory = -1;
		//setName("DBG_C");
		//setTypeName("DebugConsole");
		//setTitle("Debug Console");
		//getTheme().setDefault();
		//getTheme().backgroundColor = Color(0, 0, 0, 240);
		//getTheme().textColor = Color::Green;
		//getTheme().borderColor = Color::DarkGreen;
		//getTheme().titleBarFocusedColor = Color::DarkGreen;
		//getTheme().titleColor = Color(240, 240, 240, 255);
		//getTheme().textCharHeight = 18;
		//setBounds(10, 10, 750, (getTheme().textCharHeight * m_linesOnScreen) + getTheme().titleBarheight + (getTheme().borderWidth * 2) + m_textFiledHeight);
		setValid(false);
		/*if (ONSPool::instance().hasNamespace("console"))
			return;
		setMemberNamespace("console");
		addMember("title", eNamespaceMemberType::String);
		addMember("linesOnScreen", eNamespaceMemberType::Integer);
		addMember("textFieldHeight", eNamespaceMemberType::Integer);
		addMember("cursorWidth", eNamespaceMemberType::Integer);
		addMember("cursorDelay", eNamespaceMemberType::Integer);
		addMember("textSize", eNamespaceMemberType::Integer);
		addMember("textColor", eNamespaceMemberType::Color);*/
	}

	
	String DebugConsole::getMember(String name, eNamespaceMemberType& outType)
	{
		return NS_MEMBER_NOT_FOUND;
		if (!hasMember(name)) return NS_MEMBER_NOT_FOUND;
		if (name == "title")
		{
			outType = eNamespaceMemberType::String;
			return getTitle();
		}
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
		if (name == "textSize")
		{
			outType = eNamespaceMemberType::Integer;
			return StringBuilder(getTheme().textCharHeight).get();
		}
		if (name == "textColor")
		{
			outType = eNamespaceMemberType::Color;
			return getTheme().textColor.toString();
		}
		outType = eNamespaceMemberType::Null;
		return NS_MEMBER_NOT_FOUND;
	}

	bool DebugConsole::setMember(String name, String value)
	{
		return false;
		if (!hasMember(name)) return false;
		if (name == "title")
		{
			setTitle(value);
			return true;
		}
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
		if (name == "textSize" && value.isNumeric())
		{
			getTheme().textCharHeight = value.toInt();
			return true;
		}
		if (name == "textColor")
			return getTheme().textColor.setMember("full", value);
		return false;
	}

	void DebugConsole::onRender(GameData& gdata)
	{
		return;
		if (isInvalid() || !isVisible()) return;
		SubWindow::onRender(gdata);

		BitmapFont& f = textLayer().get().getBitmapFont();
		uint8 cw = f.getStringBounds("A", getTheme().textCharHeight).x;
		uint16 cs = 0;
		if (m_currentCommand.trim().length() > 0)
			cs = f.getStringBounds(m_currentCommand, getTheme().textCharHeight).x;
		
		boxLayer().get().addQuad(FRect(clientX(), globalY(geth() - m_textFiledHeight), getw(), getTheme().borderWidth), getTheme().borderColor);

		float yy = 0;

		for (uint32 i = m_currentScroll; i < m_linesOnScreen + m_currentScroll; i++)
		{
			if (i >= m_buffer.size()) break;
			Prompt& m_prompt = m_prompts[i];
			textLayer().renderString("[", FPoint(clientX(10), clientY(yy)), Color::Gray, getTheme().textCharHeight);
			textLayer().renderString(m_prompt.message, FPoint(clientX(10 + cw), clientY(yy)), m_prompt.msgColor, getTheme().textCharHeight);
			textLayer().renderString("]", FPoint(clientX(10 + ((1 + m_prompt.message.length()) * cw)), clientY(yy)), Color::Gray, getTheme().textCharHeight);
			textLayer().renderString(m_prompt.indicator, FPoint(clientX(10 + ((2 + m_prompt.message.length()) * cw)), clientY(yy)), m_prompt.indColor, getTheme().textCharHeight);
			textLayer().renderString(m_buffer[i].trim(), FPoint(clientX(10 + ((2 + m_prompt.indicator.length() + m_prompt.message.length()) * cw)), clientY(yy)), m_prompt.commandColor, getTheme().textCharHeight);
			yy += getTheme().textCharHeight;
		}

		textLayer().renderString(">", FPoint(clientX(10), globalY(geth() - m_textFiledHeight + 4)), getTheme().textColor, getTheme().textCharHeight);

		if (cs > 0)
			textLayer().renderString(m_currentCommand, FPoint(clientX(30), globalY(geth() - m_textFiledHeight + 4)), getTheme().textColor, getTheme().textCharHeight);

		if (m_cursorState && isFocused())
			boxLayer().get().addQuad(FRect(clientX(30 + (m_cursorPosition * cw) - 4), globalY(geth() - m_textFiledHeight + 6), m_cursorWidth, m_textFiledHeight - 10), m_cursor2ndColor);
		else if (isFocused())
			boxLayer().get().addQuad(FRect(clientX(30 + (m_cursorPosition * cw) - 4), globalY(geth() - m_textFiledHeight + 6), m_cursorWidth, m_textFiledHeight - 10), Color::Transparent);

		if (m_buffer.size() + 1 <= m_linesOnScreen) return;
		float diff = m_buffer.size() - m_linesOnScreen + 2;
		float sbh = clientBounds().h - m_textFiledHeight;
		float div = sbh / diff;
		boxLayer().get().addQuad(FRect(clientX(clientBounds().w) - 5, clientY(), 5, sbh), Color(40, 40, 40, 240));
		boxLayer().get().addQuad(FRect(clientX(clientBounds().w) - 5, clientY(div * m_currentScroll), 5, div), Color(0, 100, 255));
	}

	void DebugConsole::onUpdate(GameData& gdata)
	{
		return;
		if (isInvalid() || !isVisible()) return;
		if (m_cursorCurrent++ > m_cursorDelay)
		{
			m_cursorCurrent = 0;
			m_cursorState = !m_cursorState;
		}
	}

	void DebugConsole::log(String str, Prompt prompt)
	{
		ls_cmd.log(str, prompt);
		return;
		if (isInvalid()) return;
		if (str.trim() == "") return;
		CommandInterpreter& cmd = GameData::instance().command;
		m_prompts.push_back(prompt);
		if (str.trim().startsWith("/") && prompt.indicator.trim() == "/")
		{
			m_buffer.push_back(str.trim().substr(1).trim());
			eCommandResult ecr = cmd.run(str.trim());
			if (cmd.outputLength() > 0)
			{
				for (uint16 i = 0; i < cmd.outputLength(); i++)
					log(cmd.output()[i], cmd.outputPrompts()[i]);
			}
		}
		else
			m_buffer.push_back(str.trim());
		if (m_buffer.size() > m_linesOnScreen)
			m_currentScroll = (m_buffer.size() - m_linesOnScreen) + 1;
	}

	void DebugConsole::error(String err)
	{
		log(err, Prompt::ErrorPrompt);
	}

	void DebugConsole::print(String msg)
	{
		log(msg, Prompt::LogPrompt);
	}

	void DebugConsole::system(String msg)
	{
		log(msg, Prompt::SystemPrompt);
	}

	void DebugConsole::command(String com)
	{
		log(com, Prompt::CommandPrompt);
	}

	void DebugConsole::onMouseScrolled(sf::Event& evt, GameData& gdata)
	{
		return;
		if (isInvalid() || !isVisible() || !isFocused()) return;
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
	
	void DebugConsole::onEvent(sf::Event& evt, GameData& gdata)
	{
		return;
		if (isInvalid() || !isVisible() || !isFocused()) return;
		if (evt.type == sf::Event::TextEntered)
		{
			Flags::set(FLG_SKIP_NEXT_EVENT_ROUND);
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
			Flags::set(FLG_SKIP_NEXT_EVENT_ROUND);
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

#endif