#include "RoomManager.h"

RoomManager* RoomManager::instance = nullptr;

RoomManager::RoomManager()
{

}

RoomManager::~RoomManager()
{
}

RoomManager* RoomManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new RoomManager();
	}
	return instance;
}

/*Functions responsible on creating room, will add the user to the room itself as he is the creator
* Input - user: the creator user, data: the room data
* Output - None.
*/
void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
	Room* room = new Room(data);
	room->addUser(user);
	std::pair<unsigned int, Room*> cell(data.id, room);
	this->m_rooms.insert(cell);
	this->currentId++;
}

/*Just deleting a room
* Input - id: the id to delete
*/
void RoomManager::deleteRoom(const int& id)
{
	delete this->m_rooms[id];
	this->m_rooms.erase(id);
}

/*Gets a room states
* Input - id: the room id
* Output - the status
*/
unsigned int RoomManager::getRoomState(const int& id)
{
	return this->m_rooms.find(id)->second->getData().isActive;
}

/*Gets a room states, this time roomdata, makes it more suitable here
* Input - id: the room id
* Output - the status
*/
RoomData RoomManager::getRoomData(const int& id)
{
	return this->m_rooms.find(id)->second->getData();
}

unsigned int RoomManager::getRoomIdByUser(std::string name)
{
	return 0;
}

/*Returns all the rooms by their obj
* Input: None
* Output - All the rooms
*/
std::vector<Room*> RoomManager::getRooms()
{
	std::vector<Room*> rooms;
	for (auto iter = this->m_rooms.begin(); iter != this->m_rooms.end(); iter++)
	{
		rooms.push_back(iter->second);
	}
	return rooms;
}

/*Returns all the rooms by their data
* Input: None
* Output - All the rooms
*/
std::vector<RoomData> RoomManager::getRoomsData()
{
	std::vector<RoomData> rooms;
	for (auto iter = this->m_rooms.begin(); iter != this->m_rooms.end(); iter++)
	{
		rooms.push_back(iter->second->getData());
	}
	return rooms;
}

unsigned int RoomManager::getCurrentId() const
{
	return this->currentId;
}
