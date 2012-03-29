/**
 * @filename ni_base.h
 *
 * @brief File that includes the common abstraction of network interface.
 *
 * @author Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __ABSTRACT_NOC_NI_BASE_H__
#define __ABSTRACT_NOC_NI_BASE_H__

//#include <map>

#include <systemc>
#include <iostream>
#include <ctime>

#include "abstract_noc.h"
//#include "network_base.h"



/**
 * @brief       a class that contains network interface abstraction.
 */
class ni_base {
    public:
        /// Destructor
        virtual ~ni_base() {;}
    protected:
        /// Constructor
        //ni_base(network_base *nb, int num_out_trans, int pid) 
        //    : m_nb(nb), m_num_out_trans(num_out_trans), m_pid(pid) {;}
	// MY CONSTRUCTOR!!!
        ni_base(int num_out_trans, int pid) 
            : m_num_out_trans(num_out_trans), m_pid(pid) {;}        



#if 0
// MY COMMENTS
        /**
         * get cosi id from master id
         * @param[in]   master_id   rabbit identifier for a master
         * @return                  cosi identifier
         */
        unsigned int get_cosi_id_from_mstr(unsigned int master_id) 
            { return m_nb->get_cosi_id_from_mstr(master_id); }
        /**
         * get cosi id from slave id
         * @param[in]   slave_id    rabbit identifier for a slave
         * @return                  cosi identifier
         */
        unsigned int get_cosi_id_from_slave(unsigned int slave_id)
            { return m_nb->get_cosi_id_from_slave(slave_id); }
        /**
         * get rabbit id from the cosi id
         * @param[in]   cosi_id     cosi identifier
         * @return                  rabbit identifier
         */
        unsigned int get_rabbit_id(unsigned int cosi_id)
            { return m_nb->get_rabbit_id(cosi_id); }
#endif
// END OF MY COMMENTS
        
        //network_base *m_nb;         //< pointer of approx_network_base
        int m_num_out_trans;        //< total number of outstanding transaction
        int m_pid;                  //< process id
    
};

/**
 * @brief       a class that contains source network interface abstraction.
 * @param[out]  flit_out    sc_port that will be connected to the router
 */
template<typename trans_type>
class source_ni_base : public ni_base,
public tlm_blocking_put_if<trans_type> {

  public:
    /// common put() function for all source_ni classes of different noc models.
    virtual void put(const trans_type &trans);

    /// To record the start time of a transaction
    virtual bool start_transaction(trans_type &trans);
  

    ///Constructor: should not be called directly
    //source_ni_base(network_base *nb, int num_out_trans, int pid)
    //  : ni_base(nb, num_out_trans, pid),
    //    trans_fifo(num_out_trans) {}
	// MY CONSTRUCTOR!!!!!!!!!
    source_ni_base(int num_out_trans, int pid)
      : ni_base(num_out_trans, pid),
        trans_fifo(num_out_trans) {}
    
  protected:
    virtual trans_type fifo_get (tlm_tag <trans_type > *t=0);
    inline int get_num_bits(const trans_type &trans);


// MY COMMENTS
    ///a main SC_THREAD for all source_nis
    //virtual void process_main()=0; 

  
    tlm_fifo < trans_type > trans_fifo; //< source queue for all transactions
};

/**
 * @brief       a class that contains destination network interface abstraction.
 */

template<typename trans_type>
class dest_ni_base : public ni_base,
public tlm_blocking_get_if<trans_type> {
 
#if 0
// MY COMMENTS  
  friend bool source_ni_base< vci_request >::start_transaction(vci_request &trans);  
#endif
// END OF MY COMMENTS
  public:
    /// common get() function for all dest_ni classes of different noc models
    virtual trans_type get (tlm_tag < trans_type > *t=0);
  
  protected:
    ///Constuctor
    dest_ni_base(int num_out_trans, int pid)
        : ni_base(num_out_trans, pid),
          trans_fifo(num_out_trans) {}
#if 0
// MY COMMENTS 
    virtual void fifo_put(const trans_type &trans);
    /// To record the end time of a transaction
    virtual bool complete_transaction(const trans_type &trans, unsigned int &size, sc_time &latency);
#endif
// END OF MY COMMENTS
    tlm_fifo < trans_type > trans_fifo;
};


//} //namespace approx_noc

#include "ni_base.cc"

#endif //__NI_BASE_H__
