
#include <iostream>
#include <cstdint>
#include <cstring>
// #include <cstddef>
#include <vector>
#include <string>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <thread>  // std::this_thread
#include <chrono>


#pragma pack(1)
typedef struct t_req
{
    uint64_t tcStart;
    int pxStart;
    // void * payload;  TODO
    // size_t size;
} t_req;

typedef struct t_resp
{
    uint64_t tcStart;
    int pxStart;
    int verdict;
} t_res;
#pragma pack(0)


std::ostream & operator << (std::ostream & out, const t_req & R)
{
    out << "Req{tc " << R.tcStart << ", px " << R.pxStart;
    // out << ", payload << R.payload << ", size " << R.size;
    out << "}";
    return out;
}
std::ostream & operator << (std::ostream & out, const t_res & R)
{
    out << "Res{tc " << R.tcStart << ", px " << R.pxStart << ", verdict " << R.verdict << "}";
    return out;
}


class Client
{
    const int PORT = 8889;
    const std::string HOSTNAME = "localhost";
    const size_t INPUT_BUFFER_SZ = 1024;
    const int m_socket;
    std::vector<char> m_v_socketBuffer;
    mutable char * m_p_socketBuffer;

    void _write(const std::string & msg) const
    {
        if (write(m_socket, msg.c_str(), msg.size()) < 0)
            throw std::runtime_error("cannot send message " + msg);
    }

    void _write(const t_req & req) const
    {
        if (write(m_socket, &req, sizeof(req)) < 0)
            throw std::runtime_error("cannot send header");
        // if (write(m_socket, req.payload, req.size)) < 0)
        //    throw std::runtime_error("cannot send payload");
    }

    t_res _recv() const
    {
        t_res resp;
        const size_t SZ_res = sizeof(decltype(resp));
        auto szLeft = SZ_res;
        ssize_t szRead, szCurrent = 0;

        do
        {
            szRead = recv(m_socket, &m_p_socketBuffer[szCurrent], szLeft, 0);
            szCurrent += szRead;
            szLeft -= szRead;
        } while (szRead and (szCurrent < SZ_res));

        memcpy(&resp, m_p_socketBuffer, SZ_res);

        if (szLeft > 0)
        {
            std::stringstream ss; ss << "transfer mismatch: "<< szRead << " read, " << SZ_res << " expected bytes";
            throw std::runtime_error(ss.str());
        }
        return resp;
    }

public:

    Client() :
        m_socket(socket(AF_INET, SOCK_STREAM, 0)),
        m_v_socketBuffer(INPUT_BUFFER_SZ, '\0'),
        m_p_socketBuffer(m_v_socketBuffer.data())
    {
        if (m_socket < 0)
            throw std::runtime_error("cannot even open a socket");

        struct hostent * server = gethostbyname(HOSTNAME.c_str());
        if (server == nullptr)
            throw std::runtime_error("no such host for " + HOSTNAME);

        // build the server's Internet address
        struct sockaddr_in serveraddr;
        bzero((char *) &serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        bcopy((char *)server->h_addr, (char *) &serveraddr.sin_addr.s_addr, server->h_length);
        serveraddr.sin_port = htons(PORT);

        // connect to the server
        const socklen_t socketLenght = sizeof(serveraddr);
        struct sockaddr * socketAddress = (struct sockaddr *) &serveraddr;

        int nAttempts = 10;
        while (nAttempts > 0)
        {
            if (connect(m_socket, socketAddress, socketLenght) < 0)
            {
                std::cerr << "cannot connect to " << HOSTNAME << ":" << PORT << "\n";
                if (--nAttempts)
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                else
                    throw std::runtime_error("maximum attempts reached");
            }
            else
            {
                nAttempts = -1;
            }
        }
        std::cout << "connected on socket " << m_socket << "\n";
    }

    virtual ~ Client()
    {
        _write("exit");
        std::cout << "exit sent\n";
        close(m_socket);
        std::cout << "socket closed\n";
    }

    int predict() const
    {
        t_req req;
        t_res res;

        const auto STR("la vispa teresa");
        _write(STR);
        res = _recv();
        std::cout << STR << " ----> " << res << "\n";

        req.tcStart = 88;
        req.pxStart = 30;
        _write(req);
        res = _recv();
        std::cout << req << " ----> " << res << "\n";

        std::cout << "enter a character to terminate call: ";
        int i;
        std::cin >> i;
    }
};



int main(int argc, char ** argv)
{
    Client cl;
    cl.predict();
}
