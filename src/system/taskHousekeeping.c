/*                                 SUCHAI
 *                      NANOSATELLITE FLIGHT SOFTWARE
 *
 *      Copyright 2018, Carlos Gonzalez Cortes, carlgonz@uchile.cl
 *      Copyright 2018, Tomas Opazo Toro, tomas.opazo.t@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "taskHousekeeping.h"

static const char *tag = "Housekeeping";

enum phase_n{
    phase_a0, // 0: (A0) testing,
    phase_a1, // 1: (A1) preparation,
    phase_a,  // 2: (A) ascend,
    phase_b,  // 3: (B) equilibrium,
    phase_b1, // 4: (B1) deploy1,
    phase_b2, // 5: (B2) deploy2,
    phase_c,  // 6: (C) descend,
    phase_c1, // 7: (C1) landing
};


const int  MIN_PHASE_A  =  20;
const int  MIN_PHASE_A1 =  25;
const int  MIN_PHASE_B  = 115;
const int  MIN_PHASE_B1 = 155;
const int  MIN_PHASE_B2 = 157;
const int  MIN_PHASE_C  = 159;
const int  MIN_PHASE_C1 = 189;

/* Test Flight Plan*/
/*const int  MIN_PHASE_A  =   2;
const int  MIN_PHASE_B  =  4;
const int  MIN_PHASE_B1 =  6;
const int  MIN_PHASE_B2 =  8;
const int  MIN_PHASE_C  =  10;
const int  MIN_PHASE_C1 =  12;*/

void taskHousekeeping(void *param)
{
    LOGD(tag, "Started");

    portTick delay_ms    = 1000;            //Task period in [ms]

    unsigned int elapsed_sec = 0;           // Seconds counter
    unsigned int _10sec_check = 10;         //10[s] condition
    unsigned int _01min_check = 1*60;       //1[m] condition
    unsigned int _05min_check = 5*60;       //5[m] condition
    unsigned int _10min_check = 10*60;      //10[m] condition
    unsigned int _15min_check = 15*60;      //15[m] condition
    unsigned int _1hour_check = 60*60;      //01[h] condition

    int phase = dat_get_system_var(dat_balloon_phase);      // current phase
    int balloons = dat_get_system_var(dat_balloon_deploys); // balloons available

    portTick xLastWakeTime = osTaskGetTickCount();
    //printf("----- TICK %u %u\n", xLastWakeTime, *(&xLastWakeTime));
    while(1)
    {
        osTaskDelayUntil(&xLastWakeTime, delay_ms); // Suspend task
        elapsed_sec++; //= delay_ms/1000; // Update seconds counts

        /**
         * Phases Setup
         */
        if (elapsed_sec == 1 || elapsed_sec == 2 || elapsed_sec == 3 || elapsed_sec == 4 ) {
            cmd_t *cmd_open_la = cmd_get_str("open_dpl_la");
            cmd_send(cmd_open_la);
            /*set pi-camera looking at th horizon*/
            cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
            cmd_send(cmd_close_sm);
            
            cmd_t *cmd_send_iridium_msg1 = cmd_get_str("send_iridium_msg1");
            cmd_send(cmd_send_iridium_msg1);
        }

        /**
         * In all Phases sample prs, dpl, gps and imet every 10 seconds
         */
        if (1) {
            LOGD(tag, "elapsed second %u", elapsed_sec);
            change_system_phase();
            phase = dat_get_system_var(dat_balloon_phase); // Determine current phase

            if ((elapsed_sec % 10) == 1) {
                cmd_t *cmd_get_gps = cmd_get_str("get_gps_data");
                cmd_send(cmd_get_gps);
            }

            if ((elapsed_sec % 10) == 4) {
                cmd_t *cmd_get_prs = cmd_get_str("get_prs_data");
                cmd_send(cmd_get_prs);
            }

            if ((elapsed_sec % 10) == 7) {
                cmd_t *cmd_get_dpl = cmd_get_str("get_dpl_data");
                cmd_send(cmd_get_dpl);
            }
            if ((elapsed_sec % 10) == 9) {
                cmd_t *cmd_get_imet = cmd_get_str("get_imet_data");
                cmd_send(cmd_get_imet);
            }
            if ((elapsed_sec % _15min_check) == 0) {
                cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                cmd_send(cmd_run_sstv);
            }

            /**
             * In Phase A0: Test main systems
                -deploy actuator
                -iridium module
                -camera
                -sstv
            */
            if(phase == phase_a0) {
                /*Test open Lineal Actuator*/
                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_open_la = cmd_get_str("open_dpl_la");
                    cmd_send(cmd_open_la);
                }

                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_send_iridium = cmd_get_str("send_iridium_data");
                    cmd_send(cmd_send_iridium);
                }

                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_open_sm = cmd_get_str("open_dpl_sm");
                    cmd_send(cmd_open_sm);
                }

                if ((elapsed_sec % _01min_check) == 3) {
                    cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
                    cmd_send(cmd_close_sm);
                }

                if ((elapsed_sec % _05min_check) == 0) {
                    cmd_t *cmd_run_sstv = cmd_get_str("run_sstv");
                    cmd_send(cmd_run_sstv);
                }
            }

            /**
             * In Phase A1: Set dafualts values
                -deploy system
                -camera
                -iridiun startingMission msg
            */
            if(phase == phase_a1) {
                /*Test open Lineal Actuator*/
                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_open_la = cmd_get_str("open_dpl_la");
                    cmd_send(cmd_open_la);
                }

                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
                    cmd_send(cmd_close_sm);
                }

                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_send_iridium_msg1 = cmd_get_str("send_iridium_msg1");
                    cmd_send(cmd_send_iridium_msg1);
                }

            }


            /**
             * In Phase B1:
                -deploy linear actuator every 10 seconds
                -send data through iridium
            */
            if(phase == phase_b1) {
                if ((elapsed_sec % 10) == 0) {
                    cmd_t *cmd_close_la = cmd_get_str("close_dpl_la");
                    cmd_send(cmd_close_la);
                }

                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_send_iridium = cmd_get_str("send_iridium_data");
                    cmd_send(cmd_send_iridium);
                }
            }

            /**
             * In Phase B2:
                -point pi-camera to the starts (try cmd every 10 seconds)
                -send data through iridium
            */
            if(phase == phase_b2) {
                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_open_sm = cmd_get_str("open_dpl_sm");
                    cmd_send(cmd_open_sm);
                }

                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_send_iridium = cmd_get_str("send_iridium_data");
                    cmd_send(cmd_send_iridium);
                }
            }

            /**
             * In Phase C:
                -point pi-camera to the horizon (try cmd every 10 seconds)
                -send data through iridium
                -send final msg through iridium (just once)
            */
            if(phase == phase_c) {
                /*looking to the horizon*/
                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_close_sm = cmd_get_str("close_dpl_sm");
                    cmd_send(cmd_close_sm);
                }
                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_send_iridium = cmd_get_str("send_iridium_data");
                    cmd_send(cmd_send_iridium);
                }
            }
            if (elapsed_sec == MIN_PHASE_C1*60 || elapsed_sec == MIN_PHASE_C1*60+1 || elapsed_sec == MIN_PHASE_C1*60+2 || elapsed_sec == MIN_PHASE_C1*60+3) {
                cmd_t *cmd_send_iridium_msg2 = cmd_get_str("send_iridium_msg2");
                cmd_send(cmd_send_iridium_msg2);
            }

            /**
             * In Phase C1:
                -send data through iridium
            */
            if(phase == phase_c1) {
                if ((elapsed_sec % _01min_check) == 0) {
                    cmd_t *cmd_send_iridium = cmd_get_str("send_iridium_data");
                    cmd_send(cmd_send_iridium);
                }
            }
            /**
             * Always to do list
             */
            /* 1 min actions, update minutes alive counter*/
            if((elapsed_sec % _01min_check) == 0)
            {
                LOGD(tag, "1 hour check");
                cmd_t *cmd_1h = cmd_get_str("update_hours_alive");
                cmd_add_params_var(cmd_1h, 1); // Add 1 min
                cmd_send(cmd_1h);
                LOGI(tag, "Phase: %d", phase);
            }
        }
    }
}

void change_system_phase()
{
    int current_phase = dat_get_system_var(dat_balloon_phase);
    int min_alive= dat_get_system_var(dat_obc_hrs_alive);
    LOGD(tag, "minutes alive:  %d", min_alive);

    if( min_alive == -1)
        return;

    if(min_alive <= MIN_PHASE_A)
    {
        LOGD(tag, "######################### We are in PHASE A0 #########################");
        // We are in phase A0
        if(current_phase != phase_a0) {
            dat_set_system_var(dat_balloon_phase, phase_a0);
        }
    }
    else if (min_alive > MIN_PHASE_A && min_alive <= MIN_PHASE_A1)
    {
        LOGD(tag, "######################### We are in PHASE A1 #########################");
        // We are in phase A1
        if(current_phase != phase_a1){
            dat_set_system_var(dat_balloon_phase, phase_a1);

        }
    }
    else if (min_alive > MIN_PHASE_A1 && min_alive <= MIN_PHASE_B)
    {
        LOGD(tag, "######################### We are in PHASE A #########################");
        // We are in phase A
        if(current_phase != phase_a){
            dat_set_system_var(dat_balloon_phase, phase_a);

        }
    }
    else if (min_alive > MIN_PHASE_B && min_alive <= MIN_PHASE_B1)
    {
        LOGD(tag, "######################### We are in PHASE B #########################");
        // We are in phase B
        if(current_phase != phase_b){
            dat_set_system_var(dat_balloon_phase, phase_b);
        }

    }
    else if (min_alive > MIN_PHASE_B1 && min_alive <= MIN_PHASE_B2)
    {
        LOGD(tag, "######################### We are in PHASE B1 #########################");
        // We are in phase B1
        if(current_phase != phase_b1){
            dat_set_system_var(dat_balloon_phase, phase_b1);
        }

    }
    else if (min_alive > MIN_PHASE_B2 && min_alive <= MIN_PHASE_C)
    {
        LOGD(tag, "######################### We are in PHASE B2 #########################");
        // We are in phase B2
        if(current_phase != phase_b2){
            dat_set_system_var(dat_balloon_phase, phase_b2);
        }

    }
    else if (min_alive > MIN_PHASE_C && min_alive <= MIN_PHASE_C1)
    {
        LOGD(tag, "######################### We are in PHASE C #########################");
        // We are in phase C
        if(current_phase != phase_c) {
            dat_set_system_var(dat_balloon_phase, phase_c);
        }

    }
    else if (min_alive > MIN_PHASE_C1)
    {
        LOGD(tag, "######################### We are in PHASE C1 #########################");
        // We are in phase C1
        if(current_phase != phase_c1){
            dat_set_system_var(dat_balloon_phase, phase_c1);
        }
    }
}
