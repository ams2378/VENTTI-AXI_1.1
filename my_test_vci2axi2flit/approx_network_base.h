/**
 * @filename approx_network_base.h
 *
 * @brief     File that includes the base network declaration of Rabbits.
 *
 * @author Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __APPROX_NOC_APPROX_NETWORK_BASE_H__
#define __APPROX_NOC_APPROX_NETWORK_BASE_H__

#include <map>

#include <systemc>
#include <tlm.h>

#include "abstract_noc.h"
#include "network_base.h"

//namespace approx_noc {

//class approx_ni_base;
//class approx_source_ni_base;
//class approx_dest_ni_base;

//template<typename trans_type> class source_ni;
//template<typename trans_type> class dest_ni;

using namespace tlm;
using namespace sc_core;
using namespace std;

/**
 * @brief SystemC-TLM network base class. This abstract class
 * can be derived into another class to instantiate and connect
 * routers inside the network. To see the example, please refer
 * star_network class
 *
 * @sa star_network.h
 * @sa star_network.cpp
 */
class approx_network_base : public sc_module, 
    public network_base { 

  public:
    
    //From abstract_noc::network_base
    //virtual void connect_master(int rabbit_mstr_id, sc_port<VCI_PUT_REQ_IF> &putp, sc_port<VCI_GET_RSP_IF> &getp, int src_queue_size = -1, int dest_queue_size = -1);
    //virtual void connect_slave(int rabbit_slave_id, sc_port<VCI_GET_REQ_IF> &getp, sc_port<VCI_PUT_RSP_IF> &putp, int src_queue_size = -1, int dest_queue_size = -1);

    
    //inline functions for transaction map access
    /**
     * set a vci_request transaction into the request transaction map
     * @param[in] master_id rabbit identifier for a master
     * @param[in] req       request transaction to be saved into the map
     * @return              location where req is saved in the map
     */
    inline int set_req_trans(uint16_t master_id, vci_request &req) {
        map<int, vci_request> &src = m_req_trans_map[master_id];
        int i;
        for(i=0; i<src.size(); i++) {
            if(src.find(i) == src.end()) {
                src[i] = req;
                return i;
            }
        }
        src[i] = req;
        assert(src.size() == (i+1));
        return i;
     }
    
    /**
     * set a vci_response transaction into the response transaction map
     * @param[in] slave_id rabbit identifier for a slave
     * @param[in] res       response transaction to be saved into the map
     * @return              location where res is saved in the map
     */
    inline int set_res_trans(uint16_t slave_id, vci_response &res) {
        map<int, vci_response> &src = m_res_trans_map[slave_id];
        int i;
        for(i=0; i<src.size(); i++) {
            if(src.find(i) == src.end()) {
                src[i] = res;
                return i;
            }
        }
        src[i] = res;
        assert(src.size() == (i+1));
        return i;
    }
    
    /**
     * get a vci_request transaction from the request transaction map
     * @param[in] master_id rabbit identifier for a master
     * @param[in] trans_id  location where the vci_request object is saved in the map
     * @return              the requested vci_request object
     */
    inline vci_request &get_req_trans(uint16_t master_id, int trans_id) 
        { return m_req_trans_map[master_id].find(trans_id)->second; }
    
    /**
     * get a vci_response transaction from the response transaction map
     * @param[in] slave_id  rabbit identifier for a slave
     * @param[in] trans_id  location where the vci_response object is saved in the map
     * @return              the requested vci_response object
     */
    inline vci_response &get_res_trans(uint16_t slave_id, int trans_id) 
        { return m_res_trans_map[slave_id].find(trans_id)->second; }
    
    /**
     * remove a vci_request transaction from the request transaction map
     * @param[in] master_id  rabbit identifier for a master
     * @param[in] trans_id   location where the vci_request object is saved in the map
     */
    inline void erase_req_trans(uint16_t master_id, int trans_id)
        { m_req_trans_map[master_id].erase(trans_id); }
    /**
     * remove a vci_response transaction from the response transaction map
     * @param[in] slave_id  rabbit identifier for a slave
     * @param[in] trans_id  location where the vci_response object is saved in the map
     */
    inline void erase_res_trans(uint16_t slave_id, int trans_id)
        { m_res_trans_map[slave_id].erase(trans_id); }
 
    virtual void create_network()=0;
  
  protected:
    
    /// Constructor : should not be called directly ==> protected
    approx_network_base(sc_time clk, int flit_width, int n_masters, int n_slaves, bool sample_bandwidth=false, bool sample_latency=false, bool print_trans=false, bool sample_energy=false, int src_queue_size=8, int dest_queue_size=8, sc_module_name _n = sc_gen_unique_name("approx_noc::approx_network_base"));
    /// destructor
    ~approx_network_base();

    sc_time m_clk;                  //< clock 
    int m_flit_width;               //< flit width of the network

    //std::map<unsigned int, source_ni_base *> m_source_ni; //< collection of source network interface
    //std::map<unsigned int, dest_ni_base *> m_dest_ni;     //< collection of destination network interface

    vector< map<int, vci_request> > m_req_trans_map;        //< request transaction map
    vector< map<int, vci_response> > m_res_trans_map;       //< response transaction map

    //approx_source_ni_base *get_source_ni(unsigned int cosi_id);
    //approx_dest_ni_base *get_dest_ni(unsigned int cosi_id);
#if 0
    //NOTE: try to use the following four functions instead of directly 
    //      accessing the std::map() for the type-casting.
    source_ni<vci_request> *get_mstr_source_ni(unsigned int cosi_id);
    dest_ni<vci_response> *get_mstr_dest_ni(unsigned int cosi_id);
    
    source_ni<vci_response> *get_slave_source_ni(unsigned int cosi_id);
    dest_ni<vci_request> *get_slave_dest_ni(unsigned int cosi_id);
#endif
};

//} //namespace approx_noc


#endif //__APPROX_NOC_APPROX_NETWORK_BASE_H__
