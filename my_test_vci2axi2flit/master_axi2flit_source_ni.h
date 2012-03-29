/**
 * @filename  master_axi2flit_source_ni.h
 *
 * @brief     File that includes trans_typeLM model of SystemC source 
 *            network interface (i.e. sender module) for Rabbits.
 *
 * @author    George Faldamis
 */

#ifndef __APPROX_NOC_MASTER_AXI2FLIT_SOURCE_NI_H__
#define __APPROX_NOC_MASTER_AXI2FLIT_SOURCE_NI_H__

//#include <map>

#include <systemc>
#include <tlm.h>
using namespace tlm;

#include <iostream>
#include "abstract_noc.h"
#include "flit.h"
//#include "ni_base.h"
#include "approx_ni_base.h"
//#include "sc_clked_mutex.h"
//#include "approx_network_base.h"
#include <ctime>

//namespace approx_axi_noc {


using namespace sc_core;
//using namespace sc_ext;
using namespace std;

/**
 * @brief source network interface for the approximated network model.
 *
 * @tparam      trans_type   class for transaction type, either vci_request or vci_response
 * @param[out]  flit_out    sc_port that will be connected to the router
 * 
 * @warning         trans_typehis module assumes that there is only one flit per clock cycle 
 *                  from the network.
 */
class master_axi2flit_source_ni : public sc_module, 
public approx_source_ni_base,
public tlm_blocking_put_if<axi_waddr>,
public tlm_blocking_put_if<axi_wdata>,
public tlm_blocking_put_if<axi_raddr> { 

  public:
    //sc_port< tlm_blocking_put_if < flit > > flit_out;

	int temp_trans_id_cnt;

    SC_HAS_PROCESS(master_axi2flit_source_ni);
#if 0
// MY COMMENTS
    /// Constructor
    master_axi2flit_source_ni(approx_network_base *nb, sc_time clk, int flit_width=64, sc_module_name _n = sc_gen_unique_name("approx_axi_noc::master_axi2flit_source_ni"));
#endif
// END OF MY COMMENTS
	master_axi2flit_source_ni( sc_time clk, int flit_width=64, sc_module_name _n = sc_gen_unique_name ("approx_axi_noc::master_axi2flit_source_ni"));
    
    /// destructor
    ~master_axi2flit_source_ni() {;};

#if 0
// MY COMMENTS

// ADDED BY ME BUT PROBABLY NOT NEEDED
    virtual axi_waddr fifo_get_waddr (tlm_tag <axi_waddr > *t=0){;};
    virtual axi_wdata fifo_get_wdata (tlm_tag <axi_wdata > *t=0){;};
    virtual axi_raddr fifo_get_raddr (tlm_tag <axi_raddr > *t=0){;};
#endif
// END OF MY COMMENTS

    //Derived from tlm_blocking_put_if<trans_type>
    //void put(const trans_type &t);
    virtual void put(const axi_waddr &trans);
    virtual void put(const axi_wdata &trans);
    virtual void put(const axi_raddr &trans);


  protected:
#if 0
// MY COMMENTS
    inline approx_network_base *get_approx_network_base() 
      { return static_cast<approx_network_base *>(this->m_nb); }
#endif
// END OF MY COMMENTS
    
    inline int get_num_bits_waddr();
    inline int get_num_bits_wdata();
    inline int get_num_bits_raddr();
    
    //Derived from abstract_noc::source_ni_base <trans_type>
    virtual void process_main();
    virtual void fliticize(std::vector< flit > &v, axi_waddr& trans);
    virtual void fliticize(std::vector< flit > &v, axi_wdata& trans);
    virtual void fliticize(std::vector< flit > &v, axi_raddr& trans);
    //virtual void start_transaction(trans_type &trans);

    //tlm_fifo < trans_type > trans_fifo;  //< transaction fifo
#if 0
// MY COMMENTS
    approx_network_base *m_nb;
#endif
// END OF MY COMMENTS
    tlm_fifo < axi_waddr > axi_waddr_fifo;
    tlm_fifo < axi_wdata > axi_wdata_fifo;
    tlm_fifo < axi_raddr > axi_raddr_fifo;
};

//} //namespace approx_noc
#include "master_axi2flit_source_ni.cc"

#endif //__SOURCE_NI_H__
