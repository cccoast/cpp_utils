#ifndef __TIMER2_H__
#define __TIMER2_H__

#ifdef _MSC_VER
#include "../Util/Win32_Time.h"
#else
#include <sys/time.h>
#endif

#include "timerQueue.h"
#include <functional>
#include <memory>
#include <thread>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/system/system_error.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

using callBackFunction = std::function<void(void)>;

class timer :public boost::noncopyable
{
    unique_ptr<boost::asio::io_service> iop;
	unique_ptr<boost::asio::io_service::work> idle;
public:
	shared_ptr<boost::posix_time::time_duration> getDurationFromNow(timeEvent& time_event) {
		struct timeval tv;
		gettimeofday(&tv,NULL);
		long milliSecFromNow = (time_event.first - tv.tv_sec) * 1000 + int((time_event.second - tv.tv_usec) / 1000);
		milliSecFromNow = milliSecFromNow > 0 ? milliSecFromNow : 0;
		return make_shared<boost::posix_time::time_duration>(boost::posix_time::millisec(milliSecFromNow));
	}

	timer():
		iop(make_unique<boost::asio::io_service>()),
        idle(make_unique<boost::asio::io_service::work>(*iop))
	{
	}

	void start() {
		std::thread t(boost::bind(&boost::asio::io_service::run, iop.get()));
		t.detach();
	}

	void timeOut(const boost::system::error_code& ec,
                std::decay<callBackFunction>::type& callback_func,
                shared_ptr<boost::asio::deadline_timer>& _dtimer) {
		if(!ec)
		{
            callback_func();
		}
		else {
			cout << "Call timeOut! Error Code = " << ec << endl;
		}
	}

    //std::decay<callBackFunction>::type
	void post_timestamp(timeEvent& te, callBackFunction callback_func) {
		auto duration{std::move(getDurationFromNow(te))};
		auto task = [duration,callback_func,this]() {
			shared_ptr<boost::asio::deadline_timer> pnewTimer(new boost::asio::deadline_timer(*iop,*duration));
			pnewTimer->async_wait(boost::bind(&timer::timeOut, this,
                                              boost::asio::placeholders::error,
                                              callback_func, pnewTimer));
		};
		iop->post(task);
	}

	void post_duration(unsigned int seconds,unsigned int milli_seconds,callBackFunction callback_func){
        auto duration{std::move(make_shared<boost::posix_time::time_duration>(boost::posix_time::millisec(seconds * 1000 + milli_seconds)))};
        auto task = [duration,callback_func,this]() {
			shared_ptr<boost::asio::deadline_timer> pnewTimer(new boost::asio::deadline_timer(*iop,*duration));
			pnewTimer->async_wait(boost::bind(&timer::timeOut, this,
                                              boost::asio::placeholders::error,
                                              callback_func, pnewTimer));
		};
		iop->post(task);
	}
};


#endif
