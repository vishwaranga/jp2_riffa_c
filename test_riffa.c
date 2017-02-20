#include <stdlib.h>
#include <stdio.h>
//#include "timer.h"
#include "riffa.h"

int main(int argc, char const *argv[])
{
    fpga_t * fpga;
    fpga_info_list info;
    int id = 0,chnl = 0 , word_cnt = 16;
    int option;

    unsigned int * data_buff;
    unsigned int * data_rcv_buff;

    data_buff = (int *)malloc(16*sizeof(unsigned int));
    for (int i = 0; i < 16; ++i)
    {
        data_buff[i] = i;
    }

    if (argc < 2) {
        printf("Usage: %s <option>\n", argv[0]);
        return -1;
    }
    option = atoi(argv[1]);

// ist FPGA info
     if (option == 0)
    {
        int i;

        if (fpga_list(&info) != 0) {
            printf("Error populating fpga_info_list\n");
            return -1;
        }
        printf("Number of devices: %d\n", info.num_fpgas);
        for (i = 0; i < info.num_fpgas; i++) {
            printf("%d: id:%d\n", i, info.id[i]);
            printf("%d: num_chnls:%d\n", i, info.num_chnls[i]);
            printf("%d: name:%s\n", i, info.name[i]);
            printf("%d: vendor id:%04X\n", i, info.vendor_id[i]);
            printf("%d: device id:%04X\n", i, info.device_id[i]);
        }
    }
// send data
    else if (option == 1)
    {
        // open fpga
        fpga = fpga_open(id);
        if (fpga == NULL)
        {
            printf("fpga_not_found\n");
            return -1;
        }

        data_buff = (int *)malloc(16*sizeof(unsigned int));
        for (int i = 0; i < 16; ++i)
        {
            data_buff[i] = 0x00;
        }

        // send data
        int sent_cnt;

        sent_cnt = fpga_send(fpga,chnl,data_buff,word_cnt,0,1,25000);
        printf("word_cnt sent_cnt : %d\n",sent_cnt);

        fpga_close(fpga);
        return 0;

    }

// send ones

    else if (option == 2)
    {
        // open fpga
        fpga = fpga_open(id);
        if (fpga == NULL)
        {
            printf("fpga_not_found\n");
            return -1;
        }

        data_buff = (int *)malloc(16*sizeof(unsigned int));
        for (int i = 0; i < 16; ++i)
        {
            data_buff[i] = i;
        }

        // send data
        int sent_cnt,reciev_cnt;

        sent_cnt = fpga_send(fpga,chnl,data_buff,16,0,1,25000);
        printf("word_cnt sent_cnt : %d\n",sent_cnt);

        data_rcv_buff = (int *)malloc(16*sizeof(unsigned int));
        reciev_cnt = fpga_recv(fpga,chnl,data_rcv_buff,word_cnt,25000);

        printf("reciev_cnt : %d\n",reciev_cnt);

        for (int i = 0; i < 16; ++i)
        {
            printf("%d\n",data_rcv_buff[i]);
        }

        fpga_close(fpga);
        return 0;
    }

// reset

    else if (option == 3)
    {
        // open fpga
        fpga = fpga_open(id);
        if (fpga == NULL)
        {
            printf("fpga_not_found\n");
            return -1;
        }
        // reset
        fpga_reset(fpga);
        fpga_close(fpga);
    }



    return 0;
}
