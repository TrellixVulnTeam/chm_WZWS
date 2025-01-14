

#include <iostream>
#include "watchdog.h"
#include <chrono>
#include "chm.h"
#include "SubscriptionHandler.h"
#include "test.h"
Q_DEFINE_THIS_FILE
using namespace std;
using namespace Core_Health;
#define KICK_SIGNAL_DELAY 1


//ghghg
namespace Core_Health {
	class HealthMonitor : public QP::QActive {
	public:
		static HealthMonitor inst;

	private:
		 
		SubscriptionHandler subscription_handler;
		WatchDog&           watchdog_instance;
		QP::QTimeEvt        timeEvt_request_update;
		QP::QTimeEvt        timeEvt_kick;

		// used for testing 
		SubscriptionCmdOrWait cmd_handler;
		
	public:
		HealthMonitor();

	protected:
		Q_STATE_DECL(initial);
		Q_STATE_DECL(active);

	};
}

#if (QP_VERSION < 650U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 6.5.0 or higher required
#endif

namespace Core_Health {
QP::QActive * const AO_HealthMonitor = &HealthMonitor::inst; // "opaque" pointer to HealthMonitor AO

}

namespace Core_Health {


	HealthMonitor HealthMonitor::inst;

	HealthMonitor::HealthMonitor(): QActive(&initial), watchdog_instance(singleton<WatchDog>::getInstance()),
		timeEvt_request_update(this, UPDATE_SIG, 0U), timeEvt_kick(this, KICK_SIG, 0U){

		//start the watchdog
		watchdog_instance.Start(CHMConfig_t::T_WATCHDOG_RESET_SEC);
	};


	Q_STATE_DEF(HealthMonitor, initial) {
		// handle initialization event 
		if (e->sig == INIT_SIG) {
			cmd_handler = (Q_EVT_CAST(InitializationEvt)->cmd_or_wait);
		}
		//arm time event that fires  the signal KICK_SIG every T_UPDATE_WATCHDOG_SEC seconds
		timeEvt_kick.armX(ConvertSecondsToTicks((CHMConfig_t::T_UPDATE_WATCHDOG_SEC)) + KICK_SIGNAL_DELAY, ConvertSecondsToTicks((CHMConfig_t::T_UPDATE_WATCHDOG_SEC)) + KICK_SIGNAL_DELAY);
		//arm time event that fires the signal UPDATE_SIG every T_AO_ALIVE_SEC seconds
		timeEvt_request_update.armX(ConvertSecondsToTicks(CHMConfig_t::T_AO_ALIVE_SEC), ConvertSecondsToTicks(CHMConfig_t::T_AO_ALIVE_SEC));

		return tran(&active);
	}

	//${AOs::HealthMonitor::SM::active} ..................................................
	Q_STATE_DEF(HealthMonitor, active) {
		cmd_handler(system_clock::now());
		QP::QState status_;

		switch (e->sig) {
		case UPDATE_SIG: {
			bool no_users_in_sys = (0);                                                         //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			cout << "update" << endl;
			// if there are no subscribers, kick the watchdog
			if (no_users_in_sys) watchdog_instance.Kick();
			//publish signal REQUEST_UPDATE_SIG 
			QP::QEvt* update_evt = Q_NEW(QP::QEvt, REQUEST_UPDATE_SIG);
			QP::QF::PUBLISH(update_evt, this);
			status_ = Q_RET_HANDLED;
			break;
		}

		case ALIVE_SIG: {
			cout << "I'm alive" << endl;
			//find which user has sent the ALIVE_SIG signal (ie the appropriate system id)
			int index = (Q_EVT_CAST(MemberEvt)->memberNum);
			//update the members array with the new ALIVE signal
			subscription_handler.UpdateAliveStatus(index);
			// if all of the subscribers are alive, kick the watchdog
			if (subscription_handler.AreAllMembersResponsive() == true) watchdog_instance.Kick();
			status_ = Q_RET_HANDLED;
			break;
		}
        
		case SUBSCRIBE_SIG: {
			int user_id = (int)(Q_EVT_CAST(RegisterNewUserEvt)->id);
			int new_sys_id = INVALID_ID;

			// subscribe the user (if there is free space)
			new_sys_id = subscription_handler.SubscribeUser(user_id);
			if (new_sys_id != INVALID_ID) {
				//if we succeded in registering the new user we need notify the associated AO_Member
				MemberEvt* member_evt = Q_NEW(MemberEvt, SUBSCRIBE_MEMBER_SIG);
				member_evt->memberNum = new_sys_id;
				AO_Member[new_sys_id]->postFIFO(member_evt);
			}
			else printf("Subscribing didn't succeed\n");
			status_ = Q_RET_HANDLED;
			break;
		}

		case UNSUBSCRIBE_SIG: {
			bool unsubscribed_successfully = false;
			// find the system id (ie index in members array) of the member to unsubscribe
			int sys_id = (Q_EVT_CAST(MemberEvt)->memberNum);
			unsubscribed_successfully = subscription_handler.UnSubscribeUser(sys_id);
			if (unsubscribed_successfully) {
				//if we succeded in unsubscribing we need notify the associated AO_Member
				QP::QEvt* member_evt = Q_NEW(QP::QEvt, UNSUBSCRIBE_MEMBER_SIG);
				AO_Member[sys_id]->postFIFO(member_evt);
			}
			else printf("Unsubscribing didn't succeed\n");
			status_ = Q_RET_HANDLED;
			break;
		}

		case KICK_SIG: {
			cout << "kick" << endl;
			// kick the watchdog if all of the subscribers are alive
			if (subscription_handler.AreAllMembersResponsive() == true) watchdog_instance.Kick();
			// we need to print to log if users aren't active and then reset the system for the next cycle
			subscription_handler.LogUnResponsiveUsersAndReset();

			status_ = Q_RET_HANDLED;
			break;
		}
        

		case Q_EXIT_SIG: {
			timeEvt_request_update.disarm();
			timeEvt_kick.disarm();
			status_ = Q_RET_HANDLED;
			break;
		}

		default: {
			status_ = super(&top);
			break;
		}
		}
		return status_;
	}


}

