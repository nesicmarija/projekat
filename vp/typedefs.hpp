#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#define SC_INCLUDE_FX
#include <systemc>
#include <tlm>


#define NUMOFVAR 50
#define NUMOFSLACK 50
#define ROWSIZE (NUMOFSLACK+1)
#define COLSIZE (NUMOFSLACK+NUMOFVAR+1)
//#define BUFF_SIZE 4
#define W 32 // data width
#define I 11 // whole part witdh
#define Q sc_dt::SC_TRN // quantization
#define O sc_dt::SC_WRAP // overflow
#define DELILAC 100 ///for normalization
#define QUOT 9.99965367///for normalization

typedef sc_dt::sc_fixed_fast <W, I, Q, O> num_t;

typedef tlm::tlm_base_protocol_types::tlm_payload_type pl_t;
typedef tlm::tlm_base_protocol_types::tlm_phase_type ph_t;


#define ADDR_WIDTH 0x08
#define ADDR_HEIGHT 0x10
#define ADDR_CMD 0x04//Dusan je kriv
#define ADDR_STATUS 0x1c
#define ADDR_INIT 0x30
#define ADDR_STATUS_INIT 0x38
#define ADDR_RES 0x20

#define VP_ADDR_BRAM_BASE 0x01000000
#define VP_ADDR_BRAM_L 0x01000000
#define VP_ADDR_BRAM_H 0x02F00000 //


 //
#define VP_ADDR_HARD_BASE 0x03000000
#define VP_ADDR_HARD_BASE_INT 0x03000000
#define VP_ADDR_HARD_INT_L 0x03000000
#define VP_ADDR_HARD_INT_H 0x03F00040  //

#define VP_ADDR_HARD_BASE_NUM_T 0x04000000
#define VP_ADDR_HARD_NUM_T_L 0x04000000
#define VP_ADDR_HARD_NUM_T_H 0x04F00040

#define MAX_SIZE 64
#define MEM_RESERVED 1000000

#endif // TYPEDEFS_HPP
