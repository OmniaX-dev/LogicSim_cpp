#include "DataFiles.hpp"
#include "LSBoard.hpp"
#include "Logic.hpp"

namespace ls
{
	void LSComponentSheetFile::setup(std::vector<LSChip>& cat)
	{
		if (!m_chip.isValid()) return;
		std::sort(m_chip.m_pins.begin(), m_chip.m_pins.end(), LSComponentSheetFile::comparePins);
		for (auto& c : cat)
			if (c.getName() == m_chip.getName()) return;
		cat.push_back(m_chip);
		LSChip& chip = cat[cat.size() - 1];
		for (auto& p : chip.m_pins)
			p.parent = &chip;
		m_chip.invalidate();
	}

	bool LSComponentSheetFile::comparePins(LSPin p1, LSPin p2)
	{
		return p1.edgePosition < p2.edgePosition;
	}

	void LSComponentSheetFile::onCustomInstruction(String instr, DFD_LIST data, int paramCount)
	{
		if (instr == "chip")
		{
			m_chip.invalidate();
			m_chip.m_name = "";
			for (auto& dat : data)
			{
				if (dat.name == "name")
					m_chip.m_name = dat.value;
				else if (dat.name == "pincount")
					m_chip.m_pinCount = dat.asInt();
				else if (dat.name == "componentimage")
					m_chip.m_componentImage = dat.value;
				else if (dat.name == "componentid")
					m_chip.setID(dat.asInt());
			}
			if (m_chip.m_name != "" && m_chip.m_pinCount > 0) m_chip.setValid(true);
		}
		else if (instr == "pin")
		{
			LSPin pin;
			pin.invalidate();
			pin.signal = false;
			pin.label = "";
			pin.type = eLSCPinType::Input;
			pin.edge = eLSCPinEdge::Top;
			pin.edgePosition = 0;
			pin.signalType = eLSCSIgnalType::ActiveHigh;
			pin.function = "0";
			pin.func = nullptr;
			pin.parent = nullptr;
			bool ghost = false;
			for (auto& dat : data)
			{
				if (dat.name == "label")
					pin.label = dat.value;
				else if (dat.name == "type")
					pin.type = (eLSCPinType)dat.asInt();
				else if (dat.name == "edge")
					pin.edge = (eLSCPinEdge)dat.asInt();
				else if (dat.name == "pinpos")
					pin.edgePosition = dat.asInt();
				else if (dat.name == "signaltype")
					pin.signalType = (eLSCSIgnalType)dat.asInt();
				else if (dat.name == "function")
					pin.function = dat.value;
				else if (dat.name == "ghost")
					ghost = dat.asBool();
			}
			if (pin.label != "" && pin.edgePosition > 0) pin.setValid(true);
			if (ghost)
			{
				if (pin.type == eLSCPinType::Power)
					m_chip.m_pinVcc = new LSPin(pin); //TODO: Delete
				else if (pin.type == eLSCPinType::Ground)
					m_chip.m_pinGnd = new LSPin(pin); //TODO: Delete
			}
			else
			{
				if (pin.isValid() && m_chip.m_pins.size() < m_chip.m_pinCount) m_chip.m_pins.push_back(pin);
				if (pin.type == eLSCPinType::Power) m_chip.m_pinVcc = &m_chip.m_pins[m_chip.m_pins.size() - 1];
				else if (pin.type == eLSCPinType::Ground) m_chip.m_pinGnd = &m_chip.m_pins[m_chip.m_pins.size() - 1];
				if (pin.edge == eLSCPinEdge::Top) m_chip.m_nPinsTop++;
				else if (pin.edge == eLSCPinEdge::Bottom) m_chip.m_nPinsBottom++;
				else if (pin.edge == eLSCPinEdge::Left) m_chip.m_nPinsLeft++;
				else if (pin.edge == eLSCPinEdge::Right) m_chip.m_nPinsRight++;
			}
		}
	}

	void LSComponentSheetFile::onCustomAction(String action, DFD_LIST params)
	{

	}


//--------------------------------------------------------------------------------------------------------


	void LSCLibrary::onCustomInstruction(String instr, DFD_LIST data, int paramCount)
	{
		if (instr == "category")
		{
			LSCCategory cat;
			for (auto& dat : data)
			{
				if (dat.name == "name")
					cat.name = dat.value;
				else if (dat.name == "desc")
					cat.description = dat.value;
				else if (dat.name == "id")
					cat.setID(dat.asInt());
			}
			if (cat.getID() != -1 && cat.name.trim() != "") m_categories.push_back(cat);
		}
	}

	void LSCLibrary::onCustomAction(String action, DFD_LIST params)
	{
		LSRuntimeData& rtdata = *m_gdata;
		if (action == "load_component_sheet")
		{
			for (auto& p : params)
			{
				if (p.name == "file")
					m_csf.load(p.value, rtdata, *rtdata.application);
			}
		}
		else if (action == "load_component_file")
		{
			if (m_csf.isInvalid()) return;
			LSComponentSheetFile cf;
			int16 cat = -1;
			for (auto& p : params)
			{
				if (p.name == "category")
					cat = p.asInt();
				else if (p.name == "desc")
					continue; //TODO: Implement description
				else if (p.name == "file")
					cf = LSComponentSheetFile(p.value, rtdata, *rtdata.application, m_csf);
			}
			if (cf.isValid() && cat != -1)
			{
				for (auto& c : m_categories)
				{
					if (c.getID() == cat)
					{
						cf.setup(c.chips);
						break;
					}
				}
			}
		}
	}

	sh_ptr(LSChip) LSCLibrary::newChip(String name)
	{
		sh_ptr(LSChip) chip = nullptr;
		for (auto& cat : m_categories)
		{
			for (auto& c : cat.chips)
			{
				if (c.getName() == name)
				{
					chip = new_sh(LSChip)(c);
					for (auto& p : chip->m_pins)
					{
						p.func = new LSLogicFunction(*chip, p.function); //TODO: Delete all the functions -- memory leak
						p.func->compile();
					}
					return chip;
				}
			}
		}
		return nullptr;
	}
}