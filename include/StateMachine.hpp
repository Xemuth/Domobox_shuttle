/*******************************************************
* StateMachine -- Domobox rely is behavior on a state  *
* machine. This one rely on the GoF state pattern      *
*                                                      *
* Author:  Clément Hamon                               *
********************************************************/
#ifndef STATE_MACHINE_HEADER
#define STATE_MACHINE_HEADER
#include <memory>
/*
    Abrasct state class dedictated to the Final state machine of Domobox shuttle project
    The state machine is inspired from GoF Design pattern book. 
*/

namespace domobox{

    enum ALL_STATES{
        INITIALISATION = 0,
        CREDENTIAL_ACQUISITION = 1,
        SERVER_CREATION = 2,
        READY = 3,
        ERROR = 4
    };
    
    class DState{
        public:
            virtual ~DState(){};
            virtual ALL_STATES GetName() const = 0;
            virtual std::unique_ptr<DState> Next() = 0;
    };

    class DStateContext{
        public:
            DStateContext();
            ~DStateContext();
            void Run();

        private:
            std::unique_ptr<DState> state;
    };
}

#endif