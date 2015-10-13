#include <iostream>
#include <sys/time.h>
#include <unistd.h>

namespace moo_clock {
	struct timeval last_tic;

	double to_sec(struct timeval tm);
	double sec_since(struct timeval tm);
	double tic();
	double toc();

	double tic() {
		gettimeofday(&last_tic, NULL);	

		return to_sec(last_tic);	
	}

	double toc() {
		return sec_since(last_tic);
	}

	double sec_since(struct timeval tm) {
		struct timeval now;
		gettimeofday(&now, NULL);

		return to_sec(now) - to_sec(tm);
	}

	double to_sec(struct timeval tm) {
		return tm.tv_sec + (double)tm.tv_usec / 1000000.0;
	}
}
