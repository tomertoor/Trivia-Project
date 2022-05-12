#include "RoomManager.h"

/*Functions responsible on creating room, will add the user to the room itself as he is the creator
* Input - user: the creator user, data: the room data
* Output - None.
*/
void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
	Room room(data);
	room.addUser(user);
	std::pair<unsigned int, Room> cell(data.id, room);
	this->m_rooms.insert(cell);
}

/*Just deleting a room
* Input - id: the id to delete
*/
void RoomManager::deleteRoom(const int& id)
{
	this->m_rooms.erase(id);
}

/*Gets a room states
* Input - id: the room id
* Output - the status
*/
unsigned int RoomManager::getRoomState(const int& id)
{
	return this->m_rooms.find(id)->second.getData().isActive;
}

/*Returns all the rooms by their data
* Input: None
* Output - All the rooms
*/
std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> rooms;
	for (auto iter = this->m_rooms.begin(); iter != this->m_rooms.end(); iter++)
	{
		rooms.push_back(iter->second.getData());
	}
	return rooms;
}
