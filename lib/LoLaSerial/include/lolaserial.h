#ifndef _LOLASERIAL_H_
#define _LOLASERIAL_H_

#include "sched.h"


namespace lolaserial {


    struct proto_conf {
        int payload_bytes_len = 1; // number of bits preserved for payload size
        int crc_len = 6;
    };

    struct packet {
        uint8_t *payload;
        int len;
    };

    /*! @name Function pointer type definitions */
    typedef uint8_t (*write_fptr_t)(proto_conf *conf, uint8_t *stream, int len);

    typedef packet (*read_fptr_t)(proto_conf *conf);

    void send_blocking(proto_conf *conf);

    class Socket {
    public:
        Socket(proto_conf conf, write_fptr_t write_fptr, read_fptr_t read_fptr) :
                protocol_config{conf},
                write_fptr{write_fptr},
                read_fptr{read_fptr} {};

        ~Socket() {};

        void send();

        void read();

        const proto_conf protocol_config;
    protected:

    private:
        const write_fptr_t write_fptr;
        const read_fptr_t read_fptr;
    };
}

int main() {
    lolaserial::proto_conf test;
    lolaserial::write_fptr_t write;
    lolaserial::read_fptr_t read;
    auto raspberry_pi_spi = new lolaserial::Socket(
            lolaserial::proto_conf{},
            lolaserial::write_fptr_t{},
            lolaserial::read_fptr_t{}
    );

    raspberry_pi_spi.send("printf", "Hello World");

    return 0;
}


#endif
