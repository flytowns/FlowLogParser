# FlowLogParser
Design a flow log parser

# Assumption
- Support default log format, not custom and the only version that is supported is 2. 
- Support tcp, udp, icmp protocol. Can add more types in MapProtocolName() to support more.
- Run in C++14

# How to run
1. Use IDE to create a CMakeLists.txt in C++14 version automatically.
2. Input your absolute file path in main.cpp (LOOK_UP_TABLE_FILE_PATH & FLOW_LOG_FILE_PATH & OUTPUT_FILE_PATH)
3. Run main.cpp
4. Check output text.

# Test
I test one case which recruiter provided.

# Analysis 
Basically, the program contains three part.
1. Load lookup table from txt to local hashtable.
2. Parse Flow Log, match log with lookup table, and update counts hashtable.
3. Output counts hashtable to txt file.

The core thing is to use hashtable to store the info, I combine port and protocal in a string as the key, and the value is the tag.
Based on the file size and mapping size, the performance is good.

Thank you!
