
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

/**
    sizeof(t_req) = 8 + 4 + 8 = 20
*/
typedef struct t_req
{
    uint64_t blob_id;
    int px_start;
    size_t payld_size;
} t_req;

typedef struct t_res
{
    uint64_t blob_id;
    int px_start;
    int verdict;
} t_res;
#pragma pack(0)

static_assert(sizeof(t_req) == 20);
static_assert(sizeof(t_res) == 16);

class ReqPayload
{
public:
    t_req m_pod;

    /**b
     * Payload is NOT part of header!!! I strongly dislike N.I. dangling approach
     * TODO it's just an example, avoid spectral data copies over copies... borrow some reference or whatever
     */
    uint8_t * m_payload;

    ReqPayload()
    {
        m_pod.blob_id = 88;
        m_pod.px_start = 30;
        m_pod.payld_size = 16;
        m_payload = (uint8_t*) malloc(m_pod.payld_size);
    }

    ~ ReqPayload()
    {
        delete m_payload;
    }

    friend std::ostream & operator << (std::ostream & out, const ReqPayload & R)
    {
        out << "Req{id " << R.m_pod.blob_id << ", px " << R.m_pod.px_start;
            out << ", payload size " << R.m_pod.payld_size << " = [";
            for (auto s = 0; s < R.m_pod.payld_size; s++) out << +R.m_payload[s] << ",";
            out << "\b]";
        out << "}";
        return out;
    }
};


std::ostream & operator << (std::ostream & out, const t_res & R)
{
    out << "Res{id " << R.blob_id << ", px " << R.px_start << ", verdict " << R.verdict << "}";
    return out;
}


class Client
{
    const std::string HOSTNAME = "localhost";
    const size_t INPUT_BUFFER_SZ = 1024;
    const int m_port;
    const int m_socket;
    std::vector<char> m_v_socketBuffer;
    mutable char * m_p_socketBuffer;

    void _write(const std::string & msg) const
    {
        if (write(m_socket, msg.c_str(), msg.size()) < 0)
            throw std::runtime_error("cannot send message " + msg);
    }

    void _write(const ReqPayload & req) const
    {
        if (write(m_socket, &req.m_pod, sizeof(req.m_pod)) < 0)
            throw std::runtime_error("cannot send header");
        if (write(m_socket, req.m_payload, req.m_pod.payld_size) < 0)
            throw std::runtime_error("cannot send payload");
    }

    t_res _recv() const
    {
        t_res resp;
        const size_t SZ_RES = sizeof(decltype(resp));
        auto szLeft = SZ_RES;
        ssize_t szRead, szCurrent = 0;

        do
        {
            szRead = recv(m_socket, &m_p_socketBuffer[szCurrent], szLeft, 0);
            szCurrent += szRead;
            szLeft -= szRead;
        } while (szRead and (szCurrent < SZ_RES));

        memcpy(&resp, m_p_socketBuffer, SZ_RES);

        if (szLeft > 0)
        {
            std::stringstream ss; ss << "transfer mismatch: "<< szRead << " read, " << SZ_RES << " expected bytes";
            throw std::runtime_error(ss.str());
        }
        return resp;
    }

public:

    Client(int port) :
        m_port(port),
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
        serveraddr.sin_port = htons(m_port);

        // connect to the server
        const socklen_t socketLenght = sizeof(serveraddr);
        struct sockaddr * socketAddress = (struct sockaddr *) &serveraddr;

        int nAttempts = 10;
        while (nAttempts > 0)
        {
            if (connect(m_socket, socketAddress, socketLenght) < 0)
            {
                std::cerr << "cannot connect to " << HOSTNAME << ":" << m_port << "\n";
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
        _write("close-conn");
        std::cout << "close connection sent\n";
        close(m_socket);
        std::cout << "socket closed\n";
    }

    int predict() const
    {
        ReqPayload req;
        t_res res;

        const auto STR("la vispa teresa");
        _write(STR);
        res = _recv();
        std::cout << STR << " ----> " << res << "\n";

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
    Client cl(std::atoi(argv[1]));
    cl.predict();
}
