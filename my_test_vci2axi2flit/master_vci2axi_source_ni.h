/**
 * @filename  master_vci2axi_source_ni.h
 *
 * @brief     File that includes trans_typeLM model of SystemC source 
 *            network interface (i.e. sender module) for Rabbits.
 *
 * @author    George Faldamis
 */

#ifndef __APPROX_NOC_MASTER_VCI2AXI_SOURCE_NI_H__
#define __APPROX_NOC_MASTER_VCI2AXI_SOURCE_NI_H__

//#include <map>

#include <systemc>
#include <tlm.h>

#include "abstract_noc.h"
//#include "flit.h"
#include "ni_base.h"
//#include "approx_ni_base.h"
//#include "sc_clked_mutex.h"
//#include "approx_network_base.h"
//#include "network_base.h"
#include <ctime>

//namespace approx_axi_noc {

using namespace tlm;
using namespace sc_core;
//using namespace sc_ext;
using namespace std;
//using namespace noc;
//using namespace abstract_noc;
//using namespace approx_noc;
				
/**
 * @brief source network interface for the approximated network model.
 *
 * @tparam      trans_type   class for transaction type, either vci_request or vci_response
 * @param[out]  flit_out    sc_port that will be connected to the router
 * 
 * @warning         trans_typehis module assumes that there is only one flit per clock cycle 
 *                  from the network.
 */

class master_vci2axi_source_ni : public sc_module,
public source_ni_base <vci_request> { 

  public:
    //sc_port< tlm_blocking_put_if < flit > > flit_out;

    SC_HAS_PROCESS(master_vci2axi_source_ni);
    /// Constructor
    //master_vci2axi_source_ni(network_base *nb, int pid, int num_out_trans=1, sc_module_name _n = sc_gen_unique_name("approx_noc::master_vci2axi_source_ni"));
	master_vci2axi_source_ni(int pid, int num_out_trans=1, sc_module_name _n = sc_gen_unique_name("approx_axi_noc::master_vci2axi_source_ni"));
        


    /// destructor
    ~master_vci2axi_source_ni() {};

    //Derived from tlm_blocking_put_if<trans_type>
    //void put(const trans_type &t);
    inline void put(const vci_request &trans); 

    sc_port<AXI_PUT_WADDR_IF>                            axi_waddr_port;
    sc_port<AXI_PUT_WDATA_IF>                            axi_wdata_port;
    sc_port<AXI_PUT_RADDR_IF>                            axi_raddr_port;

  protected:
#if 0
// MY COMMENTS
    inline network_base *get_network_base() 
      { return static_cast<network_base *>(this->m_nb); }
#endif
// END OF MY COMMENTS
    

    axi_waddr vci2axiwaddr(vci_request current_trans){
		cout<<"MASTER_SOURCE_VCI2AXI: creating waddr packet @ "<<sc_time_stamp()<<endl;

	axi_waddr waddr;
	struct AWID wawid;
	struct AWSIZE wawsize;
	struct AWBURST wawburst;

	wawid.srcid = current_trans.srcid;
	wawid.trdid = current_trans.trdid;
	wawid.pktid = current_trans.pktid;
	
	wawburst.cfixed	=  current_trans.cfixed;	
	wawburst.wrap	=  current_trans.wrap;
	wawburst.cons	=  current_trans.cons;
	wawburst.contig =  current_trans.contig;

	wawsize.clen = current_trans.clen;
	wawsize.plen = current_trans.plen;

	waddr.wawid = wawid;
	waddr.address = current_trans.address;
	waddr.awlen = (current_trans.plen) / 8;
	waddr.wawsize = wawsize;
	waddr.wawburst = wawburst;
	waddr.awlock = 0;
	waddr.awcache = 0;
	waddr.awport = 0;

	waddr.initial_address = current_trans.initial_address;
	waddr.slave_id = current_trans.slave_id;

	return waddr;	
     }

    axi_wdata vci2axiwdata(vci_request current_trans){
		cout<<"MASTER_SOURCE_VCI2AXI: creating wdata packet @ "<<sc_time_stamp()<<endl;

	axi_wdata wdata;
	struct WID wwid;

	wwid.srcid = current_trans.srcid;
	wwid.trdid = current_trans.trdid;
	wwid.pktid = current_trans.pktid;

	wdata.wwid = wwid;	
	wdata.wdata[0] = current_trans.wdata[0];
	wdata.wdata[1] = current_trans.wdata[1];
	wdata.wdata[2] = current_trans.wdata[2];
	wdata.wdata[3] = current_trans.wdata[3];
	wdata.wdata[4] = current_trans.wdata[4];
	wdata.wdata[5] = current_trans.wdata[5];
	wdata.wdata[6] = current_trans.wdata[6];
	wdata.wdata[7] = current_trans.wdata[7];
	wdata.wstrb = current_trans.be;
	wdata.wlast = current_trans.eop;

	wdata.n_bits = current_trans.n_bits;
	wdata.inj_time = current_trans.inj_time;
	
	return wdata;
    }

    axi_raddr vci2axiraddr(vci_request current_trans){
		cout<<"MASTER_SOURCE_VCI2AXI: creating raddr packet @ "<<sc_time_stamp()<<endl;

	axi_raddr raddr;
	struct ARID rarid;
	struct ARSIZE rarsize;
	struct ARBURST rarburst;

	rarid.srcid = current_trans.srcid;
	rarid.trdid = current_trans.trdid;
	rarid.pktid = current_trans.pktid;
	
	rarburst.cfixed	=  current_trans.cfixed;	
	rarburst.wrap	=  current_trans.wrap;
	rarburst.cons	=  current_trans.cons;
	rarburst.contig =  current_trans.contig;

	rarsize.clen = current_trans.clen;
	rarsize.plen = current_trans.plen;

	raddr.rarid = rarid;
	raddr.address = current_trans.address;
	raddr.arlen = (current_trans.plen) / 8;
	raddr.rarsize = rarsize;
	raddr.rarburst = rarburst;
	raddr.arlock = 0;
	raddr.arcache = 0;
	raddr.arport = 0;

	raddr.initial_address = current_trans.initial_address;
	raddr.slave_id = current_trans.slave_id;
	raddr.n_bits = current_trans.n_bits;
	raddr.inj_time = current_trans.inj_time;

	return raddr;	
   }

    //Derived from abstract_noc::source_ni_base <trans_type>
    virtual void process_main();
    //virtual void fliticize(std::vector< flit > &v, trans_type& trans);
    //virtual void start_transaction(trans_type &trans);

    //tlm_fifo < trans_type > trans_fifo;  //< transaction fifo
};

//} //namespace approx_noc
#include "master_vci2axi_source_ni.cc"

#endif //__SOURCE_NI_H__
