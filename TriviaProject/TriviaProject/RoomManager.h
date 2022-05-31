#pragma once

#include "Room.h"
#include <map>

class RoomManager
{
private:
	int currentId = 0;
	std::map<unsigned int, Room> m_rooms;
public:
	void createRoom(const LoggedUser& user, const RoomData& data);
	void deleteRoom(const int& id);
	unsigned int getRoomState(const int& id) ;
	std::vector<Room> getRooms();
	std::vector<RoomData> getRoomsData();

	unsigned int getCurrentId() const;
};