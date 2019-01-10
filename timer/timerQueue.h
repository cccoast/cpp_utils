#ifndef __TIMER_QUEUE_H__
#define __TIMER_QUEUE_H__

#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <utility>

using namespace std;

template<class T1, class T2>
class timeEvenetTemplate :public pair<T1, T2>
{
public:
	void* arg;
	timeEvenetTemplate() : arg(NULL) {}
	timeEvenetTemplate(T1&& _first, T2&& _second) :arg(NULL) {
        this->first = _first, this->second = _second;
	}
	timeEvenetTemplate(T1&& _first, T2&& _second, void* _arg) :arg(_arg) {
        this->first = _first, this->second = _second;
	}
	bool operator < (const timeEvenetTemplate & tv) {
		if (this->first < tv.first) {
			return true;
		}
		else if (this->first > tv.first) {
			return false;
		}
		else {
			if (this->second < tv.second) {
				return true;
			}
			else if (this->second > tv.second) {
				return false;
			}
			else
				return false;
		}
	}
	bool operator > (const timeEvenetTemplate & tv) {
		if (this->first == tv.first && this->second == tv.second)
			return false;
		else
			return !((*this) < tv);
	}
};

typedef timeEvenetTemplate<int, int> timeEvent;
typedef std::priority_queue< timeEvent, vector<timeEvent>, greater<timeEvent> > timerQueue;

#endif
