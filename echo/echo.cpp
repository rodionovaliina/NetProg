#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

int main ()
{
    struct sockaddr_in {
        short sin_family;
        unsigned short sin_port;
        struct in_addr sin_addr;
        char sin_zero[8];
    };
    struct in_addr {
        unsigned long s_addr;
    };
    sockaddr_in * addr = new (sockaddr_in); //своя адресная структура
    addr->sin_family = AF_INET; // интернет протокол IPv4
    addr->sin_port = 0; // порт 44214
    addr->sin_addr.s_addr = 0; // localhost

    sockaddr_in * s_addr = new (sockaddr_in); //адресная структура удаленного сервера
    s_addr->sin_family = AF_INET; // интернет протокол IPv4
    s_addr->sin_port = htons(7); // порт
    s_addr->sin_addr.s_addr = inet_addr("172.16.41.130"); // localhost

    char *buf = new char[512];
    string str ("Здравствуй!\n");
    int msg = str.length();
    size_t length = str.copy(buf,msg);

    int s = socket(AF_INET, SOCK_STREAM, 0); // TSP
    if (s == -1) {
        cerr << "Socket error\n";
    }

    int rc = bind(s, (const sockaddr *) addr, sizeof (sockaddr_in));
    if (rc == -1) {
        close(s);
        cerr << "Error bind socket whit local addr\n";
    }
    rc = connect(s, (const sockaddr*) s_addr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(s);
        cerr <<"Error connect socket with remote server\n";
    } 
    //TCP — настоящее соединение
    //UDP — фиктивное соединение
    rc = send(s, buf,msg,0);
    if (rc == -1) {
        close(s);
        cerr <<"Error send message\n";
    }
    cout << "We send: " << buf << endl;
    rc = recv(s, buf, 1024,0);
    if ( rc == -1) {
        close(s);
        cerr <<"Error receive answer\n";
    }
    buf[rc]='\0';
    cout << buf << endl;
    close(s);

    delete addr;
    delete s_addr;
    delete[] buf;
    return 0;
}
