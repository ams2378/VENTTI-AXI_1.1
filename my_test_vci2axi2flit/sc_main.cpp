//
// Module: dft_tb
// Description: Testbench for Discrete Fourier Transform
// Author: Georgios Faldamis
// Class: System-on-Chip Design, Spring 2012
// 

#include "systemc.h"
#include "tlm.h"
using namespace tlm;
#include "slave_device.h"
#include "master_device.h"
#include "dummy_slave.h"
#include "dummy_master.h"
#include "ni_base.h"
#include "master_vci2axi_source_ni.h"
#include "master_axi2flit_source_ni.h"
#include "slave_flit2axi_dest_ni.h"
#include "slave_vci2axi_dest_ni.h"
//#include "approx_network_base.h"
#include <ostream>
#include <ctime>

unsigned char cummy_data[4];

int sc_main(int, char **) {

//#include "network_base.h"
//#include "ni_base.h"

	cummy_data[0]='1';
	cummy_data[1]='1';
	cummy_data[2]='1';
	cummy_data[3]='1';


	//network base creation	
	//approx_network_base *nb = new approx_network_base(sc_time(100, SC_NS), 64, 1, 1, false, false, false, false, 8, 8, "approx_network_base");
	//network_base *nb = new  network_base(1, 1, false,false,false,false);	


	//VCI TO AXI INTERFACES
	//stringstream ss1;
	//ss1 << "master_vci2axi_source_ni(mstr="<< i << ",cosi=" << i << ")";
	//master_vci2axi_source_ni *s_ni = new master_vci2axi_source_ni(nb, 0, 1, sc_gen_unique_name(ss1.str().c_str())); 

	//source_ni_base *s_ni = new source_ni_base<vci_request>(1,1); 
	master_vci2axi_source_ni *s_ni = new master_vci2axi_source_ni(1,1); 

	
	//AXI TO FLIT INTERFACES
	master_axi2flit_source_ni *s_ni2 = new master_axi2flit_source_ni(sc_time(100, SC_NS));


	// FLIT TO AXI INTERFACES
	slave_flit2axi_dest_ni *d_ni2 = new slave_flit2axi_dest_ni(sc_time(100, SC_NS));

	
	// AXI TO VCI INTERFACES
	slave_vci2axi_dest_ni *d_ni = new slave_vci2axi_dest_ni(1);

		
	//slaves
	dummy_slave *slave0 = new dummy_slave("dummy_slave");

	//master
	dummy_master *master0 = new dummy_master("dummy_master");

	//nb->connect_master(int rabbit_mstr_id, sc_port<VCI_PUT_REQ_IF> &putp, sc_port<VCI_GET_RSP_IF> &getp, int src_queue_size = -1, int dest_queue_size = -1);
	//onoc->connect_master(0, master0->put_port, master0->get_port);
	//master0->put_port (*s_ni);

	//clock
	//sc_clock *clk = new sc_clock("clk",100,SC_NS,0);

	// temp channels

//    sc_port<AXI_PUT_WADDR_IF>                            axi_waddr_port;
//    sc_port<AXI_PUT_WDATA_IF>                            axi_wdata_port;
//    sc_port<AXI_PUT_RADDR_IF>                            axi_raddr_port;

	// IMPORTANT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// FIRST BIND AXI PORTS AND THEN BIND VCI2AXI TO THE MASTER
	//tlm_fifo<axi_waddr> waddr_fifo;
	//tlm_fifo<axi_wdata> wdata_fifo;
	//tlm_fifo<axi_raddr> raddr_fifo;
	//tlm_fifo<flit> flit_fifo;
	//tlm_fifo<vci_request> req_fifo;

	
	
	tlm_fifo<vci_response> rsp_fifo;
	slave0->put_port(rsp_fifo);
	//s_ni->axi_waddr_port(waddr_fifo);
	//s_ni->axi_wdata_port(wdata_fifo);
	//s_ni->axi_raddr_port(raddr_fifo);
	s_ni->axi_waddr_port(*s_ni2);
	s_ni->axi_wdata_port(*s_ni2);
	s_ni->axi_raddr_port(*s_ni2);
	s_ni2->flit_out(*d_ni2);
	d_ni2->axi_waddr_port(*d_ni);
	d_ni2->axi_wdata_port(*d_ni);
	d_ni2->axi_raddr_port(*d_ni);
	master0->get_port(rsp_fifo);
	master0->put_port(*s_ni);
	slave0->get_port(*d_ni);







  int clk_period;
  int treset;
  int tsim;

  scanf("%d", &clk_period);
  cout << "Tclk = " << clk_period<<endl;
  scanf("%d", &treset);
  cout << "Treset = " << treset << endl;
  scanf("%d", &tsim);
  cout << "Tsim = " << tsim << endl;

  sc_clock clk("clk", clk_period, SC_NS);
  sc_signal<bool> rst("rst");
  
  rst.write(false);
  sc_start(treset,SC_NS);
//  rst.write(true);
  cout << "MAIN:  Reset " << endl;

  sc_start(tsim,SC_NS);

  sc_stop();
  return 0;	
}
