#pragma once

#include "Room.h"
#include <map>

class RoomManager
{
private:
	int currentId = 0;
	std::map<unsigned int, Room*> m_rooms;


	RoomManager();

	static RoomManager* instance;


public:
	static RoomManager* getInstance();

	RoomManager(RoomManager&) = delete;
	RoomManager& operator=(const RoomManager&) = delete;

	~RoomManager();


	void createRoom(const LoggedUser& user, const RoomData& data);
	void deleteRoom(const int& id);
	unsigned int getRoomState(const int& id) ;

	RoomData getRoomData(const int& id);
	unsigned int getRoomIdByUser(std::string name);

	std::vector<Room*> getRooms();
	std::vector<RoomData> getRoomsData();

	unsigned int getCurrentId() const;

	void addUserToRoom(int roomId, const LoggedUser& name);
};