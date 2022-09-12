#pragma once
#include "Global.h"
#include "windows.h"
#include <vector>
#include <string>
#include "types.h"
#include <glm/glm.hpp>

namespace wireObjects {

	class wireNode;
	class wireObj;

	/*
	Each wire object can have multiple nodes.
	Nodes must authenticate with eachother and both agree they are connected to eachother.
	Each node can only have one connection.
	Nodes are bi-directional, each one can read its connected partners int value. 
	Numeric nodes can hold any intiger value, bool nodes will only hold 0 or 1.
	All nodes are both inputs and outputs, though each object can specify how its nodes are used.
	Each node has one public int called "memory" that can be read from and written to outside of the nodes exec function. 
	*/

	static enum class wireObjectName : int {
		OBJ_IntBus = 0,
		OBJ_BoolBus = 1,
		OBJ_ConstantValue = 2,
		OBJ_Explosive = 3,
		OBJ_GreaterThan = 4,
		OBJ_Lamp = 5,
		OBJ_LessThan = 6,
		OBJ_Raycast = 7,
		OBJ_Button = 8,
		OBJ_BalloonDeployer = 9,
		OBJ_ANDgate = 10,
		OBJ_ORgate = 11,
		OBJ_NOTgate = 12,
		OBJ_PositionTracker = 13,
		OBJ_RadioTx = 14,
		OBJ_RadioRx = 15,
		OBJ_Clock = 16,
		OBJ_Delay = 17,
		OBJ_Toggle = 18,
		OBJ_Spawner = 19,
		OBJ_Thruster = 20,
		OBJ_Pusher = 21,
		OBJ_Power = 22
	};

	enum class nodeType {
		NT_Bool,
		NT_Numeric,
		NT_WirelessBool,
		NT_WirelessNumeric
	};

	enum class nodeResponse {
		NR_Ok,
		NR_Busy,
		NR_TypeMissmatch,
		NR_Pebcak,
		NR_Generic,
	};

	//Each node is assigned a colour, this is the colour of each voxel on the physical wire object
	enum class nodeColour : int {
		Red = 0,		//255, 000, 000
		Green = 1,		//000, 255, 000
		Blue = 2,		//000, 000, 255
		Yellow = 3,		//255, 255, 051
		Magenta = 4,	//255, 000, 255
		Cyan = 5,		//000, 255, 255
		Black = 6,		//050, 050, 050
		White = 7		//255, 255, 255
	};

	static td::Color nodeColourActive[] = {
		{255, 000, 000, 255}, 
		{000, 255, 000, 255},
		{000, 255, 255, 255},
		{255, 255, 051, 255},
		{255, 000, 255, 255},
		{000, 255, 255, 255},
		{050, 050, 050, 255},
		{255, 255, 255, 255}
	};

	static const char* nodeColourNames[] = {
		"Red",
		"Green",
		"Blue",
		"Yellow",
		"Magenta",
		"Cyan",
		"Black",
		"White"
	};

	class wireNode {
		nodeType type;
		int value = 0;
		wireObj* parent = 0;
		wireNode* connection = 0;
		nodeColour colour;
		bool activeConnection = false;
		bool isInit = false;
		glm::vec3 relPosition = { 0, 0, 0 };

	public:
		nodeType getType() { return type; }
		nodeColour getColour() { return colour; }
		glm::vec3 getPosition() { return relPosition; }
		glm::vec3 getWorldPosition();
		int getValue() { 
			if (activeConnection) {
				return connection->value;
			}
			else {
				return 0;
			}

		}
		int getOwnValue() { return value; }
		int setValue(int val) { value = val; return val; }
		bool isConnected() { return activeConnection; }

		void dispose() {
			disconnect();
			delete(this);
		}

		bool init(wireObj* parent, nodeType type, nodeColour colour, glm::vec3 position){
			if (!isInit) {
				this->parent = parent;
				this->type = type;
				this->relPosition = position;
				this->colour = colour;
				this->isInit = true;
				return true;
			}
			return false;
		}

		nodeResponse authenticate(wireNode* client) {
			if (activeConnection) {
				return wireObjects::nodeResponse::NR_Busy;
			}
			else if (client == this) {
				return wireObjects::nodeResponse::NR_Pebcak;
			}
			else if (client->getType() != this->getType()) {
				return wireObjects::nodeResponse::NR_TypeMissmatch;
			}
			else {
				return wireObjects::nodeResponse::NR_Ok;
			}
		}

		nodeResponse confirmConnection(wireNode* client) {
			connection = client;
			activeConnection = true;
			return wireObjects::nodeResponse::NR_Ok;
		}

		void disconnect() {
			if (connection && activeConnection) {
				activeConnection = false;
				connection->disconnect();
				connection = 0;
			}
		}
	};

	nodeResponse connectNodes(wireNode* nodeA, wireNode* nodeB);
	int convFloatToInt(float input);

	class wireObj {
	public:
		const char* name;
		const char* voxPath;
		wireObjectName type;

		TDShape* housing = 0;
		bool destroyed = false;

		int memory = 0;
		std::vector<wireNode*> nodes = {};

		bool getClosestNode(glm::vec3 worldPos, wireNode** out);
		DWORD drawNodes(wireNode* selected = 0);
		virtual DWORD init(TDShape* housing, int memoryVal) = 0;	//run when the object is created
		virtual DWORD exec() = 0;									//run every frame
		virtual DWORD usrExec() = 0;								//run externally when required

		void softDispose() {
			if (destroyed) { return; }
			this->disconnectAll();
			destroyed = true;
		}

		void disconnectAll() {
			for (wireNode* cNode : nodes) {
				if (cNode && cNode->isConnected()) {
					cNode->disconnect();
				}
			}
		}

		void dispose() {
			softDispose();
			for (wireNode* cNode : nodes) {
				cNode->dispose();
			}
			delete(this); //this terrifies the suicide hater
		}
	};
}