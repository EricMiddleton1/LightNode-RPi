#include "BCM2835Lock.hpp"

#include <stdexcept>

#include <bcm2835.h>

size_t BCM2835Lock::refCount = 0;
std::mutex BCM2835Lock::refMutex;

BCM2835Lock::BCM2835Lock() {
	std::unique_lock<std::mutex> refLock(refMutex);
	
	if(refCount == 0) {
		if(!bcm2835_init()) {
			throw std::runtime_error("BCM2835Lock::BCM2835Lock: Failed to init bcm2835 "
				"library");
		}
	}

	refCount++;
}

BCM2835Lock::~BCM2835Lock() {
	std::unique_lock<std::mutex> refLock(refMutex);

	refCount--;
	if(refCount == 0) {
		bcm2835_close();
	}
}
