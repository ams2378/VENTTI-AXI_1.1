/*
#include "systemc.h"
#include "tlm.h"

using namespace tlm;
using namespace sc_core;
using namespace std;

//#include <system_init.h>
#include "abstract_noc.h"

#include "dummy_slave.h"

#include <ctime>
#include <ostream>
//#include <dummy_master.h>

//using namespace noc;
*/



#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

//#include <system_init.h>
#include <systemc.h>
#include <tlm.h>
#include "abstract_noc.h"
//#include <interconnect_master.h>
//#include <network_base.h>
//#include <tvm_noc/tvm_network.h>
//#include <ideal_noc/star_network.h>
//#include <ideal_noc/cosi_network.h>
//#include <approx_noc/star_network.h>
//#include <approx_noc/cosi_network.h>
//#include <../../qemu/qemu-0.9.1/qemu_systemc.h>

//#include <qemu_wrapper.h>
#include "dummy_slave.h"
//#include <qemu_cpu_wrapper.h>
//#include <qemu_wrapper_cts.h>
////#include <interconnect.h>
//#include <sram_device.h>
//#include <dbf_device.h>
//#include <framebuffer_device.h>
//#include <timer_device.h>
//#include <tty_serial_device.h>
//#include <sem_device.h>
//#include <mem_device.h>
//#include <sl_block_device.h>
//#include <qemu_imported.h>
//#include <qemu_wrapper_cts.h>



int sc_main(int argc, char ** argv)
{
	//slaves
	dummy_slave *slave0 = new dummy_slave("dummy_slave");

	//clock
	//sc_clock *clk = new sc_clock("clk",100,SC_NS,0);

	// temp channels
	tlm_fifo<vci_request> req_fifo;
	tlm_fifo<vci_response> rsp_fifo;
	slave0->get_port(req_fifo);
	slave0->put_port(rsp_fifo);

	// network creation

/*
    is->noc_model = "tvm_noc";
    is->noc_name = "tvm_network";
    is->noc_clk_freq_mhz = 100;
    is->collect_bandwidth = false;
    is->collect_latency = false;
    is->print_trans = false;
    is->collect_power = false;
    is->flit_width = 64;
    is->src_fifo_size = 8;
    is->dest_fifo_size = 8;
    is->use_clked_mutex = false;*/
	
       // onoc = new approx_noc::star_network(is.no_cpus + 3, nslaves + 1, is.flit_width, noc_clk, is.collect_bandwidth, is.collect_latency, is.print_trans, is.collect_power, is.src_fifo_size, is.dest_fifo_size); 
	
//onoc = new approx_noc::star_network(4, 2, 6, 100, false, false, false, false, 8, 8); 
//approx_network_base(clk, flit_width, n_masters, n_slaves, sample_bandwidth, sample_latency, print_trans, sample_energy, source_queue_size, dest_queue_size, _n)

	sc_start();



}
