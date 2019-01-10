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
	boost::asio::io_service::work idle;
	boost::asio::io_service& io;
	shared_ptr<boost::asio::io_service> iop;
public:

	boost::posix_time::time_duration getDurationFromNow(timeEvent& time_event) {
		struct timeval tv;
#ifdef _MSC_VER
		gettimeofday_fast(&tv, NULL);
#else
		gettimeofday(&tv,NULL);
#endif
		long milliSecFromNow = (time_event.first - tv.tv_sec) * 1000 + int((time_event.second - tv.tv_usec) / 1000);
		milliSecFromNow = milliSecFromNow > 0 ? milliSecFromNow : 0;
		boost::posix_time::time_duration tmp = boost::posix_time::time_duration(boost::posix_time::millisec(milliSecFromNow));
		return std::move(tmp);
	}

	timer(boost::asio::io_service& _io) :
		io(_io),
		idle(_io)
	{}

	timer():
        iop(make_shared<boost::asio::io_service>()),
        io(*iop),
        idle(io)
	{}

	void start() {
		std::thread t(boost::bind(&boost::asio::io_service::run, &io));
		t.detach();
	}

	void timeOut(const boost::system::error_code& ec,callBackFunction& callback_func, shared_ptr<boost::asio::deadline_timer>& _dtimer) {

		if(!ec)
		{
            callback_func();
		}
		else {
			cout << "Call timeOut! Error Code = " << ec << endl;
		}
	}

	void post(timeEvent& te, std::decay<callBackFunction>::type callback_func) {
		auto duration = getDurationFromNow(te);
		auto task = [duration,callback_func,this]() {
			shared_ptr<boost::asio::deadline_timer> pnewTimer(new boost::asio::deadline_timer(this->io,duration));
			pnewTimer->async_wait(boost::bind(&timer::timeOut, this, boost::asio::placeholders::error,
                                                callback_func, pnewTimer));
		};
		this->io.post(task);
	}
};


#endif
