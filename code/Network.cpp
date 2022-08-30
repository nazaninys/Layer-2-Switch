#include "Network.hpp"

Network::Network(){

}
void Network::setPortsConnections(string sID1,string sID2,string port){
    portsConnections[sID1][sID2] = port;
}
void Network::disconnect(string sID1,string sID2){
        portsConnections[sID1].erase(sID2);        
}

string Network::getPort(string sID1,string sID2){
    return portsConnections[sID1][sID2];
}

void Network::addToIDs(string id){
	numbers.insert(id);
}

void Network::setNumberOfPorts(string id,int n){
    switchPorts[id] = n;
}

bool Network::idExists(string id){
    if(numbers.find(id) != numbers.end())
		return true;
	else
		return false;
}

bool Network::fileExists(string fileName){
	if(fopen(fileName.c_str(), "r") == NULL)
        return false ;
    else
        return true;

}

bool Network::portNotExists(string port,string switchID){
    if(stoi(port) > switchPorts[switchID])
        return true;
    else
        return false;
}

bool Network::portIsBusy(string port,string switchID){ 
    map<string,string>::iterator it;

    for (it = portsConnections[switchID].begin(); it != portsConnections[switchID].end(); it++){
        if(it->second == port)
            return true;      
    } 
    return false;  
}

bool Network::switchExists(string id){ 
    if(switchPorts.find(id) != switchPorts.end())
		  return true;
    else
        return false;  
}

bool Network::systemExists(string id){
    if(idExists(id) && !switchExists(id))
        return true;
    else
        return false;
}
bool Network::systemConnected(string id){
    for(auto const &ent1 : portsConnections) {
        for(auto const &ent2 : ent1.second) {
        	 if(ent2.first == id) 
	            return true;
        }
    }
    return false;  
}

bool Network::MySwitchHasError(vector<string>tokens){
    if(idExists(tokens[2])){
        cout<<"Error : switch_number already exists"<<endl;
        return true;
    }
    else 
        return false;
}
bool Network::MySystemHasError(vector<string>tokens){
    if(idExists(tokens[1])){
        cout<<"Error : system_number already exists"<<endl;
        return true;
    }
    else
        return false;
}
bool Network::ConnectCommandHasError(vector<string>tokens){
    bool Error = false;
    if(!systemExists(tokens[1])){
        Error = true;
        cout<<"Error: system with id "<<tokens[1]<<" dosen't exist."<<endl;
    }

    if(!switchExists(tokens[2])){
        Error = true;
        cout<<"Error: switch with id "<<tokens[2]<<" doesn't exist."<<endl;
    }
  	
    if(portNotExists(tokens[3],tokens[2])){
        Error = true;
        cout<<"Error: switch doesn't have port "<<tokens[3]<<endl;
    }
    if(portIsBusy(tokens[3],tokens[2])){
        Error = true;
        cout<<"Error: port is busy."<<endl;
    }
    return Error;
}

bool Network::ConnectSwitchHasError(vector<string>tokens){
    bool Error = false;
    if(!switchExists(tokens[1])){
        Error = true;
        cout<<"Error: switch with id "<<tokens[1]<<" doesn't exist."<<endl;
    }
    if(!switchExists(tokens[2])){
        Error = true;
        cout<<"Error: switch with id "<<tokens[2]<<" doesn't exist."<<endl;
    }
    if(portNotExists(tokens[3],tokens[1])){
        Error = true;
        cout<<"Error: switch doesn't have port "<<tokens[3]<<endl;   
    }
    if(portNotExists(tokens[4],tokens[2])){
        Error = true;
        cout<<"Error: switch doesn't have port "<<tokens[4]<<endl;
    }
    if(portIsBusy(tokens[3],tokens[1])){
        Error = true;
        cout<<"Error: port "<<tokens[3]<<" is busy."<<endl;
    }
    if(portIsBusy(tokens[4],tokens[2])){
        Error = true;
        cout<<"Error: port "<<tokens[4]<<" is busy."<<endl;
    }

    return Error;
}


bool Network::SendCommandHasError(vector<string>tokens){
    bool Error = false;
    if(!systemExists(tokens[1])){
        bool Error = true;
        cout<<"Error: sender_id doesn't exist."<<endl;
    }
    if(!systemExists(tokens[2])){
        bool Error = true;
        cout<<"Error: receiver_id doesn't exist."<<endl;
    }
    if(!fileExists(tokens[3])){
        Error = true;
        cout<<"Error: file doesn't exist"<<endl;
    }
    if(!systemConnected(tokens[1])){
        Error = true;
        cout<<"Error: system "<<tokens[1] <<" is not connected."<<endl;
    }
    if(!systemConnected(tokens[2])){
        Error = true;
        cout<<"Error: system "<<tokens[2] <<" is not connected."<<endl;
    }
    return Error;
}
bool Network::ReceiveCommandHasError(vector<string>tokens){
    bool Error = false;
    if(!systemExists(tokens[1])){
        bool Error = true;
        cout<<"Error: receiver_id doesn't exist."<<endl;
    }
    if(!systemExists(tokens[2])){
        bool Error = true;
        cout<<"Error: sender_id doesn't exist."<<endl;
    }
    if(!fileExists(tokens[3])){
        Error = true;
        cout<<"Error: file doesn't exist"<<endl;
    }
    if(!systemConnected(tokens[1])){
        Error = true;
        cout<<"Error: system "<<tokens[1] <<" is not connected."<<endl;
    }
    if(!systemConnected(tokens[2])){
        Error = true;
        cout<<"Error: system "<<tokens[2] <<" is not connected."<<endl;
    }
    return Error;
}

