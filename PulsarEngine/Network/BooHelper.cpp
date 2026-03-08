/*
    BooHelper.cpp
    Copyright (C) 2026 SaucyCF

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <kamek.hpp>
#include <MarioKartWii/System/Identifiers.hpp>
#include <MarioKartWii/RKNet/RKNetController.hpp>
#include <MarioKartWii/RKNet/ITEM.hpp>

namespace Pulsar {
namespace Network {

// Convert base item + mode to the proper ItemId for triple items
static ItemId ConvertToFullItemId(ItemId baseItem, u8 mode) {
    if (mode == 4) {
        switch (baseItem) {
            case GREEN_SHELL: return TRIPLE_GREEN_SHELL;
            case RED_SHELL: return TRIPLE_RED_SHELL;
            case BANANA: return TRIPLE_BANANA;
            case MUSHROOM: return TRIPLE_MUSHROOM;
            default: break;
        }
    }
    return baseItem;
}

// Get the network item for a remote player
ItemId GetNetworkPlayerItem(u8 playerId) {
    if (playerId >= 12) return ITEM_NONE;
    
    RKNet::ITEMHandler* handler = RKNet::ITEMHandler::sInstance;
    if (handler == nullptr) return ITEM_NONE;
    
    // Read from the received packet
    const RKNet::ITEMPacket& packet = handler->receivedPackets[playerId];
    ItemId storedItem = (ItemId)packet.storedItem;
    u8 mode = packet.mode;
    
    // Check for no item (mode 0)
    if (mode == 0) return ITEM_NONE;
    if (storedItem >= ITEM_NONE) return ITEM_NONE;
    
    // Convert to triple if needed
    return ConvertToFullItemId(storedItem, mode);
}

} // namespace Network
} // namespace Pulsar