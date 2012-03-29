/*
 *  Copyright (c) 2010 TIMA Laboratory
 *
 *  This file is part of Rabbits.
 *
 *  Rabbits is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Rabbits is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Rabbits.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include "dummy_master.h"
#include <ctime>
#include <iostream>

//#define DEBUG_DEVICE_TIMER

#ifdef DEBUG_DEVICE_TIMER
#define DPRINTF(fmt, args...)                               \
    do { printf("sl_timer_device: " fmt , ##args); } while (0)
#define DCOUT if (1) cout
#else
#define DPRINTF(fmt, args...) do {} while(0)
#define DCOUT if (0) cout
#endif

#define TIMER_DIV 10

dummy_master::dummy_master (sc_module_name module_name) : master_device (module_name)
{

    SC_THREAD (dummy_master_thread);
    
}

dummy_master::~dummy_master ()
{
}

#if 0
void dummy_master::write (unsigned long ofs, unsigned char be, unsigned char *data, bool &bErr)
{
    uint32_t                value;

    ofs >>= 2;
    if (be & 0xF0)
    {
        ofs++;
        value = * ((uint32_t *) data + 1);
    }
    else
        value = * ((uint32_t *) data + 0);

    switch (ofs)
    {
    case TIMER_VALUE:
        DPRINTF("Unsupported write to VALUE register\n");
        break;

    case TIMER_MODE:
        DPRINTF("Mode write : %x\n", value); 
        if((m_mode ^ value) & 0x1){
            if(value & 0x1){ /* Starting */
                m_last_period = sc_time_stamp().value();
                m_value       = 0;
            }
            m_config_mod = true;
            ev_wake.notify();
        }
        m_mode = value & 0x3;
        break;

    case TIMER_PERIOD:
        m_period      = value;
        m_next_period = (sc_time_stamp().value() - m_last_period) + 
            ((uint64_t)1000000000000ULL /* 1s -> ps */ /
            (SYSTEM_CLOCK_FV / TIMER_DIV) * m_period);

        DPRINTF("TIMER_PERIOD write : %d (curr_tick: %lld next: %lld)\n",
               value, (sc_time_stamp().value() - m_last_period)/1000,
               m_next_period/1000);
        m_config_mod = true;
        ev_wake.notify();
        break;

    case TIMER_RESETIRQ:
        m_irq = 0;
        ev_irq_update.notify();
        break;

    default:
        DPRINTF ("Bad %s::%s ofs=0x%X, be=0x%X, data=0x%X-%X!\n",
                 name (), __FUNCTION__, (unsigned int) ofs, (unsigned int) be,
                 (unsigned int) *((unsigned long*)data + 0),
                 (unsigned int) *((unsigned long*)data + 1));
        exit (1);
        break;
    }
    bErr = false;
}

void dummy_master::read (unsigned long ofs, unsigned char be, unsigned char *data, bool &bErr)
{
    int             i;
    uint32_t  *val = (uint32_t *)data;

    ofs >>= 2;
    if (be & 0xF0)
    {
        ofs++;
        val++;
    }

    *val = 0;

    switch (ofs)
    {
    case TIMER_VALUE:
        *val = (sc_time_stamp().value() - m_last_period) / 1000 * 
            (SYSTEM_CLOCK_FV / TIMER_DIV) / 1000000000;
        DPRINTF("TIMER_VALUE read: %d [sc: %lld last: %lld diff: %lld]\n",
               *val, sc_time_stamp().value()/1000, m_last_period/1000,
               (sc_time_stamp().value() -  m_last_period)/1000);
        break;

    case TIMER_MODE:
        *val = m_mode;
        break;

    case TIMER_PERIOD:
        *val = m_period;
        break;

    case TIMER_RESETIRQ:
        *val = (m_irq == 1);
        break;


    default:
        printf ("Bad %s::%s ofs=0x%X, be=0x%X!\n",
                name (), __FUNCTION__, (unsigned int) ofs, (unsigned int) be);
        exit (1);
    }
    bErr = false;
}
#endif

void dummy_master::dummy_master_thread ()
{
    //unsigned long       flags;
	unsigned long dummy_addr =0;
    //while(1) {
	printf("DUMMY MASTER: dummy master up!!\n");
	while(1){
		cout<< "DUMMY MASTER: about to wait 20ns @: "<< sc_time_stamp()<<endl;
		wait(20,SC_NS);
		dummy_addr+=100;
		this->send_req (0, dummy_addr,cummy_data,1,false);		
		cout<< "DUMMY MASTER: sent write command @: "<< sc_time_stamp()<< " and addr "<< dummy_addr<<endl;
		wait(20,SC_NS);
		dummy_addr+=100;
		send_req (0, dummy_addr, cummy_data,1,false);		
		cout<< "DUMMY MASTER: sent read command @: "<< sc_time_stamp()<< " and addr "<< dummy_addr <<endl;
	}
    //    wait(ev_irq_update);

    //    irq = (m_irq == 1);
    //}
}



/*
 * Vim standard variables
 * vim:set ts=4 expandtab tw=80 cindent syntax=c:
 *
 * Emacs standard variables
 * Local Variables:
 * mode: c
 * tab-width: 4
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
