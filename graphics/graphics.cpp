#include "graphics.h"

class Graphics::Impl
{
public:
	Impl(Graphics * graphics)
		: owner_(graphics)
	{
		if (this->singleton_)
		{
			throw std::exception("graphics is singleton.");
		}

		Impl::singleton_ = this;
	}

	virtual ~Impl(void)
	{
		Impl::singleton_ = nullptr;
	}

	Graphics * owner_;

	static Impl * singleton_;
};

Graphics::Impl * Graphics::Impl::singleton_ = nullptr;

Graphics::Graphics(void) noexcept(false)
	: impl_(std::make_unique<Impl>(this))
{

}

Graphics::Graphics(Graphics && move_from) noexcept
	: impl_(std::move(move_from.impl_))
{
	this->impl_->owner_ = this;
}

Graphics & Graphics::operator=(Graphics && move_from) noexcept
{
	this->impl_ = std::move(move_from.impl_);
	this->impl_->owner_ = this;
	return *this;
}

Graphics::~Graphics(void)
{
}

Graphics & Graphics::Get(void)
{
	if (!Impl::singleton_ || !Impl::singleton_->owner_)
		throw std::exception("graphics is a singleton.");

	return *Impl::singleton_->owner_;
}

void Graphics::AddRenderer(const std::shared_ptr<Renderer> & renderer)
{
	this->renderer_list_[renderer->draw_mode_].emplace_back(renderer);
}

void Graphics::Rendering(void)
{
	for (auto & renderer : this->renderer_list_[DRAW_MODE_DEFFERED_3D])
		this->Rendering(renderer);

	for (auto & renderer : this->renderer_list_[DRAW_MODE_DEFFERED_2D])
		this->Rendering(renderer);

	for (auto & renderer : this->renderer_list_[DRAW_MODE_BACK_BUFFER_3D])
		this->Rendering(renderer);

	for (auto & renderer : this->renderer_list_[DRAW_MODE_BACK_BUFFER_2D])
		this->Rendering(renderer);
}
