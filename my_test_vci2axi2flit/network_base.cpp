/**
 * @filename  network_base.cpp
 *
 * @brief     File that includes the base network implementation of Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 *
 * @sa        network_base.h
 */


#include "network_base.h"
#include "ni_base.h"
//#include "trans_tracer.h"

//namespace abstract_noc {

/**
 * Constructor for the class
 *
 * @param[in] n_masters       total number of masters in the system.
 * @param[in] n_slaves        total number of slaves in the system.
 */
network_base::network_base (int n_masters, int n_slaves, bool sample_bandwidth, bool sample_latency, bool print_trans, bool sample_energy)
  //: m_n_masters ( n_masters ),
    //m_n_slaves ( n_slaves ),
    //m_sl_checkpoint_in ( -1 ),
    //m_sl_checkpoint_out ( -1 )
{
	;
#if 0
// MY COMMENTS
    for(int i=0; i< m_n_masters+m_n_slaves; i++) {
        if(i < m_n_masters) {
            m_rabbit_mstr_to_cosi_id.push_back(i);
            m_cosi_to_rabbit_id.push_back(i); 
            parse_address_map(i);
        } else {
            m_rabbit_slave_to_cosi_id.push_back(i);
            m_cosi_to_rabbit_id.push_back(i-m_n_masters);
        }
    }
    cout << "[DEBUG] sample_bandwidth: " << sample_bandwidth <<", sample_latency: " << sample_latency << ", print_trans: " << print_trans <<", sample_energy: " << sample_energy << endl;
    if(sample_bandwidth || sample_latency)
      m_trans_tracer = new trans_tracer(n_masters, n_slaves, sample_latency, sample_bandwidth);
    else
      m_trans_tracer = NULL;

    if(sample_energy)
      m_energy_tracer = new energy_tracer(PWR_WATT);
    else
      m_energy_tracer = NULL;

    if(print_trans)
      m_trace_dump = new trace_dump(n_masters, n_slaves);
    else
      m_trace_dump = NULL;

    m_sl_checkpoint = new sl_checkpoint("checkpoint", m_trans_tracer, m_energy_tracer, m_trace_dump);
    m_sl_checkpoint->get_port(m_sl_checkpoint_in);
    m_sl_checkpoint->put_port(m_sl_checkpoint_out);
#endif
}

/**
 * Destructor for the class
 */
network_base::~network_base ()
{
;
#if 0
// MY COMMENTS
  if(m_trans_tracer)
    delete m_trans_tracer;
  
  if(m_energy_tracer)
    delete m_energy_tracer;
  
  if(m_trace_dump) 
    delete m_trace_dump;

  delete m_sl_checkpoint;
#endif
}

/**
 * Connect the ports of a master into a source and a destination network interfaces
 *
 * @param[in] rabbit_mstr_id    rabbit identifier for a master
 * @param[in] putp              sc_port to be connected to the source interface
 * @param[in] getp              sc_port to be connected to the destination interface
 * @param[in] source_queue_size number of outstanding transaction for source_ni 
 * @param[in] dest_queue_size   number of outstanding transaction for dest_ni 
 *
 * @todo change the queue size of both source and dest network interfaces based on 
 * integer parameters.
 */

#if 0
// MY COMMENTS
void network_base::connect_master(int rabbit_mstr_id, sc_port<VCI_PUT_REQ_IF> &putp, sc_port<VCI_GET_RSP_IF> &getp, int src_queue_size, int dest_queue_size) 
{
    //Rabbit Master Id should be already instantiated
    assert(rabbit_mstr_id < m_rabbit_mstr_to_cosi_id.size()); 
    //Source Network interface should be already instantiated already
    assert(m_source_ni.find(get_cosi_id_from_mstr(rabbit_mstr_id)) != m_source_ni.end()); 
    //Dest Network interface should be already instantiated
    assert(m_dest_ni.find(get_cosi_id_from_mstr(rabbit_mstr_id)) != m_dest_ni.end()); 
    
    int cosiId = get_cosi_id_from_mstr(rabbit_mstr_id);
    
    source_ni_base<vci_request> *src_ni = dynamic_cast< source_ni_base<vci_request> *>(m_source_ni[cosiId]);
    dest_ni_base<vci_response> *dest_ni = dynamic_cast< dest_ni_base<vci_response> *>(m_dest_ni[cosiId]);

    assert(src_ni != NULL);
    assert(dest_ni != NULL);
    //Connect source and destination to putp and getp ports
    putp(*src_ni);
    getp(*dest_ni);
}

/**
 * Connect the ports of a slave into a source and a destination network interfaces
 *
 * @param[in] rabbit_slave_id   rabbit identifier for a slave
 * @param[in] putp              sc_port to be connected to the source interface
 * @param[in] getp              sc_port to be connected to the destination interface
 * @param[in] source_queue_size number of outstanding transaction for source_ni 
 * @param[in] dest_queue_size   number of outstanding transaction for dest_ni
 *
 * @todo change the queue size of both source and dest network interfaces based on 
 * integer parameters.
 */
void network_base::connect_slave(int rabbit_slave_id, sc_port<VCI_GET_REQ_IF> &getp, sc_port<VCI_PUT_RSP_IF> &putp, int src_queue_size, int dest_queue_size)
{
    //Rabbit Slave Id should already instantiated
    assert(rabbit_slave_id < m_rabbit_slave_to_cosi_id.size()); 
    //Source network interface should be instantiated already
    assert(m_source_ni.find(get_cosi_id_from_slave(rabbit_slave_id)) != m_source_ni.end()); 
    //Dest network interface should be instantiated already
    assert(m_dest_ni.find(get_cosi_id_from_slave(rabbit_slave_id)) != m_dest_ni.end()); 
   
    int cosiId = get_cosi_id_from_slave(rabbit_slave_id);
    
    //Connect source and destination to putp and getp ports
    source_ni_base<vci_response> *src_ni = dynamic_cast< source_ni_base<vci_response> *>(m_source_ni[cosiId]);
    dest_ni_base<vci_request> *dest_ni = dynamic_cast< dest_ni_base<vci_request> *>(m_dest_ni[cosiId]);

    assert(src_ni != NULL);
    assert(dest_ni != NULL);
    putp(*src_ni);
    getp(*dest_ni);
}

/**
 * parse the address information given as a file per master
 *
 * @param[in]   master_id   Rabbit identifier for a master
 * @note    most of implementation in this function is adopted 
 *          from interconnect_master::interconnect_master
 *
 * @sa      interconnect_master.cpp
 */
inline void network_base::parse_address_map(int master_id) 
{
    int             i, k;
    FILE            *file;
    char            file_name[256];

    assert(m_address_map.find(master_id) == m_address_map.end());


    sprintf (file_name, "%s/node%d.map", MAP_FILES_DIR, master_id);
    if ((file = fopen (file_name, "rt")) == NULL)
    {
        printf ("Error (masterid=%d): Cannot open the map file %s!\n", master_id, file_name);
        sc_stop();
    }

    std::vector < addr_map_t > addr_map; 
    while (1)
    {
        addr_map_t addr;
        //read a line with the given format
        k = fscanf (file, "0x%lX 0x%lX 0x%lX %d\n", 
                    &addr.begin_address, &addr.end_address, &addr.intern_offset, &addr.slave_id);
        if (k <= 0)
            break;
        if (k > 0 && k < 4)
        {
            printf ("Error (masterid=%d): Invalid map file %s, line %d!\n", master_id, file_name, i);
            //exit (1);
            sc_stop();
        }
        //if succeeded, put the address information into addr_map vector
        addr_map.push_back(addr);
    }
    fclose (file);
    
    //put the vector into m_address_map with given master_id
    m_address_map[master_id] = addr_map;
}

/**
 * with master id and an initial address, find the slave id from the m_address_map
 * @param[in] master_id     rabbit identifier for a master
 * @param[inout] addr       address to find a slave id, adjusted with the slave id offset.
 * @return                  slave id
 */
int network_base::get_slave_id_from_mem_addr(int master_id, unsigned long &addr)
{
    assert(m_address_map.find(master_id) != m_address_map.end());
    std::vector < addr_map_t > addr_map = m_address_map[master_id];

    std::vector < addr_map_t >::iterator it;
    for(it = addr_map.begin(); it != addr_map.end(); it++) {
        if( addr >= it->begin_address && addr <= it->end_address) { 
            //adjust the address into the slave id offset
            addr = addr - it->begin_address + it->intern_offset;
            return it->slave_id;
        }
    }
    printf ("Error: 0x%lx bad address required in %s!\n",
        addr, __FUNCTION__);
    return -1;
}

/**
 * with master id, slave id, slave offset, and initial address, compute the linear address
 * @param[in] master_id     rabbit identifier for a master
 * @param[in] slave_id      rabbit identifier for a slave
 * @param[in] slave_offset  address offset for the slave
 * @param[inout] addr       address to be adjusted with the slave id offset.
 * @return                  1 on success, 0 otherwise.
 */
int network_base::get_linear_address(int master_id, int slave_id, unsigned long slave_offset, unsigned long &addr) 
{
    assert(m_address_map.find(master_id) != m_address_map.end());
    std::vector < addr_map_t > addr_map = m_address_map[master_id];

    std::vector< addr_map_t >::iterator it;

    for(it = addr_map.begin(); it != addr_map.end(); it++) {
        if(it->slave_id == slave_id) {
            addr = it->begin_address + slave_offset - it->intern_offset;
            if(addr >= it->begin_address && addr < it->end_address)
                return 1;
        }
    }
    return 0;
}

/**
 * preprocess a transaction so that it has the correct address field 
 * with respect to slave offset
 * @param[inout] trans      a vci_request transaction that will be preprocessed
 */
void network_base::preprocess_trans(vci_request &trans)
{
    assert(m_address_map.find(trans.srcid) != m_address_map.end());
    std::vector < addr_map_t > addr_map = m_address_map[trans.srcid];

    unsigned long addr = trans.address;
    std::vector < addr_map_t >::iterator it;
    for(it = addr_map.begin(); it != addr_map.end(); it++) {
        if( addr >= it->begin_address && addr < it->end_address) {
            //If address is in the range of this slave, 
            //Assign the slave_id and move the initial address with the slave offset.
            trans.initial_address = trans.address;
            trans.slave_id = it->slave_id;
            trans.address = addr - it->begin_address + it->intern_offset;
            return;
        }
    }
    printf ("Error (masterid=%d): Cannot map the address 0x%lx to a slave!\n",
        trans.srcid, addr);
    sc_stop();
}

dest_ni_base <vci_response> *network_base::get_master_dest_ni(int cosi_id) 
{ 
  return static_cast< dest_ni_base<vci_response> *>(m_dest_ni[cosi_id]); 
}
#endif
// END OF MY COMMENTS
//}
