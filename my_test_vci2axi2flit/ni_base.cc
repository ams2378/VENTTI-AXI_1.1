/**
 * @filename ni_base.cc
 *
 * @brief File that includes the common abstraction of network interface.
 *
 * @author Young Jin Yoon <youngjin@cs.columbia.edu>
 */
#if 0
//MY COMMENTS
#include "checkpoint_drv_regs.h"
#endif

//namespace abstract_noc {
#if 0
// MY COMMENTS
template<typename trans_type>
inline void dest_ni_base<trans_type>::fifo_put(const trans_type &trans)
{
    unsigned int size;
    sc_time latency;
    trans_fifo.put(trans);
    bool ret_val = complete_transaction(trans, size, latency);
/*    if(m_nb->get_trans_tracer() != NULL && ret_val)
      m_nb->get_trans_tracer()->record_transaction(trans, size, latency);*/
}

template<typename trans_type>
inline trans_type dest_ni_base<trans_type>::get(tlm_tag < trans_type > *t)
{
    return trans_fifo.get(t);
}
/**
 * parse latency and size from a transaction for the transaction tracer.
 *
 * @param[in]   trans     transaction
 * @param[out]  size      size of the transaction in bytes
 * @param[out]  latency   latency in sc_time
 * 
 * @note: template specialization
 */
template <>
inline bool dest_ni_base<vci_request>::complete_transaction(const vci_request &trans, unsigned int &size, sc_time &latency)
{
#if 0
unsigned long slave_id = trans.slave_id;
    if(slave_id == CHECKPOINT_SLAVE_ID) {
      size = 0;
      latency = SC_ZERO_TIME;
      return false;
    } else {
      size = trans.n_bits;
      assert(sc_time_stamp() >= trans.inj_time);
      latency = sc_time_stamp() - trans.inj_time;
      return true;
    }
#else
      size = trans.n_bits;
      assert(sc_time_stamp() >= trans.inj_time);
      latency = sc_time_stamp() - trans.inj_time;
      return true;

#endif
}

/**
 * parse latency and size from a transaction for the transaction tracer.
 *
 * @param[in]   trans     transaction
 * @param[out]  size      size of the transaction in bytes
 * @param[out]  latency   latency in sc_time
 *
 * @note: template specialization
 */
template <>
inline bool dest_ni_base<vci_response>::complete_transaction(const vci_response &trans, unsigned int &size, sc_time &latency)
{
#if 0
    unsigned long slave_id = trans.slave_id;
    if(slave_id == CHECKPOINT_SLAVE_ID) {
      size = 0;
      latency = SC_ZERO_TIME;
      return false;
    } else {
      size = trans.n_bits;
      assert(sc_time_stamp() >= trans.inj_time);
      latency = sc_time_stamp() - trans.inj_time;
      return true;
    }
#else
      size = trans.n_bits;
      assert(sc_time_stamp() >= trans.inj_time);
      latency = sc_time_stamp() - trans.inj_time;
      return true;

#endif
}
#endif
// END OF MY COMMENTS

template<typename trans_type>
inline void source_ni_base<trans_type>::put(const trans_type &trans) 
{
    cout << "SOURCE_NI_BASE:\t master put in me new trans @ "<<sc_time_stamp()<<endl;
    bool ret_val = start_transaction(const_cast<trans_type &>(trans));
    if(ret_val)
      trans_fifo.put(trans);
}

template<typename trans_type>
inline trans_type source_ni_base<trans_type>::fifo_get(tlm_tag < trans_type > *t)
{
    return trans_fifo.get(t);
}

/**
 * get total bytes for a transaction.
 *
 * @note: template specialization for vci_request 
 * @note: source, destination ids are counted as the part of a payload.
 * @sa: vci_request
 */
template <>
inline int source_ni_base<vci_request>::get_num_bits(const vci_request &trans)
{
    int total_bits = 32 + 8 + 8 + 1 + 1 + 1 // address + be + cmd + contig + eop + cons
                   + 8 + 1 + 1 + 1 + 8 + 8   // plen + wrap + cfixed + clen + trdid + pktid
                   + 8 + 16;                 // srcid + slave_id(i.e.dest_id)
    if(trans.cmd == CMD_WRITE) {
        total_bits += 64;                   // wdata
    }
    return total_bits;
}

/**
 * get total bytes for a transaction.
 *
 * @note: template specialization for vci_response 
 * @note: source, destination ids are counted as the part of a payload.
 * @sa: vci_response
 */
template <>
inline int source_ni_base<vci_response>::get_num_bits(const vci_response &trans)
{
    int total_bits = 1 + 1 + 16 + 8 + 8 + 8; 
    // reop + rerror + rsrcid + rtrdid + rpktid
    if(trans.rcmd == CMD_READ || trans.rcmd == CMD_LOCKED_READ) {
      total_bits += 64;        //rdata
    }
    return total_bits;
}

template <>
inline bool source_ni_base<vci_request>::start_transaction(vci_request &trans)
{
    unsigned long addr = trans.address;
    
    //m_nb->preprocess_trans(trans);
    trans.n_bits = get_num_bits(trans);
    trans.inj_time = sc_time_stamp();
#if 0    
    if(addr >= CHECKPOINT_ADDR_BASE && addr < (CHECKPOINT_ADDR_BASE+CHECKPOINT_ADDR_SIZE)) {
      cout << "[INFO]: checkpoint with " << (unsigned long)trans.cmd << " detected!"<< endl;
      cout << "        slave_id:" << trans.slave_id << endl; 
      //HACK start: manually pull out values from sl_checkpoint module, and push it back to the source.
      //            NOTE THAT EVERYTHING SHOULD HAPPEN WITHOUT ANY CLOCK CYCLE DELAYS!
      sc_time cur_time = sc_time_stamp();
      m_nb->get_checkpoint_in()->put(trans);
      dest_ni_base<vci_response> *dst = m_nb->get_master_dest_ni(get_cosi_id_from_mstr(trans.srcid));
      vci_response ret_val = m_nb->get_checkpoint_out()->get();
  
      ret_val.slave_id = trans.slave_id;
      ret_val.n_bits = 0;
      ret_val.inj_time = sc_time_stamp();
      assert(ret_val.rsrcid == trans.srcid); 
      assert(ret_val.slave_id == CHECKPOINT_SLAVE_ID);
      dst->fifo_put(ret_val); 
      assert(cur_time == sc_time_stamp());
      //HACK end :(
      return false;
    }
    if(m_nb->get_trace_dump() != NULL)
      m_nb->get_trace_dump()->print_transaction(get_cosi_id_from_mstr(trans.srcid), get_cosi_id_from_slave(trans.slave_id), trans.n_bits, trans.inj_time);

#endif
    return true;
}

template <>
inline bool source_ni_base<vci_response>::start_transaction(vci_response &trans)
{
    trans.n_bits = get_num_bits(trans);
    trans.inj_time = sc_time_stamp();
    trans.slave_id = m_pid;
#if 0    
    if(m_nb->get_trace_dump() != NULL)
      m_nb->get_trace_dump()->print_transaction(get_cosi_id_from_slave(trans.slave_id), get_cosi_id_from_mstr(trans.rsrcid), trans.n_bits, trans.inj_time);
#endif
    return true;
}
//}
