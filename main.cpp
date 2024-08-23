#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

/*
 * input your file path here.
 * use absolute path here to avoid cases like this.
 * https://stackoverflow.com/questions/29441631/cannot-open-file-with-relative-path-c-ifstream
 */
const string LOOK_UP_TABLE_FILE_PATH = "/Users/xuxingfei/code/CLionProjects/FlowLogParser/FlowLogParser/look_up_table.txt";
const string FLOW_LOG_FILE_PATH = "/Users/xuxingfei/code/CLionProjects/FlowLogParser/FlowLogParser/flow_log.txt";
const string OUTPUT_FILE_PATH = "/Users/xuxingfei/code/CLionProjects/FlowLogParser/FlowLogParser/output.txt";

class FlowLogParser {
public:
    void LoadLookUpTable(const string & lookUpFile); // load lookUpTable from text to local hashtable
    void Parse(const string & flowLogFile); // parse flow log
    void GenOutput(const string & output);

private:
    string MapProtocolName(int number);

    unordered_map<string, string> lookUpTable; // map 'dstport_protocol' to tag
    unordered_map<string, int> tagMatchCount;
    unordered_map<string, int> portProtocolMatchCount;
};

void FlowLogParser::LoadLookUpTable(const string & lookUpFile) {
    ifstream file(lookUpFile);
    if (!file.is_open()) {
        cout << "Error opening lookup table file!" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string dstPortStr, protocol, tag;

        getline(ss, dstPortStr, ',');
        getline(ss, protocol, ',');
        getline(ss, tag, ',');

        lookUpTable[dstPortStr+"_"+protocol] = tag;
    }
    file.close();
}

/*
 * Based on https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
 * Here, we just provide some common keyword such TCP, UDP, ICMP.
 * */
string FlowLogParser::MapProtocolName(int number) {
    switch(number) {
        case 1: return "icmp";
        case 6: return "tcp";
        case 17: return "udp";
        default: return "n/a";
    }
}

void FlowLogParser::Parse(const string & flowLogFile) {
    ifstream file(flowLogFile);
    if (!file.is_open()) {
        cout << "Error opening flow log file!" << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        int protocolNumber;
        string _tmp, dstPort;

        // parse flow log
        ss >> _tmp >> _tmp >> _tmp >> _tmp >> _tmp >> _tmp >> dstPort >> protocolNumber;

        // match tag
        string protocol = MapProtocolName(protocolNumber);
        string key = dstPort + "_" + protocol;

        portProtocolMatchCount[key]++;

        if (!lookUpTable.count(key)) {
            tagMatchCount["Untagged"]++;
        } else {
            string tag = lookUpTable[key];
            tagMatchCount[tag]++;
        }
    }
    file.close();
}

void FlowLogParser::GenOutput(const string & output) {
    ofstream file(output);
    file << "--------------- Tag Counts ---------------" << endl;
    file << "Tag, Count" << endl;
    for(auto e:tagMatchCount) {
        file << e.first << ", " << e.second << endl;
    }
    file << endl;

    file << "--------------- Port/Protocol Combination Counts ---------------" << endl;
    file << "Port_Protocol, Count" << endl;
    for (const auto& pair : portProtocolMatchCount) {
        file << pair.first << ", " << pair.second << endl;
    }
    file << endl;
    return;
}

int main() {
    FlowLogParser parser;

    parser.LoadLookUpTable(LOOK_UP_TABLE_FILE_PATH);
    parser.Parse(FLOW_LOG_FILE_PATH);
    parser.GenOutput(OUTPUT_FILE_PATH);

    return 0;
}
