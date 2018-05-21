#pragma once

#include <memory>

class Scene;

class System
{
public:
	System(std::shared_ptr<System> & self)
		: self_(self) {}

private:
	std::shared_ptr<System> & self_;

public:
	const std::shared_ptr<System> & self(void) { return this->self_; }

private:
	bool Remove(void) { this->OnRemove(), this->self_.reset(); }

public:
	virtual void OnAdd(void) {}
	virtual void OnRemove(void) {}
	virtual void Pause(void) {}
	virtual void Update(void) {}
	virtual void Always(void) {}
};