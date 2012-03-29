/**
 * @filename network_base.h
 *
 * @brief     File that includes the basic network interface of Rabbits.
 *
 * @author Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __ABSTRACT_NOC_NETWORK_BASE_H__
#define __ABSTRACT_NOC_NETWORK_BASE_H__

//#include <map>

#include <systemc>
#include <tlm.h>

#include "abstract_noc.h"
#if 0
#include "trans_tracer.h"
#include "energy_tracer.h"
#include "trace_dump.h"
#include "sl_checkpoint.h"
//#include "ni_base.h"
#endif
//namespace abstract_noc {

class ni_base;

//template<typename trans_type> class dest_ni_base;
//template<typename trans_type> class source_ni_base; 


/**
 * @brief SystemC-TLM network base class. This abstract class
 * can be derived into another class to instantiate and connect
 * routers inside the network. To see the example, please refer
 * star_network class
 *
 */
class network_base { 
#if 0
  public:
	
// MY COMMMENTS
    virtual void connect_master(int rabbit_mstr_id, sc_port<VCI_PUT_REQ_IF> &putp, sc_port<VCI_GET_RSP_IF> &getp, int src_queue_size = -1, int dest_queue_size = -1);
    virtual void connect_slave(int rabbit_slave_id, sc_port<VCI_GET_REQ_IF> &getp, sc_port<VCI_PUT_RSP_IF> &putp, int src_queue_size = -1, int dest_queue_size = -1);

    
    /**
     * get cosi id from master id
     * @param[in] master_id rabbit identifier for a master
     * @return              cosi identifier
     */
    unsigned inline int get_cosi_id_from_mstr(unsigned int master_id) 
        { return m_rabbit_mstr_to_cosi_id[master_id]; }
    
    /**
     * get cosi id from slave id
     * @param[in] slave_id  rabbit identifier for a slave
     * @return              cosi identifier
     */
    unsigned inline int get_cosi_id_from_slave(unsigned int slave_id) 
        { return m_rabbit_slave_to_cosi_id[slave_id]; }
    
    /**
     * get rabbit id from cosi id
     * @param[in] cosi_id   cosi identifier
     * @return              rabbit identifier
     */
    unsigned inline int get_rabbit_id(unsigned int cosi_id) 
        { return m_cosi_to_rabbit_id[cosi_id]; }

    /**
     * get transaction tracer
     * @return              a trans_tracer object
     */
/*    inline trans_tracer *get_trans_tracer()
      { return m_trans_tracer; }

    inline energy_tracer *get_energy_tracer()
      { return m_energy_tracer; }
    inline trace_dump *get_trace_dump()
      { return m_trace_dump; }
    
    inline tlm_fifo < vci_response > *get_checkpoint_out()
      { return &m_sl_checkpoint_out; }
    inline tlm_fifo < vci_request > *get_checkpoint_in()
      { return &m_sl_checkpoint_in; }*/

    dest_ni_base <vci_response> *get_master_dest_ni(int cosi_id); 


    int get_linear_address(int master_id, int slave_id, unsigned long slave_offset, unsigned long &addr);
    int get_slave_id_from_mem_addr(int masterId, unsigned long &addr);
    void preprocess_trans(vci_request &trans);
  
  protected:
#endif
// END OF MY COMMENTS
    
    /// Constructor : should not be called directly ==> protected
    network_base(int n_masters, int n_slaves, bool sample_bandwidth, bool sample_latency, bool print_trans, bool sample_energy);
    
    /// destructor
    ~network_base();

#if 0
// MY COMMENTS

    int m_n_masters;                //< total number of masters in the system
    int m_n_slaves;                 //< total number of slaves in the system

    ///tables that translate cosi id to rabbit id, or vice versa.
    std::vector<unsigned int> m_cosi_to_rabbit_id, m_rabbit_mstr_to_cosi_id, m_rabbit_slave_to_cosi_id;
    
    typedef struct
    {
        int                             slave_id;
        unsigned long                   begin_address;
        unsigned long                   end_address;
        unsigned long                   intern_offset;
    } addr_map_t;                                            //< type used for the address map

    std::map<int, std::vector< addr_map_t > > m_address_map; //< address map for masters
    
    std::map<unsigned int, ni_base *> m_source_ni; //< collection of source network interface
    std::map<unsigned int, ni_base *> m_dest_ni;     //< collection of destination network interface

    void parse_address_map(int master_id);

    trans_tracer *m_trans_tracer;
    energy_tracer *m_energy_tracer;
    trace_dump *m_trace_dump;
    sl_checkpoint *m_sl_checkpoint;
    tlm_fifo < vci_request > m_sl_checkpoint_in;
    tlm_fifo < vci_response > m_sl_checkpoint_out;
#endif
// MY COMMENTS
};

//} //namespace abstract_noc


#endif //__ABSTRACT_NOC_NETWORK_BASE_H__
