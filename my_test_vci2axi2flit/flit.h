/**
 * @filename  flit.h
 *
 * @brief     The file that includes a class of flit definition for the SystemC TLM router 
 *            implementation for Rabbits.
 *
 * @author    Young Jin Yoon <youngjin@cs.columbia.edu>
 */

#ifndef __APPROX_NOC_FLIT_H__
#define __APPROX_NOC_FLIT_H__

#include "systemc.h"
#include <string>
#include <iomanip>

#ifdef DEBUG
#ifdef NPORTS
#define MAX_SRC     NPORTS
#define MAX_DEST    NPORTS
#else
#define MAX_SRC     20
#define MAX_DEST    20
#endif
#define MAX_TRANS   20
#define MAX_FLIT    10
#endif




//namespace approx_noc {

/**
* enum: Defines five channel types supported by AXI protocol
*
*/

    enum {
        WADDR,
        WDATA,
        RADDR,
        WRESP,
        RDATA,
    };

/**
 * @brief The flit class that is used by the systemC-TLM router implementation, RouterTLM.
 * 
 */
class flit {
  public:
    int trans_id;    //!< transaction id
    int src_id;      //!< source identifier
    int dest_id;     //!< destination identifier
    int flit_id;     //!< flit id in a transaction
    bool is_tail;    //!< indiates whether this is the tail flit of the transaction
    int type;    

    /**
     * constructor
     */
    flit (int trans_id=0, int src_id=0, int dest_id=0, int flit_id=-1, bool is_tail=false, int type=WADDR) {
      this->trans_id = trans_id;
      this->src_id = src_id;
      this->dest_id = dest_id;
      this->flit_id = flit_id;
      this->is_tail = is_tail;
      this->type = type;
    }

    /**
     * equal operator overloading. 
     * @note make sure all member variables of the current object is compared to the 
     * rhs object
     */
    inline bool operator == (const flit & rhs) const {
      return (
              rhs.trans_id == trans_id &&
              rhs.src_id == src_id && 
              rhs.dest_id == dest_id &&
              rhs.flit_id == flit_id &&
              rhs.is_tail == is_tail &&
	      rhs.type == type
             );
    }

    /**
     * assignment operator overloading. 
     * @note make sure all member variables of the rhs object is assigned to the 
     * current object
     */
    inline flit& operator = (const flit& rhs) {
      src_id = rhs.src_id;
      dest_id = rhs.dest_id;
      trans_id = rhs.trans_id;
      flit_id = rhs.flit_id;
      is_tail = rhs.is_tail;
      type = rhs.type;
      return *this;
    }

    /**
     * assignment operator overloading. 
     * @note this will be helpful later to trace information. Just make sure all 
     * member variables of the v object is used by sc_trace() function.
     */
    inline friend void sc_trace(sc_trace_file *tf, const flit & v, const std::string& NAME ) {
      sc_trace(tf, v.trans_id, NAME + ".trans_id");
      sc_trace(tf, v.src_id, NAME + ".src_id");
      sc_trace(tf, v.dest_id, NAME + ".dest_id");
      sc_trace(tf, v.flit_id, NAME + ".flit_id");
      sc_trace(tf, v.is_tail, NAME + ".is_tail");
      sc_trace(tf, v.type, NAME + ".type");	
    }

    /**
     * shift-left operator overloading.
     * @note this is necessary to print the contents of an object with std::cout. 
     * Just make sure all member variables of the v object is "shift-lefted" with 
     * ostream class.
     */
    inline friend ostream& operator << ( ostream& os,  flit const & v ) {
      os << "(";
      os << "trans_id=" << v.trans_id;
      os << ", src_id=" << v.src_id;
      os << ", dest_id=" << v.dest_id;
      os << ", flit_id=" << v.flit_id;
      os << ", is_tail=" << v.is_tail;
      os << ", type=" << v.type;
      os << ")";
      return os;
    }

#ifdef DEBUG
    /**
     * create an object and randomize all variables
     * @note you may want to adjust individual variables after executing this 
     * function, especially flit::trans_id and flit::flit_id.
     */
    static inline flit &generateRandom ( flit &v ) {
        v.src_id = int(rand() % (MAX_SRC));
        do {
          v.dest_id = int(rand() % (MAX_DEST));
        } while (v.src_id == v.dest_id);
        v.trans_id = int(rand() % (MAX_TRANS));
        v.flit_id = int(rand() % (MAX_FLIT));
        v.is_tail = (int(rand() % 2))?true:false;
	v.flit_id = int(rand() % 5);
    }
#endif
};

//} //namespace approx_noc
#endif //__APPROX_NOC_FLIT_H__
