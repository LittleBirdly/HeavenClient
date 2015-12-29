/////////////////////////////////////////////////////////////////////////////
// This file is part of the Journey MMORPG client                           //
// Copyright � 2015 Daniel Allendorf                                        //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Net\Packets\MovementPacket.h"
#include "Character\Inventory\Inventory.h"
#include "Gameplay\Attack.h"

namespace Net
{
	// A packet that requests the server to warp the player to a different map.
	class ChangeMapPacket83 : public OutPacket
	{
	public:
		ChangeMapPacket83(bool died, int32_t targetid, string targetp, bool usewheel) : OutPacket(CHANGEMAP)
		{
			writech(died);
			writeint(targetid);
			writestr(targetp);
			skip(1);
			writesh(usewheel ? 1 : 0);
		}
	};

	// A packet which updates the player's position with the server.
	class MovePlayerPacket83 : public MovementPacket
	{
	public:
		MovePlayerPacket83(const vector<MovementFragment>& movements) : MovementPacket(MOVE_PLAYER)
		{
			skip(9);
			writech(static_cast<int8_t>(movements.size()));
			for (auto& mv : movements)
			{
				writemovement(mv);
			}
		}
	};

	// A packet which updates the a mob's position with the server.
	class MoveMobPacket83 : public MovementPacket
	{
	public:
		MoveMobPacket83(int32_t oid, int16_t type, int8_t skillb, int8_t skill0, int8_t skill1,
			int8_t skill2, int8_t skill3, int8_t skill4, vector2d<int16_t> startpos,
			const vector<MovementFragment>& movements) : MovementPacket(MOVE_MONSTER) {

			writeint(oid);
			writesh(type);
			writech(skillb);
			writech(skill0);
			writech(skill1);
			writech(skill2);
			writech(skill3);
			writech(skill4);
			writelg(0);
			writech(0);
			writeint(0);
			writesh(startpos.x());
			writesh(startpos.y());

			writech(static_cast<int8_t>(movements.size()));
			for (auto& mv : movements)
			{
				writemovement(mv);
			}
		}
	};

	// A packet which requests that an 'USE' item is used.
	class UseItemPacket83 : public OutPacket
	{
	public:
		UseItemPacket83(int16_t slot, int32_t itemid) : OutPacket(USE_ITEM)
		{
			writeint(0);
			writesh(slot);
			writeint(itemid);
		}
	};

	class PickupItemPacket : public OutPacket
	{
	public:
		PickupItemPacket(int32_t oid, vector2d<int16_t> position) : OutPacket(PICKUP_ITEM)
		{
			writeint(0);
			writech(0);
			writepoint(position);
			writeint(oid);
		}
	};

	using Character::Inventory;
	class MoveItemPacket : public OutPacket
	{
	public:
		MoveItemPacket(Inventory::InvType type, int16_t slot, int16_t action, int16_t qty) : OutPacket(MOVE_ITEM)
		{
			writeint(0);
			writech(type);
			writesh(slot);
			writesh(action);
			writesh(qty);
		}
	};

	class EquipItemPacket : public MoveItemPacket
	{
	public:
		EquipItemPacket(int16_t src, Character::Equipslot dest) : 
			MoveItemPacket(Inventory::EQUIP, src, -dest, 1) {}
	};

	class UnequipItemPacket : public MoveItemPacket
	{
	public:
		UnequipItemPacket(int16_t src, int16_t dest) :
			MoveItemPacket(Inventory::EQUIPPED, -src, dest, 1) {}
	};

	using Gameplay::AttackResult;
	class CloseRangeAttackPacket83 : public OutPacket
	{
	public:
		CloseRangeAttackPacket83(const AttackResult& attack) : OutPacket(CLOSE_ATTACK)
		{
			skip(1);
			writech((attack.mobcount << 4) | attack.hitcount);
			writeint(attack.skill);
			if (attack.charge > 0)
				writeint(attack.charge);
			skip(8);
			writech(attack.display);
			writech(attack.direction);
			writech(attack.stance);
			skip(1);
			writech(attack.speed);
			skip(4);

			for (auto& damagetomob : attack.damagelines)
			{
				writeint(damagetomob.first);
				skip(14);

				for (auto& singledamage : damagetomob.second)
				{
					writeint(singledamage);
				}

				if (attack.skill != 5221004)
					skip(4);
			}
		}
	};
}