/*****************************************************************************
 *
 *  $Id$
 *
 *  Copyright (C) 2007-2009  Florian Pose, Ingenieurgemeinschaft IgH
 *
 *  This file is part of the IgH EtherCAT Master.
 *
 *  The IgH EtherCAT Master is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License version 2, as
 *  published by the Free Software Foundation.
 *
 *  The IgH EtherCAT Master is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *  Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with the IgH EtherCAT Master; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *  ---
 *
 *  The license mentioned above concerns the source code only. Using the
 *  EtherCAT technology and brand is only permitted in compliance with the
 *  industrial property and similar rights of Beckhoff Automation GmbH.
 *
 ****************************************************************************/

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h> /* clock_gettime() */
#include <sys/mman.h> /* mlockall() */
#include <sched.h> /* sched_setscheduler() */

/****************************************************************************/

#include "ecrt.h"

/****************************************************************************/

/** Task period in ns. */
#define PERIOD_NS   (1000000)

#define MAX_SAFE_STACK (8 * 1024) /* The maximum stack size which is
                                     guranteed safe to access without
                                     faulting */

/****************************************************************************/

/* Constants */
#define NSEC_PER_SEC (1000000000)
#define FREQUENCY (NSEC_PER_SEC / PERIOD_NS)

/****************************************************************************/

// EtherCAT
static ec_master_t *master = NULL;
static ec_master_state_t master_state = {};

static ec_domain_t *domain1 = NULL;
static ec_domain_state_t domain1_state = {};

static ec_slave_config_t *sc_tmc8462a_in = NULL;
static ec_slave_config_state_t sc_tmc8462a_in_state = {};

/****************************************************************************/

// process data
static uint8_t *domain1_pd = NULL;

#define mtbotEasyCatPos    0, 0
#define mtbotTmcPos        0, 1
#define mtbotLan9252aEVB   0, 2

#define MTBOT_EasyCat      0x00000000, 0x000ab123
#define MTBOT_TMC8462A     0x00000000, 0x000ab123
#define MTBOT_Lan9252aEvb  0x00000000, 0x000ab123

// offsets for PDO entries
typedef struct 
{
   /* Data receives. */
   struct {
      uint32_t Rx0;
      uint32_t Rx1;
      uint32_t Rx2;
      uint32_t Rx3;
   } Receives;

   /* Data Transmit. */
   struct {
      uint32_t Tx0;
      uint32_t Tx1;
      uint32_t Tx2;
      uint32_t Tx3;
   } Transmits;
} tMtbotData;

static tMtbotData mtBotData;

const static ec_pdo_entry_reg_t domain1_regs[] = {
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x6000, 2, &mtBotData.Transmits.Tx0},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x6000, 3, &mtBotData.Transmits.Tx1},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x6000, 4, &mtBotData.Transmits.Tx2},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x6000, 5, &mtBotData.Transmits.Tx3},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x7004, 2, &mtBotData.Receives.Rx0},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x7004, 3, &mtBotData.Receives.Rx1},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x7004, 4, &mtBotData.Receives.Rx2},
    {mtbotTmcPos,  MTBOT_TMC8462A, 0x7004, 5, &mtBotData.Receives.Rx3},
    {}
};

/*****************************************************************************/

// Analog in --------------------------

static ec_pdo_entry_info_t mtbotTmc_pdo_entries[] = {
    {0x6000, 1,  8}, // channel 1 status
    {0x6000, 2,  8}, // channel 2 status
    {0x6000, 3,  8}, // channel 3 status
    {0x6000, 4,  8}, // channel 4 status
    {0x6000, 5,  8}, // channel 5 status
    {0x7004, 1,  8}, // channel 1 status
    {0x7004, 2,  8}, // channel 2 status
    {0x7004, 3,  8}, // channel 3 status
    {0x7004, 4,  8}, // channel 4 status
    {0x7004, 5,  8}, // channel 5 status
};

static ec_pdo_info_t mtbotTmc_pdos[] = {
    {0x1A00, 5, mtbotTmc_pdo_entries},
    {0x1600, 5, mtbotTmc_pdo_entries + 5}
};

static ec_sync_info_t mtbotTmc_syncs[] = {
    {2, EC_DIR_OUTPUT, 1,mtbotTmc_pdos + 1},
    {3, EC_DIR_INPUT, 1, mtbotTmc_pdos},
    {0xff}
};

static uint32_t counter = 0U;
static uint32_t countEachTimeSendData = 0U;

/*****************************************************************************/

void check_domain1_state(void)
{
    ec_domain_state_t ds;

    ecrt_domain_state(domain1, &ds);

    if (ds.working_counter != domain1_state.working_counter) {
        printf("Domain1: WC %u.\n", ds.working_counter);
    }
    if (ds.wc_state != domain1_state.wc_state) {
        printf("Domain1: State %u.\n", ds.wc_state);
    }

    domain1_state = ds;
}

/*****************************************************************************/

void check_master_state(void)
{
    ec_master_state_t ms;

    ecrt_master_state(master, &ms);

    if (ms.slaves_responding != master_state.slaves_responding) {
        printf("%u slave(s).\n", ms.slaves_responding);
    }
    if (ms.al_states != master_state.al_states) {
        printf("AL states: 0x%02X.\n", ms.al_states);
    }
    if (ms.link_up != master_state.link_up) {
        printf("Link is %s.\n", ms.link_up ? "up" : "down");
    }

    master_state = ms;
}

/*****************************************************************************/

void check_slave_config_states(void)
{
    ec_slave_config_state_t s;

    ecrt_slave_config_state(sc_tmc8462a_in, &s);

    if (s.al_state != sc_tmc8462a_in_state.al_state) {
        printf("TMC8462A: State 0x%02X.\n", s.al_state);
    }
    if (s.online != sc_tmc8462a_in_state.online) {
        printf("TMC8462A: %s.\n", s.online ? "online" : "offline");
    }
    if (s.operational != sc_tmc8462a_in_state.operational) {
        printf("TMC8462A: %soperational.\n", s.operational ? "" : "Not ");
    }

    sc_tmc8462a_in_state = s;
}

/*****************************************************************************/

void cyclic_task()
{
    // receive process data
    ecrt_master_receive(master);
    ecrt_domain_process(domain1);

    // check process data state
    check_domain1_state();

    if (counter) {
        counter--;
    } else { // do this at 1 Hz
        counter = FREQUENCY;

        // calculate new process data
        countEachTimeSendData += 1U;

        // check for master state (optional)
        check_master_state();

        // check for slave configuration state(s) (optional)
        check_slave_config_states();
#if 1
        // read process data
        printf("TMC8462A slave: Tx memory %u value %u\n", 0, EC_READ_U8(domain1_pd + mtBotData.Transmits.Tx0));
        printf("TMC8462A slave: Tx memory %u value %u\n", 1, EC_READ_U8(domain1_pd + mtBotData.Transmits.Tx1));
        printf("TMC8462A slave: Tx memory %u value %u\n", 2, EC_READ_U8(domain1_pd + mtBotData.Transmits.Tx2));
        printf("TMC8462A slave: Tx memory %u value %u\n", 3, EC_READ_U8(domain1_pd + mtBotData.Transmits.Tx3));
#endif

#if 1
    // write process data
    EC_WRITE_U8(domain1_pd + mtBotData.Receives.Rx0, countEachTimeSendData);
    EC_WRITE_U8(domain1_pd + mtBotData.Receives.Rx1, countEachTimeSendData + 1U);
    EC_WRITE_U8(domain1_pd + mtBotData.Receives.Rx2, countEachTimeSendData + 2U);
    EC_WRITE_U8(domain1_pd + mtBotData.Receives.Rx3, countEachTimeSendData + 3U);
#endif
    }


    // send process data
    ecrt_domain_queue(domain1);
    ecrt_master_send(master);
}

/****************************************************************************/

void stack_prefault(void)
{
    unsigned char dummy[MAX_SAFE_STACK];

    memset(dummy, 0, MAX_SAFE_STACK);
}

/****************************************************************************/

int main(int argc, char **argv)
{
    ec_slave_config_t *sc;
    struct timespec wakeup_time;
    int ret = 0;

    master = ecrt_request_master(0);
    if (!master) {
        return -1;
    }

    domain1 = ecrt_master_create_domain(master);
    if (!domain1) {
        return -1;
    }

    if (!(sc_tmc8462a_in = ecrt_master_slave_config(
                    master, mtbotTmcPos, MTBOT_TMC8462A))) {
        fprintf(stderr, "Failed to get slave configuration.\n");
        return -1;
    }

    printf("Configuring PDOs...\n");
    if (ecrt_slave_config_pdos(sc_tmc8462a_in, EC_END, mtbotTmc_syncs)) {
        fprintf(stderr, "Failed to configure PDOs.\n");
        return -1;
    }

    if (ecrt_domain_reg_pdo_entry_list(domain1, domain1_regs)) {
        fprintf(stderr, "PDO entry registration failed!\n");
        return -1;
    }

    printf("Activating master...\n");
    if (ecrt_master_activate(master)) {
        return -1;
    }

    if (!(domain1_pd = ecrt_domain_data(domain1))) {
        return -1;
    }

    /* Set priority */

    struct sched_param param = {};
    param.sched_priority = sched_get_priority_max(SCHED_FIFO);

    printf("Using priority %i.", param.sched_priority);
    if (sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
        perror("sched_setscheduler failed");
    }

    /* Lock memory */

    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1) {
        fprintf(stderr, "Warning: Failed to lock memory: %s\n",
                strerror(errno));
    }

    stack_prefault();

    printf("Starting RT task with dt=%u ns.\n", PERIOD_NS);

    clock_gettime(CLOCK_MONOTONIC, &wakeup_time);
    wakeup_time.tv_sec += 1; /* start in future */
    wakeup_time.tv_nsec = 0;

    while (1) {
        ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME,
                &wakeup_time, NULL);
        if (ret) {
            fprintf(stderr, "clock_nanosleep(): %s\n", strerror(ret));
            break;
        }

        cyclic_task();

        wakeup_time.tv_nsec += PERIOD_NS;
        while (wakeup_time.tv_nsec >= NSEC_PER_SEC) {
            wakeup_time.tv_nsec -= NSEC_PER_SEC;
            wakeup_time.tv_sec++;
        }
    }

    return ret;
}

/****************************************************************************/