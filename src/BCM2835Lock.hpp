#pragma once


#include <mutex>


class BCM2835Lock
{
public:
	BCM2835Lock();
	~BCM2835Lock();

	BCM2835Lock(const BCM2835Lock&) = delete;
	BCM2835Lock& operator=(const BCM2835Lock&) = delete;

private:
	static size_t refCount;
	static std::mutex refMutex;
};
