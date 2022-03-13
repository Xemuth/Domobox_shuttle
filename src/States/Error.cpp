#include "StateMachine.hpp"
#include "States/Error.hpp"
#include <esp_system.h>
namespace domobox{

    S_Error::S_Error(std::string&& error) : error_msg(std::move(error)) {
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = GPIO_MODE_OUTPUT;
        io_conf.pin_bit_mask = (1ULL << alarm_led_gpio);
        gpio_config(&io_conf);
    }

    S_Error::~S_Error(){}
    ALL_STATES S_Error::GetName() const{return ALL_STATES::INITIALISATION;}

    std::unique_ptr<DState> S_Error::Next(){
        // in error, we toggle the red led and we print error message until reset
        static bool alarm_state = false;
        alarm_state = !alarm_state;
        gpio_set_level(alarm_led_gpio, !alarm_state);
        printf("%s", error_msg.c_str());
        return {};
    }
}