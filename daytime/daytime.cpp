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
    addr->sin_port = htons(44214); // порт 44214
    addr->sin_addr.s_addr = inet_addr("127.0.0.1"); // localhost

    sockaddr_in * srv_addr = new (sockaddr_in); //адресная структура удаленного сервера
    srv_addr->sin_family = AF_INET; // интернет протокол IPv4
    srv_addr->sin_port = htons(13); // порт
    srv_addr->sin_addr.s_addr = inet_addr("172.16.40.1"); // localhost

    int s = socket(AF_INET, SOCK_DGRAM, 0); // UDP
    if (s == -1) {
        cout << "Socket error\n";
        exit(1);
    }
    char *buf = new char[1024];
    string str ("Сколько времени?\n");
    int msg = str.length();
    size_t length = str.copy(buf,msg);

    int rc = bind(s, (const sockaddr *) srv_addr, sizeof (sockaddr_in));
    if (rc == -1) {
        cout << "Error\n";
        exit(1);
    }
    rc = connect(s, (const sockaddr*) srv_addr, sizeof(sockaddr_in));
    if (rc == -1) {
        close(s);
        exit(1);
    } 
    //TCP — настоящее соединение
    //UDP — фиктивное соединение
    rc = send(s, buf,msg,0);
    if (rc == -1) {
        close(s);
        exit(1);
    }
    cout << "We send: " << buf << endl;
    rc = recv(s, buf, 1024,0);
    if ( rc == -1) {
        close(s);
        exit(1);
    }
    buf[rc]='\0';
    cout << buf << endl;
    close(s);

    delete addr;
    delete srv_addr;
    delete[] buf;
    return 0;
}

