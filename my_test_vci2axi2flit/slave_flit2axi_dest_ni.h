/**
 * @filename  dest_ni.h
 *
 * @brief     File that includes TLM model of SystemC destination 
 *            network interface (i.e. receiver or sink module) for Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __APPROX_NOC_SLAVE_FLIT2AXI_DEST_NI_H__
#define __APPROX_NOC_SLAVE_FLIT2AXI_DEST_NI_H__

//#include <map>

#include <systemc>
#include <tlm.h>
using namespace tlm;

#include "flit.h"
//#include "ni_base.h"
//#include "sc_clked_mutex.h"
//#include "approx_network_base.h"
#include "approx_ni_base.h"
//namespace approx_noc {

using namespace tlm;
using namespace sc_core;
//using namespace sc_ext;
using namespace std;

/**
 * @brief systemC-TLM destination network interface.
 *
 * @tparam      T   class for transaction type, either vci_request or vci_response
 * 
 * @warning         This module assumes that there is only one flit per clock cycle 
 *                  from the network.
 */

class slave_flit2axi_dest_ni : public sc_module, 
public approx_dest_ni_base { 

  public:

    /// Constructor
    slave_flit2axi_dest_ni( sc_time clk, int flit_width=64, sc_module_name _n = sc_gen_unique_name("approx_noc::slave_flit2axi_dest_ni"));
    /// destructor
    ~slave_flit2axi_dest_ni() {};

    sc_port<AXI_PUT_WADDR_IF>                            axi_waddr_port;
    sc_port<AXI_PUT_WDATA_IF>                            axi_wdata_port;
    sc_port<AXI_PUT_RADDR_IF>                            axi_raddr_port;

    //Derived from approx_dest_ni_base
    virtual void put(const flit &t);


	//axi_waddr get(tlm_tag< axi_waddr > *t);
	//axi_raddr get(tlm_tag< axi_raddr > *t);
	//axi_wdata get(tlm_tag< axi_wdata > *t);
   
    //Derived from abstract_noc::dest_ni_base <trans_type>
    //virtual trans_type get (tlm_tag< trans_type > *t=0);
    //virtual void complete_transaction(const trans_type &trans, unsigned int &size, sc_time &latency);
    

protected:
#if 0
// MY COMMENTS
    inline approx_network_base *get_approx_network_base() 
      { return static_cast<approx_network_base *>(this->m_nb); }
#endif
// END OF MY COMMENTS
    
    virtual void check_tail(const flit &t);

    flit head_flit;            //< head flit of a transaction
    
    //tlm_fifo < T > trans_fifo; //< transaction fifo

    // !!!!!!!!! CHANGE THE SIZE OF FIFOS !!!!!!!!!!!!!!!!
    //tlm_fifo < axi_waddr > dest_axi_waddr_fifo;
    //tlm_fifo < axi_wdata > dest_axi_wdata_fifo;
    //tlm_fifo < axi_raddr > dest_axi_raddr_fifo;
};

//} //namespace approx_noc

#include "slave_flit2axi_dest_ni.cc"

#endif //__DEST_NI_H__
