# Dump Files Server
Http File Server using C++ POCO Library

This project is c++ demo using poco library. It can be used to receive files send by http client with http-post method. Besides saving files to disk, a json file include other body data will be created in the same dir as well as a new record will be insert into history table in sqlite db.

## Backbroud
Serval months ago, I developed an app and released to all staffs of my company. It's worth congratulating me at first, but some strange crash errors were feedback as time goes on. What's worse, I could't reproduce them, even one case. So I must create a dump file when they crashed and send to server immediately. As a result, this is the dump files' sever. 

## How to use?
1. Clone or download source code, compile them using VS2017
2. The default server port is 9980, and the default dir is dumps. If you want to customize these settings, you can copy DumpServer.properties file to output path and change HTTPFormServer.port or HTTPFormServer.basePath to whatever you want.
3. After starting server, you can open http://localhost:9980(or the port yourself) in web browser to test it. If you post a file, the file will be saved in {basePath}/{client ipv4} where the {basePath} is the one you set in DumpServer.properties(or dumps default) and {client ipv4} is client ip that used to send file.

## Reference
Based on https://github.com/pocoproject/poco/tree/develop/Net/samples/HTTPFormServer, I split original .cpp file(the only one) into several files. And add subdir to basePath according to the client ip which send request. What's more, it's neccesary to analyze the request count based on ip in my scenario, I add sqlite component to help me output summary data quickly.
