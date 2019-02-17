#include "tot.h"


unsigned tot::getTimeMicros ()
{
	auto now = std::chrono::system_clock::now();
	auto duration = now.time_since_epoch();
	return static_cast<unsigned> (std::chrono::duration_cast<std::chrono::microseconds> (duration).count ());
}

 static unsigned startTime = tot::getTimeMicros ();
 static float dTime = 1;

void tot::updateDTime ()
{
	unsigned cur = getTimeMicros();
	unsigned tmp = cur;
	cur -= startTime;
	startTime = tmp;
	dTime = cur / 10000.f;
}

float tot::getDTime ()
{
	return dTime;
}