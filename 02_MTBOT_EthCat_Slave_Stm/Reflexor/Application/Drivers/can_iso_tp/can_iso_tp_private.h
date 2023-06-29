/**
 * @file can_iso_tp_private.h
 *
 * @details Contains all the types data structure used with this device.
 */

#ifndef CAN_ISO_TP_PRIVATE_H_
#define CAN_ISO_TP_PRIVATE_H_

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 INCLUDES
 *-------------------------------------------------------------------------------------------------------------------*/

#include <stdint.h>
#include "can_iso_tp.h"
#include "can_iso_tp_mcu_lock.h"
#include "can_iso_tp_fifoqueue.h"

/*---------------------------------------------------------------------------------------------------------------------
 *                                                 DEFINES
 *-------------------------------------------------------------------------------------------------------------------*/

#define MAX_EVENT (3)

/*---------------------------------------------------------------------------------------------------------------------
 *                                             GLOBAL VARIABLES
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                               TYPES DEFINE
 *-------------------------------------------------------------------------------------------------------------------*/

typedef void (*event_handle_t)(void *);

struct time_poll_par_t
{
   event_handle_t handle; // handle == 0 means this parameter is idle
   can_iso_tp_link_t_p link;
   unsigned int user_ms;
};

struct L_Data_confirm_par_t
{
   event_handle_t handle; // handle == 0 means this parameter is idle
   can_iso_tp_link_t_p link;
   int8_t error;
};

struct N_USData_request_par_t
{
   event_handle_t handle; // handle == 0 means this parameter is idle
   can_iso_tp_link_t_p link;
   uint8_t isFunction;
   uint32_t size;
   const uint8_t *payload;
};

struct L_Data_indication_par_t
{
   event_handle_t handle; // handle == 0 means this parameter is idle
   can_iso_tp_link_t_p link;
   struct CAN_msg rx_msg;
};


struct event_mange_t
{  
   mcu_lock_t lock;
   ElemType fifo_data[MAX_EVENT];
   struct FifoQueue fifo;
};

typedef enum
{
   tx_idle = 0,
   tx_sf_wait_tx,
   tx_sf_wait_confirm,
   tx_ff_wait_tx,
   tx_ff_wait_confirm,
   tx_wait_fc,
   tx_cf_wait_tx,
   tx_cf_wait_tx_retry,
   tx_cf_wait_confirm
} e_tx_status;

typedef enum
{
   rx_idle = 0,
   rx_tx_fc,
   rx_tx_fc_wait_confirm,
   rx_wait_cf,
   rx_tx_fc_overrun,
   rx_tx_fc_overrun_wait_confirm
} e_rx_status;

struct can_iso_tp_link_t
{
   struct can_iso_tp_init_t init_info;
   unsigned int init_done_flag;
   unsigned int current_time_ms; // Current timestamp

   // Sending Task-related Variables
   struct
   {
      struct CAN_msg last_msg;         // The last message sent to hardware
      unsigned int last_msg_time_ms;   // The timestamp of the last message, used to determine whether timeout for this task
      const uint8_t *current_playload; // Data pointer sent by current application layer request
      uint32_t current_size;           // Data Length of Current Application Layer Request Sending
      uint32_t current_tx_index;       // The length of data that is currently sent, for multi-frame transmission only
      uint8_t current_tx_SN;           // Data count value currently sent, for multi-frame transmission only
      uint8_t rx_BS;                   // BS parameters currently received
      uint8_t rx_BS_tx;                // Number of frame sent for the current BS block
      uint8_t rx_Stmin;                // Stmin parameters currently received
      uint8_t N_WFT_cnt;
      e_tx_status status; // Record the current sending status
   } tx_record;

   struct
   {
      struct L_Data_indication_par_t L_Data_indication_par;
      struct L_Data_confirm_par_t L_Data_confirm_par;
      struct time_poll_par_t time_poll_par;
      struct N_USData_request_par_t N_USData_request_par;
      struct event_mange_t event_manage;
   } tx_events;

   // Receiving Task-related Variables
   struct
   {
      e_rx_status status;
      uint32_t rx_index;
      uint32_t rx_len;
      uint8_t rx_SN;
      uint8_t tx_BS_cnt;
      struct CAN_msg last_msg; // The last message sent to hardware
      unsigned int last_msg_time_ms;
   } rx_record;
   
   struct
   {
      struct L_Data_indication_par_t L_Data_indication_par;
      struct L_Data_confirm_par_t L_Data_confirm_par;
      struct time_poll_par_t time_poll_par;
      struct event_mange_t event_manage;
   } rx_events;
};

/*---------------------------------------------------------------------------------------------------------------------
 *                                           VARIABLES DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------
 *                                            FUNCTION DECLARATION
 *-------------------------------------------------------------------------------------------------------------------*/

#endif /* CAN_ISO_TP_PRIVATE_H_ */
