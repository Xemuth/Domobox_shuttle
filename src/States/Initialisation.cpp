
/*******************************************************
* Initialisation state -- We init all esp32 features   *
* needed in domobox, in case of error move to error    *
* state and print the error description until reset.   *
*                                                      *
* Author:  Clément Hamon                               *
********************************************************/
#include "StateMachine.hpp"
#include "States/Initialisation.hpp"
#include "States/Credential_acquisition.hpp"
#include "Definition.hpp"
#include "esp_wifi.h"
#include "nvs_flash.h"

namespace domobox{

    S_Initialisation::S_Initialisation(){
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << led);
        gpio_config(&io_conf);
        gpio_set_level(led, true);
    }

    S_Initialisation::~S_Initialisation(){
        gpio_set_level(led, false);
    }

    ALL_STATES S_Initialisation::GetName() const{return ALL_STATES::INITIALISATION;}

    std::unique_ptr<DState> S_Initialisation::Next(){
        static bool one_stage_state = true;
        DOMOBOX_FSM_ASSERT(one_stage_state, "Initialisation state has been reached for a second time !");
        one_stage_state = false; // from now it is impossible to move to init state again

        DOMOBOX_FSM_CHECK(nvs_flash_init()); // Initialize the default NVS partition.
        DOMOBOX_FSM_CHECK(esp_netif_init()); // Initialize the underlying TCP/IP stack
        DOMOBOX_FSM_CHECK(esp_event_loop_create_default()); // Create the default task loop for wifi

        esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta(); // Creates default WIFI STA. In case of any init error this API aborts
        DOMOBOX_FSM_ASSERT(sta_netif, "Failled to create default WIFI STA.");

        wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
        DOMOBOX_FSM_CHECK(esp_wifi_init(&cfg)); //Init WiFi Alloc resource for WiFi driver
        DOMOBOX_FSM_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); // Set the WiFi operating mode
        DOMOBOX_FSM_CHECK(esp_wifi_start()); // Start wifi

        return std::unique_ptr<S_CredentialAcquisition>(new S_CredentialAcquisition);
    }
}