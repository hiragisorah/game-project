#include "engine.h"

class Engine::Impl
{
public:
	Impl(Engine * engine)
		: owner_(engine)
	{
		if (this->singleton_)
		{
			throw std::exception("engine is singleton.");
		}

		Impl::singleton_ = this;
	}

	virtual ~Impl(void)
	{
		Impl::singleton_ = nullptr;
	}

	Engine * owner_;

	static Impl * singleton_;
};

Engine::Impl * Engine::Impl::singleton_ = nullptr;

Engine::Engine(void) noexcept(false)
	: impl_(std::make_unique<Impl>(this))
{

}

Engine::Engine(Engine && move_from) noexcept
	: impl_(std::move(move_from.impl_))
{
	this->impl_->owner_ = this;
}

Engine & Engine::operator=(Engine && move_from) noexcept
{
	this->impl_ = std::move(move_from.impl_);
	this->impl_->owner_ = this;
	return *this;
}

Engine::~Engine(void)
{
}

Engine & Engine::Get(void)
{
	if (!Impl::singleton_ || !Impl::singleton_->owner_)
		throw std::exception("engine is a singleton.");

	return *Impl::singleton_->owner_;
}